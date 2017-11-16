#!/usr/bin/env python3

# vim: ai:ts=4:sw=4

import sys
from os import listdir, walk
import os, fnmatch
import re
import yaml
import pprint
import argparse
import collections

from devicetree import parse_file

# globals
compatibles = {}
phandles = {}
aliases = {}
chosen = {}
reduced = {}
defs = {}
structs = {}
struct_dict = {}
node_file = ""


def convert_string_to_label(s):
    # Transmute ,- to _
    s = s.replace("-", "_")
    s = s.replace(",", "_")
    s = s.replace("@", "_")
    return s


def get_all_compatibles(d, name, comp_dict):
    if 'props' in d:
        compat = d['props'].get('compatible')
        enabled = d['props'].get('status')

    if enabled == "disabled":
        return comp_dict

    if compat is not None:
        comp_dict[name] = compat

    if name != '/':
        name += '/'

    if isinstance(d, dict):
        if d['children']:
            for k, v in d['children'].items():
                get_all_compatibles(v, name + k, comp_dict)

    return comp_dict


def get_aliases(root):
    if 'children' in root:
        if 'aliases' in root['children']:
            for k, v in root['children']['aliases']['props'].items():
                aliases[v] = k

    return


def get_compat(node):

    compat = None

    if 'props' in node:
        compat = node['props'].get('compatible')

    if isinstance(compat, list):
        compat = compat[0]

    return compat


def get_chosen(root):

    if 'children' in root:
        if 'chosen' in root['children']:
            for k, v in root['children']['chosen']['props'].items():
                chosen[k] = v

    return


def get_phandles(root, name, handles):

    if 'props' in root:
        handle = root['props'].get('phandle')
        enabled = root['props'].get('status')

    if enabled == "disabled":
        return

    if handle is not None:
        phandles[handle] = name

    if name != '/':
        name += '/'

    if isinstance(root, dict):
        if root['children']:
            for k, v in root['children'].items():
                get_phandles(v, name + k, handles)

    return


class Loader(yaml.Loader):
    def __init__(self, stream):
        self._root = os.path.realpath(stream.name)
        super(Loader, self).__init__(stream)
        Loader.add_constructor('!include', Loader.include)
        Loader.add_constructor('!import', Loader.include)

    def include(self, node):
        if isinstance(node, yaml.ScalarNode):
            return self.extractFile(self.construct_scalar(node))

        elif isinstance(node, yaml.SequenceNode):
            result = []
            for filename in self.construct_sequence(node):
                result += self.extractFile(filename)
            return result

        elif isinstance(node, yaml.MappingNode):
            result = {}
            for k, v in self.construct_mapping(node).iteritems():
                result[k] = self.extractFile(v)
            return result

        else:
            print("Error:: unrecognised node type in !include statement")
            raise yaml.constructor.ConstructorError

    def extractFile(self, filename):
        filepath = os.path.join(os.path.dirname(self._root), filename)
        if not os.path.isfile(filepath):
            # we need to look in common directory
            # take path and back up 2 directories and tack on '/common/yaml'
            filepath = os.path.dirname(self._root).split('/')
            filepath = '/'.join(filepath[:-2])
            filepath = os.path.join(filepath + '/common/yaml', filename)
        with open(filepath, 'r') as f:
            return yaml.load(f, Loader)


def insert_defs(node_address, new_defs, new_aliases):
    if node_address in defs:
        if 'aliases' in defs[node_address]:
            defs[node_address]['aliases'].update(new_aliases)
        else:
            defs[node_address]['aliases'] = new_aliases

        defs[node_address].update(new_defs)
    else:
        new_defs['aliases'] = new_aliases
        defs[node_address] = new_defs

    return


def insert_structs(node_address, deflabel, new_structs):
    if not isinstance(new_structs, list):
        list_structs = [new_structs]
    else:
        list_structs = new_structs
    for s in list_structs:
        if node_address in structs:
            if deflabel in structs[node_address]:
                for k in s.keys():
                    if len(s[k]) > 0:
                        structs[node_address][deflabel][k].append(s[k][0])
            else:
                structs[node_address][deflabel] = s
        else:
            structs[node_address] = {}
            structs[node_address][deflabel] = s

    return


def find_node_by_path(nodes, path):
    d = nodes
    for k in path[1:].split('/'):
        d = d['children'][k]

    return d


def compress_nodes(nodes, path):
    if 'props' in nodes:
        status = nodes['props'].get('status')

    if status == "disabled":
        return

    if isinstance(nodes, dict):
        reduced[path] = dict(nodes)
        reduced[path].pop('children', None)
        if path != '/':
            path += '/'
        if nodes['children']:
            for k, v in nodes['children'].items():
                compress_nodes(v, path + k)

    return


def find_parent_irq_node(node_address):
    address = ''

    for comp in node_address.split('/')[1:]:
        address += '/' + comp
        if 'interrupt-parent' in reduced[address]['props']:
            interrupt_parent = reduced[address]['props'].get(
                'interrupt-parent')

    return reduced[phandles[interrupt_parent]]


def extract_interrupts(node_address, yaml, y_key, names, def_label):
    node = reduced[node_address]

    try:
        props = list(node['props'].get(y_key))
    except:
        props = [node['props'].get(y_key)]

    irq_parent = find_parent_irq_node(node_address)

    l_base = def_label.split('/')
    index = 0

    prop_structs = {'data': [], 'defs': [], 'members': []}
    while props:
        prop_def = {}
        prop_alias = {}
        l_idx = [str(index)]

        if y_key == 'interrupts-extended':
            cell_parent = reduced[phandles[props.pop(0)]]
            name = []
        else:
            try:
                name = [names.pop(0).upper()]
            except:
                name = []

            cell_parent = irq_parent

        cell_yaml = yaml[get_compat(cell_parent)]
        l_cell_prefix = [yaml[get_compat(irq_parent)].get(
            'cell_string', []).upper()]

        cell_data = []
        cell_defs = {'labels': [], 'aliases': []}
        for i in range(cell_parent['props']['#interrupt-cells']):
            l_cell_name = [cell_yaml['#cells'][i].upper()]
            if l_cell_name == l_cell_prefix:
                l_cell_name = []

            l_fqn = '_'.join(l_base + l_cell_prefix + l_idx + l_cell_name)
            val = props.pop(0)
            prop_def[l_fqn] = val
            cell_defs['labels'].append(l_fqn)
            cell_data.append(val)
            if len(name):
                prop_alias['_'.join(l_base +
                                    l_cell_prefix +
                                    name +
                                    l_cell_name)] = l_fqn
                cell_defs['aliases'].append('_'.join(l_base +
                                                     name + l_cell_prefix))
        prop_structs['data'].append(cell_data)
        prop_structs['defs'].append(cell_defs)
        prop_structs['members'].append(cell_yaml['#cells'])

        index += 1
        insert_defs(node_address, prop_def, prop_alias)
    insert_structs(node_address, 'interrupts', prop_structs)

    return


def extract_reg_prop(node_address, names, defs, def_label, div, post_label):

    reg = reduced[node_address]['props']['reg']
    if type(reg) is not list: reg = [ reg ]
    props = list(reg)

    address_cells = reduced['/']['props'].get('#address-cells')
    size_cells = reduced['/']['props'].get('#size-cells')
    address = ''
    for comp in node_address.split('/')[1:]:
        address += '/' + comp
        address_cells = reduced[address]['props'].get(
            '#address-cells', address_cells)
        size_cells = reduced[address]['props'].get('#size-cells', size_cells)

    if post_label is None:
        post_label = "BASE_ADDRESS"

    index = 0
    l_base = def_label.split('/')
    l_addr = [convert_string_to_label(post_label).upper()]
    l_size = ["SIZE"]

    prop_struct = {'data': [], 'defs': [], 'members': []}
    while props:
        prop_def = {}
        prop_alias = {}
        addr = 0
        size = 0
        l_idx = [str(index)]
        entry_struct = {}
        entry_members = ['base', 'size']
        entry_data = []
        entry_defs = {'labels': [], 'aliases': []}

        try:
            name = [names.pop(0).upper()]
        except:
            name = []

        for x in range(address_cells):
            addr += props.pop(0) << (32 * x)
        for x in range(size_cells):
            size += props.pop(0) << (32 * x)

        l_addr_fqn = '_'.join(l_base + l_addr + l_idx)
        l_size_fqn = '_'.join(l_base + l_size + l_idx)
        entry_data = []

        if address_cells:
            prop_def[l_addr_fqn] = hex(addr)
            entry_defs['labels'].append(l_addr_fqn)
        if size_cells:
            prop_def[l_size_fqn] = int(size / div)
            entry_defs['labels'].append(l_size_fqn)
        if len(name):
            if address_cells:
                prop_alias['_'.join(l_base + name + l_addr)] = l_addr_fqn
                entry_defs['aliases'].append(
                    {'_'.join(l_base + name + l_addr): l_addr_fqn})
            if size_cells:
                prop_alias['_'.join(l_base + name + l_size)] = l_size_fqn
                entry_defs['aliases'].append(
                    {'_'.join(l_base + name + l_size): l_size_fqn})

        if index == 0:
            if address_cells:
                prop_alias['_'.join(l_base + l_addr)] = l_addr_fqn
                entry_defs['aliases'].append(
                    {'_'.join(l_base + l_addr): l_addr_fqn})
            if size_cells:
                prop_alias['_'.join(l_base + l_size)] = l_size_fqn
                entry_defs['aliases'].append(
                    {'_'.join(l_base + l_size): l_size_fqn})

        entry_data = [hex(addr), int(size / div)]
        prop_struct['defs'].append(entry_defs)
        prop_struct['data'].append(entry_data)
        prop_struct['members'].append(entry_members)

        insert_defs(node_address, prop_def, prop_alias)
        insert_structs(node_address, 'reg', prop_struct)

        # increment index for definition creation
        index += 1

    return




def extract_cells(node_address, yaml, y_key, names, index, prefix,
                  def_label):
    try:
        props = list(reduced[node_address]['props'].get(y_key))
    except:
        props = [reduced[node_address]['props'].get(y_key)]

    cell_parent = reduced[phandles[props.pop(0)]]

    try:
        cell_yaml = yaml[get_compat(cell_parent)]
    except:
        raise Exception(
            "Could not find yaml description for " + cell_parent['name'])

    try:
        name = names.pop(0).upper()
    except:
        name = []

    l_cell = [str(cell_yaml.get('cell_string', ''))]
    l_base = def_label.split('/')
    l_base += prefix
    l_idx = [str(index)]

    prop_def = {}
    prop_alias = {}
    prop_struct = {'data': [], 'defs': [], 'members': []}

    cell_data = []
    cell_members = cell_yaml['#cells']
    cell_defs = {'labels': [], 'aliases': []}
    for k in cell_parent['props'].keys():
        if k[0] == '#' and '-cells' in k:
            for i in range(cell_parent['props'].get(k)):
                l_cellname = [str(cell_yaml['#cells'][i]).upper()]
                if l_cell == l_cellname:
                    label = l_base + l_cell + l_idx
                else:
                    label = l_base + l_cell + l_cellname + l_idx
                label_name = l_base + name + l_cellname
                val = props.pop(0)
                cell_data.append(val)
                prop_def['_'.join(label)] = val
                cell_defs['labels'].append('_'.join(label))
                if len(name):
                    prop_alias['_'.join(label_name)] = '_'.join(label)
                    cell_defs['aliases'].append(
                        {'_'.join(label_name): '_'.join(label)})

                if index == 0:
                    prop_alias['_'.join(label[:-1])] = '_'.join(label)
                    cell_defs['aliases'].append(
                        {'_'.join(label[:-1]): '_'.join(label)})


    prop_struct['data'].append(cell_data)
    prop_struct['members'].append(cell_members)
    prop_struct['defs'].append(cell_defs)
    insert_defs(node_address, prop_def, prop_alias)
    insert_structs(node_address, cell_yaml.get('cell_string'), prop_struct)

    # recurse if we have anything left
    if len(props):
        extract_cells(node_address, yaml, y_key, names,
                      index + 1, prefix, def_label)

    return


def extract_pinctrl(node_address, yaml, pinconf, names, index,
                    def_label):

    prop_list = []
    name_list = []
    if not isinstance(pinconf, list):
        prop_list.append(pinconf)
        name_list.append(names)
    else:
        prop_list = list(pinconf)
        name_list = list(names)

    def_prefix = def_label.split('_')
    target_node = node_address

    prop_def = {}
    prop_struct = {'data': [], 'defs': [], 'members': [], 'instance': []}
    for i in range(0, len(prop_list)):
        p = prop_list[i]
        name = name_list[i]
        pin_node_address = phandles[p]
        pin_entry = reduced[pin_node_address]
        parent_address = '/'.join(pin_node_address.split('/')[:-1])
        pin_subnode = '/'.join(pin_node_address.split('/')[-1:])
        pin_parent = reduced[parent_address]
        cell_yaml = yaml[get_compat(pin_parent)]
        cell_prefix = cell_yaml.get('cell_string', None)
        post_fix = []

        cell_data = []
        cell_members = []
        cell_defs = {'labels': [], 'aliases': []}
        if cell_prefix is not None:
            post_fix.append(cell_prefix)

        for subnode in reduced.keys():
            if pin_subnode in subnode and pin_node_address != subnode:
                # found a subnode underneath the pinmux handle
                pin_label = def_prefix + post_fix + subnode.split('/')[-2:]

                for i, cells in enumerate(reduced[subnode]['props']):
                    if len(cell_yaml['#cells']) == 2:
                        key_label = list(pin_label) + \
                            [cell_yaml['#cells'][0]] + [str(i)]
                        func_label = key_label[:-2] + \
                            [cell_yaml['#cells'][1]] + [str(i)]
                        key_label = convert_string_to_label(
                            '_'.join(key_label)).upper()
                        func_label = convert_string_to_label(
                            '_'.join(func_label)).upper()

                        prop_def[key_label] = reduced[subnode]['props'][cells]
                        prop_def[func_label] = \
                            reduced[subnode]['props'][cell_yaml['#cells'][1]]
                        cell_defs['labels'].append(key_label)
                        cell_defs['labels'].append(func_label)

                    elif len(cell_yaml['#cells']) == 1:
                        key_label = list(pin_label) + \
                            [cell_yaml['#cells'][0]] + [cells]
                        key_label = convert_string_to_label(
                            '_'.join(key_label)).upper()

                        prop_def[key_label] = reduced[subnode]['props'][cells]
                        cell_defs['labels'].append(key_label)

                if len(cell_yaml['#cells']) == 2:
                    pin_list = []
                    pin_list = \
                        reduced[subnode]['props'].get(cell_yaml['#cells'][0])
                    cell_members.append(name)
                    for i in pin_list:
                        pin_data = []
                        pin_data.append(i)
                        pin_data.append(
                            reduced[subnode]['props'][cell_yaml['#cells'][1]])
                        cell_data.append(pin_data)

                elif len(cell_yaml['#cells']) == 1:
                    cell_members.append(name)
                    for cells in reduced[subnode]['props'].keys():
                        cell_data.append(reduced[subnode]['props'][cells])

        prop_struct['data'].append(cell_data)
        prop_struct['members'].append(cell_members)
        prop_struct['defs'].append(cell_defs)

    if 'label' in pin_parent['props'].keys():
        prop_struct['instance'].append(pin_parent['props']['label'])

    insert_defs(node_address, prop_def, {})
    insert_structs(node_address, 'pinctrl', prop_struct)


def extract_single(node_address, yaml, prop, key, prefix, def_label):

    prop_def = {}
    prop_struct = {'data': [], 'defs': [], 'members': []}
    prop_data = []
    prop_defs = {'labels': [], 'aliases': []}

    if isinstance(prop, list):
        for i, p in enumerate(prop):
            k = convert_string_to_label(key).upper()
            label = def_label + '_' + k
            if isinstance(p, str):
                p = "\"" + p + "\""
            prop_def[label + '_' + str(i)] = p
            prop_data.append(p)
            prop_defs['labels'].append(label + '_' + str(i))
        prop_struct['data'].append(prop_data)
        prop_struct['defs'].append(prop_defs)
        insert_structs(node_address, key, prop_struct)
    else:
        k = convert_string_to_label(key).upper()
        label = def_label + '_' + k
        if isinstance(prop, str):
            prop = "\"" + prop + "\""
        prop_def[label] = prop
        prop_struct['data'].append([prop])
        prop_struct['defs'].append([label])
        insert_structs(node_address, key, prop_struct)

    if node_address in defs:
        defs[node_address].update(prop_def)
    else:
        defs[node_address] = prop_def

    return


def extract_string_prop(node_address, yaml, key, label, defs):

    prop_def = {}

    node = reduced[node_address]
    prop = node['props'][key]

    k = convert_string_to_label(key).upper()
    prop_def[label] = "\"" + prop + "\""

    if node_address in defs:
        defs[node_address].update(prop_def)
    else:
        defs[node_address] = prop_def

    return


def extract_property(node_compat, yaml, node_address, y_key, y_val, names,
                     prefix, defs, label_override):

    if 'base_label' in yaml[node_compat]:
        def_label = yaml[node_compat].get('base_label')
    else:
        def_label = convert_string_to_label(node_compat.upper())
        if '@' in node_address:
            def_label += '_' + node_address.split('@')[-1].upper()
        else:
            def_label += convert_string_to_label(node_address.upper())

    if label_override is not None:
        def_label += '_' + label_override

    if y_key == 'reg':
        extract_reg_prop(node_address, names, defs, def_label,
                         1, y_val.get('label', None))
    elif y_key == 'interrupts' or y_key == 'interupts-extended':
        extract_interrupts(node_address, yaml, y_key, names, def_label)
    elif 'pinctrl-' in y_key:
        p_index = int(y_key.split('-')[1])
        extract_pinctrl(node_address, yaml,
                        reduced[node_address]['props'][y_key],
                        names[p_index], p_index, def_label)
    elif 'clocks' in y_key:
        extract_cells(node_address, yaml, y_key,
                      names, 0, prefix, def_label)
    else:
        extract_single(node_address, yaml,
                       reduced[node_address]['props'][y_key], y_key,
                       prefix, def_label)

    return


def extract_node_include_info(reduced, root_node_address, sub_node_address,
                              yaml, defs, structs, y_sub):
    node = reduced[sub_node_address]
    node_compat = get_compat(reduced[root_node_address])
    label_override = None

    if node_compat not in yaml.keys():
        return {}, {}

    if y_sub is None:
        y_node = yaml[node_compat]
    else:
        y_node = y_sub

    if yaml[node_compat].get('use-property-label', False):
        try:
            label = y_node['properties']['label']
            label_override = convert_string_to_label(
                                    node['props']['label']).upper()
        except KeyError:
            pass

    # check to see if we need to process the properties
    for k, v in y_node['properties'].items():
            if 'properties' in v:
                for c in reduced:
                    if root_node_address + '/' in c:
                        extract_node_include_info(
                            reduced, root_node_address, c, yaml, defs, structs,
                            v)
            if 'generation' in v:

                prefix = []
                if v.get('use-name-prefix') is not None:
                    prefix = [convert_string_to_label(k.upper())]

                for c in node['props'].keys():
                    if c.endswith("-names"):
                        pass

                    if re.match(k + '$', c):

                        if 'pinctrl-' in c:
                            names = node['props'].get('pinctrl-names', [])
                        else:
                            names = node['props'].get(c[:-1] + '-names', [])
                            if not names:
                                names = node['props'].get(c + '-names', [])

                        if not isinstance(names, list):
                            names = [names]

                        extract_property(
                            node_compat, yaml, sub_node_address, c, v, names,
                            prefix, defs, label_override)

    return

def dict_merge(dct, merge_dct):
    # from https://gist.github.com/angstwad/bf22d1822c38a92ec0a9

    """ Recursive dict merge. Inspired by :meth:``dict.update()``, instead of
    updating only top-level keys, dict_merge recurses down into dicts nested
    to an arbitrary depth, updating keys. The ``merge_dct`` is merged into
    ``dct``.
    :param dct: dict onto which the merge is executed
    :param merge_dct: dct merged into dct
    :return: None
    """
    for k, v in merge_dct.items():
        if (k in dct and isinstance(dct[k], dict)
                and isinstance(merge_dct[k], collections.Mapping)):
            dict_merge(dct[k], merge_dct[k])
        else:
            dct[k] = merge_dct[k]


def yaml_traverse_inherited(node):
    """ Recursive overload procedure inside ``node``
    ``inherits`` section is searched for and used as node base when found.
    Base values are then overloaded by node values
    Additionally, 'id' key of 'inherited' dict is converted to 'gen_type'
    :param node:
    :return: node
    """

    if 'inherits' in node.keys():
        if 'id' in node['inherits'].keys():
            node['inherits']['gen_type'] = node['inherits']['id']
            node['inherits'].pop('id')
        try:
            yaml_traverse_inherited(node['inherits']['inherits'])
        except KeyError:
            dict_merge(node['inherits'], node)
            node = node['inherits']
            node.pop('inherits')
        except TypeError:
            #'node['inherits']['inherits'] type is 'list' instead of
            #expected type 'dtc'
            #Likely due to use of "-" before attribute in yaml file
            raise Exception("Element '" + str(node['title']) +
                            "' uses yaml 'series' instead of 'mapping'")
    return node


def yaml_collapse(yaml_list):

    collapsed = dict(yaml_list)

    for k, v in collapsed.items():
        v = yaml_traverse_inherited(v)
        collapsed[k]=v

    return collapsed


def print_key_value(k, v, tabstop):
    label = "#define " + k

    # calculate the name's tabs
    if len(label) % 8:
        tabs = (len(label) + 7) >> 3
    else:
        tabs = (len(label) >> 3) + 1

    sys.stdout.write(label)
    for i in range(0, tabstop - tabs + 1):
        sys.stdout.write('\t')
    sys.stdout.write(str(v))
    sys.stdout.write("\n")

    return


def generate_keyvalue_file(args):

    node_keys = sorted(defs.keys())
    for node in node_keys:
        sys.stdout.write('# ' + node.split('/')[-1])
        sys.stdout.write("\n")

        prop_keys = sorted(defs[node].keys())
        for prop in prop_keys:
            if prop == 'aliases':
                for entry in sorted(defs[node][prop]):
                    a = defs[node][prop].get(entry)
                    sys.stdout.write("%s=%s\n" % (entry, defs[node].get(a)))
            else:
                sys.stdout.write("%s=%s\n" % (prop, defs[node].get(prop)))

        sys.stdout.write("\n")


def generate_include_file(args):
    compatible = reduced['/']['props']['compatible'][0]

    sys.stdout.write("/**************************************************\n")
    sys.stdout.write(" * Generated include file for " + compatible)
    sys.stdout.write("\n")
    sys.stdout.write(" *               DO NOT MODIFY\n")
    sys.stdout.write(" */\n")
    sys.stdout.write("\n")
    sys.stdout.write("#ifndef _DEVICE_TREE_BOARD_H" + "\n")
    sys.stdout.write("#define _DEVICE_TREE_BOARD_H" + "\n")
    sys.stdout.write("\n")

    node_keys = sorted(defs.keys())
    for node in node_keys:
        sys.stdout.write('/* ' + node.split('/')[-1] + ' */')
        sys.stdout.write("\n")

        max_dict_key = lambda d: max(len(k) for k in d.keys())
        maxlength = 0
        if defs[node].get('aliases'):
            maxlength = max_dict_key(defs[node]['aliases'])
        maxlength = max(maxlength, max_dict_key(defs[node])) + len('#define ')

        if maxlength % 8:
            maxtabstop = (maxlength + 7) >> 3
        else:
            maxtabstop = (maxlength >> 3) + 1

        if (maxtabstop * 8 - maxlength) <= 2:
            maxtabstop += 1

        prop_keys = sorted(defs[node].keys())
        for prop in prop_keys:
            if prop == 'aliases':
                for entry in sorted(defs[node][prop]):
                    a = defs[node][prop].get(entry)
                    print_key_value(entry, a, maxtabstop)
            else:
                print_key_value(prop, defs[node].get(prop), maxtabstop)

        sys.stdout.write("\n")

    if args.fixup:
        for fixup in args.fixup:
            if os.path.exists(fixup):
                sys.stdout.write("\n")
                sys.stdout.write(
                    "/* Following definitions fixup the generated include */\n")
                try:
                    with open(fixup, "r") as fd:
                        for line in fd.readlines():
                            sys.stdout.write(line)
                        sys.stdout.write("\n")
                except:
                    raise Exception(
                        "Input file " + os.path.abspath(fixup) +
                        " does not exist.")

    sys.stdout.write("#endif\n")


def lookup_defs(defs, node, key):
    if node not in defs:
        return None

    if key in defs[node]['aliases']:
        key = defs[node]['aliases'][key]

    return defs[node].get(key, None)


def print_pinctrl_init_code(node_instances, node, yaml_list, instance=""):
    node_yaml_props = yaml_list[node]['properties']

    # local variables
    node_compat = convert_string_to_label(node)

    label_string = ""

    if instance != "":
        label_string = "_" + instance

    write_node_file("static const struct" + ' pin_config ' + node_compat +
                    "_pinconf" + label_string + " [] = {\n")

    for node in structs.keys():
        if 'pinctrl' in structs[node].keys():
            node_pinctrl = structs[node]['pinctrl']
            if len(node_pinctrl['instance']) > 0:
                if str(node_pinctrl['instance'][0]).strip("'") != \
                        instance:
                    continue

            for config in range(0, len(node_pinctrl['data'])):
                if 'default' in str(node_pinctrl['members'][config]):
                    for pin in range(0, len(node_pinctrl['data'][config])):
                        write_node_file("\t{")
                        write_node_file(
                            str(node_pinctrl['data'][config][pin])[1:-1])
                        write_node_file("},\n")

    write_node_file("};\n\n")

    return


def write_node_file(str):
    global file

    if file != "":
        file.write(str)
    else:
        raise Exception("Output file does not exist.")


def generate_structs_file(args, yaml_list):
    global file
    compatible = reduced['/']['props']['compatible'][0]

    if not args.structs:
        print('Usage: %s -d filename.dts -y path_to_yaml -s $(objtree)' %
              sys.argv[0])
        return 1

    # print driver code init
    for node in struct_dict:

        #Look for node with 'pinmux' type
        try:
            if yaml_list[node]['gen_type'] != 'pinmux':
                continue
        except KeyError:
                continue

        outdir_path = str(args.structs)
        node_file_path = str(outdir_path)
        node_file = node_file_path + '/' + convert_string_to_label(
            node) + '_init.h'

        if not os.path.exists(os.path.dirname(node_file)):
            try:
                os.makedirs(os.path.dirname(node_file))
            except:
                raise Exception(
                    "Could not find path: " + str(node_file_path))

        try:
            file = open(node_file, 'w')
        except:
            raise Exception("Could not open file: " + node_file)

        write_node_file("/**************************************************\n")
        write_node_file(" * Generated include file for " + node)
        write_node_file("\n")
        write_node_file(" *               DO NOT MODIFY\n");
        write_node_file(" */\n")
        write_node_file("\n")
        write_node_file("#ifndef " + str(
            convert_string_to_label(node) + '_init').upper() + "_H" + "\n");
        write_node_file("#define _" + str(
            convert_string_to_label(node) + '_init').upper() + "_H" + "\n");
        write_node_file("\n")

        # check if multiple instances of pinmux/pinctrl IP
        if len(struct_dict[node]) > 1:
            i = 0
            pinmux_table = []

            for instance in (struct_dict[node]):

                # build the array of used pinmuxes
                for node_1 in structs.keys():
                    #For each node in structs, check it has a pinctrl prop
                    if 'pinctrl' in structs[node_1].keys():
                        pinmux_label = str(
                            structs[node_1]['pinctrl']['instance'][
                                0]).strip("'")
                        #check if pinctrl instance is same as current one
                        # if pinmux_label == str(
                        #         struct_dict[node][i]['label']['data'][0][
                        #             0]).strip('"'):
                        if pinmux_label not in pinmux_table:
                            pinmux_table.append(pinmux_label)
                i = i + 1

            # write pinmux array for this instance
            for line in range(0, len(pinmux_table)):
                print_pinctrl_init_code(struct_dict[node], node, yaml_list,
                                        pinmux_table[line])

            # write the array of used pinmuxes
            write_node_file(
                "static struct instances_pinconfig " + str(convert_string_to_label(
                    node)) + "_instances" + " [] = {")

            for line in range(0, len(pinmux_table)):
                write_node_file("\n\t{\"" + str(pinmux_table[line]) + "\", ")
                write_node_file(
                    str(convert_string_to_label(node)) + "_pinconf_" + str(
                        pinmux_table[line]) + ", ")
                write_node_file("ARRAY_SIZE(" +
                    str(convert_string_to_label(node)) + "_pinconf_" + str(
                        pinmux_table[line]) + ")")
                write_node_file("},")
            write_node_file("\n};\n\n")

        else:
            # check if there is a label associated
            if 'label' in struct_dict[node][0]:
                pinmux_label = str(
                    struct_dict[node][0]['label']['data'][0][0]).strip('"')
                print_pinctrl_init_code(struct_dict[node], node, yaml_list,
                                        pinmux_label)
            else:
                print_pinctrl_init_code(struct_dict[node], node, yaml_list)

        write_node_file("#endif /* _" + str(
            convert_string_to_label(node) + '_init').upper() + "_H */" + "\n");
        node_file = ""
        file.close()


def generate_structs(args):
    # Generate structure information here
    #
    # structs structure is:
    # node_address:
    #              prop_0:
    #                     single value -or- list -or- list of dicts
    #              prop_1:
    #                     single value -or- list -or- list of dicts
    #              ...
    #              ...
    # single value: Just a single piece of data (int or string)
    # list: array of int or string
    # list of dicts: array of other structs
    #
    # skip items with None for compat.  These are 'special' (flash, etc)
    # need to run those through chosen node to see if they match and if
    # something should be done.
    #

    # iterate over the structs and reconfigure it to collate by compat
    for k, v in structs.items():
        compat = get_compat(reduced[k])
        if compat is None:
            continue
        if compat not in struct_dict:
            struct_dict[compat] = []
        struct_dict[compat].append(v)

    # now we can process it most efficiently
    # pprint.pprint(struct_dict)
    return


def parse_arguments():

    rdh = argparse.RawDescriptionHelpFormatter
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=rdh)

    parser.add_argument("-d", "--dts", help="DTS file")
    parser.add_argument("-y", "--yaml", help="YAML file")
    parser.add_argument("-s", "--structs", help="objdir path")
    parser.add_argument("-f", "--fixup", action="append",
                        help="Fixup file, we allow multiple")
    parser.add_argument("-k", "--keyvalue", action="store_true",
                        help="Generate include file for the build system")

    return parser.parse_args()


def main():
    args = parse_arguments()
    if not args.dts or not args.yaml:
        print('Usage: %s -d filename.dts -y path_to_yaml' % sys.argv[0])
        return 1

    try:
        with open(args.dts, "r") as fd:
            d = parse_file(fd)
    except:
        raise Exception(
            "Input file " + os.path.abspath(args.dts) + " does not exist.")

    # compress list to nodes w/ paths, add interrupt parent
    compress_nodes(d['/'], '/')

    # build up useful lists
    compatibles = get_all_compatibles(d['/'], '/', {})
    get_phandles(d['/'], '/', {})
    get_aliases(d['/'])
    get_chosen(d['/'])

    # find unique set of compatibles across all active nodes
    s = set()
    for k, v in compatibles.items():
        if isinstance(v, list):
            for item in v:
                s.add(item)
        else:
            s.add(v)

    # scan YAML files and find the ones we are interested in
    yaml_files = []
    for root, dirnames, filenames in os.walk(args.yaml):
        for filename in fnmatch.filter(filenames, '*.yaml'):
            yaml_files.append(os.path.join(root, filename))

    yaml_list = {}
    file_load_list = set()
    for file in yaml_files:
        for line in open(file, 'r'):
            if re.search('^\s+constraint:*', line):
                c = line.split(':')[1].strip()
                c = c.strip('"')
                if c in s:
                    if file not in file_load_list:
                        file_load_list.add(file)
                        with open(file, 'r') as yf:
                            yaml_list[c] = yaml.load(yf, Loader)

    if yaml_list == {}:
        raise Exception("Missing YAML information.  Check YAML sources")

    # collapse the yaml inherited information
    yaml_list = yaml_collapse(yaml_list)


    # load zephyr specific nodes
    flash = {}
    console = {}
    sram = {}
    if 'zephyr,flash' in chosen:
        flash = reduced[chosen['zephyr,flash']]
    if 'zephyr,console' in chosen:
        console = reduced[chosen['zephyr,console']]
    if 'zephyr,sram' in chosen:
        sram = reduced[chosen['zephyr,sram']]

    for k, v in reduced.items():
        node_compat = get_compat(v)
        if node_compat is not None and node_compat in yaml_list:
            extract_node_include_info(
                reduced, k, k, yaml_list, defs, structs, None)

    if defs == {}:
        raise Exception("No information parsed from dts file.")

    if 'zephyr,flash' in chosen:
        extract_reg_prop(chosen['zephyr,flash'], None,
                         defs, "CONFIG_FLASH", 1024, None)
    else:
        # We will add address/size of 0 for systems with no flash controller
        # This is what they already do in the Kconfig options anyway
        defs['dummy-flash'] = {'CONFIG_FLASH_BASE_ADDRESS': 0,
                               'CONFIG_FLASH_SIZE': 0}

    if 'zephyr,sram' in chosen:
        extract_reg_prop(chosen['zephyr,sram'], None,
                         defs, "CONFIG_SRAM", 1024, None)

    name_dict = {
            "CONFIG_UART_CONSOLE_ON_DEV_NAME": "zephyr,console",
            "CONFIG_BT_UART_ON_DEV_NAME": "zephyr,bt-uart",
            "CONFIG_UART_PIPE_ON_DEV_NAME": "zephyr,uart-pipe",
            "CONFIG_BT_MONITOR_ON_DEV_NAME": "zephyr,bt-mon-uart"
            }

    for k, v in name_dict.items():
        if v in chosen:
            extract_string_prop(chosen[v], None, "label", k, defs)

    # only compute the load offset if a code partition exists and it is not the
    # same as the flash base address
    load_defs = {}
    if 'zephyr,code-partition' in chosen and \
       'zephyr,flash' in chosen and \
       reduced[chosen['zephyr,flash']] is not \
            reduced[chosen['zephyr,code-partition']]:
        flash_base = lookup_defs(defs, chosen['zephyr,flash'],
                                 'CONFIG_FLASH_BASE_ADDRESS')
        part_defs = {}
        extract_reg_prop(chosen['zephyr,code-partition'], None, part_defs,
                         "PARTITION", 1, 'offset')
        part_base = lookup_defs(part_defs, chosen['zephyr,code-partition'],
                                'PARTITION_OFFSET')
        load_defs['CONFIG_FLASH_LOAD_OFFSET'] = part_base
        load_defs['CONFIG_FLASH_LOAD_SIZE'] = \
            lookup_defs(part_defs, chosen['zephyr,code-partition'],
                        'PARTITION_SIZE')
    else:
        load_defs['CONFIG_FLASH_LOAD_OFFSET'] = 0
        load_defs['CONFIG_FLASH_LOAD_SIZE'] = 0

    insert_defs(chosen['zephyr,flash'], load_defs, {})

    # generate include file
    if args.keyvalue:
        generate_keyvalue_file(args)
        generate_structs_file(args, yaml_list)
    elif args.structs:
        generate_structs(args)
        generate_include_file(args)
        generate_structs_file(args, yaml_list)
    else:
        generate_include_file(args)
        # generate_structs_file(args, yaml_list)

if __name__ == '__main__':
    main()

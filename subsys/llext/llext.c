/*
 * Copyright (c) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#include "zephyr/sys/__assert.h"
#include <zephyr/sys/util.h>
#include <zephyr/llext/elf.h>
#include <zephyr/llext/loader.h>
#include <zephyr/llext/llext.h>
#include <zephyr/llext/debug_str.h>
#include <zephyr/kernel.h>
#include <zephyr/cache.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(llext, CONFIG_LLEXT_LOG_LEVEL);

#include <string.h>

/** How an ELF symbol should be handled by the llext subsystem */
enum sym_op {
	SYM_OP_IGNORE,  /**< The symbol is not interesting */
	SYM_OP_DEFINE,  /**< The symbol is defined and may be used at link time */
	SYM_OP_RESOLVE, /**< The symbol is undefined and needs to be resolved */
};

#ifdef CONFIG_LLEXT_DEBUG_STRINGS

/* Display information about a symbol in a human-readable format */
#define LOG_ELF_SYM(ldr, ext, hdr, sym)						\
	LOG_DBG(hdr " %-7s %-7s in %-8s \"%s\": 0x%x",				\
		elf_st_bind_str(ELF_ST_BIND((sym)->st_info)),			\
		elf_st_type_str(ELF_ST_TYPE((sym)->st_info)),			\
		elf_sect_str(ldr, (sym)->st_shndx),				\
		llext_string(ldr, ext, LLEXT_MEM_STRTAB, (sym)->st_name),	\
		(sym)->st_value)

#else

/* Display numeric versions of the symbol information */
#define LOG_ELF_SYM(ldr, ext, hdr, sym)						\
	LOG_DBG(hdr " bind %d type %d shndx %d \"%s\": 0x%x",			\
		ELF_ST_BIND((sym)->st_info),					\
		ELF_ST_TYPE((sym)->st_info),					\
		(sym)->st_shndx,						\
		llext_string(ldr, ext, LLEXT_MEM_STRTAB, (sym)->st_name),	\
		(sym)->st_value)

#endif

#define LOG_LLEXT_SYM(ldr, ext, hdr, sym)					\
		LOG_DBG(hdr " llext_sym \"%s\" at %p",			        \
		(sym)->name,							\
		(sym)->addr);

K_HEAP_DEFINE(llext_heap, CONFIG_LLEXT_HEAP_SIZE * 1024);

static const char ELF_MAGIC[] = {0x7f, 'E', 'L', 'F'};

static sys_slist_t _llext_list = SYS_SLIST_STATIC_INIT(&_llext_list);

static struct k_mutex llext_lock = Z_MUTEX_INITIALIZER(llext_lock);

ssize_t llext_find_section(struct llext_loader *ldr, const char *search_name)
{
	elf_shdr_t *shdr;
	unsigned int i;
	size_t pos;

	for (i = 0, pos = ldr->hdr.e_shoff;
	     i < ldr->hdr.e_shnum;
	     i++, pos += ldr->hdr.e_shentsize) {
		shdr = llext_peek(ldr, pos);
		if (!shdr) {
			/* The peek() method isn't supported */
			return -EOPNOTSUPP;
		}

		const char *name = llext_peek(ldr,
					      ldr->sects[LLEXT_MEM_SHSTRTAB].sh_offset +
					      shdr->sh_name);

		if (!strcmp(name, search_name)) {
			return shdr->sh_offset;
		}
	}

	return -ENOENT;
}

/*
 * Note, that while we protect the global llext list while searching, we release
 * the lock before returning the found extension to the caller. Therefore it's
 * a responsibility of the caller to protect against races with a freeing
 * context when calling this function.
 */
struct llext *llext_by_name(const char *name)
{
	k_mutex_lock(&llext_lock, K_FOREVER);

	for (sys_snode_t *node = sys_slist_peek_head(&_llext_list);
	     node != NULL;
	     node = sys_slist_peek_next(node)) {
		struct llext *ext = CONTAINER_OF(node, struct llext, _llext_list);

		if (strncmp(ext->name, name, sizeof(ext->name)) == 0) {
			k_mutex_unlock(&llext_lock);
			return ext;
		}
	}

	k_mutex_unlock(&llext_lock);
	return NULL;
}

int llext_iterate(int (*fn)(struct llext *ext, void *arg), void *arg)
{
	sys_snode_t *node;
	unsigned int i;
	int ret = 0;

	k_mutex_lock(&llext_lock, K_FOREVER);

	for (node = sys_slist_peek_head(&_llext_list), i = 0;
	     node;
	     node = sys_slist_peek_next(node), i++) {
		struct llext *ext = CONTAINER_OF(node, struct llext, _llext_list);

		ret = fn(ext, arg);
		if (ret) {
			break;
		}
	}

	k_mutex_unlock(&llext_lock);
	return ret;
}

const void * const llext_find_sym(const struct llext_symtable *sym_table, const char *sym_name)
{
	if (sym_table == NULL) {
		/* Built-in symbol table */
		STRUCT_SECTION_FOREACH(llext_const_symbol, sym) {
			if (strcmp(sym->name, sym_name) == 0) {
				return sym->addr;
			}
		}
	} else {
		/* find symbols in module */
		for (size_t i = 0; i < sym_table->sym_cnt; i++) {
			if (strcmp(sym_table->syms[i].name, sym_name) == 0) {
				return sym_table->syms[i].addr;
			}
		}
	}

	return NULL;
}

/*
 * Find all relevant string and symbol tables
 */
static int llext_find_tables(struct llext_loader *ldr)
{
	int sect_cnt, i, ret;
	size_t pos;
	elf_shdr_t shdr;

	/* Find symbol and string tables */
	for (i = 0, sect_cnt = 0, pos = ldr->hdr.e_shoff;
	     i < ldr->hdr.e_shnum && sect_cnt < 3;
	     i++, pos += ldr->hdr.e_shentsize) {
		ret = llext_seek(ldr, pos);
		if (ret != 0) {
			LOG_ERR("failed seeking to position %u\n", pos);
			return ret;
		}

		ret = llext_read(ldr, &shdr, sizeof(elf_shdr_t));
		if (ret != 0) {
			LOG_ERR("failed reading section header at position %u\n", pos);
			return ret;
		}

		LOG_DBG("section %d at %x: name %d, type %d, flags %x, addr %x, size %d",
			i,
			ldr->hdr.e_shoff + i * ldr->hdr.e_shentsize,
			shdr.sh_name,
			shdr.sh_type,
			shdr.sh_flags,
			shdr.sh_addr,
			shdr.sh_size);

		switch (shdr.sh_type) {
		case SHT_SYMTAB:
		case SHT_DYNSYM:
			LOG_DBG(">>> found symtab at %d", i);
			ldr->sects[LLEXT_MEM_SYMTAB] = shdr;
			ldr->sect_map[i] = LLEXT_MEM_SYMTAB;
			sect_cnt++;
			break;
		case SHT_STRTAB:
			if (ldr->hdr.e_shstrndx == i) {
				LOG_DBG(">>> found shstrtab at %d", i);
				ldr->sects[LLEXT_MEM_SHSTRTAB] = shdr;
				ldr->sect_map[i] = LLEXT_MEM_SHSTRTAB;
			} else {
				LOG_DBG(">>> found strtab at %d", i);
				ldr->sects[LLEXT_MEM_STRTAB] = shdr;
				ldr->sect_map[i] = LLEXT_MEM_STRTAB;
			}
			sect_cnt++;
			break;
		default:
			break;
		}
	}

	if (!ldr->sects[LLEXT_MEM_SHSTRTAB].sh_type ||
	    !ldr->sects[LLEXT_MEM_STRTAB].sh_type ||
	    !ldr->sects[LLEXT_MEM_SYMTAB].sh_type) {
		LOG_ERR("Some sections are missing or present multiple times!");
		return -ENOENT;
	}

	return 0;
}

static const char *llext_string(struct llext_loader *ldr, struct llext *ext,
				enum llext_mem mem_idx, unsigned int idx)
{
	return (char *)ext->mem[mem_idx] + idx;
}

/*
 * Maps the section indexes and copies special section headers for easier use
 */
static int llext_map_sections(struct llext_loader *ldr, struct llext *ext)
{
	int i, ret;
	size_t pos;
	elf_shdr_t shdr;
	const char *name;

	for (i = 0, pos = ldr->hdr.e_shoff;
	     i < ldr->hdr.e_shnum;
	     i++, pos += ldr->hdr.e_shentsize) {
		ret = llext_seek(ldr, pos);
		if (ret != 0) {
			return ret;
		}

		ret = llext_read(ldr, &shdr, sizeof(elf_shdr_t));
		if (ret != 0) {
			return ret;
		}

		name = llext_string(ldr, ext, LLEXT_MEM_SHSTRTAB, shdr.sh_name);

		enum llext_mem mem_idx;

		if (strcmp(name, ".text") == 0) {
			mem_idx = LLEXT_MEM_TEXT;
		} else if (strcmp(name, ".data") == 0) {
			mem_idx = LLEXT_MEM_DATA;
		} else if (strcmp(name, ".rodata") == 0) {
			mem_idx = LLEXT_MEM_RODATA;
		} else if (strcmp(name, ".bss") == 0) {
			mem_idx = LLEXT_MEM_BSS;
		} else if (strcmp(name, ".exported_sym") == 0) {
			mem_idx = LLEXT_MEM_EXPORT;
		} else {
			/* ignore other sections */
			continue;
		}

		ldr->sects[mem_idx] = shdr;
		ldr->sect_map[i] = mem_idx;
	}

	return 0;
}

static int llext_copy_section(struct llext_loader *ldr, struct llext *ext,
			      enum llext_mem mem_idx)
{
	int ret;

	if (!ldr->sects[mem_idx].sh_size) {
		return 0;
	}
	ext->mem_size[mem_idx] = ldr->sects[mem_idx].sh_size;

	if (ldr->sects[mem_idx].sh_type != SHT_NOBITS &&
	    IS_ENABLED(CONFIG_LLEXT_STORAGE_WRITABLE)) {
		ext->mem[mem_idx] = llext_peek(ldr, ldr->sects[mem_idx].sh_offset);
		if (ext->mem[mem_idx]) {
			ext->mem_on_heap[mem_idx] = false;
			return 0;
		}
	}

	ext->mem[mem_idx] = k_heap_aligned_alloc(&llext_heap, sizeof(uintptr_t),
						 ldr->sects[mem_idx].sh_size,
						 K_NO_WAIT);
	if (!ext->mem[mem_idx]) {
		return -ENOMEM;
	}
	ext->alloc_size += ldr->sects[mem_idx].sh_size;

	if (ldr->sects[mem_idx].sh_type == SHT_NOBITS) {
		memset(ext->mem[mem_idx], 0, ldr->sects[mem_idx].sh_size);
	} else {
		ret = llext_seek(ldr, ldr->sects[mem_idx].sh_offset);
		if (ret != 0) {
			goto err;
		}

		ret = llext_read(ldr, ext->mem[mem_idx], ldr->sects[mem_idx].sh_size);
		if (ret != 0) {
			goto err;
		}
	}

	ext->mem_on_heap[mem_idx] = true;

	return 0;

err:
	k_heap_free(&llext_heap, ext->mem[mem_idx]);
	return ret;
}

static int llext_copy_strings(struct llext_loader *ldr, struct llext *ext)
{
	int ret = llext_copy_section(ldr, ext, LLEXT_MEM_SHSTRTAB);

	if (!ret) {
		ret = llext_copy_section(ldr, ext, LLEXT_MEM_STRTAB);
	}

	return ret;
}

static int llext_copy_sections(struct llext_loader *ldr, struct llext *ext)
{
	for (enum llext_mem mem_idx = 0; mem_idx < LLEXT_MEM_COUNT; mem_idx++) {
		/* strings have already been copied */
		if (ext->mem[mem_idx]) {
			continue;
		}

		int ret = llext_copy_section(ldr, ext, mem_idx);
		if (ret < 0) {
			return ret;
		}
	}

	for (enum llext_mem mem_idx = 0; mem_idx < LLEXT_MEM_COUNT; mem_idx++) {
#ifdef CONFIG_LLEXT_DEBUG_STRINGS
		LOG_DBG("section %s %s at %p, len 0x%x",
			llext_mem_str(mem_idx),
#else
		LOG_DBG("section %d %s at %p, len 0x%x",
			mem_idx,
#endif
			ext->mem_on_heap[mem_idx] ? "copied" : "linked",
			ext->mem[mem_idx],
			ext->mem_size[mem_idx]);
	}
	return 0;
}

/* Determine how an ELF symbol should be handled in the llext subsystem */
static enum sym_op llext_elf_sym_operation(struct llext_loader *ldr, const elf_sym_t *sym)
{
	uint32_t stt = ELF_ST_TYPE(sym->st_info);
	uint32_t stb = ELF_ST_BIND(sym->st_info);
	uint32_t shndx = sym->st_shndx;
	enum llext_mem mem_idx = (shndx < ldr->sect_cnt) ? ldr->sect_map[shndx] : LLEXT_MEM_COUNT;

	if ((mem_idx == LLEXT_MEM_EXPORT) || (stt == STT_SECTION) || (stt == STT_FILE)) {
		/* either a reserved or exported symbol, no need to handle it */
		return SYM_OP_IGNORE;
	}
	if (shndx == SHN_UNDEF) {
		/* undefined symbol, we need to resolve it somehow */
		return SYM_OP_RESOLVE;
	}
	if (stb == STB_GLOBAL) {
		/* defined global symbol, may be used at link time */
		return SYM_OP_DEFINE;
	}

	/* anything else is not interesting */
	return SYM_OP_IGNORE;
}

static int llext_count_export_syms(struct llext_loader *ldr, struct llext *ext)
{
	const elf_sym_t *elf_syms = (const elf_sym_t *) ext->mem[LLEXT_MEM_SYMTAB];
	size_t syms_size = ldr->sects[LLEXT_MEM_SYMTAB].sh_size;
	int sym_cnt = syms_size / sizeof(elf_sym_t);

	__ASSERT(ldr->sects[LLEXT_MEM_SYMTAB].sh_entsize == sizeof(elf_sym_t),
		 "Symbol table entry size mismatch");

	LOG_DBG("symbol count %u", sym_cnt);

	/* skip index 0, a dummy entry */
	for (int i = 1; i < sym_cnt; ++i) {

		elf_sym_t sym = elf_syms[i];

		enum sym_op op = llext_elf_sym_operation(ldr, &sym);

		switch (op) {
		case SYM_OP_IGNORE:
			LOG_ELF_SYM(ldr, ext, " ignore", &sym);
			break;
		case SYM_OP_RESOLVE:
			LOG_ELF_SYM(ldr, ext, "RESOLVE", &sym);
			break;
		case SYM_OP_DEFINE:
			LOG_ELF_SYM(ldr, ext, " DEFINE", &sym);
			ext->sym_tab.sym_cnt++;
			break;
		}
	}

	return 0;
}

static int llext_allocate_symtab(struct llext_loader *ldr, struct llext *ext)
{
	struct llext_symtable *sym_tab = &ext->sym_tab;
	size_t syms_size = sym_tab->sym_cnt * sizeof(struct llext_symbol);

	sym_tab->syms = k_heap_alloc(&llext_heap, syms_size, K_NO_WAIT);
	if (!sym_tab->syms) {
		return -ENOMEM;
	}
	memset(sym_tab->syms, 0, syms_size);
	ext->alloc_size += syms_size;

	return 0;
}

static int llext_export_symbols(struct llext_loader *ldr, struct llext *ext)
{
	elf_shdr_t *shdr = ldr->sects + LLEXT_MEM_EXPORT;
	struct llext_symbol *sym;
	unsigned int i;

	if (shdr->sh_size < sizeof(struct llext_symbol)) {
		/* Not found, no symbols exported */
		return 0;
	}

	struct llext_symtable *exp_tab = &ext->exp_tab;

	exp_tab->sym_cnt = shdr->sh_size / sizeof(struct llext_symbol);
	exp_tab->syms = k_heap_alloc(&llext_heap, exp_tab->sym_cnt * sizeof(struct llext_symbol),
				     K_NO_WAIT);
	if (!exp_tab->syms) {
		return -ENOMEM;
	}

	for (i = 0, sym = ext->mem[LLEXT_MEM_EXPORT];
	     i < exp_tab->sym_cnt;
	     i++, sym++) {
		exp_tab->syms[i].name = sym->name;
		exp_tab->syms[i].addr = sym->addr;
		LOG_LLEXT_SYM(ldr, ext, "export", &exp_tab->syms[i]);
	}

	return 0;
}

static int llext_copy_symbols(struct llext_loader *ldr, struct llext *ext)
{
	const elf_sym_t *elf_syms = ext->mem[LLEXT_MEM_SYMTAB];
	size_t syms_size = ldr->sects[LLEXT_MEM_SYMTAB].sh_size;
	int sym_cnt = syms_size / sizeof(elf_sym_t);
	struct llext_symtable *sym_tab = &ext->sym_tab;
	int j = 0;

	__ASSERT(ldr->sects[LLEXT_MEM_SYMTAB].sh_entsize == sizeof(elf_sym_t),
		 "Symbol table entry size mismatch");

	/* skip index 0, a dummy entry */
	for (int i = 1; i < sym_cnt; ++i) {

		elf_sym_t sym = elf_syms[i];
		uint32_t sect = sym.st_shndx;
		const char *name = llext_string(ldr, ext, LLEXT_MEM_STRTAB, sym.st_name);
		enum llext_mem mem_idx = ldr->sect_map[sect];

		enum sym_op op = llext_elf_sym_operation(ldr, &sym);

		if (op != SYM_OP_DEFINE) {
			/* only interested in symbols defined by the extension */
			continue;
		}

		sym_tab->syms[j].name = name;
		sym_tab->syms[j].addr = (void *)((uintptr_t)ext->mem[mem_idx] +
						 sym.st_value -
						 (ldr->hdr.e_type == ET_REL ? 0 :
						  ldr->sects[mem_idx].sh_addr));
		LOG_LLEXT_SYM(ldr, ext, "define", &sym_tab->syms[j]);
		j++;
	}

	return 0;
}

/*
 * Find the section, containing the supplied offset and return file offset for
 * that value
 */
static size_t llext_file_offset(struct llext_loader *ldr, size_t offset)
{
	unsigned int i;

	for (i = 0; i < LLEXT_MEM_COUNT; i++)
		if (ldr->sects[i].sh_addr <= offset &&
		    ldr->sects[i].sh_addr + ldr->sects[i].sh_size > offset)
			return offset - ldr->sects[i].sh_addr + ldr->sects[i].sh_offset;

	return offset;
}

__weak void arch_elf_relocate_local(struct llext_loader *ldr, struct llext *ext,
				    elf_rela_t *rel, size_t got_offset)
{
	__ASSERT(false, "arch_elf_relocate_local() not implemented");
}

__weak void arch_elf_relocate(elf_rela_t *rel, uintptr_t opaddr, uintptr_t opval)
{
	__ASSERT(false, "arch_elf_relocate() not implemented");
}

static int llext_link(struct llext_loader *ldr, struct llext *ext, bool do_local)
{
	uintptr_t loc = 0;
	enum llext_mem target_mem;
	elf_shdr_t shdr;
	elf_rela_t rel;
	elf_sym_t sym;
	elf_word rel_cnt = 0;
	int i, ret;
	size_t pos;

	const elf_sym_t *elf_syms = ext->mem[LLEXT_MEM_SYMTAB];
	size_t syms_size = ldr->sects[LLEXT_MEM_SYMTAB].sh_size;
	int sym_cnt = syms_size / sizeof(elf_sym_t);

	for (i = 0, pos = ldr->hdr.e_shoff;
	     i < ldr->hdr.e_shnum - 1;
	     i++, pos += ldr->hdr.e_shentsize) {
		ret = llext_seek(ldr, pos);
		if (ret != 0) {
			return ret;
		}

		ret = llext_read(ldr, &shdr, sizeof(elf_shdr_t));
		if (ret != 0) {
			return ret;
		}

		/* find relocation sections */
		if (shdr.sh_type != SHT_REL && shdr.sh_type != SHT_RELA) {
			continue;
		}

		rel_cnt = shdr.sh_size / shdr.sh_entsize;

		const char *name = llext_string(ldr, ext, LLEXT_MEM_SHSTRTAB, shdr.sh_name);

		int is_plt_section = 0;

		if (strcmp(name, ".rela.plt") == 0 ||
		   strcmp(name, ".rela.dyn") == 0) {
			is_plt_section = 1;
		} else {
			/* this is used for static linking only at this stage */
			target_mem = ldr->sect_map[shdr.sh_info];
			if ((!shdr.sh_info || (target_mem == LLEXT_MEM_COUNT))
			    && !is_plt_section) {
				LOG_WRN("Reloc section %d on unmapped section %d ignored",
					i, shdr.sh_info);
				continue;
			}

			loc = (uintptr_t)ext->mem[target_mem];
		}

#ifdef CONFIG_LLEXT_DEBUG_STRINGS
		LOG_DBG("relocation section %s (%d) for section %s has %d relocations",
			name, i, elf_sect_str(ldr, shdr.sh_info), rel_cnt);
#else
		LOG_DBG("relocation section %s (%d) for section %d has %d relocations",
			name, i, shdr.sh_info, rel_cnt);
#endif

		for (int j = 0; j < rel_cnt; j++) {
			/* get each relocation entry */
			ret = llext_seek(ldr, shdr.sh_offset + j * shdr.sh_entsize);
			if (ret != 0) {
				return ret;
			}

			ret = llext_read(ldr, &rel, shdr.sh_entsize);
			if (ret != 0) {
				return ret;
			}

			/* get corresponding symbol */
			if (ELF_R_SYM(rel.r_info) >= sym_cnt) {
				LOG_ERR("Relocation symbol index %d out of bounds",
					ELF_R_SYM(rel.r_info));
				return -EINVAL;
			}
			sym = elf_syms[ELF_R_SYM(rel.r_info)];

			name = llext_string(ldr, ext, LLEXT_MEM_STRTAB, sym.st_name);

#ifdef CONFIG_LLEXT_DEBUG_STRINGS
			LOG_DBG(">>> Relocation %d:%d type %s sym %d @ 0x%x:",
				i, j, arch_r_type_str(ELF_R_TYPE(rel.r_info)),
				ELF_R_SYM(rel.r_info), rel.r_offset);
#else
			LOG_DBG(">>> Relocation %d:%d type %d sym %d @ 0x%x:",
				i, j, ELF_R_TYPE(rel.r_info),
				ELF_R_SYM(rel.r_info), rel.r_offset);
#endif
			LOG_ELF_SYM(ldr, ext, "target", &sym);

			if (is_plt_section) {

				/* dynamic linking using GOT */

				size_t got_offset = llext_file_offset(ldr, rel.r_offset) -
					ldr->sects[LLEXT_MEM_TEXT].sh_offset;

				const void *link_addr;
				uint8_t *text = ext->mem[LLEXT_MEM_TEXT];
				uint32_t stb = ELF_ST_BIND(sym.st_info);

				switch (stb) {
				case STB_GLOBAL:
					link_addr = llext_find_sym(NULL, name);
					if (!link_addr)
						link_addr = llext_find_sym(&ext->sym_tab, name);

					if (!link_addr) {
						LOG_WRN("PLT: cannot find idx %u name %s", j, name);
						continue;
					}

					if (!rel.r_offset) {
						LOG_WRN("PLT: zero offset idx %u name %s", j, name);
						continue;
					}

					/* Resolve the symbol */
#ifdef CONFIG_LLEXT_DEBUG_STRINGS
					LOG_DBG("writing at %s a relocation to %s",
						llext_addr_str(ldr, ext,
							       (uintptr_t)(text + got_offset)),
						llext_addr_str(ldr, ext,
							       (uintptr_t)link_addr));
#else
					LOG_DBG("writing at %p a relocation to %p",
						text + got_offset, link_addr);
#endif

					*(const void **)(text + got_offset) = link_addr;
					break;
				case  STB_LOCAL:
					if (do_local) {
						arch_elf_relocate_local(ldr, ext, &rel, got_offset);
					}
				}

			} else {

				/* static linking */

				uintptr_t link_addr, op_loc;

				op_loc = loc + rel.r_offset;

				/* If symbol is undefined, then we need to look it up */
				if (sym.st_shndx == SHN_UNDEF) {
					link_addr = (uintptr_t)llext_find_sym(NULL, name);

					if (link_addr == 0) {
						LOG_ERR("Undefined symbol with no entry in "
							"symbol table %s, offset %d, "
							"link section %d",
							name, rel.r_offset, shdr.sh_link);
						return -ENODATA;
					}
				} else if (sym.st_shndx == SHN_ABS) {
					/* symbol is absolute */
					link_addr = sym.st_value + *((uintptr_t *)op_loc);
				} else {
					/* Current relocation location holds an offset into
					 * the section
					 */
					link_addr = (uintptr_t)ext->mem[ldr->sect_map[sym.st_shndx]]
						+ sym.st_value
						+ *((uintptr_t *)op_loc);
				}

#ifdef CONFIG_LLEXT_DEBUG_STRINGS
				LOG_DBG("writing at %s a relocation to %s",
					llext_addr_str(ldr, ext, op_loc),
					llext_addr_str(ldr, ext, link_addr));
#else
				LOG_DBG("writing at 0x%lx a relocation to 0x%lx",
					op_loc, link_addr);
#endif

				/* relocation */
				arch_elf_relocate(&rel, op_loc, link_addr);
			}
		}
	}

#ifdef CONFIG_CACHE_MANAGEMENT
	/* Make sure changes to ext sections are flushed to RAM */
	for (i = 0; i < LLEXT_MEM_COUNT; ++i) {
		if (ext->mem[i]) {
			sys_cache_data_flush_range(ext->mem[i], ext->mem_size[i]);
		}
	}
#endif

	return 0;
}

/*
 * Load a valid ELF as an extension
 */
static int do_llext_load(struct llext_loader *ldr, struct llext *ext,
			 struct llext_load_param *ldr_parm)
{
	int ret = 0;

	memset(ldr->sects, 0, sizeof(ldr->sects));
	ldr->sect_cnt = 0;
	ext->sym_tab.sym_cnt = 0;

	size_t sect_map_sz = ldr->hdr.e_shnum * sizeof(ldr->sect_map[0]);

	ldr->sect_map = k_heap_alloc(&llext_heap, sect_map_sz, K_NO_WAIT);
	if (!ldr->sect_map) {
		LOG_ERR("Failed to allocate memory for section map, size %u", sect_map_sz);
		ret = -ENOMEM;
		goto out;
	}
	for (int i = 0; i < ldr->hdr.e_shnum; i++) {
		ldr->sect_map[i] = LLEXT_MEM_COUNT; /* invalid */
	}

	ldr->sect_cnt = ldr->hdr.e_shnum;
	ext->alloc_size += sect_map_sz;

	LOG_DBG("Finding ELF tables...");
	ret = llext_find_tables(ldr);
	if (ret != 0) {
		LOG_ERR("Failed to find important ELF tables, ret %d", ret);
		goto out;
	}

	LOG_DBG("Allocate and copy strings...");
	ret = llext_copy_strings(ldr, ext);
	if (ret != 0) {
		LOG_ERR("Failed to copy ELF string sections, ret %d", ret);
		goto out;
	}

	LOG_DBG("Mapping ELF sections...");
	ret = llext_map_sections(ldr, ext);
	if (ret != 0) {
		LOG_ERR("Failed to map ELF sections, ret %d", ret);
		goto out;
	}

	LOG_DBG("Allocate and copy sections...");
	ret = llext_copy_sections(ldr, ext);
	if (ret != 0) {
		LOG_ERR("Failed to copy ELF sections, ret %d", ret);
		goto out;
	}

	LOG_DBG("Counting exported symbols...");
	ret = llext_count_export_syms(ldr, ext);
	if (ret != 0) {
		LOG_ERR("Failed to count exported ELF symbols, ret %d", ret);
		goto out;
	}

	LOG_DBG("Allocating memory for symbol table...");
	ret = llext_allocate_symtab(ldr, ext);
	if (ret != 0) {
		LOG_ERR("Failed to allocate extension symbol table, ret %d", ret);
		goto out;
	}

	LOG_DBG("Copying symbols...");
	ret = llext_copy_symbols(ldr, ext);
	if (ret != 0) {
		LOG_ERR("Failed to copy symbols, ret %d", ret);
		goto out;
	}

	LOG_DBG("Linking ELF...");
	ret = llext_link(ldr, ext, ldr_parm ? ldr_parm->relocate_local : true);
	if (ret != 0) {
		LOG_ERR("Failed to link, ret %d", ret);
		goto out;
	}

	LOG_DBG("Exporting symbols...");
	ret = llext_export_symbols(ldr, ext);
	if (ret != 0) {
		LOG_ERR("Failed to export, ret %d", ret);
		goto out;
	}

out:
	k_heap_free(&llext_heap, ldr->sect_map);

	if (ret != 0) {
		LOG_DBG("Failed to load extension, freeing memory...");
		for (enum llext_mem mem_idx = 0; mem_idx < LLEXT_MEM_COUNT; mem_idx++) {
			if (ext->mem_on_heap[mem_idx]) {
				k_heap_free(&llext_heap, ext->mem[mem_idx]);
			}
		}
		k_heap_free(&llext_heap, ext->exp_tab.syms);
	} else {
		LOG_DBG("loaded module, .text at %p, .rodata at %p", ext->mem[LLEXT_MEM_TEXT],
			ext->mem[LLEXT_MEM_RODATA]);
	}

	ext->sym_tab.sym_cnt = 0;
	k_heap_free(&llext_heap, ext->sym_tab.syms);
	ext->sym_tab.syms = NULL;

	return ret;
}

int llext_load(struct llext_loader *ldr, const char *name, struct llext **ext,
	       struct llext_load_param *ldr_parm)
{
	int ret;
	elf_ehdr_t ehdr;

	*ext = llext_by_name(name);

	k_mutex_lock(&llext_lock, K_FOREVER);

	if (*ext) {
		/* The use count is at least 1 */
		ret = (*ext)->use_count++;
		goto out;
	}

	ret = llext_seek(ldr, 0);
	if (ret != 0) {
		LOG_ERR("Failed to seek for ELF header");
		goto out;
	}

	ret = llext_read(ldr, &ehdr, sizeof(ehdr));
	if (ret != 0) {
		LOG_ERR("Failed to read ELF header");
		goto out;
	}

	/* check whether this is an valid elf file */
	if (memcmp(ehdr.e_ident, ELF_MAGIC, sizeof(ELF_MAGIC)) != 0) {
		LOG_HEXDUMP_ERR(ehdr.e_ident, 16, "Invalid ELF, magic does not match");
		ret = -EINVAL;
		goto out;
	}

	switch (ehdr.e_type) {
	case ET_REL:
	case ET_DYN:
		LOG_DBG("Loading relocatable or shared elf");
		*ext = k_heap_alloc(&llext_heap, sizeof(struct llext), K_NO_WAIT);
		if (*ext == NULL) {
			LOG_ERR("Not enough memory for extension metadata");
			ret = -ENOMEM;
			goto out;
		}
		memset(*ext, 0, sizeof(struct llext));

		ldr->hdr = ehdr;
		ret = do_llext_load(ldr, *ext, ldr_parm);
		if (ret < 0) {
			k_heap_free(&llext_heap, *ext);
			*ext = NULL;
			goto out;
		}

		strncpy((*ext)->name, name, sizeof((*ext)->name));
		(*ext)->name[sizeof((*ext)->name) - 1] = '\0';
		(*ext)->use_count++;

		sys_slist_append(&_llext_list, &(*ext)->_llext_list);
		LOG_INF("Loaded extension %s", (*ext)->name);

		break;
	default:
		LOG_ERR("Unsupported elf file type %x", ehdr.e_type);
		ret = -EINVAL;
	}

out:
	k_mutex_unlock(&llext_lock);
	return ret;
}

int llext_unload(struct llext **ext)
{
	__ASSERT(*ext, "Expected non-null extension");
	struct llext *tmp = *ext;

	k_mutex_lock(&llext_lock, K_FOREVER);
	__ASSERT(tmp->use_count, "A valid LLEXT cannot have a zero use-count!");

	if (tmp->use_count-- != 1) {
		unsigned int ret = tmp->use_count;

		k_mutex_unlock(&llext_lock);
		return ret;
	}

	/* FIXME: protect the global list */
	sys_slist_find_and_remove(&_llext_list, &tmp->_llext_list);

	*ext = NULL;
	k_mutex_unlock(&llext_lock);

	for (int i = 0; i < LLEXT_MEM_COUNT; i++) {
		if (tmp->mem_on_heap[i]) {
#ifdef CONFIG_LLEXT_DEBUG_STRINGS
			LOG_DBG("freeing memory region %s", llext_mem_str(i));
#else
			LOG_DBG("freeing memory region %d", i);
#endif

			k_heap_free(&llext_heap, tmp->mem[i]);
			tmp->mem[i] = NULL;
		}
	}

	k_heap_free(&llext_heap, tmp->exp_tab.syms);
	k_heap_free(&llext_heap, tmp);

	return 0;
}

int llext_call_fn(struct llext *ext, const char *sym_name)
{
	void (*fn)(void);

	fn = llext_find_sym(&ext->exp_tab, sym_name);
	if (fn == NULL) {
		return -EINVAL;
	}
	fn();

	return 0;
}

# Copyright (c) 2024 Basalte bv
#
# SPDX-License-Identifier: Apache-2.0

import argparse
from itertools import chain
import os
from pathlib import Path
import subprocess
import sys
import textwrap

from west.commands import WestCommand

from zephyr_ext_common import ZEPHYR_BASE

sys.path.append(os.fspath(Path(__file__).parent.parent))
import zephyr_module


class Pip(WestCommand):
    def __init__(self):
        super().__init__(
            "pip",
            "manage pip packages for Zephyr",
            "List and Install pip packages for Zephyr and modules",
            accepts_unknown_args=True,
        )

    def do_add_parser(self, parser_adder):
        parser = parser_adder.add_parser(
            self.name,
            help=self.help,
            description=self.description,
            formatter_class=argparse.RawDescriptionHelpFormatter,
            epilog=textwrap.dedent(
                """
            Listing pip packages:

                Run 'west pip' or 'west pip list' to list pip packages for Zephyr and all modules.
                See 'west pip list --help' for details.


            Installing pip packages:

                Run 'west pip install' to install pip packages for Zephyr and all modules.
                See 'west pip install --help' for details.
            """
            ),
        )

        subparsers_gen = parser.add_subparsers(
            metavar="<subcommand>",
            dest="subcommand",
            help="select a subcommand. If omitted, treat it as the 'list' selected.",
        )

        subparsers_gen.add_parser(
            "list",
            help="list pip packages",
            formatter_class=argparse.RawDescriptionHelpFormatter,
            epilog=textwrap.dedent(
                """
            Listing pip packages:

                Run 'west pip' or 'west pip list' to print all pip packages or requirements
                files needed by Zephyr and the active projects.

                The output is compatible with the requirements file format itself.
            """
            ),
        )

        subparsers_gen.add_parser(
            "install",
            help="install pip packages",
            formatter_class=argparse.RawDescriptionHelpFormatter,
            epilog=textwrap.dedent(
                """
            Installing pip packages:

                Run 'west pip install' to install all pip packages or requirements files
                needed by Zephyr and the active projects.

                A single 'pip install' command is built and executed, passing all arguments
                from the original 'west pip install' command. For example pass '--dry-run'
                to not actually install anything, but print what would be.
            """
            ),
        )

        return parser

    def do_run(self, args, user_args):
        packages = []
        requirements = [
            ZEPHYR_BASE / 'scripts/requirements.txt'
        ]

        for module in zephyr_module.parse_modules(ZEPHYR_BASE, self.manifest):
            pip = module.meta.get('package-managers', {}).get('pip')
            if pip is None:
                continue

            packages += pip.get('packages', [])
            requirements += [Path(module.project) / r for r in pip.get('requirements', [])]

        if args.subcommand == "list" or not args.subcommand:
            if len(user_args) > 0:
                self.die(f'west pip list does not support unknown arguments: "{user_args}"')

            self.inf('\n'.join(packages + [f'-r {r}' for r in requirements]))
            return

        # continue with installation
        subprocess.check_call([sys.executable,'-m', 'pip', 'install'] +
            packages +
            list(chain.from_iterable([('-r', r) for r in requirements])) +
            user_args
        )

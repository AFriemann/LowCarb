#! /usr/bin/env python3
# -*- coding: utf-8 -*-
"""
.. module:: TODO
   :platform: Unix
   :synopsis: TODO.

.. moduleauthor:: Aljosha Friemann aljosha.friemann@gmail.com

"""

import os, re

import click
from tabulate import tabulate

def find_sources(path):
    valid_extensions = ['.hpp', '.cpp', '.h', '.c']

    for root, dirs, files in os.walk(path):
        dirs[:] = [ d for d in dirs if d != '.git' ]
        files[:] = [ os.path.join(root, f) for f in files if os.path.splitext(f)[1] in valid_extensions ]

        if len(files) > 0:
            yield os.path.relpath(root), files

def count_todos(source):
    with open(source, 'r') as f:
        content = f.read()

    regex = r'(TODO|FIXME).*'

    return len(re.findall(regex, content, re.IGNORECASE))

CONTEXT_SETTINGS = dict(help_option_names=['-h', '--help'])

@click.command(context_settings=CONTEXT_SETTINGS)
@click.option('--output-format', default='fancy_grid', type=click.Choice(['orgtbl','plain','simple','grid','rst','latex','fancy_grid']))
@click.argument('src_dir', type=click.Path(exists=True, file_okay=False))
def cli(output_format, src_dir):
    """main"""
    total = 0

    directories = []

    for directory, sources in find_sources(src_dir):
        table = []

        directory_total = 0

        for source in sorted(sources):
            todos = count_todos(source)

            if todos > 0:
                table.append([os.path.basename(source), todos])
                directory_total += todos

        if directory_total > 0:
            print("\n%s:\n" % directory)
            print(tabulate(table, headers=['file', 'todos'],
                                  tablefmt=output_format,
                                  numalign="right"))

            directories.append([directory, directory_total])


        total += directory_total

    if total > 0:
        directories.append(['TOTAL', total])
        print("\ntotal:\n")
        print(tabulate(directories, headers=['directories', 'todos'],
                                    tablefmt=output_format,
                                    numalign="right"))
        exit(1)

def run():
    cli()

if __name__ == '__main__':
    run()

# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8

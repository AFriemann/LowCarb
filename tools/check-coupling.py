#! /usr/bin/env python3
# -*- coding: utf-8 -*-
"""
.. module:: TODO
   :platform: Unix
   :synopsis: TODO.

.. moduleauthor:: Aljosha Friemann <aljosha.friemann@gmail.com>

"""

import os, re

import click
from tabulate import tabulate

def float2percentage(f):
    max_length = 6 # 100.00
    f2str = str(round(f, 2))

    return f2str + (' '*(max_length - len(f2str))) + "%"

def find_sources(path):
    valid_extensions = ['.hpp', '.cpp', '.h', '.c']

    counter = 0
    directories = []

    for root, dirs, files in os.walk(path):
        dirs[:] = [ d for d in dirs if d != '.git' ]
        files[:] = [ os.path.join(root, f) for f in files if os.path.splitext(f)[1] in valid_extensions and not f.startswith("main.") ]

        for name in files:
            counter += 1

        if len(files) > 0:
            directories.append((os.path.relpath(root), files))

    return counter, directories

def count_included_local_files(path):
    with open(path, 'r') as f:
        content = f.read()

    counter = 0

    for match in re.finditer(r'#include ".*"', content):
        counter += 1

    return counter

CONTEXT_SETTINGS = dict(help_option_names=['-h', '--help'])

@click.command(context_settings=CONTEXT_SETTINGS)
@click.option('-t', '--threshold', type=float, default=13.37, help='coupling failure threshold')
@click.option('--ignore-main/--check-main', default=True, help='ignore/check main.cpp')
@click.option('--only-show-failures', default=False, is_flag=True, help='only print failed files')
@click.option('--output-format', default='fancy_grid', type=click.Choice(['orgtbl','plain','simple','grid','rst','latex','fancy_grid']))
@click.argument('src_dir', type=click.Path(exists=True, file_okay=False))
def cli(threshold, ignore_main, only_show_failures, output_format, src_dir):
    """
    check a source directory for coupling of local source files
    """
    print('checking local coupling with threshold of %s:' % threshold)

    source_count, directories = find_sources(src_dir)

    failures = 0

    for directory, source_files in directories:
        table = []

        for source in sorted(source_files):
            if ignore_main and source == os.path.join(src_dir, 'main.cpp'):
                continue

            coupling = (count_included_local_files(source) / source_count) * 100
            table.append([os.path.basename(source),
                          coupling])

            if coupling > threshold:
                failures += 1

        print('\n%s:\n' % directory)
        print(tabulate(table, headers=["file", "coupling"], tablefmt=output_format,
                                                            numalign="right",
                                                            floatfmt='.2f'))

    exit(failures)

def run():
    cli()

if __name__ == '__main__':
    run()

# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8

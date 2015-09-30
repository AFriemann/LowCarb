#!/bin/env python3
# -*- coding: utf-8 -*-
"""
.. moduleauthor:: Julian Bieber <schaf.xx@gmail.com>
"""

import os, logging
from lxml import html

import click
from tabulate import tabulate

CONTEXT_SETTINGS = dict(help_option_names=['-h', '--help'])

def parse_index_file(path):
    with open(path, 'r') as f:
        content = f.read()

    tree = html.fromstring(content)
    coverage_rows = tree.xpath('//center/table/tr')[2:]

    for row in coverage_rows:
        link = row.xpath('td[@class="coverFile"]/a')[0]

        name = link.xpath('text()')[0]
        index_file = link.xpath('@href')[0]

        line_percentage, function_percentage = [ e.rstrip('%') for e in row.xpath('td[starts-with(@class, "coverPer")]/text()') ]

        yield {'name': name,
               'index_file': index_file,
               'lines': float(line_percentage),
               'functions': float(function_percentage) if function_percentage != '-' else None
               }

@click.command(context_settings=CONTEXT_SETTINGS)
@click.option('-t', '--threshold', type=float, default=90.0, help='coupling failure threshold')
@click.option('--only-show-failures', default=False, is_flag=True, help='only print failed files')
@click.option('--output-format', default='fancy_grid', type=click.Choice(['orgtbl','plain','simple','grid','rst','latex','fancy_grid']))
@click.argument('coverage_dir', type=click.Path(exists=True, file_okay=False))
def cli(threshold, only_show_failures, output_format, coverage_dir):
    """
    python3.4 coverage.py index.html home/julian/LowCarb/src/index.html
    """
    failures = 0

    total_index_path = os.path.relpath(os.path.abspath('%s/index.html' % coverage_dir))

    directories = []

    for result in parse_index_file(total_index_path):
        directories.append([result['name'], result['lines'], result['functions']])

        if result['lines'] is None or result['lines'] < threshold:
            failures += 1
        print(result['lines'])
        directory_files = []

        for file_result in parse_index_file(os.path.join(coverage_dir, result['index_file'])):
            directory_files.append([file_result['name'], file_result['lines'], file_result['functions']])

        print('\n%s:\n' % result['name'])
        print(tabulate(directory_files, headers=['file', 'lines', 'functions'],
                                        tablefmt=output_format,
                                        numalign="right",
                                        floatfmt='.2f'))

    print('\ntotal directory coverage:\n')
    print(tabulate(directories, headers=['directory', 'lines', 'functions'],
                                tablefmt=output_format,
                                numalign="right",
                                floatfmt='.2f'))

    exit(failures)

if __name__ == "__main__":
    cli()

# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8

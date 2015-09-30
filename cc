#! /usr/bin/env python3
# -*- coding: utf-8 -*-
"""
.. module:: TODO
   :platform: Unix
   :synopsis: TODO.

.. moduleauthor:: Aljosha Friemann <aljosha.friemann@gmail.com>

"""

import os, shutil, subprocess, logging, datetime

import click
from tabulate import tabulate

log = logging.getLogger('cc')

PROJECT_ROOT = os.path.dirname(os.path.realpath(__file__))

TEST_RESOURCE_DIR = os.path.join(PROJECT_ROOT, 'test/resources')

BUILD_DIR = os.path.join(PROJECT_ROOT, 'build')
BIN_DIR   = os.path.join(PROJECT_ROOT, 'bin')
TOOL_DIR  = os.path.join(PROJECT_ROOT, 'tools')

TEST_RESULT_DIR = os.path.join(PROJECT_ROOT, 'test/results')
COVERAGE_DIR    = os.path.join(TEST_RESULT_DIR, 'coverage')

ERROR_COUNTER = 0
FAILED_CHECKS = []

def log_failure(name, msg):
    global FAILED_CHECKS
    global ERROR_COUNTER

    ERROR_COUNTER += 1
    FAILED_CHECKS.append([name, msg])

def require(func, *args, **kwargs):
    def inner():
        global ERROR_COUNTER
        try:
            log.info('running function %s' % func.__name__)
            if func(*args, **kwargs) != 0:
                raise RuntimeError("%s failed" % func.__name__)
        except RuntimeError as e:
            log_failure(func.__name__, e)
            raise e

    return inner()

def check(func, *args, **kwargs):
    def inner():
        global ERROR_COUNTER
        try:
            log.info('running function %s' % func.__name__)
            if func(*args, **kwargs) != 0:
                raise RuntimeError("%s failed" % func.__name__)
        except RuntimeError as e:
            log_failure(func.__name__, e)
        except KeyboardInterrupt:
            log.warning('skipping %s' % func.__name__)

    return inner()

def call(arglist, quiet=True):
    pipe = subprocess.PIPE if quiet else None
    proc = subprocess.Popen(arglist, shell=isinstance(arglist, str), stdout=pipe, stderr=pipe)
    proc.communicate()
    return proc.returncode

def remove_cmake_files(path):
    for root, dirs, files in os.walk(path):
        # dirs
        for d in ['CMakeFiles']:
            if d in dirs:
                path = os.path.join(root, d)
                shutil.rmtree(path)
                log.debug('%s removed' % path)

        # files
        for f in ['cmake_install.cmake', 'CMakeCache.txt']:
            if f in files:
                path = os.path.join(root, f)
                os.remove(path)
                log.debug('%s removed' % path)

def clean():
    """clean"""
    shutil.rmtree(BIN_DIR, ignore_errors=True)
    shutil.rmtree(BUILD_DIR, ignore_errors=True)
    shutil.rmtree(TEST_RESULT_DIR, ignore_errors=True)
    shutil.rmtree(COVERAGE_DIR, ignore_errors=True)

    remove_cmake_files(os.path.join(PROJECT_ROOT, "src"))
    remove_cmake_files(os.path.join(PROJECT_ROOT, "test"))

    return 0

def configure(ddebug, doptimize, dsse2, dopenmp, dprofile, dcoverage):
    """configure

    """
    os.makedirs(BIN_DIR, exist_ok=True)
    os.makedirs(BUILD_DIR, exist_ok=True)
    os.makedirs(TEST_RESULT_DIR, exist_ok=True)

    cmd = ['./configure', '-DDEBUG=%s' % 'ON' if ddebug else 'OFF'
                        , '-DPROFILE=%s' % 'ON' if dprofile else 'OFF'
                        , '-DCOVERAGE=%s' % 'ON' if dcoverage else 'OFF'
                        , '-DOPTIMIZE=%s' % 'ON' if doptimize else 'OFF'
                        , '-DSSE2=%s' % 'ON' if dsse2 else 'OFF'
                        , '-DOPENMP=%s' % 'ON' if dopenmp else 'OFF']

    return call(cmd, quiet=False)

def compile(make_options = []):
    """compile

    :param debug:
    :param optimize:
    :param profile:
    :param coverage:
    :param make_options:

    """
    return call(['make'] + make_options, quiet=False)

def coverage():
    """coverage"""
    info_path = os.path.join(TEST_RESULT_DIR, 'report.info')
    tmp_info_path = info_path + '.tmp'

    if call([ 'lcov', '--no-external', '--directory', PROJECT_ROOT, '--capture', '--output-file', tmp_info_path ]) != 0:
        raise RuntimeError('failed to create info file with lcov')

    source_glob = PROJECT_ROOT + '/src/*'

    if call([ 'lcov', '-e', tmp_info_path, source_glob, '--output-file', info_path ]) != 0:
        raise RuntimeError('failed to create info file with lcov from %s' % tmp_info_path)

    os.makedirs(COVERAGE_DIR, exist_ok=True)

    if call([ 'genhtml', '--output-directory', COVERAGE_DIR, info_path ]) != 0:
        raise RuntimeError('failed to generate html from %s' % info_path)

    coverage_script = os.path.join(TOOL_DIR, 'check-coverage.py')

    os.remove(tmp_info_path)
    os.remove(info_path)

    return call([ 'python3', coverage_script, COVERAGE_DIR], quiet=False)

def coupling():
    """check_coupling"""
    coupling_script = os.path.join(TOOL_DIR, 'check-coupling.py')
    src_path = os.path.join(PROJECT_ROOT, 'src')

    return call([ 'python3', coupling_script, src_path ], quiet=False)

def linting():
    """run_linter"""
    linter_script = os.path.join(TOOL_DIR, 'cpplint.py')
    src_path = os.path.join(PROJECT_ROOT, 'src')

    return call([ "python", linter_script, src_path ], quiet=False)

def profile():
    """profile"""
    log.warning('profiling not yet supported')
    return 0

def integration():
    """check_integration"""
    bin_path = os.path.join(BIN_DIR, 'low-carb')
    config_path = os.path.join(TEST_RESOURCE_DIR, 'integration/config.ini')
    output_path = os.path.join(TEST_RESULT_DIR)

    cmd = [ bin_path, '-c', config_path, '-o', output_path, '-d' ]

    try:
        return call(cmd, quiet=False)
    finally:
        pass
        # shutil.rmtree(output_path)

def todos():
    """todos"""
    todo_script = os.path.join(TOOL_DIR, "check-todo.py")
    src_path = os.path.join(PROJECT_ROOT, "src")

    return call([ 'python3', todo_script, src_path ], quiet=False)

def unittests():
    """unittests"""
    bin_path = os.path.join(BIN_DIR, 'low-carb-unittests')

    try:
        return call([ bin_path, '--random', '--log_level=message' ], quiet=False)
    finally:
        pass

CONTEXT_SETTINGS = dict(help_option_names=['-h', '--help'])

@click.command(context_settings=CONTEXT_SETTINGS)
@click.option('--time/--no-time', default=False)
@click.option('-d', '--debug/--no-debug', default=False)
@click.option('-p', '--check-profiling/--no-profiling', default=False)
@click.option('-v', '--check-coverage/--no-coverage', default=False)
@click.option('-u', '--check-coupling/--no-coupling', default=False)
@click.option('-l', '--check-linting/--no-linting', default=False)
@click.option('-i', '--check-integration/--no-integration', default=False)
@click.option('-t', '--check-todos/--no-todos', default=False)
@click.option('--check-unittests/--no-unittests', default=False)
@click.option('--optimization/--no-optimization', default=True)
@click.option('--sse2/--no-sse2', default=True)
@click.option('--openmp/--no-openmp', default=True)
@click.option('--make', metavar='OPTIONS', default='-j3')
@click.option('--output-format', default='fancy_grid', type=click.Choice(['orgtbl','plain','simple','grid','rst','latex','fancy_grid']))
@click.argument('target', nargs=-1, type=click.Choice(['clean', 'configure', 'compile', 'cc', 'nop']))
def cc(debug, time,
       check_profiling, check_coverage, check_coupling, check_linting, check_integration, check_unittests, check_todos,
       optimization, sse2, openmp, make,
       output_format,
       target):
    logging.basicConfig(format='%(asctime)s | %(levelname)s | %(message)s', level=logging.INFO)

    CWD = os.getcwd()

    os.chdir(PROJECT_ROOT)

    global ERROR_COUNTER
    ERROR_COUNTER = 0

    start = datetime.datetime.now()
    make_opts = make.split(' ') if (make is not None and make != '') else []

    try:
        try:
            if 'cc' in target:
                require(clean)
                require(configure, debug, optimization, sse2, openmp, check_profiling, check_coverage)
                require(compile, make_opts)
            else:
                if 'clean' in target:
                    require(clean)

                if 'configure' in target:
                    require(configure, debug, optimization, sse2, openmp, check_profiling, check_coverage)

                if 'compile' in target:
                    require(compile, make_opts)
        except KeyboardInterrupt:
            log.warning('aborting.')
            exit(0)

        if check_unittests:
            check(unittests)

        if check_integration:
            check(integration)

        if check_coupling:
            check(coupling)

        if check_coverage:
            if not check_unittests:
                log.warning("keep in mind, coverage check will use the last unittest results.")
            check(coverage)

        if check_profiling:
            check(profile)

        if check_linting:
            check(linting)

        if check_todos:
            check(todos)

    except RuntimeError as e:
        log.critical(e)
        exit_code = 1
    except Exception as e:
        log.exception(e)
        exit_code = 1
    finally:
        os.chdir(CWD)

        if FAILED_CHECKS != []:
            print("\nFailed checks:\n")
            print(tabulate(FAILED_CHECKS, headers=['check', 'message'],
                                          tablefmt=output_format))

    if time:
        log.info('time: %s' % (datetime.datetime.now() - start))

    exit(ERROR_COUNTER)

if __name__ == '__main__':
    cc()

# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8

#!/usr/bin/env python3
import argparse
import os
import shlex
import shutil
import subprocess
import sys
from contextlib import suppress


def red(text: str) -> str:
    """
    Make the given text red.

    """
    return '\033[91m{}\033[0m'.format(text)


def die(reason: str, exit_code: int=1) -> None:
    """
    Print the reason in red to stderr and end the process.

    """
    print(red(reason), file=sys.stderr)
    sys.exit(exit_code)


def shell(command_line: str) -> None:
    """
    Runs the given string in the shell and returns the output, or kills the
    process if the given string did not execute successfully (the shell
    returned a on-zero exit codes).

    """
    try:
        result = subprocess.run(
            shlex.split(command_line),
            check=True,
            universal_newlines=True,
            stderr=subprocess.PIPE,
        )
    except subprocess.CalledProcessError as error:
        die(error.stderr)


parser = argparse.ArgumentParser('Channel++ builder')
parser.add_argument(
    '--clean', '-c',
    help='Clear the build directory and retrigger CMake and Ninja.',
    default=False,
    action='store_true',
)
parser.add_argument(
    '--test', '-t',
    help='Run the tests after the build is done.',
    default=False,
    action='store_true',
)


if __name__ == '__main__':
    with suppress(KeyboardInterrupt):
        args = parser.parse_args()
        if args.clean:
            shutil.rmtree('build')
        os.makedirs('build', exist_ok=True)
        os.chdir('build')
        shell('cmake .. ')
        shell('make -j 6')

        if args.test:
            message = 'Starting Tests'
            padding = '=' * round((80 - 2 - len(message)) / 2)
            print(f'\n{padding} {message} {padding}\n')
            os.chdir('tests')
            shell('./test --order decl --durations yes')

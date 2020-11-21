#!/usr/bin/env python3

from os import listdir
from os.path import dirname, join, realpath, splitext
from re import findall
from subprocess import run


PROJECT_ROOT = join(dirname(realpath(__file__)), '..')
BITSYC_PATH = join(PROJECT_ROOT, 'build', 'bitsyc')
SPEC_PATH = join(PROJECT_ROOT, '..', 'bitsyspec', 'specs')


class TestCase:

    def __init__(self, spec):
        self.spec = spec

    @property
    def __spec_basename(self):
        return splitext(self.spec)[0]

    def __parse_results(self):
        with open(join(SPEC_PATH, self.spec), 'r') as file:
            content = file.read().replace('\n', ' ')
            return findall(r'\{.*?((?:-?\d+\s+)+)\}', content)[0].split()

    def run(self):
        command = [BITSYC_PATH, join(SPEC_PATH, self.spec)]
        process = run(command, capture_output=True)
        actual = process.stdout.decode('ascii').split()
        expected = self.__parse_results()
        if actual == expected:
            print(f'✅ {self.__spec_basename}')
            return True
        print(f'❌ {self.__spec_basename} expected {expected} but got {actual}.')
        return False


if __name__ == '__main__':
    exit(not all(TestCase(spec).run() for spec in listdir(SPEC_PATH)))

#!/usr/bin/env python3
import os
from typing import List, Tuple
import sys

body: str = \
    'int main()\n{\n' + \
    '\tfprintf(stderr, "[Memoery Testing] - Start\\n");\n'
footer: str = \
    '\tprintf("[Memory Testing] - Done\\n");\n' + \
    '\treturn 0;\n' + \
    '}'

def getFiles(rdir: str = '.') -> List[Tuple[str, str]]:
    files = []
    for dirpath, _, filenames in os.walk(rdir):
        for filename in filenames:
            if filename.endswith('.c'):
                files.append((dirpath, filename))
    return files

def main() -> int:
    files: List[Tuple[str, str]] = getFiles();

    header: List[str] = ['./Helper.h']
    function: List[str] = []

    for folder, name in files:
        if name == 'main.c':
            continue
        cpath: str = os.path.join(folder, name)

        print(f'Registering file: {cpath}')
        header.append(cpath)
        print(f'> Header: {cpath}')

        cpath = cpath[2:] if cpath.startswith('./') else cpath[1:]
        cpath = cpath[:len(cpath) - 2]
        cpath = cpath.replace('/', '_')
        function.append(cpath)
        print(f'> function: {function}')

    print(f'Header: {header}')
    print(f'Funciton: {function}')

    main_file: str = ''
    for hdr in header:
        main_file += f'#include \"{hdr}\"\n'
    main_file += '\n' + body
    for func in function:
        main_file += f'\n\tfprintf(stderr, "[Memory Testing] Calling test for: {folder} -> {name}\\n");\n'
        main_file += f'\t{func}_function();\n'
    main_file += footer
    os.system('cp ../../build/lib/libCContainer.so .')
    os.system('gcc main.c -o memory_test -I../../include -L. -lCContainer')
    print(f'> Generated file:\n{main_file}')

    with open('./main.c', 'w') as file:
        file.write(main_file)
    return 0

if __name__ == "__main__":
    sys.exit(main())

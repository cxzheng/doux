#!/usr/bin/env python3

#
# Changxi Zheng (cxz@cs.columbia.edu)
#

import argparse
import os
import sys
import subprocess

def get_source_files(directory, extensions):
    output = []

    for root, _, files in os.walk(directory):
        for fname in files:
            name, ext = os.path.splitext(fname)
            if len(ext) > 0 and ext in extensions:
                output.append(os.path.join(root, fname))
    return output

def main():
    # global params
    parser = argparse.ArgumentParser(prog='run-clang-format',
                                     description='Checks and run clang-format on source files')
    parser.add_argument("--file-ext", type=str,
                        default=".cpp .h .cxx .hxx .hpp .cc .ipp",
                        help="Space separated list of file extensions to check")
    parser.add_argument('--include', action='append', default=[],
                        help='directories in which the source files will be formatted')
    parser.add_argument('--clang-format-bin', type=str, default="clang-format",
                        help="The clang format binary")
    args = parser.parse_args()

    # Run gcovr to get the .gcda files form .gcno
    exts = args.file_ext.split(" ")
    if len(exts) == 0: return 1

    cmd = [args.clang_format_bin, "-style=file", "-i"]
    for folder in args.include:
        files = get_source_files(folder, exts)
        if len(files) > 0:
            print("clang-format files in [{}] ...".format(folder))
            try:
                subprocess.run(cmd + files).check_returncode()
            except subprocess.CalledProcessError as e:
                print("Error in calling clang-format [{}] on folder [{}]".format(e, folder), file = sys.stderr)

    return 0

if __name__ == '__main__':
    sys.exit(main())

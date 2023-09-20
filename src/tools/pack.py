#!/usr/bin/env python3

import sys
import argparse


# Packs the content for the given path
def packContent(path, cont):
    if isinstance(path, str):
        path = path.encode()
    if isinstance(cont, str):
        cont = cont.encode()

    prepared_path = path + (256 - len(path))*b'\0'
    prepared_content = len(cont).to_bytes(8, 'little') + cont
    return prepared_path + prepared_content


# Parses the command-line arguments
def parseArgs():
    parser = argparse.ArgumentParser(
                prog="librucksack pack.py",
                description="Packs files into a single binary to be embedded into librucksack",
                epilog="Example: ./pack.py --strip-prefix /home/user/Desktop/rootfs --output ../lib/pack /home/user/Desktop/rootfs/*")
    parser.add_argument("files", nargs="+")
    parser.add_argument("--output", "-o", required=True)
    parser.add_argument("--strip-prefix", "-s")
    parser.add_argument("--add-prefix", "-a")
    return parser.parse_args()


# packs the given file list and writes it out to output
def packAllTo(files, output, strip="", add=""):
    # pack files
    out = b""
    for file in files:
        try:
            with open(file, "rb") as fh:
                path = file
                # rewrite path if desired
                if strip:
                    path = path.replace(strip, "")
                if add:
                    path = add + file

                # read original content
                cont = fh.read()
                # pack content
                out += packContent(path, cont)
        except Exception as e:
            # Read error. Permission denied maybe?
            print(f"Failed to read file {file}: {e}", file=sys.stderr)
            continue
    # write out pack
    with open(output, "wb") as fh:
        fh.write(out)
    # inform user and exit
    print(f"Packed {len(files)} files to {output}.")


# your typical main function
def main():
    # parse args
    args = parseArgs()
    packAllTo(args.files, args.output, args.strip_prefix, args.add_prefix)
    

if __name__ == "__main__":
    main()


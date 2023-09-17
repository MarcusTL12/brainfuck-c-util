#!/usr/bin/env python3

import sys
import os
import pathlib
import subprocess

file = sys.argv[1]
p = pathlib.Path(file)
srcdir = p.parent.resolve()
filename = p.stem

subprocess.run(f"gcc -E {file} > prep.c", shell=True)

with open("tmp.c", "w") as out:
    with open("prep.c") as f:
        for l in f:
            if l[0] != '#':
                out.write(l)

    out.write("\nmain();\n")

os.remove("prep.c")
subprocess.run("c2bf.native -O=3 tmp.c", shell=True)
os.remove("tmp.c")
os.rename("a.bf", f"{srcdir}/bf/{filename}.bf")

with open(f"{srcdir}/bf/{filename}.bf") as f:
    print(f"BF length: {len(f.read())}")

#!/usr/bin/env python3

import sys
import subprocess
import pathlib
from time import time

bf_file = sys.argv[1]
inp_file = sys.argv[2]

thisdir = pathlib.Path(__file__).parent.resolve()

t = time()
subprocess.run(f"bash {thisdir}/run.sh {inp_file} {bf_file}", shell=True)
t = time() - t

print(f"Took {t:.3f} s")

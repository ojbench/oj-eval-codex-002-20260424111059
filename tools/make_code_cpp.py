#!/usr/bin/env python3
import pathlib

root = pathlib.Path(__file__).resolve().parents[1]
hdr = root / 'src' / 'include' / 'int2048.h'
impl = root / 'src' / 'int2048.cpp'
out = root / 'code.cpp'

header = hdr.read_text()
source = impl.read_text()

# Remove local include of header in the source for single-file submission
source = source.replace('#include "include/int2048.h"', '')

with out.open('w') as f:
    f.write('#include <bits/stdc++.h>\n')
    f.write('using namespace std;\n')
    f.write('\n')
    f.write(header)
    f.write('\n')
    f.write(source)
print(f'Wrote {out}')


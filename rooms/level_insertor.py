import sys
import os
import argparse
#fixme

CLL_path = os.path.join(os.path.split(__file__)[0], "CLL")
max_level = 56


def get_path(level):
    if isinstance(level,int):
        return os.path.join(CLL_path,str((level-1)//20+1), str(level)+".cll")
    else:
        return os.path.join(CLL_path, str(level)+".cll")


parser = argparse.ArgumentParser()
parser.add_argument("src", type=str)
parser.add_argument("--output", "-o", type=int, default=None)
args = parser.parse_args()

if args.output is None:
    output = int(args.src)
    args.src = "draft"
    with open(get_path(args.src), "w"):
        ...
else:
    output = args.output
    
try:
    src = int(args.src)
except ValueError:
    src = max_level+1
    os.rename(get_path(args.src), get_path(src))

plus = -1 if output > src else 1
to_move = range(output, src, plus)

for i in to_move:
    try:
        os.rename(get_path(i), get_path(str(i)+"_"))
    except FileNotFoundError:
        ...

os.rename(get_path(src), get_path(output))

for i in to_move:
    try:
        os.rename(get_path(str(i)+"_"), get_path(i+plus))
    except FileNotFoundError:
        ...

import argparse
import os

import convertor

rooms_path = os.path.join(convertor.tools_dir, os.path.pardir, "rooms")
CLL_path = os.path.join(rooms_path, "CLL")
Closs_path = os.path.join(rooms_path, "Closs")


def convert_CLL_to_Closs(src: str):
    if src.endswith(".cll"):
        name = src[:-4]
    else:
        src += ".cll"
        name = src
    src_path = os.path.join(CLL_path, src)
    output_path = os.path.join(Closs_path, name + ".json")
    convertor.convert_file(src_path, output_path)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("src", type=str)
    args = parser.parse_args()
    convert_CLL_to_Closs(args.src)


if __name__ == "__main__":
    main()

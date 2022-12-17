import argparse
import os

import convertor

rooms_path = os.path.join(convertor.tools_dir, os.path.pardir, "rooms")
CLL_path = os.path.join(rooms_path, "CLL")
Closs_path = os.path.join(rooms_path, "Closs")


def convert_CLL_to_Closs(src: str):
    src_path = os.path.abspath(os.path.join(CLL_path, src))
    if os.path.isfile(src_path) and src.endswith(".cll"):
        print("Converting", src_path)
        name = src[:-4]
        output_path = os.path.abspath(os.path.join(Closs_path, name + ".json"))
        convertor.convert_file(src_path, output_path)
        print("Outputted to", output_path)
    else:
        for file in os.listdir(src_path):
            convert_CLL_to_Closs(file)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("src", type=str)
    args = parser.parse_args()
    convert_CLL_to_Closs(args.src)


if __name__ == "__main__":
    main()

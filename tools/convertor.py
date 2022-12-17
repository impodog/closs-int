import argparse
import json
import os
import copy
import re

DEFAULT = "DEFAULT"
tools_dir = os.path.join(__file__, os.path.pardir)
template_json: dict = json.load(open(os.path.join(tools_dir, "convertor_template.json")))
"""
tile_background = -2,
tile_undefined,
tile_empty = 0,
tile_cyan,
tile_box,
tile_wall,
tile_destination
"""
repl_code = {
    "ud": -1,
    "--": 0,
    "cy": 1,
    "bx": 2,
    "wa": 3,
    "ds": 4,
    "gm": 5
}
common_define = {
    "1c": "ds<cy",
    "1x": "ds<bx"
}


def get_repl(tile: str) -> int:
    try:
        return int(tile)
    except ValueError:
        return repl_code[tile]


def parse_content(content: str) -> "list[list[list]]":
    content = content.strip("{}\n")
    content = re.sub(" +", " ", content)
    result = list()
    max_width = 0
    for line in content.split('\n'):
        line_list = list()
        for space in line.split(' '):
            space_list = list()
            for tile in space.split(','):
                if tile.find('<') == -1:
                    space_list.append(get_repl(tile))
                else:
                    space_list.append(list(map(get_repl, tile.split('<'))))
            line_list.append(space_list)
        result.append(line_list)
        width = len(line_list)
        if width > max_width:
            max_width = width
    for line in result:
        for i in range(0, max_width - len(line)):
            line.append([0])
    return result


def analyze_content(content: "list[str]") -> dict:
    def import_define(src: dict[str, str]):
        nonlocal define_repl
        for import_k, import_v in src.items():
            define_repl[import_k] = import_v

    result = copy.deepcopy(template_json)
    define_repl: dict[str, str] = dict()
    i = 0
    while i < len(content):
        line = content[i]
        prams = line.split(' ')
        key = prams[0]
        operator = " ".join(prams[1:]).strip()
        if operator == "{":
            brace_count = 1
            j = i + 1
            for d_content in content[j:]:
                brace_count += d_content.count('{') - d_content.count('}')
                if brace_count == 0:
                    break
                j += 1
            if brace_count != 0:
                raise RuntimeError("braces not closed")
            operator += "\n" + "\n".join(content[i + 1:j + 1])
            i = j
        for k, v in define_repl.items():
            operator = re.sub(r"(\b)%s(\b)" % k, r"\1%s\2" % v, operator)
        if key == "content":
            result["content"] = parse_content(operator)
            result["width"] = len(result["content"][0])
            result["height"] = len(result["content"])
        elif key == ".define":
            if operator == "common":
                import_define(common_define)
            elif operator.startswith("{"):
                import_define(eval(operator))
            else:
                k, v = operator.split("=")
                k = k.strip()
                v = v.strip()
                define_repl[k] = v
        else:
            result[key] = eval(operator)
        i += 1
    return result


def convert_file(src: str, output: str) -> None:
    content = open(src, 'r', encoding="utf-8").read()
    content = re.sub(r"\s*\n\s*", "\n", content)
    converted = analyze_content(content.split('\n'))
    json.dump(converted, open(output, 'w', encoding="utf-8"))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("src", type=str)
    parser.add_argument("output", type=str)
    args = parser.parse_args()
    convert_file(args.src, args.output)


if __name__ == "__main__":
    main()
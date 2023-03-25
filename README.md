# Closs : Inside the Tapes

NOTICE : Do NOT make changes to the existing CLL files in a pull request or do it directly. See "How to Modify - Creating Levels"

## How to Install

Clone the code from development branch, or optionally release branch(not recommended)

### System Requirements

Release Version : Win64

Source Version : Well-tested on Win11 64-bit. Should work on other windows versions. NOT tested on other platforms(that SDL2 supports).

### External Dependencies

SDL2, SDL2_image, SDL2_ttf, SDL_mixer : [See SDL website](www.libsdl.org)

json.hpp : [nlohmann/json: JSON for Modern C++ (github.com)](https://github.com/nlohmann/json)

C&C++ compiler such as [MinGW-w64](https://www.mingw-w64.org/)

[CMake](https://cmake.org/download/)

### Link the Packages (MinGW)

1. Download SDL2, SDL2_image, SDL2_ttf, SDL2_mixer (latest mingw version), json.hpp(which is in the releases)
2. Extract all SDL2 archive to your path (e.g. C:/Include/)
3. For each SDL2 directory, copy the path of **/[32bit=i686,64bit=x86_64]-w64-mingw32**
4. Now open this project and modify **/cmakelists.txt**, replace **"SDL2_PATH"**, **"SDL2_IMAGE_PATH"**, **"SDL2_TTF_PATH"**, **"SDL2_MIXER_PATH"** values with the path you copied
5. Also, copy json.hpp path, and replace **"nlohmann_INCLUDE"** value in /cmakelists.txt
6. To make the game function, you'll also need to copy the DLL files in each SDL2 directory, which are under **[SDL root]/bin/[SDL2|SDL2_image|SDL2_ttf|SDL2_mixer].dll**, and move it to **C:/Windows/System32**, or optionally under the executable parent folder

### Build with CMake

[Download CMake](https://cmake.org/download/) in case you haven't done it. Then go to the game directory

Configure the project with **"cmake ."**

Run command : **"cmake --build . --target Closs_InT"**, you should see an executable created under the game root

### All Done!

Open the executable and play!

## How to Play

### Controls

#### All Controls: See the manual page inside the game

#### Important Controls

To move: Arrow keys

Confirm: RETURN / C

Help: H

Escape / Open Settings: ESC

### Gameplay

The game is like other box games, push boxes to the destination, win levels one by one.

BUT there's more! Explore new objects such as blue boxes, and special game features.

## How to Modify

### Converting Levels

1. To convert closs level file(.cll) to standard json, you'll need to install [Python 3.11 or later](https://www.python.org/downloads/) and use the convertor from repo https://gitee.com/impodog/cl-ml.git

2. Then, test the convertor with command(under this source directory) **"python [your cl-ml path]/main.py ./rooms/CLL --no-traceback --recursive"**, *it's normal to see some fails(extra files)*
3. If the conversion succeeded, you should see some newly created json file under /rooms/Closs, which will be directly used later by the executable

### Creating Levels

You don't need to know a lot of CLL to create a level, just follow the steps:

1. Create a new folder (**use your username**) under **/rooms/CLL**, Copy&Paste **/rooms/CLL/template.cll** into it then rename the file to whatever you want (don't change the file extension though!)
2. Check the file's relative path to **/rooms/CLL/C_InT.cll**. It should be  **../C_InT.cll**
3. Now, locate "content 4d[ ... ]", that's where your precious level design should be
4. See other examples, then design the level yourself, the format should be easy, but you'll need to know a few CLL Names:

|   Object Name    |  CLL Name   |     JSON Value     |
| :--------------: | :---------: | :----------------: |
|    Undefined     |     ud      |         -1         |
|      Empty       | --, \*empty |         0          |
|       Cyan       |     cy      |         1          |
|       Box        |     bx      |         2          |
|       Wall       |     wa      |         3          |
|       Dest       |    \*ds     |         4          |
|       Gem        |    \*gem    |         5          |
|     Picture      |    \*pic    |         6          |
|      Go_To       |    goto     |         7          |
|       Blue       |  bl, \*bb   |         8          |
|      Spike       |     sp      |         9          |
|     Conveyor     |    \*cv     |         10         |
|      Robot       |    \*rb     |         11         |
|     Imitate      |    *imi     |         12         |
|   Dest + Cyan    |     1c      |       [4, 1]       |
|    Dest + Box    |     1x      |       [4, 2]       |
|   Dest + Blue    |     1b      |       [4, 8]       |
|   Dest + Robot   |     1r      |      [4, 11]       |
|  Conveyor + Up   |     2u      | [10, [1073741906]] |
| Conveyor + Left  |     2l      | [10, [1073741904]] |
| Conveyor + Down  |     2d      | [10, [1073741905]] |
| Conveyor + Right |     2r      | [10, [1073741903]] |
|    Robot + Up    |     3u      | [11, [1073741906]] |
|   Robot + Left   |     3l      | [11, [1073741904]] |
|   Robot + Down   |     3d      | [11, [1073741905]] |
|  Robot + Right   |     3r      | [11, [1073741903]] |

\* : Not recommended and never used

5. To make gems, use command **"!gem-def [Define Name] [Gem Value]"**, and use [Define Name] as an object
6. Now, modify your **title** and **help**, just type some words in the double quotes! It's fine if you only know one language, I can help to translate!
7. To make the level actually function, add **"//"** before line 3 "!note \"CHANGE THIS BEFORE USE\""
8. Convert you level to see if you made any mistakes! If you didn't, you shall see a new .json file under /rooms/Closs named just after your CLL file!

### Rules of CLL

There are some rules to improve your CLL levels:

1. Do NOT put two tiles in one space in the level design(although it is supported)
2. Do NOT write unnecessary commas, colons, or assign symbols
3. Do NOT use the json values
4. DO define your own shortcuts in a pattern (e.g. G1(Gem + ...), Xs(Box, Spike), Cr(Conveyor, Robot))
5. DO use **!note \"...\" ** to take todo messages when you publish

### Playing My New Level

You must be really excited to try your design, huh? Do this:

1. Create "/user/play.ini" and write **"Closs/[new .json file name]"**
2. Start the game, and select "Play". You should be immediately directed to your level
3. Play through it, find a perfection step, and return to CLL file, fill in the "perf [Perfection Steps]", convert again
4. Now you're ready to publish the level!

Also, delete **"/user/play.ini"** later to return to normal gameplay

### Where to Submit My Level

It's simple, remember you created a new folder right? You can simply upload like that by either a **pull request** or push **to a new branch**
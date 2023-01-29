# Closs : Inside the Tapes

## How to Install

Clone the code from development branch, or optionally release branch(not recommended)

### System Requirements

Release Version : Win64

Source Version : Well-tested on Win11 64-bit. Should work on other windows versions. NOT tested on other platforms(that SDL2 supports).

### External Packages Used

SDL2, SDL2_image, SDL2_ttf : [See SDL website](www.libsdl.org)

json.hpp : [nlohmann/json: JSON for Modern C++ (github.com)](https://github.com/nlohmann/json)

### Link the Packages (MinGW)

1. Download SDL2, SDL2_image, SDL2_ttf (latest mingw version), json.hpp(which is in the releases)
2. Extract all SDL2 archive
3. For each SDL2 directory, copy the path of /[32bit=i686,64bit=x86_64]-w64-mingw32 ()
4. Now open this project and modify /cmakelists.txt, replace "SDL2_PATH", "SDL2_IMAGE_PATH", "SDL2_TTF_PATH" with the path you copied
5. Also, copy json.hpp path, and replace "nlohmann_INCLUDE" in /cmakelists.txt
6. To make the game function, you'll also need to copy the DLL files in each SDL2 directory, which are under /(system)/bin/[SDL2|SDL2_image|SDL2_ttf].dll, and move it to C:/Windows/System32, or optionally under the executable parent folder

### Build with CMake

[Download CMake](https://cmake.org/download/) in case you haven't done it.

Run command under project directory : "cmake --build cmake-build --target Closs_InT", you should see an executable created under /cmake-build

One last thing, if you are on release branch, copy the executable under /cmake-build, and paste it to the project directory

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
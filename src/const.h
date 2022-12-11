//
// Created by Dogs-Cute on 12/10/2022.
//

#ifndef CLOSS_INT_CONST_H
#define CLOSS_INT_CONST_H

#define TITLE "Closs : Inside the Tapes"
#define WIDTH 2000
#define HEIGHT 1125

#define PATH_BEGIN "../"
#define DEFAULT_JSON "default.json"
#define USER_JSON "SAVE.json"

#include "iostream"
#include "vector"
#include "unordered_map"
#include "cmath"
#include "chrono"
#include "json.hpp"
#include "fstream"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

using namespace std;
using namespace chrono;
using namespace nlohmann;

const SDL_Color WHITE{255, 255, 255, 255};
const SDL_Color BLACK{0, 0, 0, 255};

template<typename T1>
struct Pos2D {
	T1 w, h;
	
	template<typename T2>
	Pos2D operator+(const Pos2D<T2> pos) {
		return {w + pos.w, h + pos.h};
	};
};

using TilePos = Pos2D<size_t>;
using DisplayPos = Pos2D<int>;

enum direction {
	dir_up = 1,
	dir_left,
	dir_down,
	dir_right
};
using DirPos_Vec = unordered_map<direction, DisplayPos>;
extern DirPos_Vec dir_pos_map;
#endif //CLOSS_INT_CONST_H

//
// Created by Dogs-Cute on 12/10/2022.
//

#ifndef CLOSS_INT_CONST_H
#define CLOSS_INT_CONST_H

#include <utility>
#include "iostream"
#include "vector"
#include "map"
#include "unordered_map"
#include "cmath"
#include "chrono"
#include "json.hpp"
#include "fstream"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#define TITLE "Closs : Inside the Tapes"
#define SCR_WIDTH 2000
#define SCR_HEIGHT 1125
#define TILE_BACK_SEP 5
#define TILE_BACK_SEP2 (2*TILE_BACK_SEP)
#define STANDARD_EACH 50
#define MIN_EACH 128

#define PATH_BEGIN "../"
#define DEFAULT_JSON "default.json"
#define USER_JSON "SAVE.json"
#define ROOMS_PATH PATH_BEGIN "rooms/"
#define CLOSS_ROOMS_PATH ROOMS_PATH "Closs/"
#define IMG_PATH PATH_BEGIN "img/"

#define ROOM_K_EACH "each"
#define ROOM_K_WIDTH "width"
#define ROOM_K_HEIGHT "height"
#define ROOM_K_TITLE "m_title"
#define ROOM_K_HELP "help"
#define ROOM_K_CONTENT "content"

#define KEY_SHIFT_UP SDLK_w
#define KEY_SHIFT_LEFT SDLK_a
#define KEY_SHIFT_DOWN SDLK_s
#define KEY_SHIFT_RIGHT SDLK_d

#define KEY_MOVE_UP SDLK_UP
#define KEY_MOVE_LEFT SDLK_LEFT
#define KEY_MOVE_DOWN SDLK_DOWN
#define KEY_MOVE_RIGHT SDLK_RIGHT

#define MOVEMENT_KEYS {KEY_MOVE_UP,KEY_MOVE_LEFT,KEY_MOVE_DOWN,KEY_MOVE_RIGHT}

using namespace std;
using namespace chrono;
using namespace nlohmann;

const SDL_Color WHITE{255, 255, 255, 255};
const SDL_Color BLACK{0, 0, 0, 255};
const SDL_Color TILE_BACK_COLOR{100, 100, 100, 255};

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

using c_str = const char *;
struct title_with_content {
	string title, content;
};
using help_map_t = map<title_with_content, size_t>;

using key_pos_map_t = unordered_map<SDL_Keycode, DisplayPos>;
extern key_pos_map_t key_pos_map;

#endif //CLOSS_INT_CONST_H

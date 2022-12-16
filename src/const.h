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

#define VERSION "v0.1.1"

#define TITLE "Closs : Inside the Tapes " VERSION
#define SCR_WIDTH 2000
#define SCR_HEIGHT 1125
#define TILE_BACK_SEP 5
#define TILE_BACK_SEP2 (2*TILE_BACK_SEP)
#define STANDARD_EACH 50
#define MIN_EACH 128

#define SELECTION_DEFAULT_EACH 50
#define DESTINATION_SIZE 20
#define SETTINGS_EACH 60
#define SETTINGS_SIZE 40
#define LOBBY_EACH 100
#define LOBBY_SIZE 60
#define TEXT_PAGE_EACH 100
#define TEXT_PAGE_SIZE 50

#define WINNING_SIZE 40
#define WINNING_WIDTH 500

#define RESERVED_HEIGHT 1000
#define RESERVED_FROM_B (SCR_HEIGHT - RESERVED_HEIGHT)
#define LEAVE_BLANK_WIDTH 50

#define ROOM_TITLE_SIZE 50

#define MAX_SENSITIVITY 300
#define MIN_SENSITIVITY 1

#define MAX_FRAMERATE 150
#define MIN_FRAMERATE 20


#define PATH_BEGIN "../"

#define ROOMS_PATH PATH_BEGIN "rooms/"
#define CLOSS_ROOMS_PATH ROOMS_PATH "Closs/"
#define SETTINGS_PATH ROOMS_PATH "settings.json"
#define LOBBY_PATH ROOMS_PATH "lobby.json"
#define IN_GAME_PATH ROOMS_PATH "in_game.json"
#define MANUAL_PATH ROOMS_PATH "manual.json"

#define IMG_PATH PATH_BEGIN "img/"
#define USER_PATH PATH_BEGIN "user/"
#define DEFAULT_JSON_NAME "default.json"
#define USER_JSON_NAME "user.json"
#define DEFAULT_JSON_PATH PATH_BEGIN "user/" DEFAULT_JSON_NAME
#define USER_JSON_PATH PATH_BEGIN "user/" USER_JSON_NAME

#define TTF_PATH PATH_BEGIN "ttf/"
#define ARIAL_PATH TTF_PATH "arial.ttf"
#define CONSOLAS_PATH TTF_PATH "consolas.ttf"
#define SIMHEI_PATH TTF_PATH "simhei.ttf"


#define ROOM_K_EACH "each"
#define ROOM_K_WIDTH "width"
#define ROOM_K_HEIGHT "height"
#define ROOM_K_TITLE "title"
#define ROOM_K_HELP "help"
#define ROOM_K_CONTENT "content"
#define ROOM_K_NEXT "next"
#define ROOM_K_PERF "perf"

#define USER_K_LANGUAGE "language"
#define USER_K_FRAMERATE "framerate"
#define USER_K_SENSITIVITY "sensitivity"
#define USER_K_ROOM "room"
#define USER_K_UNLOCKED "unlocked"

#define SETTINGS_K_LANGUAGE "language"
#define SETTINGS_K_SENSITIVITY "sensitivity"
#define SETTINGS_K_FRAMERATE "framerate"
#define SETTINGS_K_APPLY "apply"
#define SETTINGS_K_TO_LOBBY "to_lobby"
#define SETTINGS_K_TO_GAME "to_game"

#define LOBBY_K_PLAY "play"
#define LOBBY_K_SETTINGS "settings"
#define LOBBY_K_MANUAL "manual"
#define LOBBY_K_QUIT "quit"

#define IN_GAME_K_WINNING "winning"
#define IN_GAME_K_STEPS "steps"

#define KEY_SHIFT_UP SDLK_w
#define KEY_SHIFT_LEFT SDLK_a
#define KEY_SHIFT_DOWN SDLK_s
#define KEY_SHIFT_RIGHT SDLK_d

#define KEY_MOVE_UP SDLK_UP
#define KEY_MOVE_LEFT SDLK_LEFT
#define KEY_MOVE_DOWN SDLK_DOWN
#define KEY_MOVE_RIGHT SDLK_RIGHT

#define MOVEMENT_KEYS {KEY_MOVE_UP,KEY_MOVE_LEFT,KEY_MOVE_DOWN,KEY_MOVE_RIGHT}

#define KEY_SETTINGS SDLK_q
#define KEY_CONFIRM SDLK_c
#define KEY_HELP SDLK_h
#define KEY_RESTART SDLK_r
#define KEY_ESCAPE SDLK_ESCAPE


using namespace std;
using namespace chrono;
using namespace nlohmann;

const SDL_Color WHITE{255, 255, 255, 255};
const SDL_Color BLACK{0, 0, 0, 255};
const SDL_Color RED{200, 0, 0, 255};
const SDL_Color GREEN{0, 200, 0, 255};
const SDL_Color BLUE{0, 0, 200, 255};
const SDL_Color CYAN{0, 200, 200, 255};
const SDL_Color GREY{200, 200, 200};

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

enum tile_types {
	tile_background = -2,
	tile_undefined,
	tile_empty = 0,
	tile_cyan,
	tile_box,
	tile_wall,
	tile_destination
};

using c_str = const char *;

using help_map_t = map<string, string>;

using key_pos_map_t = const unordered_map<SDL_Keycode, DisplayPos>;
extern key_pos_map_t key_pos_map;

using usable_languages_t = const vector<string>;
extern usable_languages_t usable_languages;

using type_names_t = const unordered_map<tile_types, string>;
extern type_names_t type_names;


SDL_Rect get_srcrect(const SDL_Surface *surface);

SDL_Rect get_dstrect(const DisplayPos &pos, const SDL_Surface *surface);


#endif //CLOSS_INT_CONST_H

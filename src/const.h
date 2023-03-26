//
// Created by Dogs-Cute on 12/10/2022.
//

#ifndef CLOSS_INT_CONST_H
#define CLOSS_INT_CONST_H

#include <utility>
#include <random>
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <cmath>
#include <chrono>
#include <ctime>
#include <fstream>
#include "json.hpp"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

struct Screen_Info {
    int w = -1, h = -1;
};

void get_scr_info(Screen_Info &info);

#if defined _WIN32

#include "Windows.h"

#elif defined __linux__

#include<sys/types.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include<fcntl.h>
#include<linux/fb.h>

#else

#error "No support for this system"

#endif


#define VERSION "v0.11.1"

#define TITLE "Closs : Inside the Tapes " VERSION
#define SCR_WIDTH 2000
#define SCR_HEIGHT 1125
#define SCR_RATIO (3.0/4)
#define TILE_BACK_SEP 5
#define TILE_BACK_SEP2 (2*TILE_BACK_SEP)
#define STANDARD_EACH 50
#define MIN_EACH 128
#define STRETCHED_EACH (STANDARD_EACH * stretch_ratio)
#define STANDARD_FRAMERATE 30
#define EDGE_CROSSING_ACC 1.5

#define SELECTION_DEFAULT_EACH 50
#define DESTINATION_SIZE 20
#define GEM_SIZE DESTINATION_SIZE
#define SETTINGS_EACH 60
#define SETTINGS_SIZE 40
#define LOBBY_EACH 100
#define LOBBY_SIZE 60
#define TEXT_PAGE_EACH 100
#define TEXT_PAGE_SIZE 50
#define LEVELS_EACH 150
#define LEVELS_SIZE 70

#define WINNING_SIZE 35
#define WINNING_WIDTH 1000

#define RESERVED_HEIGHT 1000
#define RESERVED_FROM_B (SCR_HEIGHT - RESERVED_HEIGHT)
#define LEAVE_BLANK_WIDTH 50

#define ROOM_TITLE_SIZE 50

#define DEST_ALPHA_DARK 30
#define DEST_ALPHA_BRIGHT 70
#define DEST_COUNTER_MAX 50
#define DEST_COUNTER_LIM 30

#define MAX_SENSITIVITY 300
#define MIN_SENSITIVITY 1

#define MAX_FRAMERATE 150
#define MIN_FRAMERATE 20

#define MAX_ANIMATION 100
#define MIN_ANIMATION 1

#define MAX_TEXT_RENDERER 1
#define MIN_TEXT_RENDERER 0

#define MAX_LEVEL_NUMBER 2
#define MIN_LEVEL_NUMBER 1

#define MUSIC_FADE_OUT 500
#define MUSIC_FADE_IN 1000

#define DEBUGGER_FONT_SIZE 40
#define DEBUGGER_PROMPT_TIME 1.5e3


#define PATH_BEGIN "./"

#define ROOMS_PATH PATH_BEGIN "rooms/"
#define CLOSS_DIRECTORY_PATH "Closs/"
#define CLOSS_ROOMS_PATH ROOMS_PATH CLOSS_DIRECTORY_PATH
#define SETTINGS_PATH ROOMS_PATH "settings.json"
#define LOBBY_PATH ROOMS_PATH "lobby.json"
#define IN_GAME_PATH ROOMS_PATH "in_game.json"
#define MANUAL_PATH ROOMS_PATH "manual.json"

#define IMG_PATH PATH_BEGIN "img/"
#define USER_PATH PATH_BEGIN "user/"
#define DEFAULT_JSON_NAME "default.json"
#define USER_JSON_NAME "user.json"
#define DEFAULT_JSON_PATH USER_PATH DEFAULT_JSON_NAME
#define USER_JSON_PATH USER_PATH USER_JSON_NAME
#define PLAY_FILE_NAME "play.ini"
#define PLAY_FILE_PATH USER_PATH PLAY_FILE_NAME

#define TTF_PATH PATH_BEGIN "ttf/"
#define ARIAL_PATH TTF_PATH "arial.ttf"
#define CONSOLAS_PATH TTF_PATH "consolas.ttf"
#define SIMHEI_PATH TTF_PATH "simhei.ttf"
#define VERDANA_PATH TTF_PATH "verdana.ttf"

#define SOUND_PATH PATH_BEGIN "sound/"
#define SOUND_SCHEMES_PATH SOUND_PATH "schemes.json"

#define I_AM_DEBUGGER USER_PATH "IAmDebugger"

#define ROOM_K_EACH "each"
#define ROOM_K_WIDTH "width"
#define ROOM_K_HEIGHT "height"
#define ROOM_K_TITLE "title"
#define ROOM_K_HELP "help"
#define ROOM_K_CONTENT "content"
#define ROOM_K_NEXT "next"
#define ROOM_K_PERF "perf"
#define ROOM_K_UNLOCK_BONUS "unlock_bonus"
#define ROOM_K_BOX_NO_SERIAL "box_no_serial"

#define USER_K_LANGUAGE "language"
#define USER_K_FRAMERATE "framerate"
#define USER_K_SENSITIVITY "sensitivity"
#define USER_K_ANIMATION_SPEED "animation_speed"
#define USER_K_ROOM "room"
#define USER_K_UNLOCKED "unlocked"
#define USER_K_PERF "perf"
#define USER_K_GEM "gem"
#define USER_K_LEVELS "levels"
#define USER_K_BONUS "bonus"
#define USER_K_TEXT_RENDERER "text_renderer"
#define USER_K_BONUS_LEVELS "bonus_levels"
#define USER_K_DEBUGGER "debugger"
#define USER_K_MUS_SCHEME "mus_scheme"

#define SETTINGS_K_LANGUAGE "language"
#define SETTINGS_K_SENSITIVITY "sensitivity"
#define SETTINGS_K_FRAMERATE "framerate"
#define SETTINGS_K_ANIMATION "animation"
#define SETTINGS_K_TEXT_RENDERER "text_renderer"
#define SETTINGS_K_MUSIC_SCHEME "music_scheme"
#define SETTINGS_K_APPLY "apply"
#define SETTINGS_K_TO_LOBBY "to_lobby"
#define SETTINGS_K_TO_GAME "to_game"

#define LOBBY_K_PLAY "play"
#define LOBBY_K_SETTINGS "settings"
#define LOBBY_K_MANUAL "manual"
#define LOBBY_K_QUIT "quit"

#define IN_GAME_K_WINNING "winning"
#define IN_GAME_K_STEPS "steps"
#define IN_GAME_K_LEVELS "levels"
#define IN_GAME_K_BONUS_LEVELS "bonus_levels"
#define IN_GAME_K_BONUS_LEVELS_LOCKED "bonus_levels_locked"
#define IN_GAME_K_CHAPTER1 "chapter1"
#define IN_GAME_K_CHAPTER2 "chapter2"
#define IN_GAME_K_CHAPTER3 "chapter3"
#define IN_GAME_K_CHAPTER4 "chapter4"
#define IN_GAME_K_CHAPTER5 "chapter5"
#define IN_GAME_K_BONUS1 "bonus1"

#define KEY_CTRL_UNBRACED SDLK_LCTRL,SDLK_RCTRL
#define KEY_CTRL {KEY_CTRL_UNBRACED}
#define KEY_SHIFT_UNBRACED SDLK_LSHIFT,SDLK_RSHIFT
#define KEY_SHIFT {KEY_SHIFT_UNBRACED}

#define KEY_SHIFT_UP SDLK_w
#define KEY_SHIFT_LEFT SDLK_a
#define KEY_SHIFT_DOWN SDLK_s
#define KEY_SHIFT_RIGHT SDLK_d

#define KEY_MOVE_UP SDLK_UP
#define KEY_MOVE_LEFT SDLK_LEFT
#define KEY_MOVE_DOWN SDLK_DOWN
#define KEY_MOVE_RIGHT SDLK_RIGHT

#define MOVEMENT_KEYS {KEY_MOVE_UP,KEY_MOVE_LEFT,KEY_MOVE_DOWN,KEY_MOVE_RIGHT}

#define KEY_NEXT SDLK_n
#define KEY_BACK SDLK_b

#define KEY_SETTINGS_UNBRACED SDLK_q, SDLK_ESCAPE
#define KEY_SETTINGS {KEY_SETTINGS_UNBRACED}
#define KEY_CONFIRM_UNBRACED SDLK_c, SDLK_RETURN
#define KEY_CONFIRM {KEY_CONFIRM_UNBRACED}
#define KEY_HELP SDLK_h
#define KEY_RESTART SDLK_r
#define KEY_ESCAPE SDLK_ESCAPE
#define KEY_SAVE_AND_REPLAY SDLK_x

#define KEY_DEBUG_PERFECT SDLK_F1
#define KEY_DEBUG_ROLLBACK SDLK_z
#define KEY_DEBUG_WIN SDLK_F2
#define KEY_DEBUG_SET_TO_FIRST_PLAY SDLK_F3

// !!! ADD KEY NEEDED !!!
#define KEY_NEEDED {KEY_CTRL_UNBRACED,KEY_SHIFT_UNBRACED,SDLK_1,SDLK_2,SDLK_3,SDLK_4,SDLK_5,SDLK_6,SDLK_7,SDLK_8,SDLK_9,SDLK_0,\
KEY_MOVE_UP,KEY_MOVE_LEFT,KEY_MOVE_DOWN,KEY_MOVE_RIGHT,\
KEY_SHIFT_UP,KEY_SHIFT_LEFT,KEY_SHIFT_DOWN,KEY_SHIFT_RIGHT,\
KEY_NEXT,KEY_BACK,KEY_SETTINGS_UNBRACED,KEY_CONFIRM_UNBRACED,KEY_HELP,KEY_RESTART,KEY_ESCAPE,KEY_SAVE_AND_REPLAY,\
KEY_DEBUG_PERFECT, KEY_DEBUG_ROLLBACK, KEY_DEBUG_WIN, KEY_DEBUG_SET_TO_FIRST_PLAY}

#define RENDER_TEXT text_renderer

#define WH_ANY_IS_INF(wh) (isinf(wh.w) || isinf(wh.h))
#define WH_IS0(wh) (is0(wh.w) && is0(wh.h))

#define LOG_TIME(s) cout << s << " : " << system_clock::to_time_t(system_clock::now()) << endl
#define LOG_RECT(rect, end) cout << rect.x << "," << rect.y << ":" << rect.w << "," << rect.h << end
#define LOG_POS(pos, end) cout << pos.w << "," << pos.h << end

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
const SDL_Color LIGHT_GREEN{100, 200, 150, 255};
const SDL_Color HALF_LIGHT_GREEN{50, 200, 75, 255};
const SDL_Color LIGHT_CYAN{100, 200, 200, 255};
const SDL_Color GOLD{255, 215, 0, 255};
const SDL_Color LIGHT_RED{200, 50, 100, 255};

template<typename T1>
struct Pos2D {
    T1 w, h;

    template<typename T2>
    Pos2D operator+(const Pos2D<T2> pos) const {
        return {w + pos.w, h + pos.h};
    };

    template<typename T2>
    Pos2D operator*(T2 x) const {
        return {w * x, h * x};
    }

    template<typename T2>
    void operator+=(const Pos2D<T2> &pos) {
        w += pos.w;
        h += pos.h;
    }

    bool operator==(const Pos2D<T1> &pos) {
        return w == pos.w && h == pos.h;
    }

    template<typename T2>
    explicit operator Pos2D<T2>() {
        return {(T2) w, (T2) h};
    }
};

using TilePos = Pos2D<size_t>;
using DisplayPos = Pos2D<int>;
using DoublePos = Pos2D<long double>;

enum tile_types {
    tile_background = -2,
    tile_undefined,
    tile_empty = 0,
    tile_cyan,
    tile_box,
    tile_wall,
    tile_destination,
    tile_gem = 5,
    tile_picture,
    tile_go_to,
    tile_blue,
    tile_spike,
    tile_conveyor = 10,
    tile_robot,
    tile_imitate
};

enum text_renderer_types {
    text_renderer_solid = 0,
    text_renderer_blended
};

using c_str = const char *;
using direction_t = SDL_Keycode;

using help_map_t = map<string, string>;

using key_pos_map_t = const unordered_map<SDL_Keycode, DisplayPos>;
extern key_pos_map_t key_pos_map;

using usable_languages_t = const vector<string>;
extern usable_languages_t usable_languages;

using type_names_t = const unordered_map<tile_types, string>;
extern type_names_t type_names;

extern json empty_json;

extern unsigned int debugger_code;

unsigned int get_debugger_code();

SDL_Rect get_srcrect(const SDL_Surface *surface);

SDL_Rect get_dstrect(const DisplayPos &pos, const SDL_Surface *surface);

SDL_Rect get_dstrect(const DisplayPos &pos, const SDL_Surface *surface, long double stretch_ratio);

SDL_Rect get_dstrect(const DisplayPos &pos, const SDL_Rect &srcrect, long double stretch_ratio = 1);

bool is0(long double ld);

int sym(long double ld);

void output(DoublePos pos);

direction_t invert(direction_t dir);


#endif //CLOSS_INT_CONST_H

//
// Created by Dogs-Cute on 12/11/2022.
//

#ifndef CLOSS_INT_LOADER_H
#define CLOSS_INT_LOADER_H

#define OPEN_CLOSS_ROOM(path) open_room("Closs/" path)
#define USER_SENSITIVITY (int)current_user.at(USER_K_SENSITIVITY)
#define USER_ROOM (int)current_user.at(USER_K_ROOM)
#define USER_LANG (string)current_user.at(USER_K_LANGUAGE)
#define USER_FRAMERATE (int)current_user.at(USER_K_FRAMERATE)
#define USER_LEVELS (int)current_user.at(USER_K_LEVELS)
#define USER_BONUS_LEVELS (int)current_user.at(USER_K_BONUS_LEVELS)
#define USER_ANIMATION_SPEED (long double)current_user.at(USER_K_ANIMATION_SPEED)
#define USER_BONUS current_user.at(USER_K_BONUS)

#define ROOM_IS_NUMBER (current_user.at(USER_K_ROOM).is_number_integer())

#include "closs.h"

extern json default_user, current_user;
extern json txt_settings, txt_lobby, txt_in_game, txt_manual;
extern string play_name;

void init_default_user();

void init_txt();

void load_user();

void save_user();

void set_user_to_default();

RoomType open_room(const string &room_path);

int get_default_each(size_t w, size_t h);

RoomType create_room(const json &room_json);

void load_json_space(const json &json_space, const TilePos &pos, SpaceType space);

RoomType json_to_room(const json &room_json);

json room_to_json(RoomType room);

void close_room(RoomType room);

void shift_language(bool down);

void shift_sensitivity(bool down);

void shift_framerate(bool down);

void shift_text_renderer(bool down);

void shift_levels(bool down, int amount = 1);

void shift_bonus_levels(bool down);

void shift_animation(bool down);

void debug_unlock_levels();

bool can_shift_to_level(int level);

bool is_second_play();

bool is_perf_play();

bool is_gem_play();

string get_room_path();

SDL_Surface *create_text(const json &txt, int size, const SDL_Color &color = WHITE, const string &addition = "");

SDL_Surface *create_text(const json &txt, int size, bool b, const string &addition = "");

SDL_Surface *create_text(const string &str, int size, const SDL_Color &color);

bool contains_literal(const json &array, const json &value);

#endif //CLOSS_INT_LOADER_H

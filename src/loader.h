//
// Created by Dogs-Cute on 12/11/2022.
//

#ifndef CLOSS_INT_LOADER_H
#define CLOSS_INT_LOADER_H

#define OPEN_CLOSS_ROOM(path) open_room("Closs/" path)
#define USER_SENSITIVITY (int)((int)current_user.at("sensitivity") * stretch_ratio)

#include "img.h"

extern json default_user, current_user;

void init_default_user();

void load_user();

void save_user();

void set_user_to_default();


RoomType open_room(const string &room_path);

int get_default_each(size_t w, size_t h);

RoomType json_to_room(const json &room_json);

json room_to_json(RoomType room);

void close_room(RoomType room);

#endif //CLOSS_INT_LOADER_H

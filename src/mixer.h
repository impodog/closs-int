//
// Created by Dogs-Cute on 3/25/2023.
//

#ifndef CLOSS_INT_MIXER_H
#define CLOSS_INT_MIXER_H

#include "loader.h"

enum music_index {
    mus_none = 0,
    mus_chapter1,
    mus_chapter2,
    mus_chapter3,
    mus_chapter4,
    mus_chapter5_front,
    mus_chapter5_back,
    mus_chapter5plusplus_front,
    mus_chapter5plusplus_back,
    mus_bonus1,
    mus_bonus2
};

extern vector<string> scheme_names;

void init_mixer();

void free_mixer();

void play_room_music(int room);

void music_switch_to(music_index index);

void shift_music_scheme(bool down);

#endif //CLOSS_INT_MIXER_H

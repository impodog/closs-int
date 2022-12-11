//
// Created by Dogs-Cute on 12/11/2022.
//

#ifndef CLOSS_INT_LOADER_H
#define CLOSS_INT_LOADER_H

#include "closs.h"

extern json default_user, current_user;

void init_default_user();

void load_user();

void save_user();

void set_user_to_default();

#endif //CLOSS_INT_LOADER_H

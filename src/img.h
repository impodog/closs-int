//
// Created by Dogs-Cute on 12/11/2022.
//

#ifndef CLOSS_INT_IMG_H
#define CLOSS_INT_IMG_H

#include "closs.h"

using types_img_map_t = unordered_map<tile_types, SDL_Surface *>;

using FontType = TTF_Font *;

using Font_Map = unordered_map<int, FontType>;

class Font_Family : public Font_Map {
private:
	string m_file;
public:
	explicit Font_Family(string file);
	
	FontType at(int size);
};

extern types_img_map_t types_img_map;

extern Font_Family arial;

void init_img_map();

void free_img_map();

void set_white_as_colorkey(SDL_Surface *surface, ...);

SDL_Rect get_srcrect(const SDL_Surface *surface);

#endif //CLOSS_INT_IMG_H

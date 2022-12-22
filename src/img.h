//
// Created by Dogs-Cute on 12/11/2022.
//

#ifndef CLOSS_INT_IMG_H
#define CLOSS_INT_IMG_H

#include "const.h"

#define FONT_SIZE(default) (int) (default * stretch_ratio)

using types_img_map_t = unordered_map<tile_types, SDL_Surface *>;

using FontType = TTF_Font *;

using Font_Map = unordered_map<int, FontType>;

class Font_Family : public Font_Map {
private:
	string m_file;
public:
	explicit Font_Family(const string &file);
	
	~Font_Family();
	
	FontType sized(int size);
};

extern types_img_map_t types_img_map;

extern Font_Family arial, consolas, simhei;

using SDL_Surface_ptr = SDL_Surface *;
extern SDL_Surface_ptr img_arrow, img_settings, img_Closs_InT, img_help, img_manual, img_levels;
extern SDL_Surface_ptr img_none;

using language_fonts_t = unordered_map<string, Font_Family *>;
extern language_fonts_t language_fonts;

void init_img();

void free_img();

void set_white_as_colorkey(const vector<SDL_Surface *> &surfaces);

void show_surface(SDL_Renderer *renderer, SDL_Surface *surface, const DisplayPos &pos);

uint32_t MapRGB(SDL_Surface *surface, const SDL_Color &color);

#endif //CLOSS_INT_IMG_H

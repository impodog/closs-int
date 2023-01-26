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

class Font_Family {
private:
    string m_file;
    Font_Map m_font_map;
public:
    explicit Font_Family(const string &file);

    ~Font_Family();

    FontType sized(int size);
};

extern types_img_map_t types_img_map;

using Font_Family_Type = Font_Family *;
extern Font_Family_Type arial, consolas, simhei;

using SDL_Surface_ptr = SDL_Surface *;
extern SDL_Surface_ptr img_arrow, img_settings, img_Closs_InT, img_help, img_manual, img_levels;
extern SDL_Surface_ptr img_none, img_icon;
extern SDL_Surface_ptr img_chapter1, img_chapter2, img_chapter3, img_chapter4;
extern SDL_Surface_ptr img_bonus1;

using language_fonts_t = unordered_map<string, Font_Family *>;
extern language_fonts_t language_fonts;

using text_renderer_t = SDL_Surface *(*)(TTF_Font *, const char *, SDL_Color);
extern text_renderer_t text_renderer;
using text_renderer_map_t = map<text_renderer_types, text_renderer_t>;
extern text_renderer_map_t text_renderer_map;

using direction_img_t = unordered_map<direction_t, SDL_Surface *>;
extern direction_img_t direction_img_conveyor, direction_img_robot;

using direction_img_vec_t = vector<direction_img_t *>;
extern direction_img_vec_t direction_img_vec;

void init_img();

void free_img();

void init_font();

void free_font();

void set_white_as_colorkey(initializer_list<SDL_Surface *> surfaces);

void show_surface(SDL_Renderer *renderer, SDL_Surface *surface, const DisplayPos &pos);

uint32_t MapRGB(SDL_Surface *surface, const SDL_Color &color);

#endif //CLOSS_INT_IMG_H

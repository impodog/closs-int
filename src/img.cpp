//
// Created by Dogs-Cute on 12/11/2022.
//

#include "img.h"

#include <utility>

types_img_map_t types_img_map;

Font_Family::Font_Family(const string &file) {
    m_file = file;
}

Font_Family::~Font_Family() {
    for (auto pair: *this)
        TTF_CloseFont(pair.second);
    clear();
    Font_Map::~unordered_map();
}

FontType Font_Family::sized(int size) {
    try {
        return Font_Map::at(size);
    } catch (const out_of_range &) {
        auto font = TTF_OpenFont(m_file.c_str(), size);
        Font_Map::insert({size, font});
        return font;
    }
}

Font_Family arial(ARIAL_PATH), consolas(CONSOLAS_PATH), simhei(SIMHEI_PATH);

SDL_Surface_ptr img_arrow, img_settings, img_Closs_InT, img_help, img_manual, img_levels;
SDL_Surface_ptr img_none;
SDL_Surface_ptr img_chapter1;

language_fonts_t language_fonts = {
        {"en",    &arial},
        {"zh_cn", &simhei}
};

text_renderer_t text_renderer;

text_renderer_map_t text_renderer_map = {
        {text_renderer_solid,   TTF_RenderUTF8_Solid},
        {text_renderer_blended, TTF_RenderUTF8_Blended}
};

void init_img() {
    IMG_Init(IMG_INIT_PNG);

    // load tile graphics
    auto img_tile_background = IMG_Load(IMG_PATH "tile_background.png"),
            img_undefined = IMG_Load(IMG_PATH "undefined.png"),
            img_cyan = IMG_Load(IMG_PATH "cyan.png"),
            img_box = IMG_Load(IMG_PATH "box.png"),
            img_wall = IMG_Load(IMG_PATH "wall.png"),
            img_dest = IMG_Load(IMG_PATH "dest.png"),
            img_gem = IMG_Load(IMG_PATH "gem.png"),
            img_box2 = IMG_Load(IMG_PATH "box2.png"),
            img_go_to = IMG_Load(IMG_PATH "go_to.png");
    // load global graphics
    img_arrow = IMG_Load(IMG_PATH "arrow.png");
    img_settings = IMG_Load(IMG_PATH "settings.png");
    img_Closs_InT = IMG_Load(IMG_PATH "Closs_InT.png");
    img_help = IMG_Load(IMG_PATH "help.png");
    img_manual = IMG_Load(IMG_PATH "manual.png");
    img_levels = IMG_Load(IMG_PATH "levels.png");
    img_chapter1 = IMG_Load(IMG_PATH "chapter1.png");

    img_none = SDL_CreateRGBSurface(0, 1, 1, 32, 0, 0, 0, 0);


    // set color keys
    set_white_as_colorkey({
                                  img_undefined,
                                  img_box,
                                  img_wall,
                                  img_dest,
                                  img_gem,
                                  img_go_to,

                                  img_box2,

                                  img_arrow,
                                  img_settings,
                                  img_Closs_InT,
                                  img_help,
                                  img_manual,
                                  img_levels,

                                  img_chapter1
                          });

    // put into image map
    types_img_map[tile_background] = img_tile_background;
    types_img_map[tile_undefined] = img_undefined;
    types_img_map[tile_cyan] = img_cyan;
    types_img_map[tile_box] = img_box;
    types_img_map[tile_wall] = img_wall;
    types_img_map[tile_destination] = img_dest;
    types_img_map[tile_gem] = img_gem;
    types_img_map[tile_picture] = img_none;
    types_img_map[tile_go_to] = img_go_to;
}

void free_img() {
    vector<SDL_Surface *> freed_surface;
    for (auto pair: types_img_map)
        if (find(freed_surface.begin(), freed_surface.end(), pair.second) != freed_surface.end()) {
            SDL_FreeSurface(pair.second);
            freed_surface.push_back(pair.second);
        }
    types_img_map.clear();
    SDL_FreeSurface(img_settings);
    SDL_FreeSurface(img_arrow);
    SDL_FreeSurface(img_Closs_InT);
    SDL_FreeSurface(img_help);
    SDL_FreeSurface(img_manual);
    SDL_FreeSurface(img_levels);
    SDL_FreeSurface(img_none);
    SDL_FreeSurface(img_chapter1);
}

void set_white_as_colorkey(const vector<SDL_Surface *> &surfaces) {
    for (auto surface: surfaces)
        SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 255, 255));
}

void show_surface(SDL_Renderer *renderer, SDL_Surface *surface, const DisplayPos &pos) {
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    auto srcrect = get_srcrect(surface), dstrect = get_dstrect(pos, surface);
    SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
    SDL_DestroyTexture(texture);
}

uint32_t MapRGB(SDL_Surface *surface, const SDL_Color &color) {
    return SDL_MapRGB(surface->format, color.r, color.g, color.b);
}

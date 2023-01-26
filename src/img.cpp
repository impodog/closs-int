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
    for (auto pair: m_font_map) {
        TTF_CloseFont(pair.second);
    }
}

FontType Font_Family::sized(int size) {
    try {
        return m_font_map.at(size);
    } catch (const out_of_range &) {
        auto font = TTF_OpenFont(m_file.c_str(), size);
        m_font_map[size] = font;
        return font;
    }
}

Font_Family_Type arial, consolas, simhei;

SDL_Surface_ptr img_arrow, img_settings, img_Closs_InT, img_help, img_manual, img_levels;
SDL_Surface_ptr img_none, img_icon;
SDL_Surface_ptr img_chapter1, img_chapter2, img_chapter3, img_chapter4;
SDL_Surface_ptr img_bonus1;

language_fonts_t language_fonts;

text_renderer_t text_renderer;

text_renderer_map_t text_renderer_map = {
        {text_renderer_solid,   TTF_RenderUTF8_Solid},
        {text_renderer_blended, TTF_RenderUTF8_Blended}
};

direction_img_t direction_img_conveyor, direction_img_robot;
direction_img_vec_t direction_img_vec = {
        &direction_img_conveyor,
        &direction_img_robot
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
            img_go_to = IMG_Load(IMG_PATH "go_to.png"),
            img_blue = IMG_Load(IMG_PATH "blue.png"),
            img_box3 = IMG_Load(IMG_PATH "box3.png"),
            img_dest_new = IMG_Load(IMG_PATH "dest_new.png"),
            img_spike = IMG_Load(IMG_PATH "spike.png"),
            img_conveyor_up = IMG_Load(IMG_PATH "conveyor_up.png"),
            img_conveyor_lt = IMG_Load(IMG_PATH "conveyor_lt.png"),
            img_conveyor_dw = IMG_Load(IMG_PATH "conveyor_dw.png"),
            img_conveyor_rt = IMG_Load(IMG_PATH "conveyor_rt.png"),
            img_robot_up = IMG_Load(IMG_PATH "robot_up.png"),
            img_robot_lt = IMG_Load(IMG_PATH "robot_lt.png"),
            img_robot_dw = IMG_Load(IMG_PATH "robot_dw.png"),
            img_robot_rt = IMG_Load(IMG_PATH "robot_rt.png");
    // load global graphics
    img_arrow = IMG_Load(IMG_PATH "arrow.png");
    img_settings = IMG_Load(IMG_PATH "settings.png");
    img_Closs_InT = IMG_Load(IMG_PATH "Closs_InT.png");
    img_help = IMG_Load(IMG_PATH "help.png");
    img_manual = IMG_Load(IMG_PATH "manual.png");
    img_levels = IMG_Load(IMG_PATH "levels.png");
    img_icon = IMG_Load(IMG_PATH "C_InT_Icon.png");
    img_chapter1 = IMG_Load(IMG_PATH "chapter1.png");
    img_chapter2 = IMG_Load(IMG_PATH "chapter2.png");
    img_chapter3 = IMG_Load(IMG_PATH "chapter3.png");
    img_chapter4 = IMG_Load(IMG_PATH "chapter4.png");
    img_bonus1 = IMG_Load(IMG_PATH "bonus1.png");

    img_none = SDL_CreateRGBSurface(0, 1, 1, 32, 0, 0, 0, 0);


    // set color keys
    set_white_as_colorkey({
                                  img_undefined,
                                  img_box,
                                  img_wall,
                                  img_dest,
                                  img_gem,
                                  img_box2,
                                  img_go_to,
                                  img_blue,
                                  img_box3,
                                  img_dest_new,
                                  img_spike,
                                  img_conveyor_up,
                                  img_conveyor_lt,
                                  img_conveyor_dw,
                                  img_conveyor_rt,
                                  img_robot_up,
                                  img_robot_lt,
                                  img_robot_dw,
                                  img_robot_rt,


                                  img_arrow,
                                  img_settings,
                                  img_Closs_InT,
                                  img_help,
                                  img_manual,
                                  img_levels,

                                  img_icon,

                                  img_chapter1,
                                  img_chapter2,
                                  img_chapter3,

                                  img_bonus1
                          });

    // put into image map
    types_img_map[tile_background] = img_tile_background;
    types_img_map[tile_undefined] = img_undefined;
    types_img_map[tile_cyan] = img_cyan;
    types_img_map[tile_box] = img_box3;
    types_img_map[tile_wall] = img_wall;
    types_img_map[tile_destination] = img_dest_new;
    types_img_map[tile_gem] = img_gem;
    types_img_map[tile_picture] = img_none;
    types_img_map[tile_go_to] = img_go_to;
    types_img_map[tile_blue] = img_blue;
    types_img_map[tile_spike] = img_spike;
    types_img_map[tile_conveyor] = img_none;
    types_img_map[tile_robot] = img_none;
    // direction_img map
    direction_img_conveyor = {
            {KEY_MOVE_UP,    img_conveyor_up},
            {KEY_MOVE_LEFT,  img_conveyor_lt},
            {KEY_MOVE_DOWN,  img_conveyor_dw},
            {KEY_MOVE_RIGHT, img_conveyor_rt}
    };
    direction_img_robot = {
            {KEY_MOVE_UP,    img_robot_up},
            {KEY_MOVE_LEFT,  img_robot_lt},
            {KEY_MOVE_DOWN,  img_robot_dw},
            {KEY_MOVE_RIGHT, img_robot_rt}
    };
    // Free unused
    SDL_FreeSurface(img_box);
    SDL_FreeSurface(img_box2);
    SDL_FreeSurface(img_dest);
}

void free_img() {
    vector<SDL_Surface *> freed_surface;
    for (auto pair: types_img_map)
        if (find(freed_surface.begin(), freed_surface.end(), pair.second) == freed_surface.end()) {
            SDL_FreeSurface(pair.second);
            freed_surface.push_back(pair.second);
        }
    types_img_map.clear();
    for (auto direction_img: direction_img_vec) {
        for (auto pair: *direction_img)
            if (find(freed_surface.begin(), freed_surface.end(), pair.second) == freed_surface.end()) {
                SDL_FreeSurface(pair.second);
                freed_surface.push_back(pair.second);
            }
        direction_img->clear();
    }
    direction_img_vec.clear();
    SDL_FreeSurface(img_settings);
    SDL_FreeSurface(img_arrow);
    SDL_FreeSurface(img_Closs_InT);
    SDL_FreeSurface(img_help);
    SDL_FreeSurface(img_manual);
    SDL_FreeSurface(img_levels);
    SDL_FreeSurface(img_icon);
    SDL_FreeSurface(img_chapter1);
    SDL_FreeSurface(img_chapter2);
    SDL_FreeSurface(img_chapter3);
    SDL_FreeSurface(img_chapter4);
    SDL_FreeSurface(img_bonus1);
}

void init_font() {
    arial = new Font_Family(ARIAL_PATH);
    consolas = new Font_Family(CONSOLAS_PATH);
    simhei = new Font_Family(SIMHEI_PATH);
    language_fonts = {
            {"en",    arial},
            {"zh_cn", simhei}
    };
}

void free_font() {
    delete arial;
    delete consolas;
    delete simhei;
}

void set_white_as_colorkey(initializer_list<SDL_Surface *> surfaces) {
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


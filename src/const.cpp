//
// Created by Dogs-Cute on 12/10/2022.
//

#include "const.h"
#include "img.h"

#if defined _WIN32

void get_scr_info(Screen_Info &info) {
    info.w = GetSystemMetrics(SM_CXSCREEN);
    info.h = GetSystemMetrics(SM_CYSCREEN);
}

#elif defined __linux__

void get_scr_info(Screen_Info &info){
    int fd;
    struct fb_var_screeninfo screen_info;
    fd = open("/dev/fb0",O_RDWR);
    ioctl(fd,FBIOGET_VSCREENINFO,&screen_info);
    info.w = screen_info.xres;
    info.h = screen_info.yres;
    close(fd);
}

#endif

key_pos_map_t key_pos_map = {
        {KEY_MOVE_UP,    {0,  -1}},
        {KEY_MOVE_LEFT,  {-1, 0}},
        {KEY_MOVE_DOWN,  {0,  1}},
        {KEY_MOVE_RIGHT, {1,  0}}
};

usable_languages_t usable_languages = {
        "en",
        "zh_cn"
};


type_names_t type_names = {
        {tile_undefined,   "Undef"},
        {tile_empty,       "Empty"},
        {tile_cyan,        "Cyan"},
        {tile_box,         "Box"},
        {tile_wall,        "Wall"},
        {tile_destination, "Dest"}
};


SDL_Rect get_srcrect(const SDL_Surface *surface) {
    return {0, 0, surface->w, surface->h};
}

SDL_Rect get_dstrect(const DisplayPos &pos, const SDL_Surface *surface) {
    return {pos.w, pos.h, surface->w, surface->h};
}

json empty_json;
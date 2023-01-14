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
        {tile_destination, "Dest"},
        {tile_gem,         "Gem"},
        {tile_picture,     "Pic"},
        {tile_go_to,       "Goto"},
        {tile_blue,        "Blue"}
};


SDL_Rect get_srcrect(const SDL_Surface *surface) {
    return {0, 0, surface->w, surface->h};
}

SDL_Rect get_dstrect(const DisplayPos &pos, const SDL_Surface *surface) {
    return {pos.w, pos.h, surface->w, surface->h};
}

bool is0(long double ld) {
    return fabs(ld) < std::numeric_limits<long double>::epsilon();
}

int sym(long double ld) {
    if (is0(ld)) return 0;
    return ld > 0 ? 1 : -1;
}

void output(DoublePos pos) {
    cout << pos.w << "," << pos.h << endl;
}

direction_t invert(direction_t dir) {
    switch (dir) {
        case KEY_MOVE_UP:
            return KEY_MOVE_DOWN;
        case KEY_MOVE_LEFT:
            return KEY_MOVE_RIGHT;
        case KEY_MOVE_DOWN:
            return KEY_MOVE_UP;
        case KEY_MOVE_RIGHT:
            return KEY_MOVE_LEFT;
        default:
            throw runtime_error("Direction not a keycode");
    }
}

json empty_json;
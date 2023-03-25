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

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

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
        {tile_blue,        "Blue"},
        {tile_spike,       "Spike"},
        {tile_conveyor,    "Conv"},
        {tile_robot,       "Robot"}
};

unsigned int debugger_code;

unsigned int get_debugger_code() {
    long long result = 0;
    default_random_engine engine;
    for (size_t i = 0; i < sizeof VERSION / sizeof(char); i++) {
        engine.seed(VERSION[i]);
        for (size_t j = 0; j < VERSION[i]; j++)
            if (j % 3 == 0)
                result += engine();
            else
                result -= engine();
    }
    return result;
}

SDL_Rect get_srcrect(const SDL_Surface *surface) {
    return {0, 0, surface->w, surface->h};
}

SDL_Rect get_dstrect(const DisplayPos &pos, const SDL_Surface *surface) {
    return {pos.w, pos.h, surface->w, surface->h};
}

SDL_Rect get_dstrect(const DisplayPos &pos, const SDL_Surface *surface, long double stretch_ratio) {
    return {pos.w, pos.h, (int) (surface->w * stretch_ratio), (int) (surface->h * stretch_ratio)};
}

SDL_Rect get_dstrect(const DisplayPos &pos, const SDL_Rect &srcrect, long double stretch_ratio) {
    return {pos.w, pos.h, (int) (srcrect.w * stretch_ratio), (int) (srcrect.h * stretch_ratio)};
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
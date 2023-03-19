//
// Created by Dogs-Cute on 12/10/2022.
//

#ifndef CLOSS_INT_DISPLAY_H
#define CLOSS_INT_DISPLAY_H

#include "loader.h"

#undef main

#define ROOM_EACH m_room->m_each
#define KEY_UP(code) init_key_map(code)
#define KEY_DOWN(code) key_down(code)
#define IS_GAMING (m_page == nullptr)

extern key_down_map_t key_down_map;
extern key_down_map_t key_clicking_map;
extern key_down_map_t key_click_map;

void init_pages();

void free_pages();

void reload_pages();

void init_display();

void free_display();

void init_key_map();

void init_key_map(SDL_Keycode code);

void init_key_map(const initializer_list<SDL_Keycode> &code);

void init_key_map(SDL_Keycode begin, SDL_Keycode end);

void key_down(SDL_Keycode code);

bool key_d(SDL_Keycode code);

bool key_d(initializer_list<SDL_Keycode> codes);

bool key_clicking(SDL_Keycode code);

bool key_c(SDL_Keycode code);

bool key_c(initializer_list<SDL_Keycode> codes);

SDL_Surface *create_settings_text(const string &setting, const string &from_user, bool b);

SDL_Surface *create_lobby_text(const string &lobby, bool b);


class Page {
public:
    virtual void show() = 0;

    virtual void process() = 0;

    ~Page() = default;
};

using PageType = Page *;
using PageConst = const Page *;

extern PageType page_settings, page_lobby, page_levels, page_manual;

using level_pic_map_t = map<int, PageType>;
extern level_pic_map_t level_pic_map;
extern SDL_Keycode cur_key;

PageType create_text_page(SDL_Surface *, json &, bool);

class Display {
public:
    bool m_loop = true, m_debugger;
    int m_sensitivity;
    Uint32 m_delay;
    long double stretch_ratio, animation_speed, framerate_ratio;
    system_clock::time_point m_last;

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;

    img_map_t img_map;
    DisplayPos m_room_pos, m_room_min, m_room_max;
    RoomType m_room = nullptr;
    RoomType m_debug_room[9];
    PageType m_page = nullptr;

    map<string, long long> debug_prompt;

    Display();

    ~Display();

    void apply_settings();

    void collect_loop_info();

    void process_room_winning();

    void process_room();

    void process_content();

    void switch_color(const SDL_Color &color) const;

    void switch_color_fill(const SDL_Color &color) const;

    void switch_color_fill(const SDL_Color &color, const SDL_Rect &dstrect) const;

    void present() const;

    void change_room(RoomType room);

    void change_page(PageType page);

    SDL_Rect get_rect(const SDL_Surface *img, const TilePos &pos) const;

    DisplayPos get_center(const TilePos &pos) const;

    void show_tile(Tile &tile);

    DisplayPos show_img(SDL_Surface *img, const TilePos &pos, const DisplayPos &addition = {0, 0});

    void show_room_info() const;

    void show_room_winning() const;

    void show_room();

    void show_reservation_line() const;

    void clear_img_vec();

    void clear_room();

    void return_to_game();

    void move_room_to_visible();

    static void refresh_key_m();

    img_info &find_info(SDL_Surface *surface);

    void prompt(const string &s);

    void show_prompt() const;

    void debug_room();
};

using DisplayType = Display *;
extern DisplayType display;

void start_game();

void refresh_user_game();

#endif //CLOSS_INT_DISPLAY_H

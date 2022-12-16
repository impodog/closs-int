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

struct img_info {
	SDL_Texture *texture;
	SDL_Rect srcrect;
};
using img_vec_t = unordered_map<SDL_Surface *, img_info>;

extern key_down_map_t key_down_map;
extern key_down_map_t key_clicking_map;
extern key_down_map_t key_click_map;

void init_pages();

void free_pages();

void init_display();

void free_display();

void init_key_map();

void init_key_map(direction_t code);

void init_key_map(const vector<direction_t>& code);

void init_key_map(direction_t begin, direction_t end);

void key_down(direction_t code);

bool key_d(direction_t code);

bool key_clicking(direction_t code);

bool key_c(direction_t code);

SDL_Surface *create_settings_text(const string &setting, const string &from_user, bool b);

SDL_Surface *create_lobby_text(const string &lobby, bool b);


class Page {
public:
	virtual void show() = 0;
	
	virtual void process() = 0;
};

using PageType = Page *;
using PageConst = const Page *;

using selection_surface_generator_t = SDL_Surface *(*)(bool);
using selection_generators_t = vector<selection_surface_generator_t>;
using selection_generators_const_t = const vector<selection_surface_generator_t> &;

using selection_click_processor_t = void (*)();
using selection_processors_t = vector<selection_click_processor_t>;
using selection_processors_const_t = const vector<selection_click_processor_t> &;
using text_sides_t = vector<SDL_Surface *>;

class Selection_Page : public Page {
public:
	SDL_Surface *m_title;
	DisplayPos m_title_pos;
	
	selection_generators_t m_generators;
	selection_processors_t m_processors;
	
	size_t m_index = 0, m_size;
	
	int m_each;
	
	Selection_Page(SDL_Surface *title, selection_generators_const_t selections, selection_processors_const_t processors,
	               int each = SELECTION_DEFAULT_EACH);
	
	void show() override;
	
	void process() override;
};

using Selection_Page_Type = Selection_Page *;
using Selection_Page_Const = const Selection_Page *;

extern Selection_Page_Type page_settings, page_lobby;


class Text_Page : public Page {
public:
	bool m_release;
	
	SDL_Surface *m_title;
	DisplayPos m_title_pos;
	
	text_sides_t m_sides;
	size_t m_index = 0, m_size;
	
	string m_language = current_user.at(USER_K_LANGUAGE);
	
	Text_Page(SDL_Surface *title, json &help_map, bool release);
	
	~Text_Page();
	
	void show() override;
	
	void process() override;
};

using Text_Page_Type = Text_Page *;
using Text_Page_Const = const Text_Page *;
extern Text_Page_Type page_manual;

class Display {
public:
	bool m_loop = true;
	Uint32 m_delay;
	double stretch_ratio;
	system_clock::time_point m_last;
	
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Event event;
	
	img_vec_t img_vec;
	DisplayPos m_room_pos, m_room_min, m_room_max;
	RoomType m_room = nullptr;
	PageType m_page = nullptr;
	
	Display();
	
	~Display();
	
	void apply_settings();
	
	void collect_loop_info();
	
	void process_room_winning() const;
	
	void process_room();
	
	void process_content();
	
	void switch_color(const SDL_Color &color) const;
	
	void switch_color_fill(const SDL_Color &color) const;
	
	void switch_color_fill(const SDL_Color &color, const SDL_Rect &dstrect) const;
	
	void present() const;
	
	void change_room(RoomType room);
	
	void change_page(PageType page);
	
	SDL_Rect tile_rect(const Tile &tile) const;
	
	SDL_Rect get_rect(const SDL_Surface *img, const TilePos &pos) const;
	
	DisplayPos get_center(const TilePos &pos) const;
	
	void show_tile(const Tile &tile);
	
	DisplayPos show_img(SDL_Surface *img, const TilePos &pos);
	
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
	
};

using DisplayType = Display *;
extern DisplayType display;

void start_game();

#endif //CLOSS_INT_DISPLAY_H

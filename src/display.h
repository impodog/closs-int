//
// Created by Dogs-Cute on 12/10/2022.
//

#ifndef CLOSS_INT_DISPLAY_H
#define CLOSS_INT_DISPLAY_H

#include "loader.h"

#undef main

#define ROOM_EACH m_room->m_each
#define KEY_UP(codes) init_key_map(codes)
#define KEY_DOWN(codes) key_down(codes)
struct img_info {
	SDL_Texture *texture;
	SDL_Rect srcrect;
};
using img_vec_t = unordered_map<SDL_Surface *, img_info>;
using key_down_map_t = unordered_map<SDL_Keycode, bool>;

extern key_down_map_t key_down_map;
extern key_down_map_t key_click_map;

void init_key_map();

void init_key_map(SDL_Keycode code, ...);

void init_key_map(SDL_Keycode begin, SDL_Keycode end);

void key_down(SDL_Keycode code, ...);

bool key_d(SDL_Keycode code);

bool key_c(SDL_Keycode code);

class Display {
public:
	bool m_loop = true, m_gaming = false;
	Uint32 m_delay;
	double stretch_ratio;
	system_clock::time_point m_last;
	
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Event event;
	
	img_vec_t img_vec;
	DisplayPos m_room_pos;
	RoomType m_room;
	
	Display();
	
	~Display();
	
	void apply_settings();
	
	void collect_loop_info();
	
	void switch_color(const SDL_Color &color) const;
	
	void switch_color_fill(const SDL_Color &color) const;
	
	void present() const;
	
	void change_room(RoomType room);
	
	SDL_Rect tile_rect(const Tile &tile) const;
	
	SDL_Rect get_rect(const SDL_Surface *img, const TilePos &pos) const;
	
	void render_copy_tile(const Tile &tile);
	
	void render_copy_img(SDL_Surface *img, const TilePos &pos);
	
	void render_copy_room();
	
	void clear_img_vec();
	
	img_info &find_info(SDL_Surface *surface);
};


#endif //CLOSS_INT_DISPLAY_H

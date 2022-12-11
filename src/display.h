//
// Created by Dogs-Cute on 12/10/2022.
//

#ifndef CLOSS_INT_DISPLAY_H
#define CLOSS_INT_DISPLAY_H

#include "loader.h"

#undef main

#define ROOM_EACH m_room->m_each

class Display {
public:
	bool m_loop = true, m_gaming = false;
	Uint32 m_delay = 1 / 30;
	double stretch_ratio;
	system_clock::time_point m_last;
	
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Event event;
	
	SDL_Texture *m_background = nullptr;
	SDL_Rect m_background_srcrect;
	
	DisplayPos m_room_pos;
	RoomType m_room;
	
	Display();
	
	~Display();
	
	void collect_loop_info();
	
	void switch_color(const SDL_Color &color) const;
	
	void switch_color_fill(const SDL_Color &color) const;
	
	void present() const;
	
	void change_room(RoomType room);
	
	SDL_Rect tile_rect(const Tile &tile) const;
	
	void render_copy_tile(const Tile &tile) const;
	
	void render_copy_room() const;
};


#endif //CLOSS_INT_DISPLAY_H

//
// Created by Dogs-Cute on 12/10/2022.
//

#include "display.h"

Display::Display() {
	window = SDL_CreateWindow(TITLE,
	                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	                          WIDTH, HEIGHT,
	                          SDL_WINDOW_MAXIMIZED);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	collect_loop_info();
}

void Display::collect_loop_info() {
	auto now = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(now - m_last).count();
	m_last = now;
	
	SDL_PollEvent(&event);
	if (m_delay > duration) SDL_Delay(m_delay - duration);
	switch (event.type) {
		case SDL_QUIT:
			m_gaming = m_loop = false;
			break;
	}
}

void Display::switch_color(const SDL_Color &color) const {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void Display::switch_color_fill(const SDL_Color &color) const {
	switch_color(color);
	SDL_RenderClear(renderer);
}

void Display::present() const {
	SDL_RenderPresent(renderer);
}

void Display::change_room(RoomType room) {
	m_room = room;
	
}

SDL_Rect Display::tile_rect(const Tile &tile) const {
	DisplayPos half_rect{(m_each + tile.m_size.w) / 2, (m_each + tile.m_size.h) / 2};
	return {(int) (m_each * tile.m_pos.w + half_rect.w), (int) (m_each * tile.m_pos.h + half_rect.h),
	        half_rect.w, half_rect.h};
}

void Display::render_copy_tile(const Tile &tile) const {
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, tile.m_img);
	SDL_Rect srcrect = tile.srcrect(), dstrect = tile_rect(tile);
	SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
}
//
// Created by Dogs-Cute on 12/10/2022.
//

#include "display.h"

Display::Display() {
	window = SDL_CreateWindow(TITLE,
	                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	                          SCR_WIDTH, SCR_HEIGHT,
	                          SDL_WINDOW_MAXIMIZED);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	collect_loop_info();
}

Display::~Display() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
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
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
				case SDLK_w:
					m_room_pos.h -= USER_SENSITIVITY;
					break;
				case SDLK_a:
					m_room_pos.w -= USER_SENSITIVITY;
					break;
				case SDLK_s:
					m_room_pos.h += USER_SENSITIVITY;
					break;
				case SDLK_d:
					m_room_pos.w += USER_SENSITIVITY;
					break;
			}
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
	auto total_size = room->total_size();
	m_background_srcrect = {0, 0, total_size.w, total_size.h};
	m_room = room;
	m_room_pos = {0, 0};
	// draw m_background
	if (m_background != nullptr) SDL_DestroyTexture(m_background);
	auto background_surf = SDL_CreateRGBSurface(SDL_SWSURFACE, total_size.w, total_size.h, 32, 0, 0, 0, 0);
	auto color = SDL_MapRGB(background_surf->format, TILE_BACK_COLOR.r, TILE_BACK_COLOR.g, TILE_BACK_COLOR.b);
	for (size_t h = 0; h != m_room->m_size.h; h++) {
		for (size_t w = 0; w != m_room->m_size.w; w++) {
			cout << "w" << w << "h" << h << endl;
			SDL_Rect current = {(int) (ROOM_EACH * w - TILE_BACK_SEP), (int) (ROOM_EACH * h - TILE_BACK_SEP),
			                    ROOM_EACH - TILE_BACK_SEP2, ROOM_EACH - TILE_BACK_SEP2};
		}
	}
	m_background = SDL_CreateTextureFromSurface(renderer, background_surf);
	stretch_ratio = (double) (m_room->m_each) / STANDARD_EACH / 1.5;
}

SDL_Rect Display::tile_rect(const Tile &tile) const {
	DisplayPos half_rect{(int) ((ROOM_EACH + tile.m_img->w * stretch_ratio) / 2),
	                     (int) ((ROOM_EACH + tile.m_img->h * stretch_ratio) / 2)};
	return {(int) ((double) (ROOM_EACH * tile.m_pos.w + half_rect.w + m_room_pos.w)),
	        (int) ((double) (ROOM_EACH * tile.m_pos.h + half_rect.h + m_room_pos.h)),
	        half_rect.w, half_rect.h};
}

void Display::render_copy_tile(const Tile &tile) const {
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, tile.m_img);
	SDL_Rect srcrect = tile.srcrect(), dstrect = tile_rect(tile);
	SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
}

void Display::render_copy_room() const {
	SDL_Rect dstrect = {m_room_pos.w, m_room_pos.h, m_background_srcrect.w, m_background_srcrect.h};
	SDL_RenderCopy(renderer, m_background, &m_background_srcrect, &dstrect);
	for (const auto &lane: *m_room) {
		for (const auto &space: *lane) {
			for (const auto &tile: *space) {
				render_copy_tile(*tile);
			}
		}
	}
}
//
// Created by Dogs-Cute on 12/10/2022.
//

#include "display.h"

key_down_map_t key_down_map;
key_down_map_t key_click_map;

void init_key_map() {
	init_key_map(0, 8, 9, 13, 27);
	init_key_map(32, 64);
	init_key_map(91, 127);
	init_key_map(1073741881, 1073742106);
}

void init_key_map(SDL_Keycode code, ...) {
	key_down_map[code] = key_click_map[code] = false;
}

void init_key_map(SDL_Keycode begin, SDL_Keycode end) {
	for (SDL_Keycode i = begin; i != end; i++) {
		key_down_map[i] = key_click_map[i] = false;
	}
}

void key_down(SDL_Keycode code, ...) {
	key_click_map[code] = key_down_map[code];
	key_down_map[code] = true;
}

bool key_d(SDL_Keycode code) {
	return key_down_map[code];
}

bool key_c(SDL_Keycode code) {
	auto click = key_click_map[code];
	return (key_click_map[code] = key_down_map[code]) != click;
}

Display::Display() {
	window = SDL_CreateWindow(TITLE,
	                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	                          SCR_WIDTH, SCR_HEIGHT,
	                          0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	apply_settings();
	collect_loop_info();
}

Display::~Display() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

void Display::apply_settings() {
	m_delay = 1000 / (int) current_user["framerate"];
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
			KEY_DOWN(event.key.keysym.sym);
			break;
		case SDL_KEYUP:
			KEY_UP(event.key.keysym.sym);
			break;
	}
	if (key_d(KEY_SHIFT_UP)) m_room_pos.h -= USER_SENSITIVITY;
	if (key_d(KEY_SHIFT_LEFT)) m_room_pos.w -= USER_SENSITIVITY;
	if (key_d(KEY_SHIFT_DOWN)) m_room_pos.h += USER_SENSITIVITY;
	if (key_d(KEY_SHIFT_RIGHT)) m_room_pos.w += USER_SENSITIVITY;
}

void Display::play_room() {
	m_room->move_independents(key_c);
	m_room->do_pending_moves();
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
	auto total_size = m_room->total_size();
	m_room_pos = {0, 0};
	stretch_ratio = (double) (m_room->m_each) / STANDARD_EACH / 1.5;
	clear_img_vec();
}

SDL_Rect Display::tile_rect(const Tile &tile) const {
	return get_rect(tile.m_img, tile.m_pos);
}

SDL_Rect Display::get_rect(const SDL_Surface *img, const TilePos &pos) const {
	DisplayPos double_edge_rect{(int) ((ROOM_EACH - img->w * stretch_ratio)),
	                            (int) ((ROOM_EACH - img->h * stretch_ratio))};
	return {(int) ((double) (ROOM_EACH * pos.w + double_edge_rect.w / 2.0 + m_room_pos.w)),
	        (int) ((double) (ROOM_EACH * pos.h + double_edge_rect.h / 2.0 + m_room_pos.h)),
	        ROOM_EACH - double_edge_rect.w, ROOM_EACH - double_edge_rect.h};
}

void Display::render_copy_tile(const Tile &tile) {
	render_copy_img(tile.m_img, tile.m_pos);
}

void Display::render_copy_img(SDL_Surface *img, const TilePos &pos) {
	auto info = find_info(img);
	SDL_Rect dstrect = get_rect(img, pos);
	SDL_RenderCopy(renderer, info.texture, &info.srcrect, &dstrect);
}

void Display::render_copy_room() {
	size_t w, h = 0;
	for (const auto &lane: *m_room) {
		w = 0;
		for (const auto &space: *lane) {
			render_copy_img(types_img_map[tile_background], {w, h});
			for (const auto &tile: *space) {
				render_copy_tile(*tile);
			}
			w++;
		}
		h++;
	}
}

void Display::clear_img_vec() {
	if (!img_vec.empty()) {
		for (auto pair: img_vec) {
			SDL_DestroyTexture(pair.second.texture);
		}
		img_vec.clear();
	}
}

img_info &Display::find_info(SDL_Surface *surface) {
	try {
		return img_vec.at(surface);
	} catch (const out_of_range &) {
		return (img_vec[surface] = {SDL_CreateTextureFromSurface(renderer, surface), get_srcrect(surface)});
	}
}




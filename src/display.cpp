//
// Created by Dogs-Cute on 12/10/2022.
//

#include "display.h"

key_down_map_t key_down_map;
key_down_map_t key_clicking_map;
key_down_map_t key_click_map;


Selection_Page_Type page_settings, page_lobby;

void init_pages() {
	page_settings = new Selection_Page{img_settings,
	                                   {
			                                   [](bool b) {
				                                   return create_settings_text(SETTINGS_K_LANGUAGE, USER_K_LANGUAGE, b);
			                                   },
			                                   [](bool b) {
				                                   return create_settings_text(SETTINGS_K_SENSITIVITY,
				                                                               USER_K_SENSITIVITY,
				                                                               b);
			                                   },
			                                   [](bool b) {
				                                   return create_settings_text(SETTINGS_K_TO_LOBBY, "", b);
			                                   }
	                                   },
	                                   {
			                                   [] {
				                                   if (key_c(KEY_MOVE_RIGHT))
					                                   shift_language(true);
				                                   else if (key_c(KEY_MOVE_LEFT))
					                                   shift_language(false);
			                                   },
			                                   [] {
				                                   if (key_d(KEY_MOVE_RIGHT))
					                                   shift_sensitivity(true);
				                                   else if (key_d(KEY_MOVE_LEFT))
					                                   shift_sensitivity(false);
			                                   },
			                                   [] {
				                                   if (key_c(KEY_CONFIRM)) display->m_page = page_lobby;
			                                   }
	                                   },
	                                   SETTINGS_EACH
	};
	page_lobby = new Selection_Page{img_Closs_InT,
	                                {
			                                [](bool b) {
				                                return create_lobby_text(LOBBY_K_PLAY, b);
			                                },
			                                [](bool b) {
				                                return create_lobby_text(LOBBY_K_SETTINGS, b);
			                                },
			                                [](bool b) {
				                                return create_lobby_text(LOBBY_K_QUIT, b);
			                                }
	                                },
	                                {
			                                [] {
				                                if (key_c(KEY_CONFIRM));//todo
			                                },
			                                [] {
				                                if (key_c(KEY_CONFIRM)) display->m_page = page_settings;
			                                },
			                                [] {
				                                if (key_c(KEY_CONFIRM)) display->m_loop = false;
			                                }
	                                },
	                                LOBBY_EACH
	};
}

void free_pages() {
	delete page_settings;
	delete page_lobby;
}


void init_key_map() {
	init_key_map(0, 8, 9, 13, 27);
	init_key_map(32, 64);
	init_key_map(91, 127);
	init_key_map(1073741881, 1073742106);
	key_click_map = key_down_map_t(key_down_map);
}

void init_key_map(direction_t code, ...) {
	key_down_map[code] = key_clicking_map[code] = false;
}

void init_key_map(direction_t begin, direction_t end) {
	for (direction_t i = begin; i != end; i++) {
		key_down_map[i] = key_clicking_map[i] = false;
	}
}

void key_down(direction_t code, ...) {
	key_clicking_map[code] = key_down_map[code];
	key_down_map[code] = true;
}

bool key_d(direction_t code) {
	return key_down_map[code];
}

bool key_clicking(direction_t code) {
	auto click = key_clicking_map[code];
	return (key_clicking_map[code] = key_down_map[code]) != click;
}

bool key_c(direction_t code) {
	return key_click_map.at(code);
}

SDL_Surface *create_text(const json &txt, int size, bool b, const string &ref) {
	return TTF_RenderUTF8_Solid(language_fonts.at(USER_LANG)->sized(size),
	                            ((string) txt.at(USER_LANG) + ref).c_str(), b ? GREEN : WHITE);
}

SDL_Surface *create_settings_text(const string &setting, const string &from_user, bool b) {
	return create_text(txt_settings.at(setting), SETTINGS_SIZE, b,
	                   from_user.empty() ? "" : to_string(current_user.at(from_user)));
}

SDL_Surface *create_lobby_text(const string &lobby, bool b) {
	return create_text(txt_lobby.at(lobby), LOBBY_SIZE, b);
}

void init_display() {
	display = new Display;
}

void free_display() {
	delete display;
}


Selection_Page::Selection_Page(SDL_Surface *title, selection_generators_const_t generators,
                               selection_processors_const_t processors, int each) {
	m_title = title;
	m_title_pos = {(SCR_WIDTH - m_title->w) / 2, 0};
	m_generators = generators;
	m_processors = processors;
	m_each = each;
	if (m_processors.size() != (m_size = m_generators.size()))
		throw closs_page_error("Page generator number is not equal to processor number");
}

void Selection_Page::show() {
	show_surface(display->renderer, m_title, m_title_pos);
	for (size_t i = 0; i < m_size; i++) {
		bool is_selected = i == m_index;
		auto surface = m_generators.at(i)(is_selected);
		auto upper_h = (int) (m_each * (i + 1) + m_title->h);
		if (is_selected) show_surface(display->renderer, img_arrow, {0, upper_h + (m_each - img_arrow->h) / 2});
		show_surface(display->renderer, surface, {img_arrow->w, upper_h});
		SDL_FreeSurface(surface);
	}
}

void Selection_Page::process() {
	if (key_c(KEY_MOVE_UP)) m_index = m_index + m_size - 1;
	else if (key_c(KEY_MOVE_DOWN)) m_index++;
	m_index %= m_size;
	m_processors.at(m_index)();
}

Display::Display() {
	window = SDL_CreateWindow(TITLE,
	                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	                          SCR_WIDTH, SCR_HEIGHT,
	                          0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	m_page = page_lobby;
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
			m_loop = false;
			break;
		case SDL_KEYDOWN:
			KEY_DOWN(event.key.keysym.sym);
			break;
		case SDL_KEYUP:
			KEY_UP(event.key.keysym.sym);
			break;
	}
	refresh_key_m();
}

void Display::process_room() {
	if (key_d(KEY_SHIFT_UP)) m_room_pos.h -= USER_SENSITIVITY;
	if (key_d(KEY_SHIFT_LEFT)) m_room_pos.w -= USER_SENSITIVITY;
	if (key_d(KEY_SHIFT_DOWN)) m_room_pos.h += USER_SENSITIVITY;
	if (key_d(KEY_SHIFT_RIGHT)) m_room_pos.w += USER_SENSITIVITY;
	
	move_room_to_visible();
	
	m_room->move_independents(key_c);
	m_room->do_pending_moves();
	m_room->end_of_step();
}

void Display::process_content() {
	if (key_c(KEY_SETTINGS)) {
		if (m_page == nullptr) m_page = page_settings;
		else if (m_room == nullptr) m_page = page_lobby;
		else m_page = nullptr;
	}
	
	if (IS_GAMING) {
		process_room();
		show_room();
	} else {
		m_page->process();
		m_page->show();
	}
}

void Display::switch_color(const SDL_Color &color) const {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void Display::switch_color_fill(const SDL_Color &color) const {
	switch_color(color);
	SDL_RenderClear(renderer);
}

void Display::switch_color_fill(const SDL_Color &color, const SDL_Rect &dstrect) const {
	switch_color(color);
	SDL_RenderFillRect(renderer, &dstrect);
}

void Display::present() const {
	SDL_RenderPresent(renderer);
}

void Display::change_room(RoomType room) {
	m_room = room;
	auto total_size = m_room->total_size();
	DisplayPos m_room_edge = {SCR_WIDTH - total_size.w, RESERVED_HEIGHT - total_size.h};
	m_room_pos = {0, 0};
	m_room_min = {min(m_room_edge.w, 0), min(m_room_edge.h, 0)};
	m_room_max = {max(m_room_edge.w, 0), max(m_room_edge.h, 0)};
	stretch_ratio = (double) (m_room->m_each) / STANDARD_EACH / 1.5;
	clear_img_vec();
}

void Display::change_page(PageType page) {
	m_page = page;
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

DisplayPos Display::get_center(const TilePos &pos) const {
	return {(int) (ROOM_EACH * ((int) pos.w + 0.5)) + m_room_pos.w,
	        (int) (ROOM_EACH * ((int) pos.h + 0.5)) + m_room_pos.h};
}

void Display::show_tile(const Tile &tile) {
	auto pos = show_img(tile.m_img, tile.m_pos);
	tile.show_additional(renderer, pos, get_center(tile.m_pos), stretch_ratio);
}

DisplayPos Display::show_img(SDL_Surface *img, const TilePos &pos) {
	auto info = find_info(img);
	SDL_Rect dstrect = get_rect(img, pos);
	SDL_RenderCopy(renderer, info.texture, &info.srcrect, &dstrect);
	return {dstrect.w, dstrect.h};
}

void Display::show_room() {
	size_t w, h = 0;
	for (const auto &lane: *m_room) {
		w = 0;
		for (const auto &space: *lane) {
			show_img(types_img_map[tile_background], {w, h});
			for (const auto &tile: *space) {
				show_tile(*tile);
			}
			w++;
		}
		h++;
	}
	switch_color_fill(BLACK, {0, RESERVED_HEIGHT, SCR_WIDTH, RESERVED_FROM_B});
	show_reservation_line();
}

void Display::show_reservation_line() const {
	SDL_RenderDrawLine(renderer, 0, RESERVED_HEIGHT, SCR_WIDTH, RESERVED_HEIGHT);
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

void Display::move_room_to_visible() {
	m_room_pos.w = min(max(m_room_pos.w, m_room_min.w), m_room_max.w);
	m_room_pos.h = min(max(m_room_pos.h, m_room_min.h), m_room_max.h);
}

void Display::refresh_key_m() {
	for (auto pair: key_click_map) {
		key_click_map[pair.first] = key_clicking(pair.first);
	}
}

DisplayType display;




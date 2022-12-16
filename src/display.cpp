//
// Created by Dogs-Cute on 12/10/2022.
//

#include "display.h"

key_down_map_t key_down_map;
key_down_map_t key_clicking_map;
key_down_map_t key_click_map;


Selection_Page_Type page_settings, page_lobby;
Text_Page_Type page_manual;

void init_pages() {
	page_settings = new Selection_Page{img_settings,
	                                   {
			                                   [](bool b) { // 1 language
				                                   return create_settings_text(SETTINGS_K_LANGUAGE, USER_K_LANGUAGE, b);
			                                   },
			                                   [](bool b) { // 2 sensitivity
				                                   return create_settings_text(SETTINGS_K_SENSITIVITY,
				                                                               USER_K_SENSITIVITY,
				                                                               b);
			                                   },
			                                   [](bool b) { // 3 framerate
				                                   return create_settings_text(SETTINGS_K_FRAMERATE, USER_K_FRAMERATE,
				                                                               b);
			                                   },
			                                   [](bool b) { // 4 apply
				                                   return create_settings_text(SETTINGS_K_APPLY, "", b);
			                                   },
			                                   [](bool b) { // 5 to_lobby
				                                   return create_settings_text(SETTINGS_K_TO_LOBBY, "", b);
			                                   },
			                                   [](bool b) { // 6 to_game
				                                   return create_settings_text(SETTINGS_K_TO_GAME, "", b);
			                                   }
	                                   },
	                                   {
			                                   [] { // 1 language
				                                   if (key_c(KEY_MOVE_RIGHT))
					                                   shift_language(true);
				                                   else if (key_c(KEY_MOVE_LEFT))
					                                   shift_language(false);
			                                   },
			                                   [] { // 2 sensitivity
				                                   if (key_d(KEY_MOVE_RIGHT))
					                                   shift_sensitivity(true);
				                                   else if (key_d(KEY_MOVE_LEFT))
					                                   shift_sensitivity(false);
			                                   },
			                                   [] { // 3 framerate
				                                   if (key_d(KEY_MOVE_RIGHT))
					                                   shift_framerate(true);
				                                   else if (key_d(KEY_MOVE_LEFT))
					                                   shift_framerate(false);
			                                   },
			                                   [] { // 4 apply
				                                   if (key_c(KEY_CONFIRM)) display->apply_settings();
			                                   },
			                                   [] { // 5 to_lobby
				                                   if (key_c(KEY_CONFIRM)) {
					                                   display->apply_settings();
					                                   display->m_page = page_lobby;
					                                   // Also, returning to lobby will clear room
					                                   display->clear_room();
				                                   }
			                                   },
			                                   [] { // 6 to_game
				                                   if (key_c(KEY_CONFIRM)) {
					                                   display->apply_settings();
					                                   display->return_to_game();
				                                   }
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
				                                return create_lobby_text(LOBBY_K_MANUAL, b);
			                                },
			                                [](bool b) {
				                                return create_lobby_text(LOBBY_K_QUIT, b);
			                                }
	                                },
	                                {
			                                [] {
				                                if (key_c(KEY_CONFIRM)) start_game();
			                                },
			                                [] {
				                                if (key_c(KEY_CONFIRM)) display->m_page = page_settings;
			                                },
			                                [] {
				                                if (key_c(KEY_CONFIRM)) display->m_page = page_manual;
			                                },
			                                [] {
				                                if (key_c(KEY_CONFIRM)) display->m_loop = false;
			                                }
	                                },
	                                LOBBY_EACH
	};
	
	page_manual = new Text_Page{img_manual, txt_manual, false};
}

void free_pages() {
	delete page_settings;
	delete page_lobby;
	delete page_manual;
}

void reload_pages() {
	delete page_manual;
	page_manual = new Text_Page{img_manual, txt_manual, false};
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

void start_game() {
	display->change_room(open_room(get_room_path()));
	display->m_page = nullptr;
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
		show_surface(display->renderer, surface, {img_arrow->w, upper_h + (m_each - surface->h) / 2});
		SDL_FreeSurface(surface);
	}
}

void Selection_Page::process() {
	if (key_c(KEY_MOVE_UP)) m_index = m_index + m_size - 1;
	else if (key_c(KEY_MOVE_DOWN)) m_index++;
	m_index %= m_size;
	m_processors.at(m_index)();
}

Text_Page::Text_Page(SDL_Surface *title, json &help_map, bool release) {
	m_release = release;
	m_title = title;
	m_title_pos = {(SCR_WIDTH - m_title->w) / 2, 0};
	SDL_Surface *surface = nullptr;
	int height = SCR_HEIGHT;
	for (const auto &pair: help_map.at(USER_LANG).get<help_map_t>()) {
		if (height + TEXT_PAGE_EACH > SCR_HEIGHT) {
			surface = SDL_CreateRGBSurface(0, SCR_WIDTH, SCR_HEIGHT - m_title->h, 32, 0, 0, 0, 0);
			height = m_title->h;
			m_sides.push_back(surface);
		}
		
		int content_height = height + (TEXT_PAGE_EACH - TEXT_PAGE_SIZE) / 2;
		auto title_surf = create_text((string) pair.first, TEXT_PAGE_SIZE, WHITE);
		auto descr_surf = create_text((string) pair.second, TEXT_PAGE_SIZE, GREY);
		SDL_Rect title_srcrect = get_srcrect(title_surf),
				descr_srcrect = get_srcrect(descr_surf),
				title_dstrect = get_dstrect({LEAVE_BLANK_WIDTH, content_height}, title_surf),
				descr_dstrect = get_dstrect({LEAVE_BLANK_WIDTH * 2 + title_surf->w, content_height}, descr_surf);
		SDL_BlitSurface(title_surf, &title_srcrect, surface, &title_dstrect);
		SDL_BlitSurface(descr_surf, &descr_srcrect, surface, &descr_dstrect);
		
		height += TEXT_PAGE_EACH;
		SDL_FreeSurface(title_surf);
		SDL_FreeSurface(descr_surf);
	}
}

Text_Page::~Text_Page() {
	for (auto surface: m_sides) SDL_FreeSurface(surface);
	Page::~Page();
}

void Text_Page::show() {
	show_surface(display->renderer, m_title, m_title_pos);
	show_surface(display->renderer, m_sides.at(m_index), {0, m_title->h});
}

void Text_Page::process() {
	if (key_c(KEY_CONFIRM)) {
		display->return_to_game();
		if (m_release) this->~Text_Page();
	}
	if (key_c(KEY_MOVE_LEFT) || key_c(KEY_MOVE_UP)) m_index += 1;
	else if (key_c(KEY_MOVE_RIGHT) || key_c(KEY_MOVE_DOWN)) m_index -= 1;
	m_index %= m_sides.size();
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
	reload_pages();
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

void Display::process_room_winning() const {
	if (m_room->is_winning && key_c(KEY_CONFIRM)) {
		if (m_room->m_next_level.is_number_integer()) {
			int next = (int) m_room->m_next_level;
			current_user[USER_K_ROOM] = next;
			current_user[USER_K_UNLOCKED] = max(next, (int) current_user[USER_K_UNLOCKED]);
		} else if (m_room->m_next_level.is_number_float()) {
			current_user[USER_K_ROOM] = (float) m_room->m_next_level;
		} else {
			current_user[USER_K_ROOM] = (string) m_room->m_next_level;
		}
		start_game();
	}
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
	process_room_winning();
	if (key_d(KEY_HELP)) m_page = new Text_Page(img_help, m_room->m_help_map, true);
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
		if (m_page != nullptr) m_page->show();
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
	clear_room();
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

void Display::show_room_title() const {
	auto surface = create_text(m_room->m_title, ROOM_TITLE_SIZE);
	show_surface(renderer, surface, {LEAVE_BLANK_WIDTH, RESERVED_HEIGHT + (RESERVED_FROM_B - surface->h) / 2});
	SDL_FreeSurface(surface);
}

void Display::show_room_winning() const {
	if (m_room->is_winning) {
		auto surface = create_text(txt_in_game.at(IN_GAME_K_WINNING), WINNING_SIZE, GREEN);
		show_surface(renderer, surface, {WINNING_WIDTH, RESERVED_HEIGHT + (RESERVED_FROM_B - WINNING_SIZE) / 2});
		SDL_FreeSurface(surface);
	}
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
	show_room_title();
	show_room_winning();
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

void Display::clear_room() {
	close_room(m_room);
	m_room = nullptr;
}

void Display::return_to_game() {
	if (m_room == nullptr) m_page = page_lobby;
	else m_page = nullptr;
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


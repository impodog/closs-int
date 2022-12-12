//
// Created by Dogs-Cute on 12/11/2022.
//

#include "img.h"

types_img_map_t types_img_map;

Font_Family::Font_Family(string file) {
	m_file = std::move(file);
}

FontType Font_Family::at(int size) {
	try {
		return Font_Map::at(size);
	} catch (const out_of_range &) {
		return ((*this)[size] = TTF_OpenFont(m_file.c_str(), size));
	}
}

Font_Family arial("arial");

void init_img_map() {
	IMG_Init(IMG_INIT_PNG);
	
	// load graphics
	auto img_tile_background = IMG_Load(IMG_PATH "tile_background.png"),
			img_undefined = IMG_Load(IMG_PATH "undefined.png"),
			img_cyan = IMG_Load(IMG_PATH "cyan.png"),
			img_box = IMG_Load(IMG_PATH "box.png"),
			img_box2 = IMG_Load(IMG_PATH "box2.png");
	// old graphics
	auto img_old_box = IMG_Load(IMG_PATH "old_box.png");
	
	// set color keys
	set_white_as_colorkey(
			img_undefined,
			img_old_box,
			img_box,
			img_box2
	);
	
	// put into image map
	types_img_map[tile_background] = img_tile_background;
	types_img_map[tile_undefined] = img_undefined;
	types_img_map[tile_cyan] = img_cyan;
	types_img_map[tile_box] = img_box;
	types_img_map[tile_box2] = img_box2;
}

void free_img_map() {
	for (auto pair: types_img_map)
		SDL_FreeSurface(pair.second);
	types_img_map.clear();
}

void set_white_as_colorkey(SDL_Surface *surface, ...) {
	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 255, 255));
}

SDL_Rect get_srcrect(const SDL_Surface *surface) {
	return {0, 0, surface->w, surface->h};
}

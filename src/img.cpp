//
// Created by Dogs-Cute on 12/11/2022.
//

#include "img.h"

types_img_map_t types_img_map;

void init_img_map() {
	IMG_Init(IMG_INIT_PNG);
	
	// load into vars
	auto img_undefined = IMG_Load(IMG_PATH "undefined.png");
	auto img_old_box = IMG_Load(IMG_PATH "old_box.png");
	
	// set color keys
	set_white_as_colorkey(
			img_undefined,
			img_old_box
	);
	
	// put into image map
	types_img_map[tile_undefined] = img_undefined;
}

void free_img_map() {
	for (auto pair: types_img_map)
		SDL_FreeSurface(pair.second);
	types_img_map.clear();
}

void set_white_as_colorkey(SDL_Surface *surface, ...) {
	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 255, 255));
}

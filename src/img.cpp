//
// Created by Dogs-Cute on 12/11/2022.
//

#include "img.h"

#include <utility>

types_img_map_t types_img_map;

Font_Family::Font_Family(const string &file) {
	m_file = file;
}

Font_Family::~Font_Family() {
	for (auto pair: *this)
		TTF_CloseFont(pair.second);
	Font_Map::~unordered_map();
}

FontType Font_Family::sized(int size) {
	try {
		return Font_Map::at(size);
	} catch (const out_of_range &) {
		auto font = TTF_OpenFont(m_file.c_str(), size);
		Font_Map::insert({size, font});
		return font;
	}
}

Font_Family arial(ARIAL_PATH), consolas(CONSOLAS_PATH), simhei(SIMHEI_PATH);

SDL_Surface_ptr img_arrow, img_settings, img_Closs_InT, img_help, img_manual, img_levels;

language_fonts_t language_fonts = {
		{"en",    &arial},
		{"zh_cn", &simhei}
};

void init_img() {
	IMG_Init(IMG_INIT_PNG);
	
	// load tile graphics
	auto img_tile_background = IMG_Load(IMG_PATH "tile_background.png"),
			img_undefined = IMG_Load(IMG_PATH "undefined.png"),
			img_cyan = IMG_Load(IMG_PATH "cyan.png"),
			img_box = IMG_Load(IMG_PATH "box.png"),
			img_wall = IMG_Load(IMG_PATH "wall.png"),
			img_dest = IMG_Load(IMG_PATH "dest.png"),
			img_gem = IMG_Load(IMG_PATH "gem.png"),
			img_box2 = IMG_Load(IMG_PATH "box2.png");
	// load global graphics
	img_arrow = IMG_Load(IMG_PATH "arrow.png");
	img_settings = IMG_Load(IMG_PATH "settings.png");
	img_Closs_InT = IMG_Load(IMG_PATH "Closs_InT.png");
	img_help = IMG_Load(IMG_PATH "help.png");
	img_manual = IMG_Load(IMG_PATH "manual.png");
	img_levels = IMG_Load(IMG_PATH "levels.png");
	// old graphics
	auto img_old_box = IMG_Load(IMG_PATH "old_box.png");
	
	
	// set color keys
	set_white_as_colorkey({
			                      img_undefined,
			                      img_old_box,
			                      img_box,
			                      img_wall,
			                      img_dest,
			                      img_gem,
			
			                      img_box2,
			
			                      img_arrow,
			                      img_settings,
			                      img_Closs_InT,
			                      img_help,
			                      img_manual,
			                      img_levels
	                      });
	
	// put into image map
	types_img_map[tile_background] = img_tile_background;
	types_img_map[tile_undefined] = img_undefined;
	types_img_map[tile_cyan] = img_cyan;
	types_img_map[tile_box] = img_box;
	types_img_map[tile_wall] = img_wall;
	types_img_map[tile_destination] = img_dest;
	types_img_map[tile_gem] = img_gem;
}

void free_img() {
	for (auto pair: types_img_map)
		SDL_FreeSurface(pair.second);
	types_img_map.clear();
	SDL_FreeSurface(img_settings);
	SDL_FreeSurface(img_arrow);
	SDL_FreeSurface(img_Closs_InT);
	SDL_FreeSurface(img_help);
	SDL_FreeSurface(img_manual);
}

void set_white_as_colorkey(const vector<SDL_Surface *> &surfaces) {
	for (auto surface: surfaces)
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 255, 255));
}

void show_surface(SDL_Renderer *renderer, SDL_Surface *surface, const DisplayPos &pos) {
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	auto srcrect = get_srcrect(surface), dstrect = get_dstrect(pos, surface);
	SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
	SDL_DestroyTexture(texture);
}

uint32_t MapRGB(SDL_Surface *surface, const SDL_Color &color) {
	return SDL_MapRGB(surface->format, color.r, color.g, color.b);
}

//
// Created by Dogs-Cute on 12/10/2022.
//

#include "closs.h"

volatile public_code_t publicCode = 0;

Tile::Tile(const TilePos &pos, SDL_Surface *img, DisplayPos size) {
	m_code = get_public_code();
	m_pos = pos;
	m_img = img;
	m_size = size;
}

SDL_Rect Tile::srcrect() const {
	return {0, 0, m_img->w, m_img->h};
}

bool Tile::operator==(const Tile &tile) const {
	return m_code == tile.m_code;
}

Room::Room(TilePos size) {
	for (size_t h = 0; h != size.h; h++) {
		auto lane = new Lane;
		
		for (size_t w = 0; w != size.w; w++)
			lane->push_back(new Space);
		
		push_back(lane);
	}
	m_size = size;
}

Room::~Room() {
	for (size_t h = 0; h != m_size.h; h++) {
		auto lane = vector::at(h);
		for (size_t w = 0; w != m_size.w; w++) {
			delete *lane->end();
			lane->pop_back();
		}
		delete lane;
	}
}

SpaceType Room::at(const TilePos &pos) {
	return vector::at(pos.h)->at(pos.w);
}

void Room::del(TileConst tile) {
	auto space = at(tile->m_pos);
	space->erase(find_tile(&*space, tile));
}

void Room::add(TileType tile) {
	at(tile->m_pos)->push_back(tile);
}

void Room::move(TileType tile, const TilePos &dest) {
	del(tile);
	tile->m_pos = dest;
	add(tile);
}

void Room::move(TileType tile, const direction &dir) {
	auto dest = tile->m_pos + dir_pos_map[dir];
	dest.w %= m_size.w;
	dest.h %= m_size.h;
	move(tile, dest);
}

public_code_t get_public_code() {
	return publicCode++;
}

Space_iter find_tile(SpaceConst space, TileConst tile) {
	for (auto it = space->begin(); it != space->end(); it++) {
		if ((*it)->m_code == tile->m_code) {
			Space_iter result;
			std::advance(it, std::distance<Space_c_iter>(it, result));
			return result;
		}
	}
	throw out_of_range("No such tile found in space");
}

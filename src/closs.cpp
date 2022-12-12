//
// Created by Dogs-Cute on 12/10/2022.
//

#include "closs.h"

volatile public_code_t publicCode = 0;

closs_room_error::closs_room_error(const string &arg) : runtime_error(arg) {}

closs_room_error::closs_room_error(const char *arg) : runtime_error(arg) {}

Tile::Tile(TilePos pos, SDL_Surface *img) {
	m_code = get_public_code();
	m_pos = pos;
	m_img = img;
}

SDL_Rect Tile::srcrect() const {
	return {0, 0, m_img->w, m_img->h};
}

bool Tile::operator==(const Tile &tile) const {
	return m_code == tile.m_code;
}

bool Tile::is_independent() const { return false; }

direction_t Tile::acq_req(const Tile::Movement_Request &req) const { return 0; }

direction_t Tile::respond_keys(key_predicate_t predicate) const { return 0; }

Room::Room(int each, TilePos size) {
	for (size_t h = 0; h != size.h; h++) {
		auto lane = new Lane;
		
		for (size_t w = 0; w != size.w; w++)
			lane->push_back(new Space);
		
		push_back(lane);
	}
	m_each = each;
	m_size = size;
}

Room::~Room() {
	for (auto lane: *this) {
		for (auto space: *lane) {
			for (auto tile: *space) {
				delete tile;
			}
			delete space;
		}
		delete lane;
	}
	clear();
}

SpaceType Room::at(const TilePos &pos) {
	return vector::at(pos.h)->at(pos.w);
}

void Room::del(TileConst tile) {
	auto space = at(tile->m_pos);
	space->erase(find(space->begin(), space->end(), tile));
}

void Room::add(TileType tile) {
	at(tile->m_pos)->push_back(tile);
}

TilePos Room::get_dest(TileType tile, direction_t dir) const {
	auto dest = tile->m_pos + m_size + key_pos_map.at(dir);
	dest.w %= m_size.w;
	dest.h %= m_size.h;
	return dest;
}

bool Room::send_req_from(TileType tile, direction_t dir) {
	bool result = true;
	auto space = at(get_dest(tile, dir));
	Movement_Request req = {tile, dir};
	for (auto dest_tile: *space) {
		direction_t dest_dir = dest_tile->acq_req(req);
		if (dest_dir > 0) send_req_from(dest_tile, dest_dir);
		else if (dest_dir < 0) result = false;
	}
	if (result) pending_move(tile, dir);
	return result;
}

void Room::pending_move(TileType tile, direction_t dir) {
	m_pending[tile] = get_dest(tile, dir);
}

void Room::do_pending_moves() {
	for (auto pair: m_pending) {
		move_tile(pair.first, pair.second);
	}
	m_pending.clear();
}

void Room::move_tile(TileType tile, const TilePos &dest) {
	del(tile);
	tile->m_pos = dest;
	add(tile);
}

void Room::move_tile(TileType tile, direction_t dir) {
	move_tile(tile, get_dest(tile, dir));
}

DisplayPos Room::total_size() const {
	return {(int) (m_each * m_size.w), (int) (m_each * m_size.h)};
}

void Room::move_independents(key_predicate_t predicate) {
	for (auto lane: *this) {
		for (auto space: *lane) {
			for (auto tile: *space) {
				if (tile->is_independent()) {
					auto dir = tile->respond_keys(predicate);
					if (dir > 0) send_req_from(tile, dir);
				}
			}
		}
	}
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

direction_vec_t find_keys(bool (*predicate)(direction_t), const direction_vec_t &wanted_keys) {
	direction_vec_t result;
	for (auto key: wanted_keys)
		if (predicate(key)) {
			result.push_back(key);
		}
	return result;
}

Cyan::Cyan(TilePos pos, SDL_Surface *m_img) : Tile(pos, m_img) {}

bool Cyan::is_independent() const { return true; }


direction_t Cyan::acq_req(const Movement_Request &req) const { return Tile::acq_req(req); }

direction_t Cyan::respond_keys(key_predicate_t predicate) const {
	auto pending = find_keys(predicate, MOVEMENT_KEYS);
	if (pending.empty()) return 0;
	return pending[0];
}

Box::Box(TilePos pos, SDL_Surface *m_img) : Tile(pos, m_img) {}

bool Box::is_independent() const {
	return Tile::is_independent();
}

direction_t Box::acq_req(const Movement_Request &req) const {
	return req.direction;
}

TileType construct_undefined(TilePos pos, SDL_Surface *img) {
	return new Tile(pos, img);
}

TileType construct_cyan(TilePos pos, SDL_Surface *img) {
	return new Cyan(pos, img);
}

TileType construct_box(TilePos pos, SDL_Surface *img) {
	return new Box(pos, img);
}

tile_types_map_t tile_type_map = {
		{tile_undefined, construct_undefined},
		{tile_cyan,      construct_cyan},
		{tile_box,       construct_box}
};



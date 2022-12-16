//
// Created by Dogs-Cute on 12/10/2022.
//

#include "closs.h"

volatile public_code_t publicCode = 0;

closs_room_error::closs_room_error(const string &arg) : runtime_error(arg) {}

closs_room_error::closs_room_error(const char *arg) : runtime_error(arg) {}

closs_page_error::closs_page_error(const string &arg) : runtime_error(arg) {}

closs_page_error::closs_page_error(const char *arg) : runtime_error(arg) {}

Tile::Tile(TilePos pos, SDL_Surface *img) {
	m_pubCode = get_public_code();
	m_pos = pos;
	m_img = img;
}

SDL_Rect Tile::srcrect() const {
	return {0, 0, m_img->w, m_img->h};
}

bool Tile::operator==(const Tile &tile) const {
	return m_pubCode == tile.m_pubCode;
}

bool Tile::is_independent() const { return false; }

tile_types Tile::get_type() const { return tile_undefined; }

direction_t Tile::acq_req(const Movement_Request &req) const { return 0; }

direction_t Tile::respond_keys(key_predicate_t predicate) const { return 0; }

void Tile::show_additional(SDL_Renderer *renderer, const DisplayPos &pos, const DisplayPos &center,
                           double stretch_ratio) const {}

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
			for (auto tile: *space)
				delete tile;
			delete space;
		}
		delete lane;
	}
	clear();
}

void Room::refresh_dest() {
	m_dest.clear();
	for (auto lane: *this)
		for (auto space: *lane)
			for (auto tile: *space)
				if (tile->get_type() == tile_destination)
					m_dest.push_back(tile);
}

void Room::refresh_gems() {
	m_gems.clear();
	for (auto lane: *this)
		for (auto space: *lane)
			for (auto tile: *space)
				if (tile->get_type() == tile_gem) {
					if (m_is_perf_play) m_gems.push_back(tile);
					else {
						del(tile);
						delete tile;
					}
				}
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

bool Room::send_req_from(TileType tile, direction_t dir, uint8_t times) {
	if (times == 0) return false;
	bool result = true;
	auto space = at(get_dest(tile, dir));
	Movement_Request req = {tile, dir};
	for (auto dest_tile: *space) {
		direction_t dest_dir = dest_tile->acq_req(req);
		if (dest_dir > 0) result &= send_req_from(dest_tile, dest_dir, times + 1);
		else if (result && dest_dir < 0) result = false;
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
	bool next_step_flag = true;
	m_is_moving = false;
	for (auto lane: *this)
		for (auto space: *lane)
			for (auto tile: *space)
				if (tile->is_independent()) {
					auto dir = tile->respond_keys(predicate);
					if (dir > 0) {
						m_is_moving = true;
						send_req_from(tile, dir);
						if (next_step_flag) {
							next_step_flag = false;
							m_steps++;
						}
					}
				}
}

void Room::detect_gems() {
	if (m_is_moving) {
		Space pending_collection;
		for (auto gem_tile: m_gems) {
			auto space = at(gem_tile->m_pos);
			for (auto tile: *space)
				if (tile->get_type() == tile_cyan) pending_collection.push_back(gem_tile);
		}
		for (auto gem_tile: pending_collection) {
			auto gem = (Gem *) gem_tile;
			m_steps += gem->m_addition;
			del(gem);
			delete gem;
		}
	}
}

void Room::end_of_step() {
	m_is_winning = true;
	for (auto dest: m_dest)
		if (!((Destination *) dest)->detect_requirement(at(dest->m_pos))) {
			m_is_winning = false;
			break;
		}
}


public_code_t get_public_code() {
	return publicCode++;
}

Space_iter find_tile(SpaceConst space, TileConst tile) {
	for (auto it = space->begin(); it != space->end(); it++) {
		if ((*it)->m_pubCode == tile->m_pubCode) {
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

Destination::Destination(TilePos pos, SDL_Surface *img, int type) : Tile(pos, img) {
	m_req = (tile_types) type;
}

tile_types Destination::get_type() const { return tile_destination; }

bool Destination::detect_requirement(SpaceConst space) const {
	bool satisfied = false;
	for (auto tile: *space) {
		if (tile->get_type() == m_req) {
			satisfied = true;
			break;
		}
	}
	return satisfied;
}

void Destination::show_additional(SDL_Renderer *renderer, const DisplayPos &pos, const DisplayPos &center,
                                  double stretch_ratio) const {
	auto surface = TTF_RenderText_Solid(consolas.sized(FONT_SIZE(DESTINATION_SIZE)),
	                                    type_names.at(m_req).c_str(),
	                                    WHITE);
	DisplayPos show_pos = {center.w - surface->w / 2, center.h - surface->h / 2};
	auto texture = SDL_CreateTextureFromSurface(renderer, surface);
	auto srcrect = get_srcrect(surface), dstrect = get_dstrect(show_pos, surface);
	SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

TileType construct_undefined(TilePos pos, SDL_Surface *img, int) {
	return new Tile(pos, img);
}

TileType construct_cyan(TilePos pos, SDL_Surface *img, int) {
	return new Cyan(pos, img);
}

TileType construct_box(TilePos pos, SDL_Surface *img, int) {
	return new Box(pos, img);
}

TileType construct_wall(TilePos pos, SDL_Surface *img, int) {
	return new Wall(pos, img);
}

TileType construct_dest(TilePos pos, SDL_Surface *img, int type) {
	return new Destination(pos, img, type);
}

TileType construct_gem(TilePos pos, SDL_Surface *img, int addition) {
	return new Gem(pos, img, addition);
}

tile_types_map_t tile_type_map = {
		{tile_undefined,   construct_undefined},
		{tile_cyan,        construct_cyan},
		{tile_box,         construct_box},
		{tile_wall,        construct_wall},
		{tile_destination, construct_dest},
		{tile_gem,         construct_gem}
};

Cyan::Cyan(TilePos pos, SDL_Surface *m_img) : Tile(pos, m_img) {}

bool Cyan::is_independent() const { return true; }

tile_types Cyan::get_type() const { return tile_cyan; }

direction_t Cyan::acq_req(const Movement_Request &req) const { return req.direction; }

direction_t Cyan::respond_keys(key_predicate_t predicate) const {
	auto pending = find_keys(predicate, MOVEMENT_KEYS);
	if (pending.empty()) return 0;
	return pending[0];
}

Box::Box(TilePos pos, SDL_Surface *m_img) : Tile(pos, m_img) {}

tile_types Box::get_type() const { return tile_box; }

direction_t Box::acq_req(const Movement_Request &req) const {
	return req.direction;
}

Wall::Wall(TilePos pos, SDL_Surface *m_img) : Tile(pos, m_img) {}

tile_types Wall::get_type() const { return tile_wall; }

direction_t Wall::acq_req(const Movement_Request &req) const {
	return -1;
}

Gem::Gem(TilePos pos, SDL_Surface *m_img, int addition) : Tile(pos, m_img) {
	m_addition = addition;
}

tile_types Gem::get_type() const {
	return tile_gem;
}

void Gem::show_additional(SDL_Renderer *renderer, const DisplayPos &pos, const DisplayPos &center,
                          double stretch_ratio) const {
	auto surface = TTF_RenderText_Solid(consolas.sized(FONT_SIZE(DESTINATION_SIZE)),
	                                    to_string(m_addition).c_str(),
	                                    m_addition <= 0 ? GREEN : RED);
	DisplayPos show_pos = {center.w - surface->w / 2, center.h - surface->h / 2};
	auto texture = SDL_CreateTextureFromSurface(renderer, surface);
	auto srcrect = get_srcrect(surface), dstrect = get_dstrect(show_pos, surface);
	SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

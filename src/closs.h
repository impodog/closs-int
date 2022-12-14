//
// Created by Dogs-Cute on 12/10/2022.
//

#ifndef CLOSS_INT_CLOSS_H
#define CLOSS_INT_CLOSS_H

#include "img.h"

using public_code_t = unsigned long long;
using direction_t = SDL_Keycode;
using key_down_map_t = unordered_map<direction_t, bool>;
using direction_vec_t = vector<direction_t>;
using key_predicate_t = bool (*)(direction_t key);

extern volatile public_code_t publicCode;

class closs_room_error : public runtime_error {
public:
	explicit closs_room_error(const string &arg);
	
	explicit closs_room_error(const char *arg);
};

class closs_page_error : public runtime_error {
public:
	explicit closs_page_error(const string &arg);
	
	explicit closs_page_error(const char *arg);
};

class Tile {
public:
	struct Movement_Request {
		const Tile *sender;
		direction_t direction;
	};
	public_code_t m_pubCode;
	TilePos m_pos;
	SDL_Surface *m_img;
	
	Tile(TilePos pos, SDL_Surface *m_img, tile_types type);
	
	SDL_Rect srcrect() const;
	
	bool operator==(const Tile &tile) const;
	
	virtual bool is_independent() const;
	
	virtual tile_types get_type() const;
	
	virtual direction_t acq_req(const Movement_Request &req) const;
	
	virtual direction_t respond_keys(key_predicate_t predicate) const;
	
	virtual void show_additional(SDL_Renderer *renderer, const DisplayPos &pos, const DisplayPos &center,
	                             double stretch_ratio) const;
};

using TileType = Tile *;
using TileConst = const Tile *;
using Space = vector<TileType>;
using Space_iter = Space::iterator;
using Space_c_iter = Space::const_iterator;
using SpaceType = Space *;
using SpaceConst = const Space *;
using Lane = vector<SpaceType>;
using LaneType = Lane *;
using LaneConst = const Lane *;

using Movement_Request = Tile::Movement_Request;

using pending_movements_t = unordered_map<TileType, TilePos>;


class Room : public vector<vector<SpaceType> *> {
public:
	int m_each;
	size_t steps = 0;
	bool is_winning = false;
	
	json m_title, m_help_map;
	
	TilePos m_size;
	pending_movements_t m_pending;
	
	Space m_dest;
	
	explicit Room(int each, TilePos size);
	
	~Room();
	
	void refresh_dest();
	
	SpaceType at(const TilePos &pos);
	
	void del(TileConst tile);
	
	void add(TileType tile);
	
	TilePos get_dest(TileType tile, direction_t dir) const;
	
	bool send_req_from(TileType tile, direction_t dir, uint8_t times = 1);
	
	void pending_move(TileType tile, direction_t dir);
	
	void do_pending_moves();
	
	void move_tile(TileType tile, const TilePos &dest);
	
	void move_tile(TileType tile, direction_t dir);
	
	void move_independents(key_predicate_t predicate);
	
	void end_of_step();
	
	DisplayPos total_size() const;
};


using RoomType = Room *;

using tile_constructor_t = TileType (*)(TilePos, SDL_Surface *img, tile_types type);
using tile_types_map_t = const map<tile_types, tile_constructor_t>;

extern tile_types_map_t tile_type_map;
extern type_names_t type_names;

public_code_t get_public_code();

Space::iterator find_tile(SpaceConst space, TileConst tile);

direction_vec_t find_keys(key_predicate_t predicate, const direction_vec_t &wanted_keys);

class Destination : public Tile {
public:
	tile_types m_req;
	
	explicit Destination(TilePos pos, SDL_Surface *img, tile_types type);
	
	tile_types get_type() const override;
	
	bool detect_requirement(SpaceConst space) const;
	
	void show_additional(SDL_Renderer *renderer, const DisplayPos &pos, const DisplayPos &center,
	                     double stretch_ratio) const override;
};

class Cyan : public Tile {
public:
	Cyan(TilePos pos, SDL_Surface *m_img, tile_types type);
	
	bool is_independent() const override;
	
	tile_types get_type() const override;
	
	direction_t acq_req(const Movement_Request &req) const override;
	
	direction_t respond_keys(key_predicate_t predicate) const override;
};

class Box : public Tile {
public:
	Box(TilePos pos, SDL_Surface *m_img, tile_types type);
	
	tile_types get_type() const override;
	
	direction_t acq_req(const Movement_Request &req) const override;
};

class Wall : public Tile {
public:
	Wall(TilePos pos, SDL_Surface *m_img, tile_types type);
	
	tile_types get_type() const override;
	
	direction_t acq_req(const Movement_Request &req) const override;
};


#endif //CLOSS_INT_CLOSS_H

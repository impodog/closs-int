//
// Created by Dogs-Cute on 12/10/2022.
//

#ifndef CLOSS_INT_CLOSS_H
#define CLOSS_INT_CLOSS_H

#include "const.h"

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

class Tile {
public:
	struct Movement_Request {
		const Tile *sender;
		direction_t direction;
	};
	public_code_t m_code;
	TilePos m_pos;
	SDL_Surface *m_img;
	
	Tile(TilePos pos, SDL_Surface *m_img);
	
	SDL_Rect srcrect() const;
	
	bool operator==(const Tile &tile) const;
	
	virtual bool is_independent() const;
	
	virtual direction_t acq_req(const Movement_Request &req) const;
	
	virtual direction_t respond_keys(key_predicate_t predicate) const;
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
	
	string m_title;
	help_map_t m_help_map;
	
	TilePos m_size;
	pending_movements_t m_pending;
	
	explicit Room(int each, TilePos size);
	
	~Room();
	
	SpaceType at(const TilePos &pos);
	
	void del(TileConst tile);
	
	void add(TileType tile);
	
	TilePos get_dest(TileType tile, direction_t dir) const;
	
	bool send_req_from(TileType tile, direction_t dir);
	
	void pending_move(TileType tile, direction_t dir);
	
	void do_pending_moves();
	
	void move_tile(TileType tile, const TilePos &dest);
	
	void move_tile(TileType tile, direction_t dir);
	
	void move_independents(key_predicate_t predicate);
	
	DisplayPos total_size() const;
};

enum tile_types {
	tile_background = -2,
	tile_undefined,
	tile_empty,
	tile_cyan,
	tile_box,
	tile_wall
};

using RoomType = Room *;

using tile_constructor_t = TileType (*)(TilePos, SDL_Surface *img);
using tile_types_map_t = map<tile_types, tile_constructor_t>;

extern tile_types_map_t tile_type_map;

public_code_t get_public_code();

Space::iterator find_tile(SpaceConst space, TileConst tile);

direction_vec_t find_keys(key_predicate_t predicate, const direction_vec_t &wanted_keys);

class Cyan : public Tile {
public:
	Cyan(TilePos pos, SDL_Surface *m_img);
	
	bool is_independent() const override;
	
	direction_t acq_req(const Movement_Request &req) const override;
	
	direction_t respond_keys(key_predicate_t predicate) const override;
};

class Box : public Tile {
public:
	Box(TilePos pos, SDL_Surface *m_img);
	
	bool is_independent() const override;
	
	direction_t acq_req(const Movement_Request &req) const override;
};

class Wall : public Tile {
public:
	Wall(TilePos pos, SDL_Surface *m_img);
	
	bool is_independent() const override;
	
	direction_t acq_req(const Movement_Request &req) const override;
};


#endif //CLOSS_INT_CLOSS_H

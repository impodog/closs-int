//
// Created by Dogs-Cute on 12/10/2022.
//

#ifndef CLOSS_INT_CLOSS_H
#define CLOSS_INT_CLOSS_H

#include "const.h"

using public_code_t = unsigned long long;

extern volatile public_code_t publicCode;

class closs_room_error : public runtime_error {
public:
	explicit closs_room_error(const string &__arg);
	
	explicit closs_room_error(const char *__arg);
};

class Tile {
public:
	public_code_t m_code;
	TilePos m_pos;
	SDL_Surface *m_img;
	
	Tile(TilePos pos, SDL_Surface *m_img);
	
	SDL_Rect srcrect() const;
	
	bool operator==(const Tile &tile) const;
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

class Room : public vector<vector<SpaceType> *> {
public:
	int m_each;
	TilePos m_size;
	
	explicit Room(int each, TilePos size);
	
	~Room();
	
	SpaceType at(const TilePos &pos);
	
	void del(TileConst tile);
	
	void add(TileType tile);
	
	void move(TileType tile, const TilePos &dest);
	
	void move(TileType tile, const direction &dir);
	
	DisplayPos total_size() const;
};

enum tile_types {
	tile_background = -2,
	tile_undefined,
	tile_empty,
	tile_cyan
};

using RoomType = Room *;

using tile_constructor_t = TileType (*)(TilePos, SDL_Surface *img);
using tile_types_map_t = unordered_map<tile_types, tile_constructor_t>;

extern tile_types_map_t tile_type_map;

public_code_t get_public_code();

Space::iterator find_tile(SpaceConst space, TileConst tile);

class Cyan : public Tile {
public:
	Cyan(TilePos pos, SDL_Surface *m_img);
};

TileType construct_undefined(TilePos pos, SDL_Surface *img);

TileType construct_cyan(TilePos pos, SDL_Surface *img);


#endif //CLOSS_INT_CLOSS_H

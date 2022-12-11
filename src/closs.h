//
// Created by Dogs-Cute on 12/10/2022.
//

#ifndef CLOSS_INT_CLOSS_H
#define CLOSS_INT_CLOSS_H

#include "const.h"

using public_code_t = unsigned long long;

extern volatile public_code_t publicCode;

class Tile {
public:
	public_code_t m_code;
	TilePos m_pos;
	DisplayPos m_size;
	SDL_Surface *m_img;
	
	Tile(const TilePos &pos, SDL_Surface *img, DisplayPos size);
	
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

class Room : vector<vector<SpaceType> *> {
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
};

using RoomType = Room *;

public_code_t get_public_code();

Space::iterator find_tile(SpaceConst space, TileConst tile);

#endif //CLOSS_INT_CLOSS_H

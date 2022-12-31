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

extern volatile public_code_t public_code;

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

    DoublePos m_shift = {0, 0};

    Tile(TilePos pos, SDL_Surface *m_img);

    SDL_Rect srcrect() const;

    void process();

    bool operator==(const Tile &tile) const;

    virtual bool is_independent() const;

    virtual tile_types get_type() const;

    virtual direction_t acq_req(const Movement_Request &req) const;

    virtual direction_t respond_keys(key_predicate_t predicate) const;

    virtual void show_additional(SDL_Renderer *renderer, const DisplayPos &pos, const DisplayPos &center,
                                 long double stretch_ratio) const;
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
    int m_each, m_pending_go_to = 0, m_unlock_bonus = 0;
    size_t m_steps = 0, m_perf;
    bool m_is_winning = false, m_is_moving = false, m_is_end_of_animation = false, m_is_second_play = false, m_is_perf_play = false, m_is_gem_play = false;

    json m_title, m_help_map, m_next;

    TilePos m_size;
    pending_movements_t m_pending;

    Space m_dest, m_gems, m_animating;

    explicit Room(int each, TilePos size);

    ~Room();

    void refresh_dest();

    void refresh_gems();

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

    void detect_gems();

    void animate_tiles(long double animation_speed);

    void end_of_step();

    DisplayPos total_size() const;

    bool is_perf_play() const;

    bool can_get_perf_play() const;

    bool can_get_gem_play() const;
};


using RoomType = Room *;

using tile_constructor_t = TileType (*)(TilePos, SDL_Surface *, int);
using tile_types_map_t = const map<tile_types, tile_constructor_t>;

extern tile_types_map_t tile_type_map;
extern type_names_t type_names;

public_code_t get_public_code();

Space::iterator find_tile(SpaceConst space, TileConst tile);

direction_vec_t find_keys(key_predicate_t predicate, const direction_vec_t &wanted_keys);

extern RoomType public_room;

// defined in loader.cpp
json &get_user();

class Destination : public Tile {
public:
    tile_types m_req;

    explicit Destination(TilePos pos, SDL_Surface *img, int type);

    tile_types get_type() const override;

    bool detect_requirement(SpaceConst space) const;

    void show_additional(SDL_Renderer *renderer, const DisplayPos &pos, const DisplayPos &center,
                         long double stretch_ratio) const override;
};

class Cyan : public Tile {
public:
    Cyan(TilePos pos, SDL_Surface *m_img);

    bool is_independent() const override;

    tile_types get_type() const override;

    direction_t acq_req(const Movement_Request &req) const override;

    direction_t respond_keys(key_predicate_t predicate) const override;
};

class Box : public Tile {
public:
    Box(TilePos pos, SDL_Surface *m_img);

    tile_types get_type() const override;

    direction_t acq_req(const Movement_Request &req) const override;
};

class Wall : public Tile {
public:
    Wall(TilePos pos, SDL_Surface *m_img);

    tile_types get_type() const override;

    direction_t acq_req(const Movement_Request &req) const override;
};

class Gem : public Tile {
public:
    int m_addition;

    Gem(TilePos pos, SDL_Surface *m_img, int addition);

    tile_types get_type() const override;

    void show_additional(SDL_Renderer *renderer, const DisplayPos &pos, const DisplayPos &center,
                         long double stretch_ratio) const override;
};

class Picture : public Tile {
public:
    Picture(TilePos pos, SDL_Surface *m_img);

    tile_types get_type() const override;
};

class Go_To : public Tile {
public:
    int m_level;

    Go_To(TilePos pos, SDL_Surface *m_img, int level);

    tile_types get_type() const override;

    direction_t acq_req(const Movement_Request &req) const override;
};

#endif //CLOSS_INT_CLOSS_H

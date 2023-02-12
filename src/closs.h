//
// Created by Dogs-Cute on 12/10/2022.
//

#ifndef CLOSS_INT_CLOSS_H
#define CLOSS_INT_CLOSS_H

#define ROOM_CONTAINS_GEMS (m_is_perf_play && !m_is_gem_play)
#define IS_WINNING_GEM (m_is_winning && ROOM_CONTAINS_GEMS)
#define FOREACH_TILE for (auto lane: m_distribute) for (auto space: *lane) for (auto tile: *space)

#include "img.h"

using public_code_t = unsigned long long;
using key_down_map_t = unordered_map<direction_t, bool>;
using direction_vec_t = vector<direction_t>;
using key_predicate_t = bool (*)(direction_t key);

extern public_code_t public_code;

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
        Tile *sender;
        direction_t direction;
    };
    using pending_movements_t = unordered_map<Tile *, direction_t>;
    using pending_series_t = unordered_map<Tile *, Movement_Request>;
    public_code_t m_pubCode;
    TilePos m_pos;
    SDL_Surface *m_img;

    DoublePos m_shift = {0, 0};
    DisplayPos m_shift_sym = {0, 0};
    bool can_end_animation = false;

    Tile(TilePos pos, SDL_Surface *img);

    SDL_Rect srcrect() const;

    void process();

    bool operator==(const Tile &tile) const;

    string get_info() const;

    virtual bool is_independent() const;

    virtual tile_types get_type() const;

    virtual direction_t acq_req(Movement_Request req);

    virtual direction_t respond_keys(key_predicate_t predicate) const;

    virtual void show_additional(SDL_Renderer *renderer, const DisplayPos &pos, const DisplayPos &center,
                                 long double stretch_ratio);

    virtual void end_of_step();

    /*Notice: Do NOT use this function to move the tile itself, use .is_independent() + .respond_keys() instead*/
    virtual void add_to_parser(pending_series_t &pending_series);

    virtual void begin_request(direction_t dir);

    virtual void react_to_movement_result(bool result);

    virtual bool suppress_request(const Movement_Request &req);
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

using pending_movements_t = Tile::pending_movements_t;
using pending_series_t = Tile::pending_series_t;

using tile_distribute_t = vector<vector<SpaceType> *>;

using type_parsing_seq_t = const vector<tile_types>;

extern const type_parsing_seq_t type_parsing_seq;

struct Animation_Info {
    bool is_edge = false;
};
struct Dest_Info {
    TilePos dest;
    Animation_Info info;
};
using animations_t = unordered_map<TileType, Animation_Info>;

class Room {
public:
    tile_distribute_t m_distribute;
    int m_each, m_pending_go_to = 0, m_unlock_bonus = 0;
    size_t m_steps = 0, m_perf, m_parsing_index = 0;
    DisplayPos m_display_size;
    bool m_is_winning = false, m_is_moving = false, m_is_end_of_animation = false, m_is_second_play = false, m_is_perf_play = false, m_is_gem_play = false, m_end_animation_flag = false, m_can_move_flag = false;

    json m_title, m_help_map, m_next;

    TilePos m_size;
    pending_movements_t m_pending_move;
    pending_series_t m_pending_series;

    Space m_dest, m_gems;
    animations_t m_animating;

    Room(int each, TilePos size);

    ~Room();

    void refresh_dest();

    void refresh_gems();

    SpaceType at(const TilePos &pos);

    void remove(TileConst tile);

    void add(TileType tile);

    void destroy(TileType tile);

    TilePos get_dest(TileType tile, direction_t dir) const;

    Dest_Info get_dest_info(TileType tile, direction_t dir) const;

    bool send_req_from(TileType tile, direction_t dir, list<TileType> *infinite_prevention = nullptr);

    void pending_move(TileType tile, direction_t dir);

    void parse_series();

    void pending_series(TileType tile, Movement_Request req);

    void do_pending_moves();

    void move_tile(TileType tile, const TilePos &dest, const Animation_Info &info = {});

    void move_tile(TileType tile, direction_t dir);

    void move_independents(key_predicate_t predicate);

    void detect_gems();

    void animate_tiles(long double animation_speed, const DisplayPos &room_pos);

    void end_of_step();

    void clear_move_status();

    bool is_perf_play() const;

    bool can_get_perf_play() const;

    bool can_get_gem_play() const;

    bool can_move_independents() const;

    bool can_parse_movements() const;
};

using RoomType = Room *;

using tile_constructor_t = TileType (*)(TilePos, SDL_Surface *, int);
using tile_types_map_t = const map<tile_types, tile_constructor_t>;

extern tile_types_map_t tile_type_map;
extern type_names_t type_names;

public_code_t get_public_code();

Space::iterator find_tile(SpaceConst space, TileConst tile);

direction_vec_t find_keys(key_predicate_t predicate, const direction_vec_t &wanted_keys);

dest_img_info *get_dest_surf(SDL_Renderer *renderer, tile_types type);

extern RoomType public_room;

// defined in loader.cpp
extern json &public_user;

class Destination : public Tile {
public:
    size_t m_counter = 0;
    tile_types m_req;
    dest_img_info *m_info = nullptr;

    Destination(TilePos pos, SDL_Surface *img, int type);

    tile_types get_type() const override;

    bool detect_requirement(SpaceConst space) const;

    void show_additional(SDL_Renderer *renderer, const DisplayPos &pos, const DisplayPos &center,
                         long double stretch_ratio) override;
};

class Cyan : public Tile {
public:
    Cyan(TilePos pos, SDL_Surface *img);

    bool is_independent() const override;

    tile_types get_type() const override;

    direction_t acq_req(Movement_Request req) override;

    direction_t respond_keys(key_predicate_t predicate) const override;
};

class Box : public Tile {
public:
    Box(TilePos pos, SDL_Surface *img);

    tile_types get_type() const override;

    direction_t acq_req(Movement_Request req) override;
};

class Wall : public Tile {
public:
    Wall(TilePos pos, SDL_Surface *img);

    tile_types get_type() const override;

    direction_t acq_req(Movement_Request req) override;
};

class Gem : public Tile {
public:
    int m_addition;

    Gem(TilePos pos, SDL_Surface *img, int addition);

    tile_types get_type() const override;

    void show_additional(SDL_Renderer *renderer, const DisplayPos &pos, const DisplayPos &center,
                         long double stretch_ratio) override;
};

class Picture : public Tile {
public:
    Picture(TilePos pos, SDL_Surface *img);

    tile_types get_type() const override;
};

class Go_To : public Tile {
public:
    int m_level;

    Go_To(TilePos pos, SDL_Surface *img, int level);

    tile_types get_type() const override;

    direction_t acq_req(Movement_Request req) override;
};

class Blue : public Tile {
public:
    Blue(TilePos pos, SDL_Surface *img);

    tile_types get_type() const override;

    direction_t acq_req(Movement_Request req) override;
};

class Spike : public Tile {
public:
    Spike(TilePos pos, SDL_Surface *img);

    tile_types get_type() const override;

    direction_t acq_req(Movement_Request req) override;

    void end_of_step() override;
};

class Conveyor : public Tile {
public:
    direction_t m_dir;

    Conveyor(TilePos pos, SDL_Surface *, direction_t dir);

    tile_types get_type() const override;

    direction_t acq_req(Movement_Request req) override;

    void add_to_parser(pending_series_t &pending_series) override;
};

class Robot : public Tile {
public:
    direction_t m_dir;
    bool m_is_moved = false;

    Robot(TilePos pos, SDL_Surface *, direction_t dir);

    bool is_independent() const override;

    tile_types get_type() const override;

    direction_t acq_req(Movement_Request req) override;

    direction_t respond_keys(key_predicate_t) const override;

    void begin_request(direction_t dir) override;

    void change_dir(direction_t dir);

    void end_of_step() override;

    void react_to_movement_result(bool result) override;

    bool suppress_request(const Movement_Request &req) override;
};

#endif //CLOSS_INT_CLOSS_H

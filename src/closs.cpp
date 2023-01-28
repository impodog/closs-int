//
// Created by Dogs-Cute on 12/10/2022.
//

#include "closs.h"

volatile public_code_t public_code = 0;

RoomType public_room;

const type_parsing_seq_t type_parsing_seq = {
        tile_cyan,
        tile_robot,
        tile_conveyor,
        tile_undefined,
};

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

string Tile::get_info() const {
    return type_names.at(get_type()) + ":" + to_string(public_code);
}

bool Tile::is_independent() const { return false; }

tile_types Tile::get_type() const { return tile_undefined; }

direction_t Tile::acq_req(Movement_Request req) { return 0; }

direction_t Tile::respond_keys(key_predicate_t predicate) const { return 0; }

void Tile::show_additional(SDL_Renderer *renderer, const DisplayPos &pos, const DisplayPos &center,
                           long double stretch_ratio) const {}

void Tile::process() {}

void Tile::end_of_step() {}

void Tile::add_to_parser(pending_series_t &pending_series) {}

void Tile::begin_request(direction_t dir) {}

void Tile::react_to_movement_result(bool result) {}

bool Tile::suppress_request(const Movement_Request &req) { return false; }

Room::Room(int each, TilePos size) {
    for (size_t h = 0; h != size.h; h++) {
        auto lane = new Lane;

        for (size_t w = 0; w != size.w; w++)
            lane->push_back(new Space);

        m_distribute.push_back(lane);
    }
    m_each = each;
    m_size = size;
}

Room::~Room() {
    for (auto lane: m_distribute) {
        for (auto space: *lane) {
            for (auto tile: *space)
                delete tile;
            delete space;
        }
        delete lane;
    }
}

void Room::refresh_dest() {
    m_dest.clear();
    FOREACH_TILE if (tile->get_type() == tile_destination)
                    m_dest.push_back(tile);
}

void Room::refresh_gems() {
    m_gems.clear();
    FOREACH_TILE if (tile->get_type() == tile_gem) {
                    if (ROOM_CONTAINS_GEMS) m_gems.push_back(tile);
                    else destroy(tile);
                }
}

SpaceType Room::at(const TilePos &pos) {
    return m_distribute.at(pos.h)->at(pos.w);
}

void Room::remove(TileConst tile) {
    auto space = at(tile->m_pos);
    space->erase(find(space->begin(), space->end(), tile));
}

void Room::add(TileType tile) {
    at(tile->m_pos)->push_back(tile);
}

void Room::destroy(TileType tile) {
    remove(tile);
    delete tile;
}

TilePos Room::get_dest(TileType tile, direction_t dir) const {
    auto dest = tile->m_pos + m_size + key_pos_map.at(dir);
    dest.w %= m_size.w;
    dest.h %= m_size.h;
    return dest;
}

Dest_Info Room::get_dest_info(TileType tile, direction_t dir) const {
    Animation_Info info;
    auto &dir_ref = key_pos_map.at(dir);
    auto dest = tile->m_pos + m_size + dir_ref;
    info.is_edge = tile->m_pos.w + dir_ref.w > m_size.w - 1 || tile->m_pos.h + dir_ref.h > m_size.h - 1;
    dest.w %= m_size.w;
    dest.h %= m_size.h;
    return {dest, info};
}

bool Room::send_req_from(TileType tile, direction_t dir, list<TileType> *infinite_prevention) {
    bool is_first = false, result = true;
    if (infinite_prevention == nullptr) {
        infinite_prevention = new list<TileType>;
        tile->begin_request(dir);
        is_first = true;
    } else if (find(infinite_prevention->begin(), infinite_prevention->end(), tile) == infinite_prevention->end())
        infinite_prevention->push_back(tile);
    else return false;
    auto space = at(get_dest(tile, dir));
    Movement_Request req = {tile, dir};
    for (auto dest_tile: *space) {
        direction_t dest_dir = dest_tile->acq_req(req);
        if (dest_dir > 0) result &= send_req_from(dest_tile, dest_dir, infinite_prevention);
        else if (result && dest_dir < 0) result = false;
    }
    for (auto dest_tile: *space)
        tile->react_to_movement_result(result);
    if (result) pending_move(tile, dir);
    if (is_first) delete infinite_prevention;
    return result;
}

void Room::pending_move(TileType tile, direction_t dir) {
    m_pending_move[tile] = dir;
}

void Room::pending_series(TileType tile, Movement_Request req) {
    m_pending_series[tile] = req;
}

void Room::parse_series() {
    Space moved_tiles;
    tile_types parsing_type = type_parsing_seq.at(m_parsing_index);
    FOREACH_TILE if (tile->get_type() == parsing_type) {
                    tile->add_to_parser(m_pending_series);
                }
    for (auto pair: m_pending_series) {
        if (parsing_type == tile_undefined || pair.first->get_type() == parsing_type) {
            if (!pair.first->suppress_request(pair.second))
                send_req_from(pair.first, pair.second.direction);
            moved_tiles.push_back(pair.first);
        }
    }
    for (auto tile: moved_tiles)
        m_pending_series.erase(tile);
    for (auto pair: m_pending_move)
        if (pair.first->get_type() == tile_cyan) m_can_move_flag = true;
    if (++m_parsing_index == type_parsing_seq.size())
        m_parsing_index = 0;
}

void Room::do_pending_moves() {
    for (auto pair: m_pending_move) {
        move_tile(pair.first, pair.second);
    }
    m_pending_move.clear();
}

void Room::move_tile(TileType tile, const TilePos &dest, const Animation_Info &info) {
    remove(tile);
    tile->m_shift = {(long double) tile->m_pos.w - dest.w, (long double) tile->m_pos.h - dest.h};
    tile->m_shift_sym = {sym(tile->m_shift.w), sym(tile->m_shift.h)};
    tile->can_end_animation = false;
    tile->m_pos = dest;
    m_animating.insert({tile, info});
    add(tile);
}

void Room::move_tile(TileType tile, direction_t dir) {
    auto dest_info = get_dest_info(tile, dir);
    move_tile(tile, dest_info.dest, dest_info.info);
}

DisplayPos Room::total_size() const {
    return {(int) (m_each * m_size.w), (int) (m_each * m_size.h)};
}

void Room::move_independents(key_predicate_t predicate) {
    bool next_step_flag = true;
    m_is_moving = false;
    FOREACH_TILE if (tile->is_independent()) {
                    auto dir = tile->respond_keys(predicate);
                    if (dir > 0) {
                        pending_series(tile, {tile, dir});
                        if (next_step_flag && tile->get_type() == tile_cyan)
                            next_step_flag = false;
                    }
                }
    m_is_moving = !next_step_flag && !m_pending_series.empty();
}

void Room::detect_gems() {
    if (ROOM_CONTAINS_GEMS) {
        Space pending_collection;
        for (auto gem_tile: m_gems) {
            auto space = at(gem_tile->m_pos);
            for (auto tile: *space)
                if (tile->get_type() == tile_cyan) pending_collection.push_back(gem_tile);
        }
        for (auto gem_tile: pending_collection) {
            auto gem = (Gem *) gem_tile;
            if (gem->m_addition >= 0 || m_steps > -gem->m_addition) m_steps += gem->m_addition;
            else m_steps = 0;
            m_gems.erase(std::find(m_gems.begin(), m_gems.end(), gem));
            destroy(gem);
        }
    }
}

void Room::animate_tiles(long double animation_speed) {
    Space pending_remove;
    for (auto pair: m_animating) {
        bool end_cur_animation_flag = false;
        if (pair.second.is_edge) {
            if ((fabs(pair.first->m_shift.w) > m_size.w * 2 || fabs(pair.first->m_shift.h) > m_size.h * 2) &&
                !pair.first->can_end_animation) {
                pair.first->m_shift.w = -pair.first->m_shift.w + pair.first->m_shift_sym.w;
                pair.first->m_shift.h = -pair.first->m_shift.h + pair.first->m_shift_sym.h;
                pair.first->can_end_animation = true;
            } else {
                pair.first->m_shift.w += pair.first->m_shift_sym.w * animation_speed;
                pair.first->m_shift.h += pair.first->m_shift_sym.h * animation_speed;
                if (pair.first->can_end_animation) {
                    if (pair.first->m_shift_sym.w * pair.first->m_shift.w > 0) pair.first->m_shift.w = INFINITY;
                    if (pair.first->m_shift_sym.h * pair.first->m_shift.h > 0) pair.first->m_shift.h = INFINITY;
                }
            }
            end_cur_animation_flag = WH_ANY_IS_INF(pair.first->m_shift);
        } else {
            pair.first->can_end_animation = true;
            pair.first->m_shift.w =
                    pair.first->m_shift_sym.w * max(fabs(pair.first->m_shift.w) - animation_speed, 0.0l);
            pair.first->m_shift.h =
                    pair.first->m_shift_sym.h * max(fabs(pair.first->m_shift.h) - animation_speed, 0.0l);
            end_cur_animation_flag = WH_IS0(pair.first->m_shift);
        }
        if (end_cur_animation_flag && pair.first->can_end_animation) {
            pair.first->m_shift = {0, 0};
            pending_remove.push_back(pair.first);
        }
    }
    for (auto tile: pending_remove) {
        m_animating.erase(tile);
        m_end_animation_flag = true;
    }
    m_is_end_of_animation = m_end_animation_flag && m_animating.empty() && m_parsing_index == 0;
}


void Room::end_of_step() {
    FOREACH_TILE tile->end_of_step();
    m_can_move_flag = m_end_animation_flag = false;
    m_is_winning = true;
    for (auto dest: m_dest)
        if (!((Destination *) dest)->detect_requirement(at(dest->m_pos))) {
            m_is_winning = false;
            break;
        }
    m_steps++;
}

void Room::clear_move_status() {
    m_pending_series.clear();
    m_pending_move.clear();
    m_animating.clear();
    m_end_animation_flag = false;
}

bool Room::is_perf_play() const {
    return m_steps == m_perf;
}

bool Room::can_get_perf_play() const {
    return is_perf_play() && m_is_second_play && !m_is_perf_play;
}

bool Room::can_get_gem_play() const {
    return is_perf_play() && m_is_perf_play && m_gems.empty() && !m_is_gem_play;
}

bool Room::can_move_independents() const {
    return m_parsing_index == 0 && m_animating.empty();
}

bool Room::can_parse_movements() const {
    return m_is_moving && m_animating.empty();
}

public_code_t get_public_code() {
    return public_code++;
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
                                  long double stretch_ratio) const {
    auto surface = RENDER_TEXT(consolas->sized(FONT_SIZE(DESTINATION_SIZE)),
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

TileType construct_picture(TilePos pos, SDL_Surface *, int type) {
    return new Picture(pos, types_img_map[(tile_types) type]);
}

TileType construct_go_to(TilePos pos, SDL_Surface *img, int level) {
    return new Go_To(pos, img, level);
}

TileType construct_blue(TilePos pos, SDL_Surface *img, int) {
    return new Blue(pos, img);
}

TileType construct_spike(TilePos pos, SDL_Surface *img, int) {
    return new Spike(pos, img);
}

TileType construct_conveyor(TilePos pos, SDL_Surface *img, int dir) {
    return new Conveyor(pos, img, dir);
}

TileType construct_robot(TilePos pos, SDL_Surface *img, int dir) {
    return new Robot(pos, img, dir);
}

tile_types_map_t tile_type_map = {
        {tile_undefined, construct_undefined},
        {tile_cyan, construct_cyan},
        {tile_box, construct_box},
        {tile_wall, construct_wall},
        {tile_destination, construct_dest},
        {tile_gem, construct_gem},
        {tile_picture, construct_picture},
        {tile_go_to, construct_go_to},
        {tile_blue, construct_blue},
        {tile_spike, construct_spike},
        {tile_conveyor, construct_conveyor},
        {tile_robot, construct_robot}
};

Cyan::Cyan(TilePos pos, SDL_Surface *img) : Tile(pos, img) {}

bool Cyan::is_independent() const { return true; }

tile_types Cyan::get_type() const { return tile_cyan; }

direction_t Cyan::acq_req(Movement_Request req) { return req.direction; }

direction_t Cyan::respond_keys(key_predicate_t predicate) const {
    auto pending = find_keys(predicate, MOVEMENT_KEYS);
    if (pending.empty()) return 0;
    return pending[0];
}

Box::Box(TilePos pos, SDL_Surface *img) : Tile(pos, img) {}

tile_types Box::get_type() const { return tile_box; }

direction_t Box::acq_req(Movement_Request req) {
    return req.direction;
}

Wall::Wall(TilePos pos, SDL_Surface *img) : Tile(pos, img) {}

tile_types Wall::get_type() const { return tile_wall; }

direction_t Wall::acq_req(Movement_Request req) {
    return -1;
}

Gem::Gem(TilePos pos, SDL_Surface *img, int addition) : Tile(pos, img) {
    m_addition = addition;
}

tile_types Gem::get_type() const {
    return tile_gem;
}

void Gem::show_additional(SDL_Renderer *renderer, const DisplayPos &pos, const DisplayPos &center,
                          long double stretch_ratio) const {
    auto surface = RENDER_TEXT(consolas->sized(FONT_SIZE(DESTINATION_SIZE)),
                               to_string(m_addition).c_str(),
                               m_addition <= 0 ? GREEN : RED);
    DisplayPos show_pos = {center.w - surface->w / 2, center.h - surface->h / 2};
    auto texture = SDL_CreateTextureFromSurface(renderer, surface);
    auto srcrect = get_srcrect(surface), dstrect = get_dstrect(show_pos, surface);
    SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

Picture::Picture(TilePos pos, SDL_Surface *img) : Tile(pos, img) {}

tile_types Picture::get_type() const { return tile_picture; }

Go_To::Go_To(TilePos pos, SDL_Surface *img, int level) : Tile(pos, img) {
    m_level = level;
}

tile_types Go_To::get_type() const { return tile_go_to; }

direction_t Go_To::acq_req(Movement_Request req) {
    if (req.sender->get_type() == tile_cyan) {
        public_room->m_pending_go_to = m_level;
    }
    return Tile::acq_req(req);
}

Blue::Blue(TilePos pos, SDL_Surface *img) : Tile(pos, img) {}

tile_types Blue::get_type() const { return tile_blue; }

direction_t Blue::acq_req(Movement_Request req) {
    return req.direction;
}

Spike::Spike(TilePos pos, SDL_Surface *img) : Tile(pos, img) {}

tile_types Spike::get_type() const { return tile_spike; }

direction_t Spike::acq_req(Movement_Request req) { return 0; }

void Spike::end_of_step() {
    for (auto tile: *public_room->at(m_pos))
        if (tile->get_type() == tile_cyan)
            public_room->destroy(tile);
}

Conveyor::Conveyor(TilePos pos, SDL_Surface *img, direction_t dir) : Tile(pos, img) {
    m_dir = dir;
    m_img = direction_img_conveyor.at(m_dir);
}

tile_types Conveyor::get_type() const {
    return tile_conveyor;
}

direction_t Conveyor::acq_req(Movement_Request req) { return 0; }

void Conveyor::add_to_parser(pending_series_t &pending_series) {
    for (auto tile: *public_room->at(m_pos))
        if (tile != this) {
            pending_series.insert({tile, {this, m_dir}});
        }
}

Robot::Robot(TilePos pos, SDL_Surface *img, direction_t dir) : Tile(pos, img) {
    m_dir = dir;
    m_img = direction_img_robot.at(m_dir);
}

bool Robot::is_independent() const {
    return true;
}

tile_types Robot::get_type() const {
    return tile_robot;
}

direction_t Robot::acq_req(Movement_Request req) {
    change_dir(req.direction);
    if (req.sender->get_type() == tile_cyan) m_is_moved = true;
    return m_dir;
}

direction_t Robot::respond_keys(key_predicate_t) const {
    return m_dir;
}

void Robot::begin_request(direction_t dir) {
    change_dir(dir);
}

void Robot::change_dir(direction_t dir) {
    m_dir = dir;
    m_is_moved = true;
    m_img = direction_img_robot.at(dir);
}

void Robot::end_of_step() {
    m_is_moved = false;
}

void Robot::react_to_movement_result(bool result) {
    if (!result) {
        change_dir(invert(m_dir));
        m_is_moved = false;
    }
}

bool Robot::suppress_request(const Movement_Request &req) {
    return m_is_moved && req.sender == this;
}


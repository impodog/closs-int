//
// Created by Dogs-Cute on 12/11/2022.
//

#include "loader.h"

json default_user, current_user;
json txt_settings, txt_lobby, txt_in_game, txt_manual;
json &public_user = current_user;
string play_name;

void init_default_user() {
    ifstream default_file(DEFAULT_JSON_PATH, ios::in);
    default_file >> default_user;
}

void init_txt() {
    auto settings_file = ifstream(SETTINGS_PATH, ios::in);
    settings_file >> txt_settings;
    auto lobby_file = ifstream(LOBBY_PATH, ios::in);
    lobby_file >> txt_lobby;
    auto in_game_file = ifstream(IN_GAME_PATH, ios::in);
    in_game_file >> txt_in_game;
    auto manual_file = ifstream(MANUAL_PATH, ios::in);
    manual_file >> txt_manual;
}

void load_user() {
    ifstream user_file(USER_JSON_PATH, ios::in), play_file(PLAY_FILE_PATH, ios::in);
    if (user_file.is_open())
        user_file >> current_user;
    else
        set_user_to_default();
    if (play_file.is_open())
        play_file >> play_name;
}

void save_user() {
    if (ROOM_IS_NUMBER && current_user.at(USER_K_ROOM) > 0)
        current_user[USER_K_LEVELS] = current_user.at(USER_K_ROOM);
    ofstream user_file(USER_JSON_PATH, ios::out);
    user_file << current_user;
}

void set_user_to_default() {
    current_user = default_user;
}


RoomType open_room(const string &room_path) {
    string rel_path = ROOMS_PATH + room_path;
    ifstream room_file(rel_path, ios::in);
    if (!room_file.is_open())
        throw runtime_error("cannot open " + rel_path);
    json room_json;
    room_file >> room_json;
    return json_to_room(room_json);
}

int get_default_each(size_t w, size_t h) {
    return max((int) min(SCR_WIDTH / w, RESERVED_HEIGHT / h), MIN_EACH);
}

RoomType create_room(const json &room_json) {
    int each;
    size_t w, h;
    // Get width and height
    w = room_json.at(ROOM_K_WIDTH);
    h = room_json.at(ROOM_K_HEIGHT);
    // Get m_each(-1 or none for default)
    try {
        each = room_json.at(ROOM_K_EACH);
        if (each == -1) throw out_of_range("user explicitly using default \"m_each\" value");
        else if (each < MIN_EACH)
            throw closs_room_error("room \"m_each\" shouldn't be lower than " + to_string(MIN_EACH));
    } catch
            (const out_of_range &) {
        each = get_default_each(w, h);
    }
    // create object
    auto room = new Room(each, {w, h});
    room->m_title = room_json.at(ROOM_K_TITLE);
    room->m_help_map = room_json.at(ROOM_K_HELP);
    int next = room_json.at(ROOM_K_NEXT);
    if (ROOM_IS_NUMBER)
        room->m_next = next == -1 ? sym(USER_ROOM) * (abs(USER_ROOM) + 1) : next;
    else
        room->m_next = current_user.at(USER_K_ROOM);
    room->m_perf = (size_t) room_json.at(ROOM_K_PERF);
    room->m_unlock_bonus = room_json.at(ROOM_K_UNLOCK_BONUS);
    room->m_is_second_play = is_second_play();
    room->m_is_perf_play = is_perf_play();
    room->m_is_gem_play = is_gem_play();
    room->m_box_no_serial = room_json.at(ROOM_K_BOX_NO_SERIAL);
    return room;
}

void load_json_space(const json &json_space, const TilePos &pos, SpaceType space) {
    space->clear();
    for (auto &json_tile: json_space) {
        tile_types code;
        type_arg_t args;
        if (json_tile.is_number_integer()) {
            code = json_tile;
            args = {tile_undefined};
        } else {
            if (json_tile.size() != 2)
                throw closs_room_error("arrayed(typed) tiles are only allowed to be sized 2");
            code = json_tile[0];
            args = json_tile[1].get<type_arg_t>();
        }

        if (code == 0) continue;

        try {
            space->push_back(NEW_TILE(pos, code, args));
        } catch (const out_of_range &) {
            throw closs_room_error("invalid type code " + to_string(code));
        }
    }
}

RoomType json_to_room(const json &room_json) {
    auto room = create_room(room_json);
    // the content's width and height(width is the maximum lane's)
    size_t count_w = 0, loop_count_h = 0;
    // add lanes
    for (const auto &json_lane: room_json.at(ROOM_K_CONTENT)) {
        size_t loop_count_w = 0;
        // space in lanes
        for (const auto &json_space: json_lane) {
            auto space = room->at({loop_count_w, loop_count_h});
            // read the code
            load_json_space(json_space, {loop_count_w, loop_count_h}, space);
            loop_count_w++;
        }
        // change width if this lane is the longer than ever
        if (count_w < loop_count_w) count_w = loop_count_w;
        loop_count_h++;
    }
    // throw an error if not matching
    if (count_w != room->m_size.w || loop_count_h != room->m_size.h)
        throw closs_room_error("width or height does not match room content's ones");
    room->refresh_dest();
    room->refresh_gems();
    return room;
}

void close_room(RoomType room) {
    delete room;
}

void shift_language(bool down) {
    auto iter = std::find(usable_languages.begin(), usable_languages.end(), current_user.at(USER_K_LANGUAGE));
    if (down) {
        iter++;
        if (iter == usable_languages.end())
            iter = usable_languages.begin();
    } else {
        if (iter == usable_languages.begin())
            iter = usable_languages.end();
        iter--;
    }
    current_user[USER_K_LANGUAGE] = *iter;
}

void shift_sensitivity(bool down) {
    int sense = USER_SENSITIVITY;
    if (down) { if (sense < MAX_SENSITIVITY) current_user[USER_K_SENSITIVITY] = sense + 1; }
    else if (sense > MIN_SENSITIVITY) current_user[USER_K_SENSITIVITY] = sense - 1;
}

void shift_framerate(bool down) {
    int framerate = USER_FRAMERATE;
    if (down) { if (framerate < MAX_FRAMERATE) current_user[USER_K_FRAMERATE] = framerate + 1; }
    else if (framerate > MIN_FRAMERATE) current_user[USER_K_FRAMERATE] = framerate - 1;
}

void shift_text_renderer(bool down) {
    int renderer = current_user.at(USER_K_TEXT_RENDERER);
    if (down) { if (renderer < MAX_TEXT_RENDERER) current_user[USER_K_TEXT_RENDERER] = renderer + 1; }
    else if (renderer > MIN_TEXT_RENDERER) current_user[USER_K_TEXT_RENDERER] = renderer - 1;
}

void shift_levels(bool down, int amount) {
    int levels = USER_LEVELS;
    if (down) {
        if (levels + amount - 1 < current_user.at(USER_K_UNLOCKED))
            current_user[USER_K_LEVELS] = levels + amount;
    } else if (levels > MIN_LEVEL_NUMBER + amount - 1) current_user[USER_K_LEVELS] = levels - amount;
}

void shift_bonus_levels(bool down) {
    auto &user_bonus_levels = current_user[USER_K_BONUS_LEVELS];
    int levels = user_bonus_levels;
    auto vec = USER_BONUS.get<vector<int>>();
    auto iter = std::find(vec.begin(), vec.end(), levels);
    if (iter == vec.end()) {
        if (vec.empty()) return;
        iter = vec.begin();
    }
    if (down) {
        iter++;
        if (iter == vec.end())
            iter = vec.begin();
    } else {
        if (iter == vec.begin())
            iter = vec.end();
        iter--;
    }
    user_bonus_levels = *iter;
}

void shift_animation(bool down) {
    int animation_speed = USER_ANIMATION_SPEED;
    if (down) { if (animation_speed < MAX_ANIMATION) current_user[USER_K_ANIMATION_SPEED] = animation_speed + 1; }
    else if (animation_speed > MIN_ANIMATION) current_user[USER_K_ANIMATION_SPEED] = animation_speed - 1;
}

void debug_unlock_levels() {
    current_user[USER_K_UNLOCKED] = 128;
    current_user[USER_K_BONUS] = {1, 2, 3, 4, 5};
}

bool can_shift_to_level(int level) {
    return level >= MIN_LEVEL_NUMBER && level <= current_user.at(USER_K_UNLOCKED);
}

bool is_second_play() {
    return current_user.at(USER_K_ROOM) < current_user.at(USER_K_UNLOCKED);
}

bool is_perf_play() {
    return contains_literal(current_user.at(USER_K_PERF), current_user.at(USER_K_ROOM));
}

bool is_gem_play() {
    return contains_literal(current_user.at(USER_K_GEM), current_user.at(USER_K_ROOM));
}

string get_room_path() {
    auto room = current_user.at(USER_K_ROOM);
    return room.is_number() ? (CLOSS_DIRECTORY_PATH + to_string(current_user.at(USER_K_ROOM)) + ".json") : (ROOMS_PATH +
                                                                                                            (string) room);
}

SDL_Surface *create_text(const json &txt, int size, const SDL_Color &color, const string &addition) {
    return RENDER_TEXT(language_fonts.at(USER_LANG)->sized(size),
                       ((string) txt.at(USER_LANG) + addition).c_str(), color);
}

SDL_Surface *create_text(const json &txt, int size, bool b, const string &addition) {
    return create_text(txt, size, b ? GREEN : WHITE, addition);
}

SDL_Surface *create_text(const string &str, int size, const SDL_Color &color) {
    return RENDER_TEXT(language_fonts.at(USER_LANG)->sized(size),
                       str.c_str(), color);
}

bool contains_literal(const json &array, const json &value) {
    return std::any_of(array.begin(), array.end(), [value](const json &cmp) { return cmp == value; });
}



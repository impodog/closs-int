//
// Created by Dogs-Cute on 12/11/2022.
//

#include "loader.h"

json default_user, current_user;

void init_default_user() {
	ifstream default_json(PATH_BEGIN "user/" DEFAULT_JSON, ios::in);
	if (!default_json.is_open())
		throw runtime_error("cannot open " PATH_BEGIN "user/" DEFAULT_JSON);
	default_json >> default_user;
}

void load_user() {
	ifstream user_file(PATH_BEGIN "user/" USER_JSON, ios::in);
	if (user_file.is_open())
		user_file >> current_user;
	else
		set_user_to_default();
}

void save_user() {
	ofstream user_file(PATH_BEGIN "user/" USER_JSON, ios::out);
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
	return (int) min(SCR_WIDTH / w, SCR_HEIGHT / h);
}

RoomType json_to_room(const json &room_json) {
	int each;
	size_t w, h;
	// Get width and height
	try {
		w = room_json.at(ROOM_K_WIDTH);
		h = room_json.at(ROOM_K_HEIGHT);
	} catch (const out_of_range &) {
		throw closs_room_error("width or height configuration missing for room");
	}
	// Get each(-1 or none for default)
	try {
		each = room_json.at(ROOM_K_EACH);
		if (each == -1) throw out_of_range("user explicitly using default \"each\" value");
	} catch
			(const out_of_range &) {
		each = get_default_each(w, h);
	}
	// create object
	auto room = new Room(each, {w, h});
	// the content's width and height(width is the maximum lane's)
	size_t count_w = 0, loop_count_h = 0;
	// add lanes
	for (const auto &json_lane: room_json.at(ROOM_K_CONTENT)) {
		size_t loop_count_w = 0;
		// space in lanes
		for (const auto &json_space: json_lane) {
			auto space = room->at({loop_count_w, loop_count_h});
			// read the code
			for (int code: json_space) {
				if (code) {
					auto type_code = (tile_types) code;
					try {
						space->push_back(
								tile_type_map.at(type_code)({loop_count_w, loop_count_h},
								                            types_img_map.at(type_code)));
					} catch (const out_of_range &) {
						throw closs_room_error("invalid type code " + to_string(code));
					}
				}
			}
			loop_count_w++;
		}
		// change width if this lane is the longer than ever
		if (count_w < loop_count_w) count_w = loop_count_w;
		loop_count_h++;
	}
	// throw an error if not matching
	if (count_w != w || loop_count_h != h) throw closs_room_error("width or height does not match room content's ones");
	return room;
}

void close_room(RoomType room) {
	delete room;
}

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
	ifstream user_json(PATH_BEGIN "user/" USER_JSON, ios::in);
	if (user_json.is_open())
		user_json >> current_user;
	else
		set_user_to_default();
}

void save_user() {
	ofstream user_json(PATH_BEGIN "user/" USER_JSON, ios::out);
	user_json << current_user;
}

void set_user_to_default() {
	current_user = default_user;
}
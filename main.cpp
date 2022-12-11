#include "src/display.h"

void do_all_init() {
	init_default_user();
	load_user();
	init_img_map();
	init_key_map();
}

void do_all_free() {
	save_user();
	free_img_map();
}

int main() {
	do_all_init();
	Display display;
	auto room = OPEN_CLOSS_ROOM("test.json");
	display.change_room(room);
	do {
		display.switch_color_fill(BLACK);
		
		display.collect_loop_info();
		
		display.render_copy_room();
		
		display.present();
	} while (display.m_loop);
	close_room(room);
	do_all_free();
	return 0;
}
/*
int main() {
	auto start = system_clock::now();
	for (int i = 0; i != 100; i++) {
		auto room = new Room(50, {100, 200});
		delete room;
	}
	auto duration = duration_cast<milliseconds>(system_clock::now() - start).count();
	cout << "Time used : " << duration << endl;
	return 0;
}
*/
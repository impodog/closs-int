#include "src/display.h"

int main() {
	Display display;
	auto room = new Room(50, {5, 4});
	init_default_user();
	load_user();
	do {
		display.switch_color_fill(BLACK);
		
		display.collect_loop_info();
		
		display.present();
	} while (display.m_loop);
	save_user();
	delete room;
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
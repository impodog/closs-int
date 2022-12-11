#include <iostream>
#include "src/display.h"

int main() {
	Display display;
	Room room({5, 4});
	do {
		display.switch_color_fill(BLACK);
		
		display.collect_loop_info();
		
		display.present();
	} while (display.m_loop);
	return 0;
}

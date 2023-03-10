#include "src/display.h"

void do_all_init() {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    init_default_user();
    load_user();
    init_img();
    init_font();
    init_key_map();
    init_txt();
    init_pages();
    init_display();
}

void do_all_free() {
    save_user();
    free_img();
    free_font();
    free_pages();
    free_display();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

int main() {
    do_all_init();
    do {
        display->switch_color_fill(BLACK);
        display->collect_loop_info();
        display->process_content();
        display->present();

    } while (display->m_loop);
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
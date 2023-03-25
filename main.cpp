#include "src/page.h"

void do_all_init() {
    debugger_code = get_debugger_code();
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    init_default_user();
    load_user();
    init_img();
    init_font();
    init_mixer();
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
    free_mixer();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

void try_output_debugger_code() {
    bool is_opened;
    {
        ifstream f_in_test(I_AM_DEBUGGER, ios::in);
        is_opened = f_in_test.is_open();
    }
    if (is_opened) {
        ofstream f_out(I_AM_DEBUGGER, ios::out);
        f_out << debugger_code << endl;
    }
}

int main() {
    do_all_init();
    try_output_debugger_code();
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
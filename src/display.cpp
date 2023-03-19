//
// Created by Dogs-Cute on 12/10/2022.
//

#include "display.h"

key_down_map_t key_down_map;
key_down_map_t key_clicking_map;
key_down_map_t key_click_map;

SDL_Keycode cur_key;

void init_key_map() {
    /* INIT ALL KEYS
    init_key_map({0, 8, 9, 13, 27, 127, 1073741957, 1073741958});
    init_key_map(32, 64);
    init_key_map(91, 122);
    init_key_map(1073741901, 1073741923);
    init_key_map(1073741925, 1073741953);
    init_key_map(1073741977, 1073741988);
    init_key_map(1073742000, 1073742045);
    init_key_map(1073742048, 1073742055);
    */
    init_key_map(KEY_NEEDED);
    key_click_map = key_down_map_t(key_down_map);
}

void init_key_map(SDL_Keycode code) {
    key_down_map[code] = key_clicking_map[code] = false;
}

void init_key_map(const initializer_list<SDL_Keycode> &codes) {
    for (auto code: codes)
        init_key_map(code);
}

void init_key_map(SDL_Keycode begin, SDL_Keycode end) {
    for (SDL_Keycode i = begin; i != end; i++) {
        key_down_map[i] = key_clicking_map[i] = false;
    }
}

void key_down(SDL_Keycode code) {
    key_clicking_map[code] = key_down_map[code];
    key_down_map[code] = true;
}

bool key_d(SDL_Keycode code) {
    return key_down_map[code];
}

bool key_d(initializer_list<SDL_Keycode> codes) {
    return any_of(codes.begin(), codes.end(), [](SDL_Keycode code) { return key_d(code); });
}

bool key_clicking(SDL_Keycode code) {
    auto click = key_clicking_map[code];
    bool result = (key_clicking_map[code] = key_down_map[code]) != click;
    return result;
}

bool key_c(SDL_Keycode code) {
    return key_click_map.at(code);
}

bool key_c(initializer_list<SDL_Keycode> codes) {
    return any_of(codes.begin(), codes.end(), [](SDL_Keycode code) { return key_c(code); });
}

SDL_Surface *create_settings_text(const string &setting, const string &from_user, bool b) {
    return create_text(txt_settings.at(setting), SETTINGS_SIZE, b,
                       from_user.empty() ? "" : to_string(current_user.at(from_user)));
}

SDL_Surface *create_lobby_text(const string &lobby, bool b) {
    return create_text(txt_lobby.at(lobby), LOBBY_SIZE, b);
}

void init_display() {
    display = new Display;
}

void free_display() {
    delete display;
}

void start_game() {
    try {
        auto room = open_room(get_room_path());
        display->change_room(room);
        display->m_page = nullptr;
    } catch (const std::runtime_error &) {
        current_user.at(USER_K_ROOM) = (int) current_user.at(USER_K_ROOM) - 1;
    }
}

void refresh_user_game() {
    save_user();
    start_game();
}

level_pic_map_t level_pic_map;


Display::Display() {
    Screen_Info info;
    get_scr_info(info);
    window = SDL_CreateWindow(TITLE,
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              (int) (info.w * SCR_RATIO), (int) (info.h * SCR_RATIO),
                              SDL_WINDOW_RESIZABLE);
    SDL_SetWindowIcon(window, img_icon);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(renderer, SCR_WIDTH, SCR_HEIGHT);
    m_page = page_lobby;
    for (auto &debug_room: m_debug_room)
        debug_room = nullptr;
    apply_settings();
    collect_loop_info();
}

Display::~Display() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void Display::apply_settings() {
    framerate_ratio = (long double) STANDARD_FRAMERATE / USER_FRAMERATE;
    m_sensitivity = (int) max(USER_SENSITIVITY * framerate_ratio, 1.0l);
    m_delay = 1000 / USER_FRAMERATE;
    animation_speed = USER_ANIMATION_SPEED / MAX_ANIMATION;
    m_debugger = current_user.at(USER_K_DEBUGGER) == debugger_code;
    if (m_debugger) {
        debug_unlock_levels();
        prompt("DEBUGGER mode is turned on with code " + to_string(debugger_code));
    }
    text_renderer = text_renderer_map.at(current_user.at(USER_K_TEXT_RENDERER));
    reload_pages();
}

void Display::collect_loop_info() {
    auto now = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(now - m_last).count();
    if (m_debugger && !debug_prompt.empty()) {
        list<const string *> to_delete;
        for (auto &prompt_info: debug_prompt)
            if ((prompt_info.second -= duration) <= 0) {
                prompt_info.second = 0;
                to_delete.push_back(&prompt_info.first);
            }
        for (auto key_to_delete: to_delete)
            debug_prompt.erase(*key_to_delete);
    }
    m_last = now;


    SDL_PollEvent(&event);
    if (m_delay > duration) SDL_Delay(m_delay - duration);
    switch (event.type) {
        case SDL_QUIT:
            m_loop = false;
            break;
        case SDL_KEYDOWN:
            KEY_DOWN(event.key.keysym.sym);
            break;
        case SDL_KEYUP:
            KEY_UP(event.key.keysym.sym);
            break;
    }
    refresh_key_m();
}

void Display::process_room_winning() {
    bool confirm = key_c(KEY_CONFIRM);
    PageType chapter_end = nullptr;
    int to_shift = 0;
    if (confirm && m_room->m_pending_go_to != 0) {
        current_user[USER_K_ROOM] = m_room->m_pending_go_to;
        refresh_user_game();
    } else if (m_room->m_is_winning && (confirm || key_c(KEY_SAVE_AND_REPLAY))) {
        if (!m_room->m_is_second_play && ROOM_IS_NUMBER &&
            level_pic_map.find(current_user.at(USER_K_ROOM)) != level_pic_map.end())
            chapter_end = level_pic_map.at(current_user[USER_K_ROOM]);
        if (m_room->can_get_perf_play())
            current_user[USER_K_PERF].push_back(current_user.at(USER_K_ROOM));
        if (m_room->can_get_gem_play()) {
            current_user[USER_K_GEM].push_back(current_user.at(USER_K_ROOM));
            if (m_room->m_unlock_bonus != 0 && !contains_literal(USER_BONUS, m_room->m_unlock_bonus))
                current_user[USER_K_BONUS].push_back(m_room->m_unlock_bonus);
        }
        if (m_room->m_next.is_number_integer() && m_room->m_next != -1) {
            int next = (int) m_room->m_next;
            if (confirm) current_user[USER_K_ROOM] = next;
            current_user[USER_K_UNLOCKED] = max(next, (int) current_user[USER_K_UNLOCKED]);
        } else if (m_room->m_next.is_number_float()) {
            if (confirm) current_user[USER_K_ROOM] = (float) m_room->m_next;
        } else if (confirm) {
            current_user[USER_K_ROOM] = (string) m_room->m_next;
        }
        refresh_user_game();
        if (chapter_end != nullptr)
            m_page = chapter_end;
    } else if (key_c(KEY_NEXT))
        to_shift = USER_ROOM + 1;
    else if (key_c(KEY_BACK))
        to_shift = USER_ROOM - 1;
    if (can_shift_to_level(to_shift) || (m_debugger && to_shift != 0)) {
        current_user[USER_K_ROOM] = to_shift;
        refresh_user_game();
    }
}

void Display::process_room() {
    if (key_d(KEY_SHIFT_UP)) m_room_pos.h -= m_sensitivity;
    if (key_d(KEY_SHIFT_LEFT)) m_room_pos.w -= m_sensitivity;
    if (key_d(KEY_SHIFT_DOWN)) m_room_pos.h += m_sensitivity;
    if (key_d(KEY_SHIFT_RIGHT)) m_room_pos.w += m_sensitivity;
    if (key_c(KEY_RESTART))
        refresh_user_game();

    if (m_debugger) debug_room();

    move_room_to_visible();

    if (m_room->can_move_independents())
        m_room->move_independents(key_c);

    if (m_room->can_parse_movements()) {
        m_room->parse_series();
        if (!m_room->m_can_move_flag) m_room->clear_move_status(); // this is usable because cyan always comes first
        m_room->do_pending_moves();
    }
    m_room->animate_tiles(animation_speed * framerate_ratio, m_room_pos);
    if (m_room->m_is_end_of_animation) {
        m_room->detect_gems();
        m_room->end_of_step();
    }
    process_room_winning();

    if (key_d(KEY_HELP)) m_page = create_text_page(img_help, m_room->m_help_map, true);
}

void Display::process_content() {
    if (key_c(KEY_SETTINGS)) {
        apply_settings();
        if (m_page == nullptr) m_page = page_settings;
        else if (m_room == nullptr) m_page = page_lobby;
        else m_page = nullptr;
    }

    if (IS_GAMING) {
        process_room();
        show_room();
    } else {
        m_page->process();
        if (m_page != nullptr) m_page->show();
    }
}

void Display::switch_color(const SDL_Color &color) const {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void Display::switch_color_fill(const SDL_Color &color) const {
    switch_color(color);
    SDL_RenderClear(renderer);
}

void Display::switch_color_fill(const SDL_Color &color, const SDL_Rect &dstrect) const {
    switch_color(color);
    SDL_RenderFillRect(renderer, &dstrect);
}

void Display::present() const {
    show_prompt();
    SDL_RenderPresent(renderer);
}

void Display::change_room(RoomType room) {
    clear_room();
    public_room = m_room = room;
    DisplayPos m_room_edge = {SCR_WIDTH - m_room->m_display_size.w, RESERVED_HEIGHT - m_room->m_display_size.h};
    m_room_min = {min(m_room_edge.w, 0), min(m_room_edge.h, 0)};
    m_room_max = {max(m_room_edge.w, 0), max(m_room_edge.h, 0)};
    stretch_ratio = (long double) (m_room->m_each) / STANDARD_EACH / 1.5;
    m_room_pos = {0, 0};
    if (m_room_min.w == 0) m_room_pos.w = m_room_edge.w / 2;
    if (m_room_min.h == 0) m_room_pos.h = m_room_edge.h / 2;
    clear_img_vec();
}

void Display::change_page(PageType page) {
    m_page = page;
}

SDL_Rect Display::get_rect(const SDL_Surface *img, const TilePos &pos) const {
    DisplayPos double_edge_rect{(int) ((ROOM_EACH - img->w * stretch_ratio)),
                                (int) ((ROOM_EACH - img->h * stretch_ratio))};
    return {(int) ((double) (ROOM_EACH * pos.w + double_edge_rect.w / 2.0 + m_room_pos.w)),
            (int) ((double) (ROOM_EACH * pos.h + double_edge_rect.h / 2.0 + m_room_pos.h)),
            ROOM_EACH - double_edge_rect.w, ROOM_EACH - double_edge_rect.h};
}

DisplayPos Display::get_center(const TilePos &pos) const {
    return {(int) (ROOM_EACH * ((int) pos.w + 0.5)) + m_room_pos.w,
            (int) (ROOM_EACH * ((int) pos.h + 0.5)) + m_room_pos.h};
}

void Display::show_tile(Tile &tile) {
    auto pos = show_img(tile.m_img, tile.m_pos, DisplayPos(tile.m_shift * ROOM_EACH));
    tile.show_additional(renderer, pos, get_center(tile.m_pos), stretch_ratio);
}

DisplayPos Display::show_img(SDL_Surface *img, const TilePos &pos, const DisplayPos &addition) {
    auto info = find_info(img);
    SDL_Rect dstrect = get_rect(img, pos);
    dstrect.x += addition.w;
    dstrect.y += addition.h;
    SDL_RenderCopy(renderer, info.texture, &info.srcrect, &dstrect);
    return {dstrect.w, dstrect.h};
}

void Display::show_room_info() const {
    auto surface = create_text(m_room->m_title, ROOM_TITLE_SIZE);
    show_surface(renderer, surface, {LEAVE_BLANK_WIDTH, RESERVED_HEIGHT + (RESERVED_FROM_B - surface->h) / 2});
    int title_end_w = LEAVE_BLANK_WIDTH + surface->w;
    SDL_FreeSurface(surface);
    SDL_Color steps_color = WHITE;
    string steps_text = " " + to_string(m_room->m_steps);
    if (m_room->m_is_second_play && m_room->m_perf != 0) {
        if (m_room->m_perf < m_room->m_steps) steps_color = RED;
        else if (m_room->m_perf == m_room->m_steps) {
            if (m_room->m_is_perf_play) {
                if (m_room->m_gems.empty()) steps_color = GOLD;
                else steps_color = WHITE;
            } else steps_color = GREEN;
        } else {
            long double steps_percent = m_room->m_steps / (long double) m_room->m_perf, rev_percent =
                    1 - steps_percent;
            steps_color = {(Uint8) (200 * steps_percent), (Uint8) (200 * rev_percent), (Uint8) (200 * rev_percent)};
        }
        steps_text += "/" + to_string(m_room->m_perf);
    }
    surface = create_text(txt_in_game.at(IN_GAME_K_STEPS), ROOM_TITLE_SIZE, steps_color, steps_text);
    show_surface(renderer, surface,
                 {title_end_w + LEAVE_BLANK_WIDTH, RESERVED_HEIGHT + (RESERVED_FROM_B - surface->h) / 2});
    SDL_FreeSurface(surface);
}

void Display::show_room_winning() const {
    if (m_room->m_is_winning) {
        auto surface = create_text(txt_in_game.at(IN_GAME_K_WINNING), WINNING_SIZE, GREEN);
        show_surface(renderer, surface, {WINNING_WIDTH, RESERVED_HEIGHT + (RESERVED_FROM_B - WINNING_SIZE) / 2});
        SDL_FreeSurface(surface);
    }
}

void Display::show_room() {
    size_t w, h = 0;
    for (const auto &lane: m_room->m_distribute) {
        w = 0;
        for (const auto &space: *lane) {
            show_img(types_img_map[tile_background], {w, h});
            w++;
        }
        h++;
    }
    for (const auto &lane: m_room->m_distribute)
        for (const auto &space: *lane)
            for (const auto &tile: *space) {
                show_tile(*tile);
            }
    switch_color_fill(BLACK, {0, RESERVED_HEIGHT, SCR_WIDTH, RESERVED_FROM_B});
    show_reservation_line();
    show_room_info();
    show_room_winning();
}

void Display::show_reservation_line() const {
    SDL_RenderDrawLine(renderer, 0, RESERVED_HEIGHT, SCR_WIDTH, RESERVED_HEIGHT);
}

void Display::clear_img_vec() {
    if (!img_map.empty()) {
        for (auto pair: img_map) {
            SDL_DestroyTexture(pair.second.texture);
        }
        img_map.clear();
    }
}

void Display::clear_room() {
    close_room(m_room);
    m_room = nullptr;
}

void Display::return_to_game() {
    if (m_room == nullptr) m_page = page_lobby;
    else m_page = nullptr;
}

img_info &Display::find_info(SDL_Surface *surface) {
    try {
        return img_map.at(surface);
    } catch (const out_of_range &) {
        return (img_map[surface] = {SDL_CreateTextureFromSurface(renderer, surface), get_srcrect(surface)});
    }
}

void Display::move_room_to_visible() {
    m_room_pos.w = min(max(m_room_pos.w, m_room_min.w), m_room_max.w);
    m_room_pos.h = min(max(m_room_pos.h, m_room_min.h), m_room_max.h);
}

void Display::refresh_key_m() {
    for (const auto &pair: key_click_map) {
        key_click_map[pair.first] = key_clicking(pair.first);
        if (key_click_map[pair.first])
            cur_key = pair.first;
        else if (cur_key == pair.first)
            cur_key = 0;
    }
}

void Display::prompt(const string &s) {
    debug_prompt.insert({s + '\n', DEBUGGER_PROMPT_TIME});
}

void Display::show_prompt() const {
    int height = 0;
    for (auto &prompt_info: debug_prompt) {
        auto prompt = TTF_RenderText_Solid_Wrapped(verdana->sized(DEBUGGER_FONT_SIZE), prompt_info.first.c_str(),
                                                   WHITE,
                                                   0);
        show_surface(renderer, prompt, {(SCR_WIDTH - prompt->w) / 2, height});
        height += prompt->h;
        SDL_FreeSurface(prompt);
    }
}

void Display::debug_room() {
    if (key_c(KEY_DEBUG_PERFECT)) {
        auto old_steps = m_room->m_steps;
        m_room->m_steps = m_room->m_perf;
        if (m_room->can_get_perf_play()) {
            current_user[USER_K_PERF].push_back(USER_ROOM);
            refresh_user_game();
        }
        m_room->m_steps = old_steps;
        prompt("Room is set to perfection.");
    } else if (SDLK_1 <= cur_key && cur_key <= SDLK_9) {
        if (m_room->m_is_moving)
            prompt("Please wait until the animation is done!");
        else {
            auto index = cur_key - SDLK_1;
            auto key_str = to_string(index + 1);
            bool is_null = m_debug_room[index] == nullptr;
            if (key_d(KEY_CTRL)) {
                if (!is_null)
                    close_room(m_debug_room[index]);
                if (key_d(KEY_SHIFT)) {
                    m_debug_room[index] = nullptr;
                    if (is_null)
                        prompt("Room status #" + key_str + " is empty. No need to delete.");
                    else
                        prompt("Room status #" + key_str + " deleted.");
                } else {
                    m_debug_room[index] = new Room(m_room);
                    if (is_null)
                        prompt("Room status #" + key_str + " saved.");
                    else
                        prompt("Room status #" + key_str + " overwritten.");
                }
            } else {
                if (is_null)
                    prompt("Room status #" + key_str + " is empty. Abort reverting.");
                else {
                    change_room(new Room(m_debug_room[index]));
                    prompt("Room reverted to status #" + key_str);
                }
            }
        }
    } else if (key_c(KEY_DEBUG_WIN)) {
        m_room->m_is_winning = true;
        prompt("Room is now at winning status.");
    } else if (key_c(KEY_DEBUG_SET_TO_FIRST_PLAY)) {
        m_room->m_is_second_play = m_room->m_is_perf_play = m_room->m_is_gem_play = false;
        prompt("Room status is temporarily set to first play.");
    }
}


DisplayType display;


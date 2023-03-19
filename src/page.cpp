//
// Created by Dogs-Cute on 3/17/2023.
//

#include "page.h"

PageType page_settings, page_lobby, page_levels;
PageType page_manual = nullptr;

void init_pages() {
    page_settings = new Selection_Page{img_settings,
                                       {
                                               [](bool b) { // 1 language
                                                   return create_settings_text(SETTINGS_K_LANGUAGE, USER_K_LANGUAGE, b);
                                               },
                                               [](bool b) { // 2 sensitivity
                                                   return create_settings_text(SETTINGS_K_SENSITIVITY,
                                                                               USER_K_SENSITIVITY,
                                                                               b);
                                               },
                                               [](bool b) { // 3 framerate
                                                   return create_settings_text(SETTINGS_K_FRAMERATE, USER_K_FRAMERATE,
                                                                               b);
                                               },
                                               [](bool b) { // 4 animation
                                                   return create_settings_text(SETTINGS_K_ANIMATION,
                                                                               USER_K_ANIMATION_SPEED, b);
                                               },
                                               [](bool b) { // 5 text renderer
                                                   return create_settings_text(SETTINGS_K_TEXT_RENDERER,
                                                                               USER_K_TEXT_RENDERER, b);
                                               },
                                               [](bool b) { // 6 apply
                                                   return create_settings_text(SETTINGS_K_APPLY, "", b);
                                               },
                                               [](bool b) { // 7 to_lobby
                                                   return create_settings_text(SETTINGS_K_TO_LOBBY, "", b);
                                               },
                                               [](bool b) { // 8 to_game
                                                   return create_settings_text(SETTINGS_K_TO_GAME, "", b);
                                               }
                                       },
                                       {
                                               [] { // 1 language
                                                   if (key_c(KEY_MOVE_RIGHT))
                                                       shift_language(true);
                                                   else if (key_c(KEY_MOVE_LEFT))
                                                       shift_language(false);
                                               },
                                               [] { // 2 sensitivity
                                                   if (key_d(KEY_MOVE_RIGHT))
                                                       shift_sensitivity(true);
                                                   else if (key_d(KEY_MOVE_LEFT))
                                                       shift_sensitivity(false);
                                               },
                                               [] { // 3 framerate
                                                   if (key_d(KEY_MOVE_RIGHT))
                                                       shift_framerate(true);
                                                   else if (key_d(KEY_MOVE_LEFT))
                                                       shift_framerate(false);
                                               },
                                               [] { // 4 animation
                                                   if (key_d(KEY_MOVE_RIGHT))
                                                       shift_animation(true);
                                                   else if (key_d(KEY_MOVE_LEFT))
                                                       shift_animation(false);
                                               },
                                               [] { // 5 text renderer
                                                   if (key_d(KEY_MOVE_RIGHT))
                                                       shift_text_renderer(true);
                                                   else if (key_d(KEY_MOVE_LEFT))
                                                       shift_text_renderer(false);
                                               },
                                               [] { // 6 apply
                                                   if (key_c(KEY_CONFIRM)) display->apply_settings();
                                               },
                                               [] { // 7 to_lobby
                                                   if (key_c(KEY_CONFIRM)) {
                                                       display->apply_settings();
                                                       display->m_page = page_lobby;
                                                       // Also, returning to lobby will clear room
                                                       display->clear_room();
                                                   }
                                               },
                                               [] { // 8 to_game
                                                   if (key_c(KEY_CONFIRM)) {
                                                       display->apply_settings();
                                                       display->return_to_game();
                                                   }
                                               }
                                       },
                                       SETTINGS_EACH
    };
    page_lobby = new Selection_Page{img_Closs_InT,
                                    {
                                            [](bool b) {
                                                return create_lobby_text(LOBBY_K_PLAY, b);
                                            },
                                            [](bool b) {
                                                return create_lobby_text(LOBBY_K_SETTINGS, b);
                                            },
                                            [](bool b) {
                                                return create_lobby_text(LOBBY_K_MANUAL, b);
                                            },
                                            [](bool b) {
                                                return create_lobby_text(LOBBY_K_QUIT, b);
                                            }
                                    },
                                    {
                                            [] {
                                                if (key_c(KEY_CONFIRM)) {
                                                    if (play_name.empty())
                                                        display->m_page = page_levels;
                                                    else {
                                                        current_user[USER_K_ROOM] = play_name;
                                                        start_game();
                                                    }
                                                }
                                            },
                                            [] {
                                                if (key_c(KEY_CONFIRM)) display->m_page = page_settings;
                                            },
                                            [] {
                                                if (key_c(KEY_CONFIRM)) display->m_page = page_manual;
                                            },
                                            [] {
                                                if (key_c(KEY_CONFIRM)) display->m_loop = false;
                                            }
                                    },
                                    LOBBY_EACH
    };
    page_levels = new Selection_Page{img_levels,
                                     {
                                             [](bool b) {
                                                 SDL_Color color = WHITE;
                                                 json levels = current_user.at(USER_K_LEVELS);
                                                 if (b) {
                                                     if (contains_literal(current_user.at(USER_K_PERF),
                                                                          levels)) {
                                                         if (contains_literal(current_user.at(USER_K_GEM),
                                                                              levels))
                                                             color = GOLD;
                                                         else color = HALF_LIGHT_GREEN;
                                                     } else color = LIGHT_CYAN;
                                                 }
                                                 return create_text(txt_in_game.at(IN_GAME_K_LEVELS), LEVELS_SIZE,
                                                                    color,
                                                                    to_string(levels) + "/" +
                                                                    to_string(current_user.at(USER_K_UNLOCKED)));
                                             },
                                             [](bool b) {
                                                 json bonus_levels = current_user.at(USER_K_BONUS_LEVELS);
                                                 SDL_Color color = WHITE;
                                                 if (b) color = bonus_levels == 0 ? LIGHT_RED : LIGHT_CYAN;
                                                 return create_text(txt_in_game.at(
                                                                            USER_BONUS.empty() ? IN_GAME_K_BONUS_LEVELS_LOCKED
                                                                                               : IN_GAME_K_BONUS_LEVELS),
                                                                    LEVELS_SIZE,
                                                                    color,
                                                                    to_string(bonus_levels));
                                             }
                                     },
                                     {
                                             [] {
                                                 if (key_c(KEY_MOVE_RIGHT)) {
                                                     shift_levels(true, 1);
                                                 } else if (key_c(KEY_MOVE_LEFT)) {
                                                     shift_levels(false, 1);
                                                 } else if (key_c(KEY_NEXT)) {
                                                     shift_levels(true, 10);
                                                 } else if (key_c(KEY_BACK)) {
                                                     shift_levels(false, 10);
                                                 } else if (key_c(KEY_CONFIRM)) {
                                                     current_user[USER_K_ROOM] = current_user.at(USER_K_LEVELS);
                                                     refresh_user_game();
                                                 }
                                             },
                                             [] {
                                                 if (key_c(KEY_MOVE_RIGHT)) {
                                                     shift_bonus_levels(true);
                                                 } else if (key_c(KEY_MOVE_LEFT)) {
                                                     shift_bonus_levels(false);
                                                 } else if (key_c(KEY_CONFIRM) &&
                                                            current_user.at(USER_K_BONUS_LEVELS) != 0) {
                                                     current_user[USER_K_ROOM] =
                                                             (-(int) current_user.at(USER_K_BONUS_LEVELS)) * 5 - 1;
                                                     refresh_user_game();
                                                 }
                                             }
                                     },
                                     LEVELS_EACH
    };
}

void free_variable_pages() {
    delete page_manual;
    for (auto pair: level_pic_map)
        delete pair.second;
}

void free_pages() {
    delete page_settings;
    delete page_lobby;
    free_variable_pages();
}


void reload_pages() {
    free_variable_pages();
    page_manual = new Text_Page{img_manual, txt_manual, false};
    level_pic_map = {
            {20,  new Text_Page{img_chapter1, txt_in_game.at(IN_GAME_K_CHAPTER1), false}},
            {40,  new Text_Page{img_chapter2, txt_in_game.at(IN_GAME_K_CHAPTER2), false}},
            {60,  new Text_Page{img_chapter3, txt_in_game.at(IN_GAME_K_CHAPTER3), false}},
            {80,  new Text_Page{img_chapter4, txt_in_game.at(IN_GAME_K_CHAPTER4), false}},
            {100, new Text_Page{img_chapter5, txt_in_game.at(IN_GAME_K_CHAPTER5), false}},
            {-10, new Text_Page{img_bonus1, txt_in_game.at(IN_GAME_K_BONUS1), false}}
    };
}

PageType create_text_page(SDL_Surface *title, json &text_map, bool release) {
    return new Text_Page{title, text_map, release};
}

Selection_Page::Selection_Page(SDL_Surface *title, selection_generators_const_t generators,
                               selection_processors_const_t processors, int each) {
    m_title = title;
    m_title_pos = {(SCR_WIDTH - m_title->w) / 2, 0};
    m_generators = generators;
    m_processors = processors;
    m_each = each;
    if (m_processors.size() != (m_size = m_generators.size()))
        throw closs_page_error("Page generator number is not equal to processor number");
}

void Selection_Page::show() {
    show_surface(display->renderer, m_title, m_title_pos);
    for (size_t i = 0; i < m_size; i++) {
        bool is_selected = i == m_index;
        auto surface = m_generators.at(i)(is_selected);
        auto upper_h = (int) (m_each * (i + 1) + m_title->h);
        if (is_selected) show_surface(display->renderer, img_arrow, {0, upper_h + (m_each - img_arrow->h) / 2});
        show_surface(display->renderer, surface, {img_arrow->w, upper_h + (m_each - surface->h) / 2});
        SDL_FreeSurface(surface);
    }
}

void Selection_Page::process() {
    if (key_c(KEY_MOVE_UP)) m_index += m_size - 1;
    else if (key_c(KEY_MOVE_DOWN)) m_index++;
    m_index %= m_size;
    m_processors.at(m_index)();
}

Text_Page::Text_Page(SDL_Surface *title, json &text_map, bool release) {
    m_release = release;
    m_title = title;
    m_title_pos = {(SCR_WIDTH - m_title->w) / 2, 0};
    SDL_Surface *surface = nullptr;
    int height = SCR_HEIGHT;
    bool goto_flag = true;
    auto &text_json = text_map.at(USER_LANG);
    bool is_array = text_json.is_array();
    for (const auto &element: text_json.items()) {
        content_height_refresh:
        int content_height = height + (TEXT_PAGE_EACH - TEXT_PAGE_SIZE) / 2;
        if (content_height > SCR_HEIGHT - TEXT_PAGE_EACH - m_title->h && goto_flag) {
            surface = SDL_CreateRGBSurface(0, SCR_WIDTH, SCR_HEIGHT - m_title->h, 32, 0, 0, 0, 0);
            height = TEXT_PAGE_SIZE;
            m_sides.push_back(surface);
            goto_flag = false;
            goto content_height_refresh;
        } else
            goto_flag = true;

        SDL_Surface_ptr title_surf, descr_surf;
        if (is_array) {
            title_surf = create_text((string) element.value().at(0), TEXT_PAGE_SIZE, WHITE);
            descr_surf = create_text((string) element.value().at(1), TEXT_PAGE_SIZE, GREY);
        } else {
            title_surf = create_text((string) element.key(), TEXT_PAGE_SIZE, WHITE);
            descr_surf = create_text((string) element.value(), TEXT_PAGE_SIZE, GREY);
        }
        SDL_Rect title_dstrect = get_dstrect({LEAVE_BLANK_WIDTH, content_height}, title_surf),
                descr_dstrect = get_dstrect({LEAVE_BLANK_WIDTH * 2 + title_surf->w, content_height}, descr_surf);
        SDL_BlitSurface(title_surf, nullptr, surface, &title_dstrect);
        SDL_BlitSurface(descr_surf, nullptr, surface, &descr_dstrect);

        height += TEXT_PAGE_EACH;
        SDL_FreeSurface(title_surf);
        SDL_FreeSurface(descr_surf);
    }
    m_size = m_sides.size();
}

Text_Page::~Text_Page() {
    for (auto surface: m_sides) SDL_FreeSurface(surface);
}

void Text_Page::show() {
    show_surface(display->renderer, m_title, m_title_pos);
    show_surface(display->renderer, m_sides.at(m_index), {0, m_title->h});
}

void Text_Page::process() {
    if (key_c(KEY_CONFIRM)) {
        display->return_to_game();
        if (m_release) this->~Text_Page();
    }
    if (key_c(KEY_MOVE_LEFT) || key_c(KEY_MOVE_UP)) m_index += m_size - 1;
    else if (key_c(KEY_MOVE_RIGHT) || key_c(KEY_MOVE_DOWN)) m_index++;
    m_index %= m_size;
}

#include "page.h"

//
// Created by Dogs-Cute on 3/17/2023.
//

#ifndef CLOSS_INT_PAGE_H
#define CLOSS_INT_PAGE_H

#include "display.h"

using selection_surface_generator_t = SDL_Surface *(*)(bool);
using selection_generators_t = vector<selection_surface_generator_t>;
using selection_generators_const_t = const vector<selection_surface_generator_t> &;

using selection_click_processor_t = void (*)();
using selection_processors_t = vector<selection_click_processor_t>;
using selection_processors_const_t = const vector<selection_click_processor_t> &;
using text_sides_t = vector<SDL_Surface *>;


class Selection_Page : public Page {
public:
    SDL_Surface *m_title;
    DisplayPos m_title_pos;

    selection_generators_t m_generators;
    selection_processors_t m_processors;

    size_t m_index = 0, m_size;

    int m_each;

    Selection_Page(SDL_Surface *title, selection_generators_const_t selections, selection_processors_const_t processors,
                   int each = SELECTION_DEFAULT_EACH);

    void show() override;

    void process() override;
};

using Selection_Page_Type = Selection_Page *;
using Selection_Page_Const = const Selection_Page *;

class Text_Page : public Page {
public:
    bool m_release;

    SDL_Surface *m_title;
    DisplayPos m_title_pos{};

    text_sides_t m_sides;
    size_t m_index = 0, m_size;

    string m_language = current_user.at(USER_K_LANGUAGE);

    Text_Page(SDL_Surface *title, json &text_map, bool release);

    ~Text_Page();

    void show() override;

    void process() override;
};

using Text_Page_Type = Text_Page *;
using Text_Page_Const = const Text_Page *;

#endif //CLOSS_INT_PAGE_H

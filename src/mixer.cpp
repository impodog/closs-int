//
// Created by Dogs-Cute on 3/25/2023.
//

#include "mixer.h"

#define SCHEME_NONE "None"

vector<string> scheme_names = {SCHEME_NONE};
using music_map_t = unordered_map<music_index, Mix_Music *>;
music_map_t musics;
unordered_map<string, music_map_t> music_schemes;
const map<pair<int, int>, music_index> music_chapters = {
        {{1,   20},  mus_chapter1},
        {{21,  40},  mus_chapter2},
        {{41,  60},  mus_chapter3},
        {{61,  80},  mus_chapter4},
        {{81,  98},  mus_chapter5_front},
        {{99,  100}, mus_chapter5_back},
        {{101, 120}, mus_chapter5plusplus_front},
        {{121, 128}, mus_chapter5plusplus_back},
        {{-10, -6},  mus_bonus1},
        {{-15, -11}, mus_bonus2}
};
music_index cur_music = mus_none;
string cur_scheme = SCHEME_NONE;
json schemes_json;

void free_scheme(const string &scheme) {
    Mix_HaltMusic();
    for (auto &music: music_schemes.at(scheme))
        Mix_FreeMusic(music.second);
    music_schemes.erase(scheme);
}

void free_schemes() {
    for (auto &scheme: music_schemes)
        for (auto &music: scheme.second)
            Mix_FreeMusic(music.second);
    music_schemes.clear();
}

void load_scheme(const string &scheme) {
    scheme_names.push_back(scheme);
    music_schemes.insert({scheme, {}});
    auto &scheme_map = music_schemes.at(scheme);
    int number = 1;
    for (auto &music: schemes_json.at(scheme)) {
        string music_path = SOUND_PATH + (string) scheme + "/" + (string) music;
        auto music_ptr = Mix_LoadMUS(music_path.c_str());
        if (music_ptr == nullptr)
            throw runtime_error("Cannot open music resource \"" + music_path + "\"");
        scheme_map.insert({(music_index) number, music_ptr});
        number += 1;
    }
}

void switch_scheme(const string &scheme) {
    Mix_PauseMusic();
    if (scheme == SCHEME_NONE) {
        free_schemes();
        musics.clear();
        return;
    }

    if (music_schemes.find(scheme) == music_schemes.end())
        load_scheme(scheme);
    for (auto &music: music_schemes.at(scheme))
        musics[music.first] = music.second;
}


void init_mixer() {
    Mix_Init(MIX_INIT_OGG);
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
    ifstream schemes_file(SOUND_SCHEMES_PATH, ios::in);
    schemes_file >> schemes_json;
    for (auto &scheme: schemes_json.get<map<string, json>>())
        scheme_names.push_back(scheme.first);
    switch_scheme(current_user.at(USER_K_MUS_SCHEME));
}

void free_mixer() {
    free_schemes();
    Mix_Quit();
}

void play_room_music(int room) {
    for (auto &chapter: music_chapters)
        if (chapter.first.first <= room && room <= chapter.first.second) {
            if (cur_music == chapter.second && cur_scheme == current_user.at(USER_K_MUS_SCHEME)) {
                Mix_ResumeMusic();
            } else {
                music_switch_to(chapter.second);
                cur_music = chapter.second;
                cur_scheme = current_user.at(USER_K_MUS_SCHEME);
            }
            return;
        }
    cur_music = mus_none;
    Mix_FadeOutMusic(MUSIC_FADE_OUT);
}

void music_switch_to(music_index index) {
    if (Mix_PausedMusic())
        Mix_HaltMusic();
    else
        Mix_FadeOutMusic(MUSIC_FADE_OUT);
    try {
        Mix_FadeInMusic(musics.at(index), -1, MUSIC_FADE_IN);
    } catch (const out_of_range &) {}
}

void shift_music_scheme(bool down) {
    auto iter = std::find(scheme_names.begin(), scheme_names.end(), current_user.at(USER_K_MUS_SCHEME));
    if (down) {
        iter++;
        if (iter == scheme_names.end())
            iter = scheme_names.begin();
    } else {
        if (iter == scheme_names.begin())
            iter = scheme_names.end();
        iter--;
    }
    switch_scheme(current_user[USER_K_MUS_SCHEME] = *iter);
}

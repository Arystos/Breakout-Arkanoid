//
// Created by Aristide Sessa on 09/08/25.
//

#ifndef BREAKOUT_STATE_PAUSE_HPP
#define BREAKOUT_STATE_PAUSE_HPP

#include "State.hpp"
#include "UI.hpp"
#include "Entity_Ball.hpp"
#include <iostream>
#include <SDL_ttf.h>

class State_Pause : public State {
public:
    void handleInput(Game &game, const SDL_Event &event) override;
    void update(Game &game, float dt) override;
    void render(Game &game) override;
    void onEnter(Game& game) override;
    void onExit(Game& game) override;
    const char* getName() const override { return "Pause"; }
    
private:
    int selected{0};
    std::vector<std::string> items{"Resume", "Quit"};
    
    // Text rendering
    TTF_Font* font = nullptr;
    struct Label {
        std::string text;
        SDL_Texture *tex = nullptr;
        SDL_Rect dst{0, 0, 0, 0};
    };
    Label pauseLabel{"Game Paused"};
    std::vector<Label> itemLbls;
    
    UI::Label title;
    UI::Button buttonResume;
    UI::Button buttonQuit;

    // Cache colors
    SDL_Color titleColor{230, 230, 240, 255};
    SDL_Color textColor{240, 240, 240, 255};
};

#endif //BREAKOUT_STATE_PAUSE_HPP

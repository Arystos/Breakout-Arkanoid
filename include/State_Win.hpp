//
// Created by Aristide Sessa on 26/08/25.
//

#ifndef BREAKOUT_STATE_WIN_HPP
#define BREAKOUT_STATE_WIN_HPP

#include "State.hpp"
#include "UI.hpp"
#include <SDL.h>
#include <SDL_ttf.h>

class State_Win : public State {
public:
    void onEnter(Game &game) override;
    void onExit(Game &game) override {};
    void handleInput(Game &game, const SDL_Event &event) override;
    void update(Game &game, float dt) override {};
    void render(Game &game) override;
    const char *getName() const override { return "Win"; }

    std::vector<Entity *> getEntities() override;

private:
    TTF_Font* font = nullptr;
    UI::Label titleText{"You won"};
    UI::Label msgText{"I have no more levels, I guess you are really good"};
    UI::Label msg2Text{"Press Enter to go to Main Menu"};
    
    UI::Label title;
    UI::Label msg;
    UI::Label msg2;
    
    // Cache colors
    SDL_Color titleColor{50, 240, 50, 255}; // green
    SDL_Color textColor{240, 240, 240, 255}; // white
};

#endif //BREAKOUT_STATE_WIN_HPP

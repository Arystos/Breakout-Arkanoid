#pragma once
#include "State.hpp"
#include "Game.hpp"
#include "UI.hpp"
#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <string>
#include <iostream>

class State_MainMenu : public State {
public:
    void handleInput(Game& game, const SDL_Event& event) override;
    void update(Game& game, float dt) override;
    void render(Game& game) override;
    void onEnter(Game& game) override;
    void onExit(Game& game) override;
    const char* getName() const override { return "MainMenu"; }
private:
    int selected{0};
    std::vector<std::string> items{"Start", "Quit"};

    // Text rendering
    TTF_Font* font = nullptr;
    UI::Label titleLbl{"Breakout"};
    std::vector<UI::Label> itemLbls;
    
    // UI
    UI::Label title;
    UI::Button buttonPlay;
    UI::Button buttonQuit;

    // Cache colors
    SDL_Color titleColor{230, 230, 240, 255};
    SDL_Color textColor{240, 240, 240, 255};
};
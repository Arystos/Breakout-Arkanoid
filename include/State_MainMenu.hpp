#pragma once
#include "State.hpp"
#include <vector>
#include <string>
#include <iostream>

class State_MainMenu : public State {
public:
    void handleInput(Game& game, const SDL_Event& event) override;
    void update(Game& game, float dt) override;
    void render(Game& game) override;
    void onEnter(Game& game) override {
        std::cout << "Main Menu State" << std::endl;
    }
private:
    int selected{0};
    std::vector<std::string> items{"Start", "Quit"};
};
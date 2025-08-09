//
// Created by Aristide Sessa on 09/08/25.
//

#ifndef BREAKOUT_STATE_PAUSE_HPP
#define BREAKOUT_STATE_PAUSE_HPP

#include "State.hpp"
#include <iostream>

class State_Pause : public State {
public:
    void handleInput(Game &game, const SDL_Event &event) override;
    void update(Game &game, float dt) override;
    void render(Game &game) override;
    void onEnter(Game& game) override {
        std::cout << "Pause State" << std::endl;
    }
};

#endif //BREAKOUT_STATE_PAUSE_HPP

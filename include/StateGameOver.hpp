//
// Created by Aristide Sessa on 09/08/25.
//

#ifndef BREAKOUT_STATEGAMEOVER_HPP
#define BREAKOUT_STATEGAMEOVER_HPP


#include "State.hpp"
#include <iostream>

class State_GameOver : public State {
public:
    explicit State_GameOver(int score) : score(score) {} // Constructor to initialize the score
    void handleInput(Game &game, const SDL_Event &event) override;
    void update(Game &game, float dt) override;
    void render(Game &game) override;
    void onEnter(Game& game) override {
        std::cout << "Game Over State" << std::endl;
    }
private:
    int score = 0; // Player's score
};


#endif //BREAKOUT_STATEGAMEOVER_HPP

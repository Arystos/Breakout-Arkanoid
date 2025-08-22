//
// Created by Aristide Sessa on 09/08/25.
//

#ifndef BREAKOUT_STATEPLAY_HPP
#define BREAKOUT_STATEPLAY_HPP


#include "State.hpp"

class State_Play: public State {
public:
    void handleInput(Game &game, const SDL_Event &event) override;
    void update(Game &game, float deltaTime) override;
    void render(Game &game) override;
    void onEnter(Game &game) override;
    void onExit(Game &game) override;
    
    [[nodiscard]] float getDeltaTime() const { return deltaTime; }
    
private:
    bool paused = false; // Flag to check if the game is paused
    int score = 0; // Player's score
    float deltaTime = 0.0f; // Time since last update, used for movement calculations
    // TODO: Add paddle, ball, bricks

    Entity_Paddle paddle;
};


#endif //BREAKOUT_STATEPLAY_HPP

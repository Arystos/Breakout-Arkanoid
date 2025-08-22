//
// Created by Aristide Sessa on 09/08/25.
//

#ifndef BREAKOUT_STATEPLAY_HPP
#define BREAKOUT_STATEPLAY_HPP


#include "State.hpp"
#include "Entity_Paddle.hpp"
#include "Entity_Ball.hpp"
#include "Entity_Brick.hpp"

class State_Play: public State {
public:
    void handleInput(Game &game, const SDL_Event &event) override;
    void update(Game &game, float deltaTime) override;
    void render(Game &game) override;
    void onEnter(Game &game) override;
    void onExit(Game &game) override;
    std::vector<Entity*> getEntities() override;
    const char* getName() const override { return "Play"; }
    
    [[nodiscard]] float getDeltaTime() const { return deltaTime; }
    Entity_Paddle& getPaddle() { return paddle; }
    Entity_Ball& getBall() { return ball; }

    std::vector<Entity *> &entities();

private:
    bool paused = false; // Flag to check if the game is paused
    int score = 0; // Player's score
    float deltaTime = 0.0f; // Time since last update, used for movement calculations
    // TODO: Add paddle, ball, bricks

    Entity_Paddle paddle;
    Entity_Ball ball;
    Entity_Brick brick;
};


#endif //BREAKOUT_STATEPLAY_HPP

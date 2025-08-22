//
// Created by Aristide Sessa on 09/08/25.
//

#include "StatePlay.hpp"
#include "State_Pause.hpp"
#include <iostream>

void State_Play::handleInput(Game &game, const SDL_Event &event) {
    // Handle input events for the play state
    if (event.type == SDL_KEYDOWN)  {
        switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_ESCAPE:
                // Pause the game
                game.pushState(std::make_unique<State_Pause>());
                break;
            case SDL_SCANCODE_SPACE:
                // Detach the ball from the paddle if it's stuck
                if (ball.stuckToPaddle) ball.stuckToPaddle = false;
                break;
                // TODO: Improove paddle smothness
            default:
                break;
        }
    }
}

void State_Play::update(Game &game, float dt) {
    (void)game; (void)dt; // Avoid unused parameter warnings
    deltaTime = dt;
    
    // Update paddle position based on input
    if (paddle.active) paddle.update(dt);
    // Update ball position
    if (ball.active) ball.update(dt);
}

void State_Play::render(Game &game) {
    SDL_Renderer* r = game.getRenderer();
    // placeholder: draw a playfield box
    SDL_Rect box{40, 40, game.Width() - 80, game.Height() - 80 };
    SDL_SetRenderDrawColor(r, 60, 60, 60, 255);
    SDL_RenderDrawRect(r, &box);
    
    // render paddle
    paddle.render(r);
    
    // render ball
    ball.render(r);
}

void State_Play::onEnter(Game &game) {
    // init level, paddle, ball etc. later
    std::cout << "Playing State" << std::endl;
    State::onEnter(game);
    
    // acttivate the paddle
    paddle.active = true;
    
    // Place ball on top of the paddle
    //ball.radius = 10.0f;
    ball.attachTo(paddle);
    ball.stuckToPaddle = false;
    ball.active = true;
}

void State_Play::onExit(Game &game) {
    State::onExit(game);
}

std::vector<Entity *> State_Play::getEntities() {
    std::vector<Entity*> entities;
    if (paddle.active) entities.push_back(&paddle);
    if (ball.active) entities.push_back(&ball);

    // TODO: Implement bricks
    /*
    for (auto& brick : bricks) {
        if (brick.active) {
            entities.push_back(&brick);
        }
    }
    */
    return entities;
}

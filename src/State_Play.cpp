//
// Created by Aristide Sessa on 09/08/25.
//

#include "StatePlay.hpp"
#include "State_MainMenu.hpp"
#include "State_Pause.hpp"

void State_Play::handleInput(Game &game, const SDL_Event &event) {
    // Handle input events for the play state
    if (event.type == SDL_KEYDOWN)  {
        switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_LEFT: 
                game.paddle.move(-1.0f, getDeltaTime()); // Move paddle left
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
    if (game.paddle.active) game.paddle.update(dt);
}

void State_Play::render(Game &game) {
    SDL_Renderer* r = game.getRenderer();
    // placeholder: draw a playfield box
    SDL_Rect box{ 40, 40, game.getWidth()-80, game.getHeight()-80 };
    SDL_SetRenderDrawColor(r, 60, 60, 60, 255);
    SDL_RenderDrawRect(r, &box);
    
    // render paddle
    game.paddle.render(r);
}

void State_Play::onEnter(Game &game) {
    // init level, paddle, ball etc. later
    std::cout << "Playing State" << std::endl;
    State::onEnter(game);
    
    // Place paddle near the bottom of the screen
    game.paddle.position = { static_cast<float>(game.getWidth() / 2 - game.paddle.size.x / 2), 
                             static_cast<float>(game.getHeight() - game.paddle.size.y - 100) };
    game.paddle.size = { 100.0f, 20.0f };
    game.paddle.velocity = { 0.0f, 0.0f };
    game.paddle.active = true;
}

void State_Play::onExit(Game &game) {
    State::onExit(game);
}

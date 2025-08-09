//
// Created by Aristide Sessa on 09/08/25.
//

#include "StatePlay.hpp"
#include "State_MainMenu.hpp"
#include "State_Pause.hpp"

void State_Play::handleInput(Game &game, const SDL_Event &event) {
    if (event.type == SDL_KEYDOWN)  {
        switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_ESCAPE:
                game.pushState(std::make_unique<State_Pause>());
                break;
            default:
                break;
        }
    }
}

void State_Play::update(Game &game, float dt) {
    (void)game; (void)dt; // Avoid unused parameter warnings
    // update world; if the ball is lost or the level is completed, change state
}

void State_Play::render(Game &game) {
    SDL_Renderer* r = game.getRenderer();
    // placeholder: draw a playfield box
    SDL_Rect box{ 40, 40, game.getWidth()-80, game.getHeight()-80 };
    SDL_SetRenderDrawColor(r, 60, 60, 60, 255);
    SDL_RenderDrawRect(r, &box);
}

void State_Play::onEnter(Game &game) {
    // init level, paddle, ball etc. later
    std::cout << "Playing State" << std::endl;
    State::onEnter(game);
}

void State_Play::onExit(Game &game) {
    State::onExit(game);
}

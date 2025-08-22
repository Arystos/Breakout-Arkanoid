//
// Created by Aristide Sessa on 09/08/25.
//

#include "State_Pause.hpp"
#include "Game.hpp"

void State_Pause::handleInput(Game &game, const SDL_Event &event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_ESCAPE:
                game.popState(); // Go back to the previous state (e.g., play state)
                std::cout << "Resuming game..." << std::endl;
                break;
            case SDL_SCANCODE_RETURN:
                game.popState(); // Go back to the previous state (e.g., play state)
                std::cout << "Resuming game..." << std::endl;
                break;
            case SDL_SCANCODE_SPACE:
                game.popState(); // Go back to the previous state (e.g., play state)
                std::cout << "Resuming game..." << std::endl;
                break;
            default:
                break;
        }
    }
}

void State_Pause::update(Game &game, float dt) {

}

void State_Pause::render(Game &game) {
    // draw semi-transparent overlay
    SDL_Renderer* r = game.getRenderer();
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 160);
    SDL_Rect full{0, 0, game.Width(), game.Height()};
    SDL_RenderFillRect(r, &full);

    // paused banner
    SDL_SetRenderDrawColor(r, 180, 180, 180, 255);
    SDL_Rect banner{game.Width() / 2 - 150, game.Height() / 2 - 30, 300, 60 };
    SDL_RenderFillRect(r, &banner);
}

//
// Created by Aristide Sessa on 09/08/25.
//

#include "StateGameOver.hpp"
#include "State_MainMenu.hpp"

void State_GameOver::handleInput(Game &game, const SDL_Event &event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_RETURN:
                // Restart the game or go back to the main menu
                game.changeState(std::make_unique<State_MainMenu>());
                break;
            case SDL_SCANCODE_SPACE:
                // Restart the game or go back to the main menu
                game.changeState(std::make_unique<State_MainMenu>());
                break;
            case SDL_SCANCODE_ESCAPE:
                // Exit the game
                game.quit();
                break;
            default:
                break;
        }
    }
}

void State_GameOver::update(Game &game, float dt) {

}

void State_GameOver::render(Game &game) {
    SDL_Renderer* r = game.getRenderer();
    // banner
    SDL_SetRenderDrawColor(r, 180, 50, 50, 255);
    SDL_Rect banner{ game.getWidth()/2 - 180, game.getHeight()/2 - 40, 360, 80 };
    SDL_RenderFillRect(r, &banner);
    // (Text later via SDL_ttf; for now just the block)
}

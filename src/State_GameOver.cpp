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
    
    UI::DrawLabel(r, title);
    //UI::DrawLabel(r, scoreText);
    UI::DrawLabel(r, instruction);
}

void State_GameOver::onEnter(Game &game) {
    State::onEnter(game);
    std::cout << "Game Over State" << std::endl;
    SDL_SetWindowTitle(game.getWindow(), getName());
    
    auto* renderer = game.getRenderer();
    auto* uiFont = game.uiFont();
    
    // Title
    UI::BuildLabel(renderer, title, "Game Over", uiFont, titleColor, UI::AlignH::Center);
    title.dst.w *= 2;
    title.dst.h *= 2;
    title.dst.x = (game.Width() - title.dst.w) / 2; // center horizontally
    title.dst.y = game.Height() / 3;
    
    
    // Score
    /*
    std::string scoreTextStr = "Score: " + std::to_string(score);
    UI::BuildLabel(renderer, scoreText, scoreTextStr, uiFont, textColor, UI::AlignH::Center);
    scoreText.dst.x = (game.Width() - scoreText.dst.w) / 2; // center horizontally
    scoreText.dst.y = game.Height() / 2; // position at 1/2 of the screen height
    */ 
    
    // Instruction
    UI::BuildLabel(renderer, instruction, "Press Enter to return to Main Menu", uiFont, textColor, UI::AlignH::Center);
    instruction.dst.x = (game.Width() - instruction.dst.w) / 2; // center horizontally
    instruction.dst.y = game.Height() / 2; // position at 2/3rd of the screen height
}

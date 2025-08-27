//
// Created by Aristide Sessa on 09/08/25.
//

#include "State_Pause.hpp"
#include "Game.hpp"
#include "State_MainMenu.hpp"

void State_Pause::handleInput(Game &game, const SDL_Event &event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_UP:
                selected = (selected + (int)items.size() - 1) % (int)items.size();
                break;
            case SDL_SCANCODE_DOWN:
                selected = (selected + 1) % (int)items.size();
                break;
            case SDL_SCANCODE_RETURN:
            case SDL_SCANCODE_SPACE:
                // TODO: test if this works
                if (selected == 0) {
                    game.popState(); // Resume game by popping the pause state
                } else {
                    game.changeState(std::make_unique<State_MainMenu>()); // Quit to main menu
                }
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
    SDL_Renderer* renderer = game.getRenderer();
    
    // Update hover states
    buttonResume.hovered = (selected == 0); // hovered is true if selected is 0
    buttonQuit.hovered = (selected == 1); // hovered is true if selected is 1 ...
    
    // Semi-transparent background
    /*
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
    SDL_Rect fullScreenRect{0, 0, game.Width(), game.Height()};
    SDL_RenderFillRect(renderer, &fullScreenRect);
    */
    
    UI::DrawLabel(renderer, title);
    UI::DrawButton(renderer, buttonResume);
    UI::DrawButton(renderer, buttonQuit);
}

void State_Pause::onEnter(Game &game) {
    State::onEnter(game);
    std::cout << "Pause State" << std::endl;
    
    // Pause timers
    game.timerManager.pauseAll();

    auto* renderer = game.getRenderer();
    auto* uiFont = game.uiFont();
    
    // Title
    UI::BuildLabel(renderer, title, "Game Paused", uiFont, titleColor, UI::AlignH::Center);
    title.dst.x = (game.Width() - title.dst.w) / 2;
    title.dst.y = game.Height() / 3;
    
    // Buttons
    const int containerW = game.Width();
    const int startY = game.Height() / 2;
    const int spacing = 24;
    UI::BuildButton(renderer, buttonResume, "Resume", uiFont, textColor, 0, startY, containerW, true);
    UI::BuildButton(renderer, buttonQuit, "Quit to Menu", uiFont, textColor, 0, startY + buttonResume.rect.h + spacing, containerW, true);
    // Button colors
    buttonResume.bgHover   = SDL_Color{60, 80, 60, 255};
    buttonResume.bgPressed = SDL_Color{80, 110, 80, 255};
    buttonQuit.bgHover     = SDL_Color{80, 50, 50, 255};
    buttonQuit.bgPressed   = SDL_Color{110, 70, 70, 255};
    
    // State initialization
    selected = 0;
    buttonResume.hovered = true;
    buttonQuit.hovered = false;
}

void State_Pause::onExit(Game &game) {
    State::onExit(game);
    // resume timers
    game.timerManager.resumeAll();
}

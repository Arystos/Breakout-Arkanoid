#include "State_MainMenu.hpp"
#include "Game.hpp"
#include "StatePlay.hpp"
#include <algorithm>


void State_MainMenu::onEnter(Game &game) {
    std::cout << "Main Menu State" << std::endl;
    SDL_SetWindowTitle(game.getWindow(), "Breakout");

    auto* renderer = game.getRenderer();
    auto* uiFont = game.uiFont();

    if (game.LevelIndex() > 0) {
        game.SetLevelIndex(game.LevelIndex() -1);
    }

    // Title
    UI::BuildLabel(renderer, title, "Breakout", uiFont, titleColor, UI::AlignH::Center);
    title.dst.x = (game.Width() - title.dst.w) / 2;
    title.dst.y = game.Height() / 6;

    // Buttons
    const int containerW = game.Width();
    const int startY = game.Height() / 3;
    const int spacing = 24;

    UI::BuildButton(renderer, buttonPlay, "Play", uiFont, textColor, 0, startY, containerW, true);
    UI::BuildButton(renderer, buttonQuit, "Exit", uiFont, textColor, 0, startY + buttonPlay.rect.h + spacing, containerW, true);

    // Button colors
    buttonPlay.bgHover   = SDL_Color{60, 60, 80, 255};
    buttonPlay.bgPressed = SDL_Color{80, 80, 110, 255};
    buttonQuit.bgHover   = SDL_Color{80, 50, 50, 255};
    buttonQuit.bgPressed = SDL_Color{110, 70, 70, 255};

    // State initialization
    selected = 0;
    buttonPlay.hovered = true;
    buttonQuit.hovered = false;
}

void State_MainMenu::handleInput(Game& game, const SDL_Event& event) {
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
                    game.changeState(std::make_unique<State_Play>());
                } else game.quit();
                break;
            case SDL_SCANCODE_ESCAPE: game.quit(); break;
            default: break;
        }
    }
}

void State_MainMenu::update(Game&, float) {}

void State_MainMenu::render(Game& game) {
    SDL_Renderer* renderer = game.getRenderer();

    // Update hover states
    buttonPlay.hovered = (selected == 0); // hovered is true if selected is 0
    buttonQuit.hovered = (selected == 1); // hovered is true if selected is 1 ... 

    UI::DrawLabel(renderer, title);
    UI::DrawButton(renderer, buttonPlay);
    UI::DrawButton(renderer, buttonQuit);
}

void State_MainMenu::onExit(Game &game) {
    State::onExit(game);
    UI::Destroy(title);
    UI::Destroy(buttonPlay);
    UI::Destroy(buttonQuit);
}

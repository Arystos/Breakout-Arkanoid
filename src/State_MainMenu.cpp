#include "State_MainMenu.hpp"
#include "Game.hpp"
#include "StatePlay.hpp"
#include <SDL.h>

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
    // Minimal placeholder rendering (rects as text bars)
    SDL_Renderer* renderer = game.getRenderer();
            
    const int w = game.getWidth(), h = game.getHeight();

    // Title bar
    SDL_Rect title{ w/2 - 200, 80, 400, 60 };
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    SDL_RenderFillRect(renderer, &title);

    // Menu items
    for (int i = 0; i < (int)items.size(); ++i) {
        SDL_Rect item{ w/2 - 120, 200 + i*70, 240, 50 };
        if (i == selected) SDL_SetRenderDrawColor(renderer, 200, 200, 50, 255);
        else SDL_SetRenderDrawColor(renderer, 90, 90, 90, 255);
        SDL_RenderFillRect(renderer, &item);
    }
}
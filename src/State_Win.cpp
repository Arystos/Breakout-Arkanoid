//
// Created by Aristide Sessa on 26/08/25.
//

#include "State_Win.hpp"
#include "Game.hpp"
#include "State_MainMenu.hpp" // assicurati che esista e sia includibile
#include <string>

static SDL_Texture* createTextTexture(SDL_Renderer* r, TTF_Font* f, const std::string& text, SDL_Rect &outRect) {
    if (!f || !r) return nullptr;
    SDL_Surface* s = TTF_RenderUTF8_Blended(f, text.c_str(), SDL_Color{255,255,255,255});
    if (!s) return nullptr;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(r, s);
    outRect.w = s->w;
    outRect.h = s->h;
    SDL_FreeSurface(s);
    return tex;
}

void State_Win::onEnter(Game &game) {
    State::onEnter(game);
    std::cout << "Win State" << std::endl;
    SDL_SetWindowTitle(game.getWindow(), getName());

    auto* renderer = game.getRenderer();
    auto* uiFont = game.uiFont();
    
    // First msg text on top
    UI::BuildLabel(renderer, msg, msgText.text, uiFont, textColor, UI::AlignH::Center);
    msg.dst.w *= 1;
    msg.dst.h *= 1;
    msg.dst.x = (game.Width() - msg.dst.w) / 2; // center horizontally
    msg.dst.y = game.Height() / 6;
    
    // You won below it
    UI::BuildLabel(renderer, title, titleText.text, uiFont, titleColor, UI::AlignH::Center);
    title.dst.w *= 2;
    title.dst.h *= 2;
    title.dst.x = (game.Width() - title.dst.w) / 2; // center horizontally
    title.dst.y = game.Height() / 2.5f;
    
    // Instruction at the bottom
    UI::BuildLabel(renderer, msg2, msg2Text.text, uiFont, textColor, UI::AlignH::Center);
    msg2.dst.w *= 1.2;
    msg2.dst.h *= 1.2;
    msg2.dst.x = (game.Width() - msg2.dst.w) / 2; // center horizontally
    msg2.dst.y = game.Height() / 1.3f;
    
}

void State_Win::handleInput(Game &game, const SDL_Event &event) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER) {
            // Torna al menu principale
            game.changeState(std::make_unique<State_MainMenu>());
        }
    }
}

void State_Win::render(Game &game) {
    SDL_Renderer* r = game.getRenderer();

    UI::DrawLabel(r, title);
    UI::DrawLabel(r, msg);
    UI::DrawLabel(r, msg2);
}

std::vector<Entity *> State_Win::getEntities() {
    return State::getEntities();
}

#include "State_MainMenu.hpp"
#include "Game.hpp"
#include "StatePlay.hpp"
#include <algorithm>


void State_MainMenu::onEnter(Game &game) {
    std::cout << "Main Menu State" << std::endl;
    game.getCurrentState();
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
    // Minimal placeholder rendering (rects as text bars)
    SDL_Renderer* renderer = game.getRenderer();
    const int w = game.Width(), h = game.Height();
    TTF_Font* font = game.uiFont();

    // Title
    UI::BuildLabel(renderer, title_, "Breakout", font, titleColor_, UI::AlignH::Center);
    title_.dst.x = (game.Width() - title_.dst.w) / 2;
    title_.dst.y = game.Height() / 6;
    UI::DrawLabel(renderer, title_);
    
    // Buttons
    const int containerW = game.Width();
    const int startY = game.Height() / 3;
    const int spacing = 20;

    UI::BuildButton(renderer, buttonPlay, "Play", font, textColor,
                    0, startY, containerW, true);

    UI::BuildButton(renderer, buttonQuit, "Exit", font, textColor,
                    0, startY + buttonPlay.rect.h + spacing, containerW, true);

    buttonPlay.bgHover  = SDL_Color{60, 60, 80, 255};
    buttonPlay.bgPressed= SDL_Color{80, 80, 110, 255};
    buttonQuit.bgHover  = SDL_Color{80, 50, 50, 255};
    buttonQuit.bgPressed= SDL_Color{110, 70, 70, 255};
    
    UI::DrawButton(renderer, buttonPlay);
    UI::DrawButton(renderer, buttonQuit);
    
    // Highlight selected button
    std::vector<UI::Button*> buttons{ &buttonPlay, &buttonQuit };
    for (auto* b : buttons) { // clear
        b->hovered = false;
    }
    if (selected >= 0 && selected < buttons.size()) {
        buttons[selected]->hovered = true;
    }
}

SDL_Texture* State_MainMenu::makeText(SDL_Renderer* r, TTF_Font* f, const std::string& s, SDL_Color col) const {
    SDL_Surface* surf = TTF_RenderUTF8_Blended(f, s.c_str(), col);
    if (!surf) { SDL_Log("TTF_RenderUTF8_Blended: %s", TTF_GetError()); return nullptr; }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(r, surf);
    SDL_FreeSurface(surf);
    if (!tex) SDL_Log("CreateTextureFromSurface: %s", SDL_GetError());
    return tex;
}



void State_MainMenu::destroyLabels(SDL_Renderer*) {
    if (titleLbl.tex) { SDL_DestroyTexture(titleLbl.tex); titleLbl.tex=nullptr; }
    for (auto& L : itemLbls) if (L.tex) { SDL_DestroyTexture(L.tex); L.tex=nullptr; }
    itemLbls.clear();
    if (font) { TTF_CloseFont(font); font=nullptr; }
}

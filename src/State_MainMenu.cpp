#include "State_MainMenu.hpp"
#include "Game.hpp"
#include "StatePlay.hpp"
#include <algorithm>

static void querySize(SDL_Texture* t, SDL_Rect& dst){
    int w=0,h=0; 
    SDL_QueryTexture(t,nullptr,nullptr,&w,&h); 
    dst.w=w; dst.h=h; }

void State_MainMenu::onEnter(Game &game) {
    std::cout << "Main Menu State" << std::endl;
    game.getCurrentState();

    // Assumi che TTF_Init sia già stato chiamato in Game init
    SDL_Renderer* r = game.getRenderer();
    font = TTF_OpenFont("assets/fonts/Roboto-Regular.ttf", 40);
    if (!font) { SDL_Log("TTF_OpenFont: %s", TTF_GetError()); return; }

    // Title
    titleLbl.tex = makeText(r, font, titleLbl.text, SDL_Color{255,255,255,255});
    if (titleLbl.tex) {
        querySize(titleLbl.tex, titleLbl.dst);
        int w=0,h=0; SDL_GetRendererOutputSize(r,&w,&h);
        titleLbl.dst.x = (w - titleLbl.dst.w)/2;
        titleLbl.dst.y = h/5;
    }

    // Menu items
    itemLbls.clear();
    itemLbls.reserve(items.size());
    for (auto& s : items) {
        Label L{s};
        L.tex = makeText(r, font, s, SDL_Color{220,220,220,255});
        if (L.tex) querySize(L.tex, L.dst);
        itemLbls.push_back(L);
    }
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

    // Title bar
    SDL_Rect titleBar{ w/2 - 200, 80, 400, 60 };
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    SDL_RenderFillRect(renderer, &titleBar);

    // Title text at the centre of the title bar
    if (titleLbl.tex) SDL_RenderCopy(renderer, titleLbl.tex, nullptr, &titleLbl.dst);
    titleLbl.dst.x = w/2 - titleLbl.dst.w/2;
    titleLbl.dst.y = titleBar.y + (titleBar.h - titleLbl.dst.h)/2;

    // layout verticale v-gap
    const int startY = h/2 - (int)itemLbls.size()*35;
    for (int i=0;i<(int)itemLbls.size();++i) {
        SDL_Rect box{ w/2 - 140, startY + i*70 - 10, 280, 60 };
        if (i == selected) SDL_SetRenderDrawColor(renderer, 200,200,50,255); // highlight selected
        else SDL_SetRenderDrawColor(renderer, 90,90,90,255); // normal
        SDL_RenderFillRect(renderer, &box);

        // aggiorna dst centrato
        auto& L = itemLbls[i];
        L.dst.x = w/2 - L.dst.w/2;
        L.dst.y = box.y + (box.h - L.dst.h)/2;
        if (L.tex) SDL_RenderCopy(renderer, L.tex, nullptr, &L.dst);
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

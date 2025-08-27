#pragma once
#include <SDL.h>
#include <vector>
#include <glm/vec2.hpp>
#include <iostream>

struct Color {
    Uint8 r, g, b, a;
};

struct ColorRGB {
    Uint8 r, g, b;
};

struct SDL_Deleter { 
    void operator()(SDL_Texture* t) const { if (t) SDL_DestroyTexture(t); } 
};
using TextureUPtr = std::unique_ptr<SDL_Texture, SDL_Deleter>;

struct Entity {
    // SDL vector for position and size
    glm::vec2 position{0.0f, 0.0f};
    glm::vec2 size{0.0f, 0.0f};
    glm::vec2 velocity{0.0f, 0.0f};
    bool active{true};
    bool toBeDestroyed{false};
    SDL_Texture *texture{nullptr}; // texture for rendering
    virtual void update(float dt) {}
    virtual void render(SDL_Renderer* r) {};
    virtual void onCollision(Entity& other) {}
    virtual ~Entity() = default;

    TextureUPtr MakeCircleTexture(SDL_Renderer *r, int diameter) {
        SDL_Surface* s = SDL_CreateRGBSurfaceWithFormat(0, diameter, diameter, 32, SDL_PIXELFORMAT_RGBA32);
        auto* px = (Uint32*)s->pixels;
        int rad = diameter/2; int cx = rad, cy = rad;
        Uint32 white = SDL_MapRGBA(s->format, 255,255,255,255);
        Uint32 transparent = SDL_MapRGBA(s->format, 0,0,0,0);
        for (int y=0; y<diameter; ++y)
            for (int x=0; x<diameter; ++x) {
                int dx=x-cx, dy=y-cy;
                px[y*diameter+x] = (dx*dx+dy*dy <= rad*rad) ? white : transparent;
            }
        SDL_Texture* raw = SDL_CreateTextureFromSurface(r, s);
        if (!raw) throw std::runtime_error(SDL_GetError());
        return TextureUPtr{raw};
    }
    
    Color color{255, 255, 255, 255}; // default white color

    SDL_FRect getRect() const {                // ritorna per valore, non reference a static
        return SDL_FRect{ position.x, position.y, size.x, size.y };
    }
};



#pragma once
#include <SDL.h>
#include <vector>
#include <glm/vec2.hpp>

struct Color {
    Uint8 r, g, b, a;
};

struct Entity {
    // SDL vector for position and size
    glm::vec2 position{0.0f, 0.0f};
    glm::vec2 size{0.0f, 0.0f};
    glm::vec2 velocity{0.0f, 0.0f};
    bool active{true};
    bool toBeDestroyed{false};
    SDL_Texture *texture{nullptr}; // texture for rendering
    virtual void update(float dt) {}
    virtual void render(SDL_Renderer* r);
    virtual void onCollision(Entity& other) {}
    virtual ~Entity() = default;
    
    Color color{255, 255, 255, 255}; // default white color

    const SDL_FRect &getRect() {
        // return SDL_FRect{position.x, position.y, size.x, size.y};
        static SDL_FRect rect;
        rect.x = position.x;
        rect.y = position.y;
        rect.w = size.x;
        rect.h = size.y;
        return rect;
    }
};

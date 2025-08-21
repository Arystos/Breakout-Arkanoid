#pragma once
#include <SDL.h>
#include <vector>
#include <glm/vec2.hpp>

struct Entity {
    // SDL vector for position and size
    glm::vec2 position{0.0f, 0.0f};
    glm::vec2 size{0.0f, 0.0f};
    SDL_FPoint velocity{0.0f, 0.0f};
    bool active{true};
    SDL_Texture *texture{nullptr}; // texture for rendering
    virtual void update(float dt) {}
    virtual void render(SDL_Renderer* r);
    virtual ~Entity() = default;
};

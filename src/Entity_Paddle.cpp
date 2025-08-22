//
// Created by Aristide Sessa on 09/08/25.
//

#include "Entity_Paddle.hpp"
#include "Game.hpp"

// constructor
Entity_Paddle::Entity_Paddle() {
    Game& game = Game::getInstance();
    position = {
            (game.Width() * 0.5f - size.x * 0.5f),
            (game.Height() - size.y - 100.0f)
    };
    size = {100.0f, 20.0f};
    speed = 300.0f; // default speed
    active = false;
}


void Entity::render(SDL_Renderer *r) {
    // Print debug information
    SDL_Log("Rendering Paddle");
    if (texture) {
        SDL_Rect rect{(int)position.x, (int)position.y, (int)size.x, (int)size.y};
        SDL_RenderCopy(r, texture, nullptr, &rect);
    } else {
        // Fallback rendering if no texture is set
        SDL_SetRenderDrawColor(r, 255, 255, 255, 255); // White color
        SDL_Rect rect{(int)position.x, (int)position.y, (int)size.x, (int)size.y};
        SDL_RenderFillRect(r, &rect);
    }
}

void Entity_Paddle::render(SDL_Renderer *r) {
    if (texture == nullptr) {
        // Create a rectangle for the paddle
        SDL_SetRenderDrawColor(r, 255, 255, 255, 255); // White color
        SDL_Rect rect{(int)position.x, (int)position.y, (int)size.x, (int)size.y};
        SDL_RenderFillRect(r, &rect);
    } else {
        SDL_Rect rect{(int)position.x, (int)position.y, (int)size.x, (int)size.y};
        SDL_RenderCopy(r, texture, nullptr, &rect);
    }
}

void Entity_Paddle::update(float dt) {
    
    // paddle movement
    const Uint8 *state = SDL_GetKeyboardState(nullptr);
    float dir = 0.0f;
    if (state[SDL_SCANCODE_LEFT] or state[SDL_SCANCODE_A]) {
        dir = -1.0f; // Move left
        move(dir, dt);
    } else if (state[SDL_SCANCODE_RIGHT] or state[SDL_SCANCODE_D]) {
        dir = 1.0f; // Move right
        move(dir, dt);
    } else {
        // No movmement
        if (velocity.x != 0.0f) {
            move(0.0f, dt);
        }
    }
    
}

void Entity_Paddle::move(float dir, float deltaTime) {
    // Update paddle position based on direction and speed
    position.x += dir * speed * deltaTime;
    
    // Clamp paddle position to bounds
    if (position.x < bounds.minX) {
        position.x = bounds.minX;
    } else if (position.x + size.x > bounds.maxX) {
        position.x = bounds.maxX - size.x;
    }
    
}
//
// Created by Aristide Sessa on 09/08/25.
//

#include "Entity_Paddle.hpp"

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

void Entity_Paddle::initialize(float paddleWidth, float paddleHeight, float paddleSpeed) {
    
}

void Entity_Paddle::moveLeft() {
    // Set velocity to move left
    velocity.x = -speed;
}

void Entity_Paddle::moveRight() {
    // Set velocity to move right
    velocity.x = speed;
}

void Entity_Paddle::move(float dir, float deltaTime) {
    position.x += dir * speed * deltaTime;

    // Clamp
    if (position.x < bounds.minX)
        position.x = bounds.minX;
    if (position.x + size.x > bounds.maxX)
        position.x = bounds.maxX - size.x;
}

void Entity_Paddle::update(float dt) {
    // input-driven movement is handled via Input class; velocity.x set there
    position.x += velocity.x * dt;
    // clamp to play area
    if (position.x < bounds.minX) position.x = bounds.minX;
    if (position.x + size.x > bounds.maxX) position.x = bounds.maxX - size.x;
    // reset velocity for next frame
    velocity = {0.0f, 0.0f};
}

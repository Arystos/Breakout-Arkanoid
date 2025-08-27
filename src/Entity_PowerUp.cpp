//
// Created by Aristide Sessa on 09/08/25.
//

#include "Entity_PowerUp.hpp"
#include "Entity_Paddle.hpp"
#include "Physics.hpp"
#include <random>

Entity_PowerUp::Entity_PowerUp() {
    active = true;
    size = {20.0f, 20.0f};
    // Randomly assign a power-up type
    static std::mt19937 rng{std::random_device{}()}; //generate random seed
    std::uniform_int_distribution<int> dist(0, static_cast<int>(PowerUpType::Count) - 1);
    type = static_cast<PowerUpType>(dist(rng));
    //type = static_cast<PowerUpType>(5); // for testing
    std::cout << "PowerUp created with type " << static_cast<int>(type) << std::endl; // debug
}

void Entity_PowerUp::update(float dt) {
    
    position.y += fallSpeed * dt; // move downwards
    
    if (position.y > float(game.Height())) {
        toBeDestroyed = true;
        return;
    }
    
    // check collision with paddle
    for (Entity* entity : game.getCurrentState()->getEntities()) {
        if (!entity || entity == this || !entity->active) continue;
        if (auto *paddle = dynamic_cast<Entity_Paddle *>(entity)) {
            glm::vec2 normal;
            if (Physics::circleVsAABB({position.x + size.x / 2, position.y + size.y / 2}, size.x / 2,
                                      paddle->getRect(), normal)) {
                onCollision(*paddle);
                break;
            }
        }
    }
        
}

void Entity_PowerUp::render(SDL_Renderer *renderer) {
    // Draw the ball as a filled circle
    SDL_Rect rect{(int)position.x, (int)position.y, (int)(size.x), (int)(size.x)};
    const int padding = 2;
    SDL_Rect visualRect{rect.x + padding, rect.y + padding, rect.w - 2*padding, rect.h - 2*padding};
    if (!texture) {
        texture = MakeCircleTexture(renderer, (int)(size.x));
        SDL_SetTextureColorMod(texture.get(), 255, 255, 0); // Yellow color for power-ups
        //SDL_SetTextureAlphaMod(texture.get(), 400); // Slightly transparent
    }
        
    else
        SDL_RenderCopy(renderer, texture.get(), nullptr, &visualRect); // render the texture
        
}

void Entity_PowerUp::onCollision(Entity &other) {
    // dynamic_cast to paddle
    if (auto* paddle = dynamic_cast<Entity_Paddle*>(&other)) {
        if (!active) return;
        active = false;
        toBeDestroyed = true;
        paddle->onCollision(*this);
    }
}

const std::string &Entity_PowerUp::typeToString() {
    // TODO: improve with map or array
    return (type == PowerUpType::ExpandPaddle) ? "Expand Paddle" :
           (type == PowerUpType::ShrinkPaddle) ? "Shrink Paddle" :
           (type == PowerUpType::MultiBall) ? "Multi Ball" :
           (type == PowerUpType::SlowBall) ? "Slow Ball" :
           (type == PowerUpType::FastBall) ? "Fast Ball" :
           (type == PowerUpType::StickyPaddle) ? "Sticky Paddle" :
           "Unknown";
}

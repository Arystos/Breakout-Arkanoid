//
// Created by Aristide Sessa on 09/08/25.
//

#include "Entity_Brick.hpp"
#include "Game.hpp"
#include "StatePlay.hpp"

Entity_Brick::Entity_Brick() {
    size = {60.0f, 20.0f};
    health = 1;
    active = true;

    // TODO: Load textures
    /*
    Game& game = Game::getInstance();
    normalTexture = game.loadTexture("assets/brick_normal.png");
    strongTexture = game.loadTexture("assets/brick_strong.png");
    indestructibleTexture = game.loadTexture("assets/brick_indestructible.png");
    mysteryTexture = game.loadTexture("assets/brick_mystery.png");
    */
}

void Entity_Brick::render(SDL_Renderer *r) {
    // se color è float 0..1
    auto to8 = [](float v){ return static_cast<Uint8>(std::clamp(v, 0.0f, 1.0f) * 255.0f); };

    SDL_Rect rect{
            static_cast<int>(position.x),
            static_cast<int>(position.y),
            std::max(1, static_cast<int>(size.x)),
            std::max(1, static_cast<int>(size.y))
    };

    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE); // evita alpha se non vuoi blending
    SDL_SetRenderDrawColor(r, to8(color.r), to8(color.g), to8(color.b), to8(color.a));
    SDL_RenderFillRect(r, &rect);
}

SDL_Texture *Entity_Brick::textureForType(BrickType t) {
    switch (t) {
        case BrickType::Normal: return normalTexture;
        case BrickType::Strong: return strongTexture;
        case BrickType::Indestructible: return indestructibleTexture;
        case BrickType::Mystery: return mysteryTexture;
        default: return nullptr; // should never happen
    }
}

void Entity_Brick::onCollision(Entity &other) {
    if (type != BrickType::Indestructible) { 
        health--; 
        active = health > 0;
    }
    if (type == BrickType::Strong) {
        // change color to green
        color = {0, 255, 0, 255};
    }
    if (type == BrickType::Mystery) {
        dropPowerUp = true;
        // In a real game, you would spawn a power-up entity here
        if (auto* playState = dynamic_cast<State_Play*>(Game::getInstance().getCurrentState())) {
            playState->spawnPowerUp(position + size / 2.0f);
            dropPowerUp = false; // reset flag after dropping
        }
    }
}

void Entity_Brick::setColor() {
    switch (type) {
        case BrickType::Normal:
            color = {0, 255, 0, 255}; // Green
            break;
        case BrickType::Strong:
            color = {255, 165, 0, 255}; // Orange
            break;
        case BrickType::Indestructible:
            color = {128, 128, 128, 255}; // Gray
            break;
        case BrickType::Mystery:
            color = {255, 0, 255, 255}; // Magenta
            break;
        default:
            color = {255, 255, 255, 255}; // White
            break;
    }
}

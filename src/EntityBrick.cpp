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
    SDL_Rect rect{
            static_cast<int>(position.x),
            static_cast<int>(position.y),
            static_cast<int>(size.x),
            static_cast<int>(size.y)
    };
    SDL_SetRenderDrawColor(r, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(r, &rect);
    
    // TODO: Proper rendering with textures
    /*
    SDL_Rect rect{(int)position.x, (int)position.y, (int)size.x, (int)size.y};
    SDL_RenderCopy(r, textureForType(type), nullptr, &rect);
     */
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

Entity_Brick::~Entity_Brick() {
    Game& game = Game::getInstance();
    // get game state
    State* currentState = game.getCurrentState();
    if (auto *playState = dynamic_cast<State_Play*>(currentState)) {
        if (playState->getBrickCount() == 1) {
            // TODO: Level complete, last brick destroyed
            game.changeState(std::make_unique<State_Play>());
        }
    }
}

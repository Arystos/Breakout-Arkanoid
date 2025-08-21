//
// Created by Aristide Sessa on 09/08/25.
//

#ifndef BREAKOUT_ENTITYBRICK_HPP
#define BREAKOUT_ENTITYBRICK_HPP

#include <SDL.h>
#include "Entity.hpp"

enum class BrickType { 
    Normal, Strong, Indestructible, Mystery 
};

class Entity_Brick : public Entity {
public:
    int health;
    BrickType type;
    bool dropPowerUp{false};
    void hit() { if (type != BrickType::Indestructible) { health--; active = health > 0; } }
    void render(SDL_Renderer* r) override {
        SDL_Rect rect{(int)position.x, (int)position.y, (int)size.x, (int)size.y};
        SDL_RenderCopy(r, textureForType(type), nullptr, &rect);
    }
private:
    SDL_Texture* textureForType(BrickType t) {
        switch (t) {
            case BrickType::Normal: return normalTexture;
            case BrickType::Strong: return strongTexture;
            case BrickType::Indestructible: return indestructibleTexture;
            case BrickType::Mystery: return mysteryTexture;
            default: return nullptr; // should never happen
        }
    }

    SDL_Texture* normalTexture{nullptr};
    SDL_Texture* strongTexture{nullptr};
    SDL_Texture* indestructibleTexture{nullptr};
    SDL_Texture* mysteryTexture{nullptr};
};



#endif //BREAKOUT_ENTITYBRICK_HPP

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
    // Costructor
    Entity_Brick();
    // destructor
    ~Entity_Brick() override;
    int health;
    BrickType type;
    bool dropPowerUp{false};
    void onCollision(Entity& other) override;
    void render(SDL_Renderer* r) override;
    
    void setColor();
private:
    SDL_Texture* textureForType(BrickType t);
    
    SDL_Texture* normalTexture{nullptr};
    SDL_Texture* strongTexture{nullptr};
    SDL_Texture* indestructibleTexture{nullptr};
    SDL_Texture* mysteryTexture{nullptr};
};


#endif //BREAKOUT_ENTITYBRICK_HPP

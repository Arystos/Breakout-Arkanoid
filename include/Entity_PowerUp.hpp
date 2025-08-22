//
// Created by Aristide Sessa on 09/08/25.
//

#ifndef BREAKOUT_ENTITY_POWERUP_HPP
#define BREAKOUT_ENTITY_POWERUP_HPP

#include <SDL.h>
#include "Entity.hpp"
#include "Game.hpp"

enum class PowerUpType { 
    ExpandPaddle, ShrinkPaddle, MultiBall, SlowBall, FastBall,
    StickyPaddle, Laser, ExtraLife 
};

class Game;

class Entity_PowerUp : public Entity {
public:
    PowerUpType type;
    float fallSpeed{150.0f};
    void update(float dt) override {
        position.y += fallSpeed * dt;
        if (position.y > Game().Height()) {
            active = false; // deactivate if it falls off screen
        }
    }
    void render(SDL_Renderer* r) override {
        // draw icon based on type
    }
    
};


#endif //BREAKOUT_ENTITY_POWERUP_HPP

//
// Created by Aristide Sessa on 09/08/25.
//

#ifndef BREAKOUT_ENTITY_POWERUP_HPP
#define BREAKOUT_ENTITY_POWERUP_HPP

#include "Entity.hpp"
#include "Game.hpp"

enum class PowerUpType { 
    ExpandPaddle, ShrinkPaddle, MultiBall, SlowBall, FastBall,
    StickyPaddle, Laser, ExtraLife, Count
};

class Game;

class Entity_PowerUp : public Entity {
public:
    Entity_PowerUp();
    PowerUpType type;
    float fallSpeed{150.0f};
    void update(float dt) override;
    void render(SDL_Renderer* renderer) override;
    void onCollision(Entity& other) override;
    
private:
    TextureUPtr texture;
    Game& game = Game::getInstance();

};


#endif //BREAKOUT_ENTITY_POWERUP_HPP

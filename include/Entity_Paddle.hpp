//
// Created by Aristide Sessa on 09/08/25.
//

#ifndef BREAKOUT_ENTITY_PADDLE_HPP
#define BREAKOUT_ENTITY_PADDLE_HPP

#include "Entity.hpp"
#include <SDL.h>

struct Bounds { float minX, maxX; };

class Entity_Paddle : public Entity {
public:
    Entity_Paddle();
    float maxWidth{};
    bool sticky{false};
    
    void update(float dt) override;
    void render(SDL_Renderer* r) override;
    void move (float dir, float deltaTime);
    void onCollision(Entity& other) override;

    [[nodiscard]] glm::vec2 getVelocity() const { return velocity; }
    float getSpeedX() const { return velocity.x; }
    int MoveSign() const { return moveSign; } // -1, 0, +1
    
private:
    Bounds bounds{0.f, 0.f};
    float speed {500.0f}; // default speed (pixels per second)
    float prevX{0.f};
    glm::vec2 velocity{0.f, 0.f};
    int moveSign{0}; // -1 left, +1 right, 0 none
};


#endif //BREAKOUT_ENTITY_PADDLE_HPP

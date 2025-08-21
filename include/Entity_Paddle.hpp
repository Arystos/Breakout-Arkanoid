//
// Created by Aristide Sessa on 09/08/25.
//

#ifndef BREAKOUT_ENTITY_PADDLE_HPP
#define BREAKOUT_ENTITY_PADDLE_HPP

#include "Entity.hpp"
#include <SDL.h>


class Entity_Paddle : public Entity {
public:
    float speed {500.0f}; // speed of paddle movement
    
    float maxWidth;
    bool sticky{false};
    void update(float dt) override;
    void render(SDL_Renderer* r) override;
    void initialize (float paddleWidth, float paddleHeight, float paddleSpeed);
    
    void move (float dir, float deltaTime);

private:
    struct {
        float minX{0.0f};
        float maxX{800.0f}; // TODO: set to actual play area width
    } bounds;
};


#endif //BREAKOUT_ENTITY_PADDLE_HPP

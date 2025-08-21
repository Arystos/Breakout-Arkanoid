//
// Created by Aristide Sessa on 09/08/25.
//

#ifndef BREAKOUT_ENTITY_BALL_HPP
#define BREAKOUT_ENTITY_BALL_HPP

#include "Entity.hpp"
#include "Entity_Paddle.hpp"
#include <SDL.h>


class Entity_Ball : public Entity {
public:
    float radius;
    bool stickyMode{false};
    bool stuckToPaddle{true};
    void attachTo(Entity_Paddle& p) {
        stuckToPaddle = true;
        position.x = p.position.x + p.size.x / 2 - radius;
        position.y = p.position.y - 2 * radius;
    }
    void update(float dt) override {
        if (!stuckToPaddle) {
            position.x += velocity.x * dt;
            position.y += velocity.y * dt;
        }
    }
    void render(SDL_Renderer* r) override {
        SDL_Rect rect{(int)position.x, (int)position.y, (int)(radius*2), (int)(radius*2)};
        SDL_RenderCopy(r, texture, nullptr, &rect);
    }
};


#endif //BREAKOUT_ENTITY_BALL_HPP

//
// Created by Aristide Sessa on 09/08/25.
//

#include "Entity_Ball.hpp"
#include "Physics.hpp"
#include "StatePlay.hpp"
#include <iostream>

Entity_Ball::Entity_Ball() {
    size = {radius * 2.0f, radius * 2.0f};
    velocity = {300.0f, -300.0f};
    active = false;
}

void Entity_Ball::attachTo(Entity_Paddle &p) {
    paddle = &p; // keep reference to paddle
    stuckToPaddle = true;
    position.x = p.position.x + p.size.x / 2 - radius;
    position.y = p.position.y - 2 * radius;
    // TODO: Find a better way to get current state
    currentState = game.getCurrentState();
}

void Entity_Ball::update(float dt) {
    if (!active) return;
    if (!stuckToPaddle) {
        position.x += velocity.x * dt;
        position.y += velocity.y * dt;
    }
    
    // If the ball hits the screen borders bounce it back
    if (position.x <= 0) { // Left border
        position.x = 0;
        velocity.x = -velocity.x;
    } else if (position.x + radius * 2.0f >= game.Width()) { // Right border
        position.x = game.Width() - radius * 2.0f;
        velocity.x = -velocity.x;
    } else if (position.y <= 0) { // Top border
        position.y = 0.0f;
        velocity.y = -velocity.y;
    } else if (position.y + radius * 2.0f >= game.Height()) { // Bottom border
        // TODO: Trigger game over or life lost
        position.y = game.Height() - radius * 2.0f;
        velocity.y = -velocity.y;
        // For now, just bounce back
    }
    
    // TODO: Optimize collision detection
    // Check collision with all entities in the current state
    for (Entity* entity : currentState->getEntities()) {
        if (entity != this && entity->active) {
            if (Physics::circleVsAABB(
                {position.x + radius, position.y + radius}, // ball center
                radius,
                entity->getRect(), normal
                ))
            {
                onCollision(*entity);
            }
        }
    }
    
    

    /*
    // Collision paddle
    if (paddle && !stuckToPaddle) {
        glm::vec2 center = { position.x + radius, position.y + radius };
        glm::vec2 n;
        SDL_FRect box = paddle->getRect();

        if (Physics::circleVsAABB(center, radius, box, n)) {
            // Trigger onCollision event
            onCollision(*paddle);
            Physics::reflectBall(*this, n);
        }
    }
    */
}

void Entity_Ball::render(SDL_Renderer *r) {
    SDL_Rect rect{(int)position.x, (int)position.y, (int)(radius*2), (int)(radius*2)};
    const int padding = 2;
    SDL_Rect visualRect{rect.x + padding, rect.y + padding, rect.w - 2*padding, rect.h - 2*padding};
    if (!texture) {
        texture = MakeCircleTexture(r, (int)(radius*2));
    } else {
        SDL_SetTextureColorMod(texture, 255, 255, 255);
    }
    SDL_RenderCopy(r, texture, nullptr, &visualRect);
}

SDL_Texture *Entity_Ball::MakeCircleTexture(SDL_Renderer *r, int diameter) {
    SDL_Surface* s = SDL_CreateRGBSurfaceWithFormat(0, diameter, diameter, 32, SDL_PIXELFORMAT_RGBA32);
    auto* px = (Uint32*)s->pixels;
    int rad = diameter/2; int cx = rad, cy = rad;
    Uint32 white = SDL_MapRGBA(s->format, 255,255,255,255);
    Uint32 transparent = SDL_MapRGBA(s->format, 0,0,0,0);
    for (int y=0; y<diameter; ++y)
        for (int x=0; x<diameter; ++x) {
            int dx=x-cx, dy=y-cy;
            px[y*diameter+x] = (dx*dx+dy*dy <= rad*rad) ? white : transparent;
        }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(r, s);
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    SDL_FreeSurface(s);
    return tex;
}

void Entity_Ball::onCollision(Entity &other) {
    // If ball collides with paddle
    if (dynamic_cast<Entity_Paddle*>(&other)) {
        // Print debug information
        std::cout << "Ball collided with Paddle" << std::endl;
        Physics::reflectBall(*this, {0, -1}); // Reflect upwards
    } else if (dynamic_cast<Entity_Brick*>(&other)) {
        // If ball collides with a brick
        
    }
}

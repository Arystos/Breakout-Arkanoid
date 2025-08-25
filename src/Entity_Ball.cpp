//
// Created by Aristide Sessa on 09/08/25.
//

#include "Entity_Ball.hpp"
#include "Physics.hpp"
#include "StatePlay.hpp"
#include "StateGameOver.hpp"
#include <iostream>
#include <glm/trigonometric.hpp>
#include <glm/ext/scalar_constants.hpp>

Entity_Ball::Entity_Ball() {
    size = {radius * 2.0f, radius * 2.0f};
    //velocity = {300.0f, -300.0f};
    game.setBallCount(game.BallCount() + 1);
    active = false;
}

void Entity_Ball::attachTo(Entity_Paddle &p) {
    paddle = &p; // keep reference to paddle
    stuckToPaddle = true;
    position.x = p.position.x + p.size.x / 2 - radius;
    position.y = p.position.y - 2 * radius;
    setCurrentState(game.getCurrentState());
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
    } else if (position.y + radius * 2.0f >= float(game.Height())) { // Bottom border
        game.setBallCount(game.BallCount() - 1);
        toBeDestroyed = true; // Mark for removal
        if (game.BallCount() <= 0) {
            // Transition to Game Over state
            game.changeState(std::make_unique<State_GameOver>());
        }
        return;
    }
    
    
    
    // check collision with all active entities in the current state
    for (Entity* entity : currentState->getEntities()) {
        if (!entity || entity == this || !entity->active) continue;
        
        if (!Physics::circleVsAABB({position.x + radius, position.y + radius},
                                   radius, entity->getRect(), normal))
            continue;

        onCollision(*entity);
    }
    
    
}

void Entity_Ball::render(SDL_Renderer *r) {
    // Draw the ball as a filled circle
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

void Entity_Ball::onCollision(Entity &entity) {
    
#pragma region Paddle Collision
    if (auto* paddle = dynamic_cast<Entity_Paddle*>(&entity)) {
        if (stickyMode) {
            stuckToPaddle = true;
            position.x = paddle->position.x + paddle->size.x / 2 - radius;
            position.y = paddle->position.y - 2 * radius;
            velocity = {0.0f, 0.0f};
            return;
        }

        // reflection with angle based on hit position
        float paddleCenter = paddle->position.x + paddle->size.x / 2;
        float ballCenter = position.x + radius;
        float relativeIntersect = (ballCenter - paddleCenter);
        float normalizedRelativeIntersection = (relativeIntersect / (paddle->size.x / 2));
        // bounce angle between -60 and +60 degrees
        float bounceAngle = normalizedRelativeIntersection * (glm::pi<float>() / 3.0f);
        
        // new velocity and direction
        float speed = glm::length(velocity);
        velocity.x = speed * glm::sin(bounceAngle);
        velocity.y = -speed * glm::cos(bounceAngle);
        // give some of the paddle's horizontal velocity to the ball
        velocity.x += paddle->getVelocity().x * 0.5f; // fattore 0.5 regolabile
        // clamp to max speed
        if (glm::length(velocity) > maxSpeed) {
            velocity = glm::normalize(velocity) * maxSpeed;
        }

        // Deprecated
        //position.y = paddle->position.y - 2 * radius; // reposition above paddle
        //position.x += velocity.x * 0.016f; // small offset to avoid sticking
    }
#pragma endregion

#pragma region Brick Collsion
    if (auto* brick = dynamic_cast<Entity_Brick*>(&entity)) {
        // reflection
        velocity = glm::reflect(velocity, normal);
        position += normal * (radius * 0.5f);
        // Clamp the bouce angle to avoid too horizontal or vertical trajectories
        float angle = glm::atan(velocity.y, velocity.x);
        // between -60 and +60 degrees
        const float minAngle = glm::radians(20.0f);
        const float maxAngle = glm::radians(160.0f);
        if (std::abs(angle) < minAngle) {
            angle = (angle < 0) ? -minAngle : minAngle;
        } else if (std::abs(angle) > maxAngle) {
            angle = (angle < 0) ? -maxAngle : maxAngle;
        }
        float speed = glm::length(velocity);
        velocity.x = speed * glm::cos(angle);
        velocity.y = speed * glm::sin(angle);
        if (glm::length(velocity) > maxSpeed) {
            velocity = glm::normalize(velocity) * maxSpeed;
        }
        brick->onCollision(*this);
        // destroy the brick if health <= 0
        if (!brick->active) {
            if (auto* playState = dynamic_cast<State_Play*>(currentState))
                playState->destroyEntity(brick);
        }
    }
#pragma endregion
    
}

// destructor 
Entity_Ball::~Entity_Ball() {
    active = false;
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

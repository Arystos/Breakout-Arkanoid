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
    } else if (position.x + radius * 2.0f >= float (game.Width())) { // Right border
        position.x = float(game.Width()) - radius * 2.0f;
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
        // ignore power-ups
        if (dynamic_cast<Entity_PowerUp*>(entity)) continue;
        // ignore other balls
        if (dynamic_cast<Entity_Ball*>(entity)) continue;
        
        if (!Physics::circleVsAABB({position.x + radius, position.y + radius},
                                   radius, entity->getRect(), normal))
            continue;

        onCollision(*entity);
    }
    
}

void Entity_Ball::render(SDL_Renderer *renderer) {
    // Draw the ball as a filled circle
    SDL_Rect rect{(int)position.x, (int)position.y, (int)(radius*2), (int)(radius*2)};
    const int padding = 2;
    SDL_Rect visualRect{rect.x + padding, rect.y + padding, rect.w - 2*padding, rect.h - 2*padding};
    if (!texture) // create texture if not already done
        texture = MakeCircleTexture(renderer, (int)(radius * 2));
    else 
        SDL_RenderCopy(renderer, texture.get(), nullptr, &visualRect); // render the texture
        
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

void Entity_Ball::onCollision(Entity &entity) {
    
#pragma region Paddle Collision
    if (auto* paddle_ = dynamic_cast<Entity_Paddle*>(&entity)) {
        if (stickyMode) {
            stuckToPaddle = true;
            position.x = paddle_->position.x + paddle_->size.x / 2 - radius;
            position.y = paddle_->position.y - 2 * radius;
            velocity = {0.0f, 0.0f};
            return;
        }

        // reflection with angle based on hit position
        float paddleCenter = paddle_->position.x + paddle_->size.x / 2;
        float ballCenter = position.x + radius;
        float relativeIntersect = (ballCenter - paddleCenter);
        float normalizedRelativeIntersection = (relativeIntersect / (paddle_->size.x / 2));
        // bounce angle between -60 and +60 degrees
        float bounceAngle = normalizedRelativeIntersection * (glm::pi<float>() / 3.0f);
        
        // new velocity and direction
        float speed = glm::length(velocity);
        velocity.x = speed * glm::sin(bounceAngle);
        velocity.y = -speed * glm::cos(bounceAngle);
        // give some of the paddle_'s horizontal velocity to the ball
        velocity.x += paddle_->getVelocity().x * 0.5f; // fattore 0.5 regolabile
        // clamp to max speed
        if (glm::length(velocity) > maxSpeed) {
            velocity = glm::normalize(velocity) * maxSpeed;
        }

        // Deprecated
        //position.y = paddle_->position.y - 2 * radius; // reposition above paddle_
        //position.x += velocity.x * 0.016f; // small offset to avoid sticking
    }
#pragma endregion

#pragma region Brick Collsion
    if (auto* brick_ = dynamic_cast<Entity_Brick*>(&entity)) {
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
        brick_->onCollision(*this);
    }
#pragma endregion
    
}

// destructor 
Entity_Ball::~Entity_Ball() {
    active = false;
    /*
    if (texture) {
        SDL_DestroyTexture(texture.release());
        texture = nullptr;
    }
     */
}

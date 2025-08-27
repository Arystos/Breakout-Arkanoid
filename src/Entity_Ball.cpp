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
    // start stuck to paddle
    stuckToPaddle = true;
    // inactive until attached to paddle
    active = false;
}

void Entity_Ball::attachToCentre(Entity_Paddle &p) {
    paddle = &p; // keep reference to paddle
    stuckToPaddle = true;
    position.x = p.position.x + p.size.x / 2 - radius;
    position.y = p.position.y - 2 * radius;
    setCurrentState(game.getCurrentState());
}

void Entity_Ball::update(float dt) {
    if (!active) return;
    if (!stuckToPaddle) {
        position += velocity * dt * game.BallSpeedModifier() * (1.0f + game.BallBounces() * 0.0001f);
    } else if (paddle) {
        position = paddle->position + stickOffset;
        position.y = paddle->position.y - 2 * radius; // stay above the paddle
        // Edge clamping
        if (position.x + radius < paddle->position.x)
            position.x = paddle->position.x - radius + 10.0f; // clamp to left edge
        else if (position.x + radius > paddle->position.x + paddle->size.x)
            position.x = paddle->position.x + paddle->size.x - radius - 10.0f; // clamp to right edge
    }
    
    // If the ball hits the screen borders bounce it back
    if (position.x <= 0) { // Left border
        position.x = 0;
        velocity.x = -velocity.x;
        onCollision(*this);
    } else if (position.x + radius * 2.0f >= float (game.Width())) { // Right border
        position.x = float(game.Width()) - radius * 2.0f;
        velocity.x = -velocity.x;
        onCollision(*this);
    } else if (position.y <= 0) { // Top border
        position.y = 0.0f;
        velocity.y = -velocity.y;
        onCollision(*this);
    } else if (position.y + radius * 2.0f >= float(game.Height())) { // Bottom border
        std::cout << "Ball lost!" << std::endl;
        game.setBallCount(game.BallCount() - 1);
        toBeDestroyed = true;
        // decrement lives and check for game over
        if (auto* playState = dynamic_cast<State_Play*>(game.getCurrentState())){
            // check if there are other balls still active
            if (game.BallCount() <= 0) {
                playState->playerLives--;
                game.resetBallBounces();
            }
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

    if (trail.enabled) updateTrail(dt);
    
}

void Entity_Ball::render(SDL_Renderer *renderer) {
    if (!active) return;
    
    // Draw the ball as a filled circle
    SDL_Rect rect{(int)position.x, (int)position.y, (int)(radius*2), (int)(radius*2)};
    const int padding = 2;
    SDL_Rect visualRect{rect.x + padding, rect.y + padding, rect.w - 2*padding, rect.h - 2*padding};
    if (!texture) // create texture if not already done
        texture = MakeCircleTexture(renderer, (int)(radius * 2));
    else 
        SDL_RenderCopy(renderer, texture.get(), nullptr, &visualRect); // render the texture
        
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // render trail first
    if (trail.enabled) renderTrail(renderer);
}

void Entity_Ball::onCollision(Entity &entity) {
    
#pragma region Paddle Collision
    if (auto* paddle_ = dynamic_cast<Entity_Paddle*>(&entity)) {
        if (paddle_->sticky && !stuckToPaddle) {
            StickTo(*paddle_);
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
        glm::vec2 previousVelocity = velocity;
        velocity = { speed * glm::sin(bounceAngle), -speed * glm::cos(bounceAngle) };
        // give some of the paddle_'s horizontal velocity to the ball
        velocity.x += paddle_->getVelocity().x * 0.5f; // fattore 0.5 regolabile
        // if the new velocity is bigger than before of 10%
        if (glm::length(velocity) > glm::length(previousVelocity) * 1.1f &&
            game.BallSpeedModifier() >= 1.0f) // only if not slowed down
        {
            TrailEnable(0.25f);
            auto t = game.timerManager.create(
                    0.25f, false, "ball_trail", {},
                    [this](auto &&) { TrailStopEmitting(); }
            );
            (void)t;
        }
        // clamp speed
        if (glm::length(velocity) > maxSpeed)
            velocity = glm::normalize(velocity) * maxSpeed;
        if (glm::length(velocity) < minSpeed)
            velocity = glm::normalize(velocity) * minSpeed;
        paddle_->onCollision(*this);
        //enableTrail(0.25f); // enable trail for 0.25 seconds
        return;
    }
#pragma endregion

#pragma region Brick Collsion
    if (auto* brick_ = dynamic_cast<Entity_Brick*>(&entity)) {
        std::cout << "Ball hit brick!" << std::endl;
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
        // clamp speed
        if (glm::length(velocity) > maxSpeed)
            velocity = glm::normalize(velocity) * maxSpeed;
        if (glm::length(velocity) < minSpeed)
            velocity = glm::normalize(velocity) * minSpeed;
        brick_->onCollision(*this);
        return;
    }
#pragma endregion

    // increment total bounces
    game.setBallBounces(game.BallBounces() + 1);
    
}

void Entity_Ball::StickTo(Entity_Paddle& p) {
    paddle = &p;
    stuckToPaddle = true;
    // print centre of the paddle
    std::cout << "Paddle centre: " << (p.position.x + p.size.x / 2) << std::endl;
    stickOffset = position - p.position; // top-left vs top-left
    std::cout << "Ball stuck to paddle at offset: (" 
              << stickOffset.x << ", " << stickOffset.y << ")" << std::endl;
}

void Entity_Ball::Release() {
    if (!stuckToPaddle) return;
    stuckToPaddle = false;
    velocity = {paddle ? paddle->getVelocity().x * 0.5f : 0.0f, -300.0f};
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

void Entity_Ball::updateTrail(float dt) {
    if (!trail.enabled) return;

    if (trail.emitting) {
        trail.acc += dt;
        while (trail.acc >= trail.sampleDt) {
            trail.acc -= trail.sampleDt;
            trail.trailNodes.push_front({ position, 0.f });
            if ((int)trail.trailNodes.size() > trail.max) trail.trailNodes.pop_back();
        }
    }

    for (auto& n : trail.trailNodes) n.age += dt;
    while (!trail.trailNodes.empty() && trail.trailNodes.back().age > trail.lifetime)
        trail.trailNodes.pop_back();

    if (!trail.emitting && trail.trailNodes.empty())
        trail.enabled = false;
}

void Entity_Ball::renderTrail(SDL_Renderer* r) {
    if (!texture || trail.trailNodes.empty()) return;

    for (int i = (int)trail.trailNodes.size() - 1; i >= 0; --i) {
        const auto& n = trail.trailNodes[i];
        float t = glm::clamp(n.age / trail.lifetime, 0.0f, 1.0f); // 0 nuovo -> 1 vecchio

        // Alpha con curva
        Uint8 alpha = (Uint8)(trail.trailMaxAlpha * glm::pow(1.0f - t, trail.trailAlphaPow));
        SDL_SetTextureAlphaMod(texture.get(), alpha);

        // Scala non lineare: parte da trailBaseScale e scende fino a trailMinScale*trailBaseScale
        float s01   = trail.trailMinScale + (1.0f - trail.trailMinScale) * glm::pow(1.0f - t, trail.trailScalePow);
        float scale = trail.trailBaseScale * s01;

        // opzionale: leggero fattore per indice per accentuare il restringimento lungo la scia
        float idxFactor = 1.0f - 0.08f * (float)i;              // 8% per nodo
        scale = glm::max(0.02f, scale * glm::max(0.6f, idxFactor)); // clamp minimo

        int base = int(radius * 2);
        int w = glm::max(1, int(base * scale));
        int h = w;

        float cx = n.pos.x + radius;
        float cy = n.pos.y + radius;
        SDL_Rect dst{ int(cx - w * 0.5f), int(cy - h * 0.5f), w, h };
        SDL_RenderCopy(r, texture.get(), nullptr, &dst);
    }
    SDL_SetTextureAlphaMod(texture.get(), 255);
}

void Entity_Ball::enableTrail(float seconds) {
    trail.enabled = true;
    trail.emitting = true;
    trail.acc = 0.f;
    //trail.lifetime = seconds;
    trail.trailNodes.clear();
    //trail.timer = 0.f;
}

void Entity_Ball::TrailEnable(float seconds) {
    trail.enabled  = true;
    trail.emitting = true;
    trail.trailNodes.clear();
    trail.acc = 0.f;
    trail.timer = 0.f;
    trail.lifetime = seconds;
}

void Entity_Ball::TrailStopEmitting() {
    trail.emitting = false;
}

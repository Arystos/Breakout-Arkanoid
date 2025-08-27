//
// Created by Aristide Sessa on 09/08/25.
//

#include "Entity_Paddle.hpp"
#include "Game.hpp"
#include "Entity_PowerUp.hpp"
#include "StatePlay.hpp"
#include <algorithm>

// constructor
Entity_Paddle::Entity_Paddle() {
    Game& game = Game::getInstance();
    position = {
            ( game.PlayableWidth() * 0.5f - size.x * 0.5f),
            ( game.PlayableHeight() - size.y - 20.0f)
    };
    size = {100.0f, 20.0f};
    speed = 500.0f; // default speed
    prevX = position.x;
    velocity = {0.f, 0.f};
    moveSign = 0;
    active = true;
}


/*
void Entity::render(SDL_Renderer *r) {
    // Print debug information
    SDL_Log("Rendering Paddle");
    if (texture) {
        SDL_Rect rect{(int)position.x, (int)position.y, (int)size.x, (int)size.y};
        SDL_RenderCopy(r, texture, nullptr, &rect);
    } else {
        // Fallback rendering if no texture is set
        SDL_SetRenderDrawColor(r, 255, 255, 255, 255); // White color
        SDL_Rect rect{(int)position.x, (int)position.y, (int)size.x, (int)size.y};
        SDL_RenderFillRect(r, &rect);
    }
}
 */

void Entity_Paddle::render(SDL_Renderer *r) {
    if (texture == nullptr) {
        // Create a rectangle for the paddle
        if (sticky)
            SDL_SetRenderDrawColor(r, 255, 255, 0, 255); // yellow
        else {
            SDL_SetRenderDrawColor(r, 255, 255, 255, 255); // White color
        }
        SDL_Rect rect{(int)position.x, (int)position.y, (int)size.x, (int)size.y};
        SDL_RenderFillRect(r, &rect);
    } else {
        SDL_Rect rect{(int)position.x, (int)position.y, (int)size.x, (int)size.y};
        SDL_RenderCopy(r, texture, nullptr, &rect);
    }
}

void Entity_Paddle::update(float dt) {
    
    // paddle movement
    const Uint8 *state = SDL_GetKeyboardState(nullptr);
    float dir = 0.0f;
    if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A]) {
        dir = -1.0f; // Move left
        move(dir, dt);
    } else if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]) {
        dir = 1.0f; // Move right
        move(dir, dt);
    } else {
        // No movmement
        if (velocity.x != 0.0f) {
            move(0.0f, dt);
        }
    }
    
}

void Entity_Paddle::move(float dir, float deltaTime) {
    
    if (deltaTime <= 0.f) deltaTime = 1.f / 60.f; // guard
    const float oldX = position.x;

    // update position
    position.x += std::clamp(dir, -1.f, 1.f) * speed * deltaTime;

    // clamp to bounds
    bool clampedLeft = false, clampedRight = false;
    if (position.x < Game::getInstance().LeftBorder()) {
        position.x = Game::getInstance().LeftBorder();
        clampedLeft = true;
    } else if (position.x + size.x > Game::getInstance().RightBorder()) {
        position.x = Game::getInstance().RightBorder() - size.x;
        clampedRight = true;
    }

    // paddle velocity
    velocity.x = (position.x - oldX) / deltaTime;
    velocity.y = 0.f;

    // movement direction (-1 left, +1 right, 0 none)
    if ((clampedLeft && dir < 0.f) || (clampedRight && dir > 0.f))
        moveSign = 0;
    else
        moveSign = (velocity.x > 1e-3f) ? 1 : (velocity.x < -1e-3f ? -1 : 0);

    prevX = position.x;
    
}

void Entity_Paddle::onCollision(Entity &other) {
    if (auto* powerUp = dynamic_cast<Entity_PowerUp*>(&other)) {
        switch (powerUp->type) {
            case PowerUpType::MultiBall: // spawn an extra ball
                if (auto *playState = dynamic_cast<State_Play *>(
                        Game::getInstance().getCurrentState()))
                    playState->spawnBall();
                break;
            case PowerUpType::ExpandPaddle: {
                size.x *= sizeModifier; // expand paddle size
                // move slightly to left to keep centre position
                position.x -= (size.x * (sizeModifier - 1.0f)) / 2.0f;
                std::cout << "Paddle expanded" << std::endl;
                uint64_t t = Game::getInstance().timerManager.create(
                        powerUpDuration, false, "powerup_expand_paddle", {},
                        [this](auto &&tag) {
                            onPaddleGrowTimerEnd(std::forward<decltype(tag)>(tag));
                        }
                );(void)t;
                break;
            }
            case PowerUpType::ShrinkPaddle: {
                size.x /= sizeModifier; // shrink paddle size
                // move slightly to right to keep centre position
                position.x += (size.x * (sizeModifier - 1.0f)) / 2.0f;
                std::cout << "Paddle shrunk" << std::endl;
                uint64_t t = Game::getInstance().timerManager.create(
                        powerUpDuration, false, "powerup_shrink_paddle", {},
                        [this](auto &&tag) {
                            onPaddleShrinkTimerEnd(std::forward<decltype(tag)>(tag));
                        }
                );(void)t;
                break;
            }
            case PowerUpType::SlowBall: {
                // TODO: slow down ball speed
                if (auto *playState = dynamic_cast<State_Play *>(
                        Game::getInstance().getCurrentState()))
                    for (auto &ball: playState->getBalls()) {
                        // slow down
                        Game::getInstance().setBallSpeedModifier(
                                Game::getInstance().BallSpeedModifier() / ballSpeedModifier);
                        // Start Timer
                        uint64_t t = Game::getInstance().timerManager.create(
                                powerUpDuration, false, "powerup_slow_ball", {},
                                [this, &ball](auto &&tag) {
                                    onBallSlowTimerEnd(std::forward<decltype(tag)>(tag), &ball);
                                }
                        );(void)t;
                    }
                break;
            }
            case PowerUpType::FastBall: {
                // TODO: speed up ball speed
                if (auto *playState = dynamic_cast<State_Play *>(
                        Game::getInstance().getCurrentState()))
                    for (auto &ball: playState->getBalls()) {
                        // speed up
                        Game::getInstance().setBallSpeedModifier(
                                Game::getInstance().BallSpeedModifier() * ballSpeedModifier);
                        // Start Timer
                        uint64_t t = Game::getInstance().timerManager.create(
                                powerUpDuration, false, "powerup_fast_ball", {},
                                [this, &ball](auto &&tag) {
                                    onBallFastTimerEnd(std::forward<decltype(tag)>(tag), &ball);
                                }
                        );(void)t;
                    }
                break;
            }
            case PowerUpType::StickyPaddle: {
                sticky = true;// get the name of this paddle instance
                // If the timer is already running, reset it
                Game::getInstance().timerManager.endByTag("powerup_sticky", {});
                uint64_t t = Game::getInstance().timerManager.create(
                        powerUpDuration, false, "powerup_sticky", {},
                        [this](auto && tag) { 
                            onStickyTimerEnd(std::forward<decltype(tag)>(tag)); }
                );(void)t;
                break;
            }
            default:
                break;
        }
        //Set the power-up label text
        // show power-up label in the play state
        if (auto* playState = dynamic_cast<State_Play*>(Game::getInstance().getCurrentState())) {
            playState->SetPowerUpLabelText(powerUp->typeToString());
            playState->SetPowerUpLabelVisible(true);
            
            // Info text for specific power-ups
            if (powerUp->type == PowerUpType::StickyPaddle) {
                playState->SetPowerUpLabelInfoText("Press Space to release the ball");
                playState->SetPowerUpLabelInfoVisible(true);
            }
            
            // stop if any previous timer is running
            Game::getInstance().timerManager.endByTag("powerup_label", {});
            // Start a timer to hide the label after a short duration
            uint64_t t = Game::getInstance().timerManager.create(
                    powerUpDuration, false, "powerup_label", {},
                    [this](auto &&tag) {
                        onPowerUpCollectedTimerEnd(std::forward<decltype(tag)>(tag));
                    }
            );(void)t;
        }
    }
}

void Entity_Paddle::onStickyTimerEnd(uint64_t id) {
    (void)id;
    sticky = false;
    if (auto* playState = dynamic_cast<State_Play*>(Game::getInstance().getCurrentState())) {
        for (auto& ball : playState->getBalls())
            if (ball->stuckToPaddle) ball->Release();
    }
}

void Entity_Paddle::onPaddleGrowTimerEnd(uint64_t id) {
    (void)id;
    // shrink paddle back to normal size
    size.x /= sizeModifier;
}

void Entity_Paddle::onPaddleShrinkTimerEnd(uint64_t id) {
    (void)id;
    size.x *= sizeModifier;
}

void Entity_Paddle::onBallSlowTimerEnd(uint64_t id, std::unique_ptr<struct Entity_Ball> *ball) {
    (void)id;
    //ball->get()->setMaxSpeed(ball->get()->MaxSpeed() * ballSpeedModifier); // restore speed
    //ball->get()->velocity *= ballSpeedModifier;
    Game::getInstance().setBallSpeedModifier(
            Game::getInstance().BallSpeedModifier() * ballSpeedModifier);
}

void Entity_Paddle::onBallFastTimerEnd(uint64_t id, std::unique_ptr<Entity_Ball> *ball) {
    (void)id;
    //ball->get()->setMaxSpeed(ball->get()->MaxSpeed() / ballSpeedModifier); // restore speed
    //ball->get()->velocity /= ballSpeedModifier;
    Game::getInstance().setBallSpeedModifier(
            Game::getInstance().BallSpeedModifier() / ballSpeedModifier);
}

void Entity_Paddle::onPowerUpCollectedTimerEnd(uint64_t id) {
    (void)id;
    if (auto* playState = dynamic_cast<State_Play*>(Game::getInstance().getCurrentState())) {
        // check if there are active timers for other power-ups
        for (const auto& tag : Game::getInstance().activePowerUpTags) {
            if (Game::getInstance().timerManager.isTagActive(tag, {})) {
                break; // do not hide the label yet
            }
        }
        // Hide power-up label
        playState->SetPowerUpLabelVisible(false);
        playState->SetPowerUpLabelInfoVisible(false);
    };
}

//
// Created by Aristide Sessa on 09/08/25.
//

#ifndef BREAKOUT_ENTITY_BALL_HPP
#define BREAKOUT_ENTITY_BALL_HPP

#include "Entity.hpp"
#include "Entity_Paddle.hpp"
#include "Entity_Brick.hpp"
#include "Game.hpp"
#include <SDL.h>

class Entity_Ball : public Entity {
public:
    Entity_Ball();
    ~Entity_Ball() override;
    
    bool stickyMode{false};     // when true, ball sticks to paddle on collision
    bool stuckToPaddle{false};   // ball starts stuck to paddle
    void attachToCentre(Entity_Paddle& p);
    void update(float dt) override;
    void render(SDL_Renderer* renderer) override;
    void onCollision(Entity& other) override;

    glm::vec2 velocity{300.0f, -300.0f};
    
    [[nodiscard]] float Radius() const { return radius; }
    [[nodiscard]] float Size() const { return radius * 2.0f; } // diameter
    [[nodiscard]] float MaxSpeed() const { return maxSpeed; }
    [[nodiscard]] float MinSpeed() const { return minSpeed; }
    float setMaxSpeed(float s) { maxSpeed = s; return maxSpeed; }
    float setMinSpeed(float s) { minSpeed = s; return minSpeed; }
    
    State* setCurrentState(State* s) { currentState = s; return currentState; }

    void StickTo(Entity_Paddle& p);
    void Release();

    TextureUPtr texture;
    
private:
    float radius = 10.0f; // default radius
    float maxSpeed = 500.0f;
    float minSpeed = 200.0f;
    glm::vec2 normal{}; // collision normal
    glm::vec2 stickOffset{};
    Game& game = Game::getInstance();
    State* currentState{};
    Entity_Paddle* paddle{nullptr}; // reference to paddle for collision
};


#endif //BREAKOUT_ENTITY_BALL_HPP

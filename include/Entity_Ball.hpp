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
    void attachTo(Entity_Paddle& p);
    void update(float dt) override;
    void render(SDL_Renderer* r) override;
    void onCollision(Entity& other) override;
    
    [[nodiscard]] float Radius() const { return radius; }
    [[nodiscard]] float Size() const { return radius * 2.0f; } // diameter
    
    State* setCurrentState(State* s) { currentState = s; return currentState; }
    
private:
    float radius = 10.0f; // default radius
    glm::vec2 normal; // collision normal
    SDL_Texture* MakeCircleTexture(SDL_Renderer* r, int diameter);
    Game& game = Game::getInstance();
    State* currentState;
    Entity_Paddle* paddle{nullptr}; // reference to paddle for collision
};


#endif //BREAKOUT_ENTITY_BALL_HPP

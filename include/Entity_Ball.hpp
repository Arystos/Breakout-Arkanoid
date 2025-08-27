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
#include <deque>

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

    glm::vec2 velocity{0.0f, 0.0f};
    
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

    void enableTrail(float seconds);
    
private:
    float radius = 10.0f; // default radius
    float maxSpeed = 500.0f;
    float minSpeed = 350.0f;
    glm::vec2 normal{}; // collision normal
    glm::vec2 stickOffset{};
    Game& game = Game::getInstance();
    State* currentState{};
    Entity_Paddle* paddle{nullptr}; // reference to paddle for collision

    void updateTrail(float dt);
    void renderTrail(SDL_Renderer* renderer);
    void TrailEnable(float seconds);
    void TrailStopEmitting();

    struct TrailNode {
        glm::vec2 pos;   // top-left della ball al sample
        float age;  // secondi da quando è stato campionato
    };
    
    struct TrailInfo {
        std::deque<TrailNode> trailNodes;
        float lifetime = 0.25f;   // durata scia totale 0.25f
        int max = 12;       // numero massimo di afterimage
        float sampleDt = 0.016f;  // ogni quanti secondi campionare (~60Hz)
        float timer = 0.f; // timer for sampling
        float acc = 0.f; // accumulator for sampling
        bool enabled = false; // enable or disable trail
        bool emitting = false; // if true, keep emitting new trail nodes
        // Trail selection
        float trailBaseScale = 0.95f;  // scala iniziale degli afterimage (<1 per farli già più piccoli della palla)
        float trailMinScale  = 0.1f;  // scala minima a fine vita (più piccolo)
        float trailScalePow  = 0.1f;   // curva di riduzione (più alto = restringe più in fretta)
        float trailAlphaPow  = 1.3f;   // curva di alpha
        Uint8 trailMaxAlpha  = 200;    // alpha massimo degli afterimage
    } trail;
    TrailInfo& getTrail() { return trail; }
    
};


#endif //BREAKOUT_ENTITY_BALL_HPP

//
// Created by Aristide Sessa on 09/08/25.
//

#ifndef BREAKOUT_ENTITYBRICK_HPP
#define BREAKOUT_ENTITYBRICK_HPP

#include <SDL.h>
#include "Entity.hpp"
#include <random>
#include <vector>

enum class BrickType { 
    Normal, Strong, Indestructible, Mystery 
};

class Entity_Brick : public Entity {
public:
    Entity_Brick();
    ~Entity_Brick() override = default;
    
    int health;
    BrickType type;
    bool dropPowerUp{false};
    void update(float dt) override;
    void onCollision(Entity& other) override;
    void render(SDL_Renderer* r) override;
    
    void setColor();
private:
    SDL_Texture* textureForType(BrickType t);
    
    SDL_Texture* normalTexture{nullptr};
    SDL_Texture* strongTexture{nullptr};
    SDL_Texture* indestructibleTexture{nullptr};
    SDL_Texture* mysteryTexture{nullptr};


    struct BrickParticle {
        glm::vec2 pos;
        glm::vec2 vel;
        float age;        // s
        float lifetime;   // s
        float size0;      // px iniziale (lato quad)
        float size1;      // px finale
        Uint8  a0;        // alpha iniziale
        Uint8  a1;        // alpha finale
        SDL_Color col;    // colore base (r,g,b) del brick al momento della rottura
        float gravityMul;
    };
    
    struct BrickParticleInfo {
        // ---- Particle system interno al brick ----
        std::vector<BrickParticle> particles_;
        bool particlesActive_ = false;
        int  particleBudget_  = 48;    // max particelle per rottura
        float pLifeMin_ = 0.18f;
        float pLifeMax_ = 0.35f;
        float pSpdMin_  = 120.f;       // px/s
        float pSpdMax_  = 280.f;
        float pSize0_   = 7.f;
        float pSize1_   = 1.5f;        // si restringono
        float pGravity_ = 680.f;       // “caduta” leggera
        std::minstd_rand rng_{0};
    } particleInfo;

    void spawnParticles();         // chiamata alla distruzione
    void updateParticles(float dt);
    void renderParticles(SDL_Renderer* r);
};


#endif //BREAKOUT_ENTITYBRICK_HPP

//
// Created by Aristide Sessa on 09/08/25.
//

#include "Entity_Brick.hpp"
#include "Game.hpp"
#include "StatePlay.hpp"
#include <algorithm>
#include <random>
#include <vector>

Entity_Brick::Entity_Brick() {
    size = {60.0f, 20.0f};
    health = 1;
    active = true;
}

void Entity_Brick::update(float dt) {
    if (particleInfo.particlesActive_) { updateParticles(dt); }
}

void Entity_Brick::render(SDL_Renderer *r) {
    // se color è float 0..1
    SDL_Rect rect{
            static_cast<int>(position.x),
            static_cast<int>(position.y),
            std::max(1, static_cast<int>(size.x)),
            std::max(1, static_cast<int>(size.y))
    };

    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(r, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(r, &rect);

    if (particleInfo.particlesActive_) { renderParticles(r); }
}

void Entity_Brick::onCollision(Entity &other) {
    if (type != BrickType::Indestructible) { 
        health--; 
        active = health > 0;
        spawnParticles();
    }
    if (type == BrickType::Strong) {
        // change color to green
        //color = {0, 255, 0, 255};
    }
    if (type == BrickType::Mystery) {
        dropPowerUp = true;
        // In a real game, you would spawn a power-up entity here
        if (auto* playState = dynamic_cast<State_Play*>(Game::getInstance().getCurrentState())) {
            playState->spawnPowerUp(position + size / 2.0f);
            dropPowerUp = false; // reset flag after dropping
        }
    }
}

void Entity_Brick::setColor() {
    switch (type) {
        case BrickType::Normal:
            color = {0, 255, 0, 255}; // Green
            break;
        case BrickType::Strong:
            color = {255, 165, 0, 255}; // Orange
            break;
        case BrickType::Indestructible:
            color = {128, 128, 128, 255}; // Gray
            break;
        case BrickType::Mystery:
            color = {255, 0, 255, 255}; // Magenta
            break;
        default:
            color = {255, 255, 255, 255}; // White
            break;
    }
}

void Entity_Brick::spawnParticles() {
    auto& c = particleInfo;

    c.particles_.clear();
    c.particles_.reserve(c.particleBudget_);
    c.particlesActive_ = true;

    // distribuzioni
    std::uniform_real_distribution<float> u01(0.f, 1.f);
    std::uniform_real_distribution<float> life(c.pLifeMin_, c.pLifeMax_);
    std::uniform_real_distribution<float> spd (c.pSpdMin_,  c.pSpdMax_);
    std::uniform_real_distribution<float> jitterA0(160.f, 255.f);
    std::uniform_real_distribution<float> gravMul(0.85f, 1.25f);
    std::uniform_int_distribution<int>   colJit(-25, 25);

    // centro brick e semiassi per spawn nel disco
    const float cx = position.x + size.x * 0.5f;
    const float cy = position.y + size.y * 0.5f;
    const float rx = size.x * 0.5f;
    const float ry = size.y * 0.5f;

    for (int i = 0; i < c.particleBudget_; ++i) {
        BrickParticle p{};

        // POS: uniforme nel disco (evita pattern a griglia)
        float u = u01(Game::rng), v = u01(Game::rng);
        float r = std::sqrt(u);                  // uniforme su area
        float th = 6.2831853f * v;               // 2π
        p.pos.x = cx + r * std::cos(th) * rx;
        p.pos.y = cy + r * std::sin(th) * ry;

        // VEL: modulo casuale e direzione casuale
        float s = spd(Game::rng);
        float a = 6.2831853f * u01(Game::rng);
        p.vel = { std::cos(a) * s, std::sin(a) * s };

        // LIFE / SIZE / ALPHA con varianza per-particella
        p.age = 0.f;
        p.lifetime = life(Game::rng);
        p.size0 = c.pSize0_ * (0.8f + 0.6f * u01(Game::rng));
        p.size1 = c.pSize1_ * (0.8f + 0.6f * u01(Game::rng));
        p.a0 = static_cast<Uint8>(jitterA0(Game::rng));
        p.a1 = 0;

        // Colore con micro-jitter
        auto jit = [&](Uint8 base){
            int j = int(base) + colJit(Game::rng);
            return (Uint8)std::clamp(j, 0, 255);
        };
        p.col = { jit(color.r), jit(color.g), jit(color.b), 255 };

        // opzionale: moltiplicatore di gravità per varianza in update
        p.gravityMul = gravMul(Game::rng); // assicurati che BrickParticle abbia questo campo

        c.particles_.push_back(p);
    }
}

void Entity_Brick::updateParticles(float dt) {
    auto& pInfo = particleInfo;
    if (!pInfo.particlesActive_) return;

    for (auto& particle : pInfo.particles_) {
        if (particle.age > particle.lifetime) continue;
        particle.age += dt;

        // integrazione semplice + gravità
        particle.vel.y += pInfo.pGravity_ * dt;
        particle.pos   += particle.vel * dt;
    }

    // rimuovi morte
    pInfo.particles_.erase(
            std::remove_if(pInfo.particles_.begin(), pInfo.particles_.end(),
                           [](const BrickParticle& p){ return p.age > p.lifetime; }),
            pInfo.particles_.end()
    );

    if (pInfo.particles_.empty()) pInfo.particlesActive_ = false;
}

void Entity_Brick::renderParticles(SDL_Renderer* r) {
    auto& P = particleInfo;
    if (!P.particlesActive_) return;

    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);

    for (const auto& p : P.particles_) {
        float t = std::clamp(p.age / p.lifetime, 0.f, 1.f);

        // curve morbide per fade e shrink
        float ease = std::pow(1.f - t, 1.6f);
        Uint8 a = static_cast<Uint8>(p.a0 * ease + p.a1 * (1.f - ease));

        float size = p.size0 * ease + p.size1 * (1.f - ease);
        int w = std::max(1, (int)std::round(size));
        int h = w;

        SDL_SetRenderDrawColor(r, p.col.r, p.col.g, p.col.b, a);
        SDL_Rect dst{
                (int)std::round(p.pos.x - w * 0.5f),
                (int)std::round(p.pos.y - h * 0.5f),
                w, h
        };
        SDL_RenderFillRect(r, &dst);
    }
}

//
// Created by Aristide Sessa on 28/08/25.
//

#ifndef BREAKOUT_STARFIELD_HPP
#define BREAKOUT_STARFIELD_HPP

#include <SDL.h>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>
#include "Game.hpp"

class Starfield {
public:
    //
    struct Params {
        int width = 720;
        int height = 520;
        int stars = 200;        // totale
        int layers = 3;         // profondità/parallax
        unsigned int seed = 1337;
        float baseSpeed = 18.f; // px/s per layer più lontano
        float jitter = 1.2f;    // accelerazione casuale px/s^2
        bool wrap = true;       // riapparizione lato opposto
    };

    explicit Starfield(const Params& p) { init(p); }

    void init(const Params& p) {
        params = p;
        dist01 = std::uniform_real_distribution<float>(0.f, 1.f);
        halfW = params.width * 0.5f;
        halfH = params.height * 0.5f;
        stars.clear();
        stars.reserve(params.stars);

        for (int i = 0; i < params.stars; ++i) {
            int layer = i % std::max(1, params.layers); // distribuzione uniforme
            float depth = 1.f + layer;                  // 1..layers
            float speed = params.baseSpeed * depth;     // più “vicino” = più veloce
            Uint8 b = (Uint8)std::clamp(140 + 30 * layer + (int)(dist01(Game::rng) * 30), 0, 255);

            Star s;
            // POSIZIONE IN SPAZIO LOCALE CENTRATO: x∈[-halfW, halfW), y∈[-halfH, halfH)
            s.x = lerp(-halfW, halfW, dist01(Game::rng));
            s.y = lerp(-halfH, halfH, dist01(Game::rng));
            // direzione iniziale quasi verticale, con piccola variazione
            float ang = -M_PI_2 + (dist01(Game::rng) - 0.5f) * 0.6f;
            s.vx = std::cos(ang) * speed * 0.2f; // parallax: orizzontale leggero
            s.vy = std::sin(ang) * speed;
            s.maxSpeed = speed;
            s.brightness = b;
            s.size = 1 + (layer > 0 ? 1 : 0) + (layer > 1 ? (dist01(Game::rng) < 0.25f) : 0); // 1..3
            s.layer = layer;
            s.jitterTimer = 0.f;
            stars.push_back(s);
        }
        updateViewportScreenRect();
    }

    // Cambia dimensioni *logiche* del rettangolo centrato
    void setViewportSize(int w, int h) {
        params.width = std::max(1, w);
        params.height = std::max(1, h);
        halfW = params.width * 0.5f;
        halfH = params.height * 0.5f;
        updateViewportScreenRect();
        for (auto& s : stars) {
            clampLocal(s.x, -halfW, halfW);
            clampLocal(s.y, -halfH, halfH);
        }
    }

    // Chiamala quando la finestra cambia size o prima del render, se la prendi dal Game
    void updateViewportScreenRect() {
        const int winW = Game::getInstance().Width();
        const int winH = Game::getInstance().Height();
        // rettangolo centrato sullo schermo
        screenRect.x = (winW - params.width) / 2;
        screenRect.y = (winH - params.height) / 2;
        screenRect.w = params.width;
        screenRect.h = params.height;
        // centro schermo per trasformare da spazio locale centrato a pixel
        screenCX = screenRect.x + screenRect.w * 0.5f;
        screenCY = screenRect.y + screenRect.h * 0.5f;
    }

    void resize(int w, int h) {
        params.width = w; params.height = h;
        for (auto& s : stars) { // ricolloca se fuori
            s.x = std::fmod(std::max(0.f, s.x), (float)w);
            s.y = std::fmod(std::max(0.f, s.y), (float)h);
        }
    }

    void setSeed(unsigned int seed) {
        params.seed = seed;
        init(params);
    }

    void update(float dt) {
        const float jitter = params.jitter;
        for (auto& s : stars) {
            s.jitterTimer += dt;
            if (s.jitterTimer > 0.02f) {
                s.jitterTimer = 0.f;
                float jx = (dist01(Game::rng) - 0.5f) * jitter;
                float jy = (dist01(Game::rng) - 0.5f) * jitter * 1.6f;
                s.vx += jx;
                s.vy += jy;

                float sp = std::sqrt(s.vx*s.vx + s.vy*s.vy);
                if (sp > s.maxSpeed) {
                    float k = s.maxSpeed / std::max(sp, 1e-6f);
                    s.vx *= k; s.vy *= k;
                }
                if (sp < 0.25f * s.maxSpeed) {
                    float ang = -M_PI_2 + (dist01(Game::rng) - 0.5f) * 0.3f;
                    s.vx += std::cos(ang) * 0.35f * s.maxSpeed;
                    s.vy += std::sin(ang) * 0.35f * s.maxSpeed;
                }
            }

            s.x += s.vx * dt;
            s.y += s.vy * dt;

            // wrapping/clamp nello spazio locale [-halfW,halfW) x [-halfH,halfH)
            if (params.wrap) {
                const float w2 = 2.f * halfW;
                const float h2 = 2.f * halfH;
                if (s.x < -halfW) s.x += w2;
                if (s.x >=  halfW) s.x -= w2;
                if (s.y < -halfH) s.y += h2;
                if (s.y >=  halfH) s.y -= h2;
            } else {
                if (s.x < -halfW || s.x >= halfW || s.y < -halfH || s.y >= halfH) {
                    s.x = lerp(-halfW, halfW, dist01(Game::rng));
                    s.y = -halfH; // rientra dall’alto locale
                }
            }
        }
    }

    void render(SDL_Renderer* r) const {
        SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
        for (const auto& s : stars) {
            const int sx = (int)std::lround(screenCX + s.x);
            const int sy = (int)std::lround(screenCY + s.y);

            SDL_SetRenderDrawColor(r, s.brightness, s.brightness, s.brightness, 230);
            if (s.size <= 1) {
                SDL_RenderDrawPoint(r, sx, sy);
            } else {
                SDL_Rect px{ sx, sy, s.size, s.size };
                SDL_RenderFillRect(r, &px);
                if (s.layer >= 2) {
                    SDL_SetRenderDrawColor(r, s.brightness, s.brightness, s.brightness, 120);
                    SDL_RenderDrawLine(r, sx, sy - 1, sx, sy + s.size + 1);
                }
            }
        }
    }

private:
    struct Star {
        float x, y;
        float vx, vy;
        float maxSpeed;
        Uint8 brightness;
        int size;
        int layer;
        float jitterTimer;
    };

    Params params;
    std::vector<Star> stars;
    std::uniform_real_distribution<float> dist01{0.f, 1.f};

    float halfW = 320.f;
    float halfH = 240.f;

    SDL_Rect screenRect{0,0,0,0};
    float screenCX = 0.f, screenCY = 0.f;

    static float lerp(float a, float b, float t) { return a + (b - a) * t; }

    static void wrapCoord(float& v, float max) {
        if (v < 0.f) v += max;
        if (v >= max) v -= max;
    }

    static void clampLocal(float& v, float min, float max) {
        if (v < min) v = min;
        if (v >= max) v = std::nextafter(max, min);
    }
};

#endif //BREAKOUT_STARFIELD_HPP

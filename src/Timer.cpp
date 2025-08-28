//
// Created by Aristide Sessa on 26/08/25.
//

#include "Timer.hpp"

void Timer::start(uint64_t id, float durationSeconds, bool repeat) {
    id = id;
    duration = durationSeconds;
    elapsed = 0.f;
    running = durationSeconds > 0.f;
    paused = false;
    repeat = repeat;
    if (!running) {
        // trigger immediato
        if (onEnd) onEnd(id);
    }
}

void Timer::stop() {
    running = false;
    paused = false;
    elapsed = 0.f;
    onEnd = nullptr;
}

void Timer::end() {
    running = false;
    paused = false;
    elapsed = duration;
    if (onEnd) onEnd(id);
    onEnd = nullptr;
}

void Timer::pause() {
    if (running) paused = true;
}

void Timer::resume() {
    if (running) paused = false;
}

void Timer::update(float dt) {
    if (!running || paused) return;
    elapsed += dt;
    if (elapsed >= duration) {
        if (onEnd) onEnd(id);
        if (repeat) {
            elapsed = std::fmod(elapsed, duration);
        } else {
            running = false;
            onEnd = nullptr; // prevent multiple calls
        }
    }
}
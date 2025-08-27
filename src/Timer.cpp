//
// Created by Aristide Sessa on 26/08/25.
//

#include "Timer.hpp"

void Timer::start(uint64_t id, float durationSeconds, bool repeat) {
    id_ = id;
    duration_ = durationSeconds;
    elapsed_ = 0.f;
    running_ = durationSeconds > 0.f;
    paused_ = false;
    repeat_ = repeat;
    if (!running_) {
        // trigger immediato
        if (onEnd) onEnd(id_);
    }
}

void Timer::stop() {
    running_ = false;
    paused_ = false;
    elapsed_ = 0.f;
}

void Timer::end() {
    running_ = false;
    paused_ = false;
    elapsed_ = duration_;
    if (onEnd) onEnd(id_);
}

void Timer::pause() {
    if (running_) paused_ = true;
}

void Timer::resume() {
    if (running_) paused_ = false;
}

void Timer::update(float dt) {
    if (!running_ || paused_) return;
    elapsed_ += dt;
    if (elapsed_ >= duration_) {
        if (onEnd) onEnd(id_);
        if (repeat_) {
            elapsed_ = std::fmod(elapsed_, duration_);
        } else {
            running_ = false;
        }
    }
}
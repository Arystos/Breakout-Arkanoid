//
// Created by Aristide Sessa on 26/08/25.
//

#include "TimerManager.hpp"
#include "iostream"

uint64_t TimerManager::create(float duration, bool repeat, const std::string& tag,
                              std::any payload, Timer::EndCallback onEnd) {
    Timer t;
    uint64_t id = nextId_++;
    t.payload = std::move(payload);
    t.tag = tag;
    t.onEnd = onEnd;
    t.start(id, duration, repeat);
    timers_.push_back(std::move(t));
    return id;
}

void TimerManager::update(float dt) {
    for (auto &t : timers_) t.update(dt);
    flush(); // remove finished timers without onEnd callback
}

bool TimerManager::stop(uint64_t id) {
    if (auto* t = findTimer(id)) { t->stop(); return true; }
    return false;
}

bool TimerManager::isRunning(uint64_t id) const {
    for (auto &t : timers_) if (t.getId() == id) return t.isRunning();
    return false;
}

std::optional<std::string> TimerManager::getTag(uint64_t id) const {
    for (auto &t : timers_) if (t.getId() == id) return t.tag;
    return std::nullopt;
}

std::optional<std::any> TimerManager::getPayload(uint64_t id) const {
    for (auto &t : timers_) if (t.getId() == id) return t.payload;
    return std::nullopt;
}

Timer* TimerManager::findTimer(uint64_t id) {
    for (auto &t : timers_) if (t.getId() == id) return &t;
    return nullptr;
}

void TimerManager::stopByTag(const char *string, std::any payload) {
    for (auto &t : timers_) {
        if (t.tag == string && t.payload.type() == typeid(std::string)) {
            if (std::any_cast<std::string>(t.payload) == std::any_cast<std::string>(payload)) {
                t.stop();
                if (t.onEnd) t.onEnd(t.getId()); // 
            }
        }
    }
}

void TimerManager::endByTag(const char *string, std::any payload) {
    for (auto &t : timers_) {
        if (t.tag == string && t.payload.type() == typeid(std::string)) {
            if (std::any_cast<std::string>(t.payload) == std::any_cast<std::string>(payload)) {
                t.end();
            }
        }
    }
}

void TimerManager::pauseAll() {
    for (auto &t : timers_) t.pause();
}

void TimerManager::resumeAll() {
    for (auto &t : timers_) t.resume();
}

void TimerManager::stopAll() {
    for (auto &t : timers_) t.stop();
}

void TimerManager::endAll() {
    for (auto &t : timers_) t.end();
}

bool TimerManager::isTagActive(const std::string &tag, std::any payload) const {
    for (const auto &t : timers_) {
        if (t.tag == tag && t.payload.type() == typeid(std::string)) {
            if (std::any_cast<std::string>(t.payload) == std::any_cast<std::string>(payload)) {
                if (t.isRunning()) return true;
            }
        }
    }
    return false;
}

void TimerManager::flush() {
    timers_.erase(std::remove_if(timers_.begin(), timers_.end(),
                                 [](const Timer& t){
                                     return !t.isRunning() && !t.onEnd;
                                 }), timers_.end());
}

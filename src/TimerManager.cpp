//
// Created by Aristide Sessa on 26/08/25.
//

#include "TimerManager.hpp"

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
    // rimuovi i timer non running (non ripetuti) se vuoi
    timers_.erase(std::remove_if(timers_.begin(), timers_.end(),
                                 [](const Timer& x){ return !x.isRunning() && x.onEnd==nullptr; }), timers_.end());
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
            }
        }
    }

}

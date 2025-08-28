//
// Created by Aristide Sessa on 26/08/25.
//

#ifndef BREAKOUT_TIMERMANAGER_HPP
#define BREAKOUT_TIMERMANAGER_HPP

#include "Timer.hpp"
#include <vector>
#include <optional>
#include <cstdint>

class TimerManager {
public:
    // Create a timer with duration in seconds, optional repeat, tag, payload and end callback
    uint64_t create(float duration, bool repeat = false, const std::string& tag = {},
                    std::any payload = {}, Timer::EndCallback onEnd = nullptr);
    // Simple delay function with optional callback
    void delay(float seconds, const std::function<void()>& cb = nullptr) {
        create(seconds, false, {}, {}, [cb](uint64_t){ if (cb) cb(); });
    }

    void update(float dt);
    bool stop(uint64_t id);
    bool isRunning(uint64_t id) const;
    std::optional<std::string> getTag(uint64_t id) const;
    std::optional<std::any> getPayload(uint64_t id) const;
    std::vector<Timer> Timers () const { return timers_; }

    void stopByTag(const char *string, std::any payload);
    void endByTag(const char *string, std::any payload); // with onEnd callback

    void pauseAll();
    void resumeAll();
    void stopAll();
    void endAll(); // with onEnd callback
    
    void flush(); // remove all non-running timers without onEnd callback
    
    bool isTagActive(const std::string& tag, std::any payload) const;
    int size() const { return static_cast<int>(timers_.size()); }

private:
    uint64_t nextId_{1};
    std::vector<Timer> timers_;
    Timer* findTimer(uint64_t id);
};


#endif //BREAKOUT_TIMERMANAGER_HPP

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
    uint64_t create(float duration, bool repeat = false, const std::string& tag = {},
                    std::any payload = {}, Timer::EndCallback onEnd = nullptr);
    // Simple delay (no tag, no payload, no callback)
    uint64_t  delay(float duration) {
        return create(duration, false, "delay", {}, nullptr);
    }

    void update(float dt);
    bool stop(uint64_t id);
    bool isRunning(uint64_t id) const;
    std::optional<std::string> getTag(uint64_t id) const;
    std::optional<std::any> getPayload(uint64_t id) const;
    std::vector<Timer> Timers () const { return timers_; }

    void stopByTag(const char *string, std::any payload);

    void pauseAll();
    void resumeAll();
    void stopAll();
    
    bool isTagActive(const std::string& tag, std::any payload) const;

private:
    uint64_t nextId_{1};
    std::vector<Timer> timers_;
    Timer* findTimer(uint64_t id);
};


#endif //BREAKOUT_TIMERMANAGER_HPP

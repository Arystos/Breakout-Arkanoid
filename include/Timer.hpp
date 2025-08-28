//
// Created by Aristide Sessa on 26/08/25.
//

#ifndef BREAKOUT_TIMER_HPP
#define BREAKOUT_TIMER_HPP

#include <functional>
#include <any>
#include <string>

class Timer {
public:
    // Callbacks
    using StartCallback = std::function<void(uint64_t)>;
    using EndCallback = std::function<void(uint64_t)>;
    using TickCallback = std::function<void(float /*Elapsed*/, float /*Remaining*/)>;
    using StopCallback = std::function<void()>;
    
    Timer() = default;

    // Start il timer. duration in seconds. repeat = true (repeat indefinitely)
    void start(uint64_t id, float durationSeconds, bool repeat = false);
    void stop(); // stop without onEnd callback
    void end(); // stop with onEnd callback
    void pause();
    void resume();
    void update(float dt);

    bool isRunning() const { return running; }
    bool isPaused()  const { return paused; }
    float Elapsed()  const { return elapsed; }
    float Remaining()const { return (duration - elapsed) > 0.f ? (duration - elapsed) : 0.f; }

    // metadata
    std::any payload; // custom data 
    std::string tag; // optional tag for grouping timers

    // Callbacks
    StartCallback onStart;
    EndCallback   onEnd;
    TickCallback  onTick;
    StopCallback  onStop;
    
    uint64_t getId() const { return id; }

private:
    uint64_t id{0};
    float duration{0.0f};
    float elapsed{0.0f};
    bool running{false};
    bool paused{false};
    bool repeat{false};
};


#endif //BREAKOUT_TIMER_HPP

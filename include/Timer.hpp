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
    using TickCallback = std::function<void(float /*elapsed*/, float /*remaining*/)>;
    using StopCallback = std::function<void()>;
    
    Timer() = default;

    // Avvia il timer. duration in secondi. repeat = true per ripetere.
    void start(uint64_t id, float durationSeconds, bool repeat = false);
    void stop(); // stop without onEnd callback
    void end(); // stop with onEnd callback
    void pause();
    void resume();
    void update(float dt);

    bool isRunning() const { return running_; }
    bool isPaused()  const { return paused_; }
    float elapsed()  const { return elapsed_; }
    float remaining()const { return (duration_ - elapsed_) > 0.f ? (duration_ - elapsed_) : 0.f; }

    // metadata
    std::any payload;        // dati custom (es. pointer, string, struct)
    std::string tag;        // etichetta leggibile

    // Callbacks
    StartCallback onStart; // chiamato quando il timer parte
    EndCallback   onEnd;   // chiamato quando il timer finisce
    TickCallback  onTick;  // chiamato ogni update con elapsed e remaining
    StopCallback  onStop;  // chiamato quando il timer viene fermato
    
    uint64_t getId() const { return id_; }

private:
    uint64_t id_{0};
    float duration_{0.0f};
    float elapsed_{0.0f};
    bool running_{false};
    bool paused_{false};
    bool repeat_{false};
};


#endif //BREAKOUT_TIMER_HPP

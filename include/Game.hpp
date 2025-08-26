#ifndef BREAKOUT_GAME_HPP
#define BREAKOUT_GAME_HPP

#include <memory>
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include "State.hpp"
#include "UI.hpp"
#include "Timer.hpp"
#include "TimerManager.hpp"


//class State; // Forward declaration

class Game {
public:
    Game();
    ~Game();
    
    // Singleton access
    static Game &getInstance() { static Game instance; return instance; }

#pragma region Initialization and Cleanup
    bool init(const char* title, bool fullscreen);
    void run(); // Main game loop
    void quit(); // Clean up and exit the game
    
    int Width() const { return width; }
    int Height() const { return height; }
#pragma endregion

#pragma region State Management
    void changeState(std::unique_ptr<State> newState); // switch to a new state
    void pushState(std::unique_ptr<State> newState); // push a new state onto the stack (pause current)
    void popState(); // pop the current state off the stack (resume previous)
    State* getCurrentState(); // get a pointer to the current state
#pragma endregion

    SDL_Renderer* getRenderer() const { return renderer; }
    SDL_Window* getWindow() const { return window; }
    bool isRunning() const { return running; }
    
    //AssetManager& assets();
    TTF_Font* uiFont() const { return uiFont_; }
    
    int BallCount() const { return ballCount; }
    int getBallCount() const { return ballCount; }
    int setBallCount(int count) { ballCount = count; return ballCount; }
    int LevelIndex() const { return levelIndex; }
    int IncrementLevelIndex() { return ++levelIndex; }
    int SetLevelIndex(int index) { levelIndex = index; return levelIndex; }

    /*
#pragma region Timer Management
    Uint32 last = SDL_GetTicks();
    std::vector<Timer> timers;
    Timer& createTimer() {
        timers.emplace_back();
        return timers.back();
    }
    
    // Update all timers
    void updateTimers(float dt) {
        for (auto& timer : timers) {
            timer.update(dt);
        }
        // cleanup stopped timers
        timers.erase(std::remove_if(timers.begin(), timers.end(),
                                    [](const Timer &t) { return !t.isRunning(); }), timers.end());
    }
    
    
    
#pragma endregion
     */

    TimerManager timerManager;


private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    std::vector<std::unique_ptr<State>> states = {};
    TTF_Font* uiFont_ = nullptr;
    
    bool running = false;
    int width = 800;  // Default width
    int height = 600; // Default height
    //AssetManager assetManager;
    float FPS = 60.0f; // Frames per second
    int ballCount = 0;
    int levelIndex;
    
    // text rendering
    TTF_Font* font = nullptr;
    struct Label {
        std::string text;
        SDL_Texture *tex = nullptr;
        SDL_Rect dst{0, 0, 0, 0};
    };
    UI::Label fpsLabel;
    
    // color cache
    SDL_Color fpsColor{255, 255, 0, 255};
    
    // UI elements
    UI::Label fpsCounter;
};

#endif //BREAKOUT_GAME_HPP

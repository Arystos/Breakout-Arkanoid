#ifndef BREAKOUT_GAME_HPP
#define BREAKOUT_GAME_HPP

#include <memory>
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <random>
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

    // TimerManager
    TimerManager timerManager;

    // Random number generator
    static inline std::mt19937 rng{std::random_device{}()};
    
    // --- Initialization, Main Loop, and Cleanup ---
    bool init(const char* title, bool fullscreen);
    void run(); // Main game loop
    void quit() { running = false; } // Stop the game loop
    
    // --- State Management ---
    void changeState(std::unique_ptr<State> newState); // switch to a new state (replace current)
    void pushState(std::unique_ptr<State> newState); // push a new state onto the stack (pause current)
    void popState(); // pop the current state off the stack (resume previous)
    State* getCurrentState(); // get a pointer to the current state

    SDL_Renderer* getRenderer() const { return renderer; }
    SDL_Window* getWindow() const { return window; }
    bool isRunning() const { return running; }
    TTF_Font* uiFont() const { return uiFont_; }


    // --- Accessors and Mutators ---
    // Game Window and borders
    int Width() const { return width; }
    int Height() const { return height; }
    int PlayableWidth() const { return playableWidth; }
    int PlayableHeight() const { return playableHeight; }
    int LeftBorder() const { return lBorder; }
    int RightBorder() const { return rBorder; }
    int TopBorder() const { return tBorder; }
    int BottomBorder() const { return bBorder; }
    
    // Balls 
    int BallCount() const { return ballCount; }
    int getBallCount() const { return ballCount; }
    int setBallCount(int count) { ballCount = count; return ballCount; }
    float BallSpeedModifier() const { return ballSpeedModifier; }
    float setBallSpeedModifier(float mod) { ballSpeedModifier = mod; return ballSpeedModifier; }
    int BallBounces() const { return ballBounces; }
    int setBallBounces(int count) { ballBounces = count; return ballBounces; }
    int TotalBallBounces() const { return totalBallBounces; }
    int setTotalBallBounces(int count) { totalBallBounces = count; return totalBallBounces; }
    void resetBallBounces() { totalBallBounces = ballBounces; ballBounces = 0; }
    
    // Active power-ups
    int ActivePowerUps() const { return activePowerUps; }
    int setActivePowerUps(int count) { activePowerUps = count; return activePowerUps; }
    int IncrementActivePowerUps() { return ++activePowerUps; }
    int DecrementActivePowerUps() { return --activePowerUps; }
    
    // Level
    int LevelIndex() const { return levelIndex; }
    int IncrementLevelIndex() { return ++levelIndex; }
    int SetLevelIndex(int index) { levelIndex = index; return levelIndex; }
    
    // Player lives
    int PlayerLives() const { return playerLives; }
    int setPlayerLives(int lives) { playerLives = lives; return playerLives; }
    
    // FPS
    float CurrentFPS() const { return currentFPS; }
    float FPSCap() const { return FPS; }

    // Power-up tags
    std::vector<std::string> activePowerUpTags{
            "powerup_expand_paddle",
            "powerup_shrink_paddle",
            "powerup_slow_ball",
            "powerup_fast_ball",
            "powerup_sticky"
    };


private:
    // -- SDL components ---
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    TTF_Font* uiFont_ = nullptr;

    // --- Game State Management ---
    std::vector<std::unique_ptr<State>> states = {};
    
    // --- Game properties ---
    bool running = false;
    int width = 800;  // Default width
    int height = 600; // Default height
    int playableWidth = 720;
    int playableHeight = 520;
    int lBorder = (width - playableWidth) / 2;
    int rBorder = width - lBorder;
    int tBorder = (height - playableHeight) / 2;
    int bBorder = height - tBorder;
    float FPS = 60.0f; // Frames per second
    float currentFPS = 0.0f;
    int ballCount = 0; // current number of balls in play
    int levelIndex; // current level index
    int ballBounces = 0; // total ball bounces
    int totalBallBounces = 0; // all time ball bounces
    float ballSpeedModifier{1.0f};
    int playerLives{3};
    int activePowerUps = 0;
};

#endif //BREAKOUT_GAME_HPP

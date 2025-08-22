#ifndef BREAKOUT_GAME_HPP
#define BREAKOUT_GAME_HPP

#include <memory>
#include <vector>
#include <SDL.h>
#include <iostream>


class State;

class Game {
public:
    Game();
    ~Game();

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
#pragma endregion

    SDL_Renderer* getRenderer() const { return renderer; }
    SDL_Window* getWindow() const { return window; }
    bool isRunning() const { return running; }
    //AssetManager& assets();

    // Singleton access
    static Game &getInstance() { static Game instance; return instance; }
    // Get the current state
    State* getCurrentState();

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    std::vector<std::unique_ptr<State>> states = {};
    bool running = false;
    int width = 800;  // Default width
    int height = 600; // Default height
    //AssetManager assetManager;
    float FPS = 60.0f; // Frames per second
};

#endif //BREAKOUT_GAME_HPP

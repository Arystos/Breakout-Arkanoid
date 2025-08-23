#include <Game.hpp>
#include "State.hpp"
#include <iostream>

Game::Game() {}

Game::~Game() {
    states.clear(); // Clear all states
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

#pragma region Initialization and Cleanup

bool Game::init(const char *title, bool fullscreen) {
    
    Uint32 flags = SDL_WINDOW_SHOWN;
    if (fullscreen) {
        flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
    if (!window) {
        SDL_Log("Could not create window: %s", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(
            window, -1, SDL_RENDERER_ACCELERATED // Use hardware acceleration
             | SDL_RENDERER_PRESENTVSYNC // Enable VSync
             | SDL_RENDERER_TARGETTEXTURE // Allow rendering to texture
            ); 
    if (!renderer) {
        SDL_Log("Could not create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        return false;
    }

    running = true;
    return running;
}

// Game.cpp (simplified run loop)
void Game::run() {
    std::cout << "Game is running..." << std::endl;
    
    const float targetFrameTime = 1.0f / FPS;
    Uint64 now = SDL_GetPerformanceCounter();
    
    while (running) {
        Uint64 frameStart = SDL_GetPerformanceCounter();
        float dt = (frameStart - now) / static_cast<float>(SDL_GetPerformanceFrequency());
        now = frameStart;
        if (dt > 0.1f) dt = 0.1f; // avoid spiral of death (dt too large)
        
        // Input handling
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) { quit(); break; }
            if (!states.empty()) states.back()->handleInput(*this, event);
        }
        
        // Update (if any state is active)
        if (!states.empty()) states.back()->update(*this, dt);
        
        // render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        if (!states.empty()) states.back()->render(*this);
        SDL_RenderPresent(renderer);
        
        // Frame cap
        float frameTime = (SDL_GetPerformanceCounter() - frameStart) / static_cast<float>(SDL_GetPerformanceFrequency());
        if (frameTime < targetFrameTime) SDL_Delay(static_cast<Uint32>((targetFrameTime - frameTime) * 1000.0f));
        
        // Show current fps as text on top left corner with SDL text
        
        
    }
    
}

void Game::quit() { running = false; }

#pragma endregion

#pragma region State Management
void Game::changeState(std::unique_ptr<State> newState) {
    if (!states.empty()) {
        states.back()->onExit(*this); // Notify onExit
        states.pop_back(); // Remove the current state
    }
    states.push_back(std::move(newState)); // Add the new state
    states.back()->onEnter(*this); // Notify onEnter
}

void Game::pushState(std::unique_ptr<State> newState) {
    states.push_back(std::move(newState)); // Add the new state
    states.back()->onEnter(*this); // Notify onEnter
}

void Game::popState() {
    if (!states.empty()) {
        states.back()->onExit(*this); // Notify onExit
        states.pop_back(); // Remove the current state
    }
    if (states.empty()) quit(); // If no states left, quit the game 
}

State *Game::getCurrentState() {
    if (states.empty()) return nullptr;
    return states.back().get();
}

#pragma endregion
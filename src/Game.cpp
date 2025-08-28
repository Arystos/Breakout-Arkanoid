#include <Game.hpp>
#include "UI.hpp"
#include "State_MainMenu.hpp"
#include <iostream>

Game::Game() = default;

Game::~Game() {
    states.clear(); // Clear all states
    if (!uiFont_) return;
    if (!renderer) return;
    if (!window) return;
    
    TTF_CloseFont(uiFont_); uiFont_ = nullptr;

    // reset render target
    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderFlush(renderer);
    SDL_RenderPresent(renderer);
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;

    // Destroy window and quit SDL
    SDL_DestroyWindow(window); window = nullptr;
    TTF_Quit();
    SDL_Quit();
}

#pragma region Initialization and Cleanup
bool Game::init(const char *title, bool fullscreen) {
    
    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0"); // High-DPI ON

    // create window
    Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI;
    if (fullscreen) flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
    if (!window) { SDL_Log("Could not create window: %s", SDL_GetError()); return false;}

    // create renderer
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
    
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"); // no linear filtering
    
    // reset scale and viewport
    SDL_RenderSetScale(renderer, 1.0f, 1.0f);
    SDL_RenderSetViewport(renderer, nullptr);
    
    // set logical size to handle resolution independence
    SDL_RenderSetLogicalSize(renderer, width, height);
    SDL_RenderSetIntegerScale(renderer, SDL_TRUE); // niente blur

    // Initialize SDL and TTF
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }
    if (TTF_Init() == -1) {
        SDL_Log("TTF_Init failed: %s", TTF_GetError());
        return false;
    }

    // Load a default font for UI
    uiFont_ = TTF_OpenFont("assets/fonts/Roboto-Regular.ttf", 28);
    if (!uiFont_) { SDL_Log("TTF_OpenFont: %s", TTF_GetError()); return false; }

    // Initialization successful
    running = true;
    return running;
}
#pragma endregion

#pragma region Main Game Loop
void Game::run() {
    std::cout << "Game is running..." << std::endl;
    
    // Frame timing and FPS calculation
    const float targetFrameTime = 1.0f / FPS;
    Uint64 now = SDL_GetPerformanceCounter();

    // Main menu state
    Game::getInstance().changeState(std::make_unique<State_MainMenu>());
    
    // Main game loop
    while (running) {
        // Calculate delta time
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
        
        // Calculate current FPS
        currentFPS = 1.0f / dt;

        timerManager.update(dt); // update timers
    }
    
}

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
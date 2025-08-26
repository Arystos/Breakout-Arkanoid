#include <Game.hpp>
#include "UI.hpp"
#include "State_MainMenu.hpp"
#include <iostream>

static Uint64 nowPerf() { return SDL_GetPerformanceCounter(); } // current time in ticks

Game::Game() = default;

Game::~Game() {
    states.clear(); // Clear all states
    if (uiFont_) TTF_CloseFont(uiFont_); uiFont_ = nullptr;
    if (renderer) {
        SDL_SetRenderTarget(renderer, nullptr); // reset target se ne avevi uno
        SDL_RenderFlush(renderer);              // svuota la coda
        SDL_RenderPresent(renderer);            // opzionale, chiude il frame
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window) SDL_DestroyWindow(window); window = nullptr;
    TTF_Quit();
    SDL_Quit();
}

#pragma region Initialization and Cleanup

bool Game::init(const char *title, bool fullscreen) {

    // Set high-DPI support
    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");          // High-DPI ON

    // crea la window CON high-DPI
    Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI;
    if (fullscreen) flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

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

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"); // niente filtro

// reset di stato renderer
    SDL_RenderSetScale(renderer, 1.0f, 1.0f);
    SDL_RenderSetViewport(renderer, nullptr);

// usa una logical size fissa per il layout
    const int designW = 1280;
    const int designH = 720;
    SDL_RenderSetLogicalSize(renderer, width, height);
    SDL_RenderSetIntegerScale(renderer, SDL_TRUE); // niente blur

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

    running = true;
    return running;
}

// Game.cpp (simplified run loop)
void Game::run() {
    std::cout << "Game is running..." << std::endl;
    
    const float targetFrameTime = 1.0f / FPS;
    Uint64 now = SDL_GetPerformanceCounter();

    // Main menu state
    Game::getInstance().changeState(std::make_unique<State_MainMenu>());
    
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

        timerManager.update(dt);
        
        // Update timers
        //updateTimers(dt);
        
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
        
        // TODO: show FPS in top left corner (for debugging)
        auto *renderer = getRenderer();
        if (renderer) {
            std::string fpsText = "FPS: " + std::to_string(static_cast<int>(1.0f / dt));
            UI::BuildLabel(renderer, fpsLabel, fpsText, uiFont_, fpsColor, UI::AlignH::Center);
            UI::DrawLabel(renderer, fpsLabel);
        } 
        
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
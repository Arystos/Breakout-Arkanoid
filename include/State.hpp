#pragma once

#include "Game.hpp"

class Game;

class State  {
public:
    virtual ~State() = default;

#pragma region Lifecycle
    virtual void handleInput(Game& game, const SDL_Event& event) = 0;
    virtual void update(Game& game, float dt) = 0;
    virtual void render(Game& game) = 0;
#pragma endregion

#pragma region Notifiers
    virtual void onEnter(Game& game) {} // Optional, can be overridden by derived states
    virtual void onExit(Game& game) {}
#pragma endregion
};

using StatePtr = std::unique_ptr<State>;
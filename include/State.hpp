#pragma once

//#include "Game.hpp"
//#include "Entity.hpp"
#include <vector>
#include <memory>
#include <SDL.h>

class Game;
class Entity;

class State  {
public:
    virtual ~State() = default;

#pragma region Lifecycle
    virtual void handleInput(Game& game, const SDL_Event& event) = 0;
    virtual void update(Game& game, float dt) = 0;
    virtual void render(Game& game) = 0;
    virtual std::vector<Entity*> getEntities() { return {}; }
    [[nodiscard]] virtual const char* getName() const = 0; // Default name
#pragma endregion

#pragma region Notifiers
    virtual void onEnter(Game& game) {} // Optional, can be overridden by derived states
    virtual void onExit(Game& game) {}
#pragma endregion
};

using StatePtr = std::unique_ptr<State>;
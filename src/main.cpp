#define SDL_MAIN_HANDLED
#include <iostream>
#include <SDL.h>
#include <Game.hpp>
#include "State_MainMenu.hpp"

int main() {
    SDL_SetMainReady();
    Game& game = Game::getInstance();
    if (!game.init("Breakout", false)) return 1;
    game.run();
    return 0;
}

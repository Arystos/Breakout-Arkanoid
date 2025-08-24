#include <iostream>
#include <Game.hpp>
#include "State_MainMenu.hpp"

int main() {
    Game& game = Game::getInstance();
    if (!game.init("Breakout", false)) return 1;
    game.run();
    return 0;
}

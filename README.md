# Breakout-Arkanoid
An arcade game in C++

## Project structure:
```
breakout/
├─ CMakeLists.txt
├─ README.md
├─ assets/
│   ├─ textures/ (paddle.png, ball.png, bricks.png, powerups.png, backgrounds...)
│   ├─ fonts/
│   ├─ sounds/
│   ├─ music/
│   └─ levels/ (text files defining brick layouts)
├─ include/
│   ├─ Game.hpp
│   ├─ State.hpp, MainMenuState.hpp, PlayState.hpp, PauseState.hpp, GameOverState.hpp
│   ├─ Entity.hpp, Paddle.hpp, Ball.hpp, Brick.hpp, PowerUp.hpp
│   ├─ Physics.hpp
│   ├─ AssetManager.hpp
│   ├─ UI.hpp
│   └─ Input.hpp
├─ src/
│   ├─ main.cpp
│   ├─ Game.cpp
│   ├─ State classes (MainMenuState.cpp, PlayState.cpp, etc.)
│   ├─ Entity classes (Paddle.cpp, Ball.cpp, Brick.cpp, PowerUp.cpp)
│   ├─ Physics.cpp
│   ├─ AssetManager.cpp
│   ├─ UI.cpp
│   ├─ Input.cpp
│   └─ Utils.hpp / Utils.cpp
└─ tests/
    └─ (optional) unit tests for collision functions using Catch2 or similar
```

## Build Instructions
1. Ensure you have CMake and a C++ compiler installed.
2. Clone the repository:
   ```bash
   git clone https://github.com/Arystos/Breakout-Arkanoid.git
   cd Breakout-Arkanoid
   ```
3. Create a build directory:
   ```bash
   mkdir build
   cd build
   ```
4. Run CMake to configure the project:
   ```bash
   cmake ..
   ```
5. Build the project:
   ```bash
   cmake --build .
   ```
6. Run the game:
   ```bash
   ./Breakout-Arkanoid
   ```
   
## Features
- Classic Breakout/Arkanoid gameplay with paddle, ball, and bricks.
- Power-ups that enhance gameplay (e.g., paddle size increase, ball speed change).
- Sound effects and background music.
- Simple and intuitive UI for navigating menus and displaying scores.
- Responsive controls for paddle movement.
- Collision detection between ball, paddle, bricks, and power-ups.
- Game states for main menu, playing, paused, and game over.
- Asset management for textures, sounds, and fonts.
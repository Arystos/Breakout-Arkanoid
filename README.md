# Breakout-Arkanoid
An arcade game in C++

# Installation

On windows you can use vcpkg to install the dependencies:
Double click on the `install_dependencies.bat` file to install the dependencies.

After run this command int the terminal
```bash
rm -r -Force build
>> cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE="$PWD/vcpkg/scripts/buildsystems/vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows
>> cmake --build build --config Debugvcpkg integrate install

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

## Inputs Controls
- **Paddle Movement**: Use the left and right arrow keys to move the paddle. 
`Note: The paddle movement is the only one to get the handling input in the update
otherwise the game will not be responsive enough`
- **Pause Game**: Press the `ESC` key to pause or resume the game.
- **Restart Game**: Press the `R` key to restart the game from the main
- **Exit Game**: Press the `Q` key to quit the game.
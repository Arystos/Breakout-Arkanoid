# Breakout-Arkanoid
An arcade game in C++

# Installation

## Windows
Double click on the `setup_deps.bat` file to install the dependencies.
Do not close the process until it finish!
You should see something like this at the end:
``` 
All requested installations completed successfully in: 1.3 s
```

If you close it earlier simply delete the directory named vcpkg and run again the script.

You now should be able to run the project (F5 for debug or Ctrl+F5 for release)

## MacOS
Download the dependencies using homebrew
```bash
brew install sdl2 sdl2_image sdl2_mixer sdl2_ttf
```
## Linux (Ubuntu/Debian) 
Install the dependencies using apt-get
```bash
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev
```
Note: Linux has not been texted yet, if you encounter any issue please open an issue.

## Build Instructions (Cross-Platform)
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
- Simple and intuitive UI for navigating menus and displaying scores.
- Responsive controls for paddle movement.
- Collision detection between ball, paddle, bricks, and power-ups.
- Game states for main menu, playing, paused, and game over.
- Particle effects like ball trail, floating starfield particles and block destruction effects.
- Timer system for managing timed events (e.g., power-up durations).
- Background starfield effect for visual enhancement.
- Multiple levels with increasing difficulty.
- Restart and exit options.
- Cross-platform compatibility (Windows, macOS, Linux to be tested).

## Inputs Controls
- **Paddle Movement**: Use the left and right arrow keys or `A` and `D` to move the paddle. Press the `Space` to release the ball when is stuck on the paddle.
- **Pause Game**: Press the `ESC` key to pause or resume the game. Here you can go to main menu or quit the game.
- **Menu Navigation**: Use the arrow keys to navigate through menu options and `Enter` to select.

## Resources:
- [SDL2](https://www.libsdl.org/) - Simple DirectMedia Layer for graphics, input, and audio.
- [SDL2 Unofficial Doc](https://thenumb.at/cpp-course/index.html#sdl) - SDL2 documentation that I followed
- [CMake](https://cmake.org/) - Cross-platform build system.
- [Starfield](https://web.archive.org/web/20160114180422/http://freespace.virgin.net/hugo.elias/graphics/x_stars.htm) - Background starfield effect.
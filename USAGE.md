# Breakout-Arkanoid Game Documentation

## Overview

This documentation serve aspects a general Overview and documentation for anyone interested in looking into the codebase.
This project is made with SDL2 and glm libraries, see shown to install them in the README.md file

## Game Features

### Core Gameplay
- **Classic Breakout mechanics**: Control a paddle to bounce a ball and destroy bricks
- **Multiple levels**: 5 different level layouts with varying difficulty
- **Power-up system**: 6 different power-ups, see below
- **Lives system**: Player starts with 3 lives
- **State management**: Clean transitions between menu, gameplay, pause, game over, and win states

### Power-ups
The game includes the following power-ups that drop from destroyed bricks:

1. **Expand Paddle**: Makes the paddle larger for easier ball control
2. **Shrink Paddle**: Makes the paddle smaller for increased difficulty
3. **Multi Ball**: Spawns additional balls for faster brick destruction
4. **Slow Ball**: Reduces ball speed for better control
5. **Fast Ball**: Increases ball speed for more challenging gameplay
6. **Sticky Paddle**: Ball sticks to paddle on contact, allowing precise aiming

### Level System
- **5 Custom Levels**: Each level has a unique brick layout
- **Progressive Difficulty**: Later levels feature more complex patterns
- **Brick Types**: Different colored bricks may have different properties
- **Level Files**: Levels are defined in text files using a grid system (0=empty, 1-4=different brick types)

## Technical Architecture

### Core Classes

#### Game Class (Singleton)
- **Purpose**: Central game manager handling initialization, main loop, and global state
- **Key Features**:
  - SDL2 initialization and cleanup
  - State stack management
  - Rendering and event handling
  - Global game properties (window size, player lives, level index, etc.)
  - Timer management system
  - Random number generation

#### State System
The game uses a state machine pattern with the following states:
- **State_MainMenu**: Main menu interface
- **State_Play**: Active gameplay state
- **State_Pause**: Pause overlay
- **State_GameOver**: Game over screen
- **State_Win**: Victory screen

Each state is a child class of the abstract `State` class.

The states are stored in a stack, allowing for easy transitions and overlays.
The most important one is `State_Pause` which is pushed on top of `State_Play` when the game is paused.
This allows the game to resume exactly where it left off, without needing to reinitialize the play state.

This is the code that handles the state stack:
```cpp
states.push(std::move(state));
```

To resume:
```cpp
states.pop();
```

The function `changeState` is used to replace the current state with a new one, and is the only way to change the state without stacking:
```cpp
changeState(std::make_unique<State_Play>()); // Transition to Play state
```

Each State has its own `update`, `render`, and `handleEvent` methods, see the `State` class for more details.

#### Entity System
All game objects inherit from a base `Entity` class:

- **Entity_Paddle**: Player-controlled paddle
  - Position and movement
  - Size modification from power-ups
  - Sticky ball functionality

- **Entity_Ball**: Game ball(s)
  - Physics-based movement
  - Collision detection
  - Speed modifications
  - Bounce mechanics

- **Entity_Brick**: Destructible bricks
  - Health/hit points
  - Color/type variations
  - Power-up drop chances

- **Entity_PowerUp**: Collectible power-ups
  - Falling animation
  - Effect type identification
  - Collision with paddle

Each entity handles its own rendering and updates, promoting modularity.

#### Supporting Systems

- **Physics**: Collision detection and response system
- **UI**: User interface rendering and management
- **Timer/TimerManager**: Timed events and power-up duration management
- **Starfield**: Background visual effects

### File Structure

```
Breakout-Arkanoid/
├── CMakeLists.txt              # Build configuration
├── README.md                   # Basic setup instructions
├── USAGE.mkd                   # This documentation file
├── setup_deps.bat              # Windows dependency installer
├── uninstall_deps.bat          # Windows dependency uninstaller
├── assets/                     # Game assets
│   ├── fonts/
│   │   └── Roboto-Regular.ttf  # UI font
│   ├── grid.txt                # Level layout example
│   └── level1.txt - level5.txt # Level definitions
├── include/                    # Header files
│   ├── Game.hpp                # Main game class
│   ├── State*.hpp              # Game state headers
│   ├── Entity*.hpp             # Game entity headers
│   ├── Physics.hpp             # Physics system
│   ├── UI.hpp                  # User interface
│   ├── Timer*.hpp              # Timer management
│   ├── EffectType.hpp          # Power-up effect enums
│   └── Starfield.hpp           # Background effects
└── src/                        # Source files
    ├── main.cpp                # Application entry point
    ├── Game.cpp                # Game implementation
    ├── State_*.cpp             # State implementations
    ├── Entity_*.cpp            # Entity implementations
    ├── Physics.cpp             # Physics implementation
    ├── UI.cpp                  # UI implementation
    └── Timer*.cpp              # Timer implementations
```

## Level Creation

Levels are defined in text files using a simple grid format:
- `0`: Empty space
- `1-4`: Different brick types
- Grid dimensions: 10 columns × 8 rows

Example level format:
```
0 0 0 0 3 3 0 0 0 0
0 0 0 1 1 1 1 0 0 0
0 0 2 2 2 2 2 2 0 0
0 1 1 1 4 4 1 1 1 0
```

To create custom levels:
1. Create a new `.txt` file in the `assets/` directory
2. Follow the grid format shown above
3. Update the game code in the `State_Play` there is a vector called `levels`

An exemple on how to add a new level:
```cpp
levels.push_back("LevelName", "assets/levelFileName.txt");
```

## Game Configuration

Key game parameters can be modified in the `Game` class:
- Window dimensions
- Playable area borders
- Ball speed and physics
- Power-up effects and durations
- Player starting lives
- FPS cap

Please create getters and setters if you want to modify these parameters at runtime, try to avoid using public variables.

## Development Notes

### Design Patterns Used
- **Singleton Pattern**: Game class for global access
- **State Pattern**: Game state management
- **Entity-Component Pattern**: Game object hierarchy
- **Observer Pattern**: Collision and event handling

### Dependencies
- **SDL2**: Graphics, input, and window management
- **SDL2_ttf**: Text rendering
- **GLM**: Mathematics and vector operations

### Memory Management
- Smart pointers used throughout for automatic memory management
- RAII principles followed for resource management

## Extending the Game

### Adding New Power-ups
Inherit from base `Entity` class
1. Add new effect type to `EffectType.hpp`, give it a enum value and a name
2. Implement effect logic in `Entity_PowerUp.cpp` or in `Entity_Paddle.cpp` if is collected by it (look the cases)
3. Update the function

### Adding New Brick Types
Inherit from base `Entity` class
1. Extend brick type enum in `Entity_Brick.hpp`
2. Modify `Entity_Brick.cpp` to handle new type. Add color in `setColor()`, handle hits in `onCollision()`, you can even setup particles on him
3. Put it in a level file or create a new one
4. Add visual differentiation

### Creating New Game States
1. Inherit from base `State` class
2. Implement required virtual methods (`update`, `render`, `handleEvent`)
3. Register state in game state management system

### Creating UI Text
1. Select the state you want to add text to and create a `UI::Label` object
2. Set its properties (position, size, color, font) (optional)
3. Add the UI witht the method `UI::BuildLabel`
4. Go to the render method of the state and call `UI::Draw()`

If you want to update the text, create a std::string or  another `UI::Label` object with only text
Then call `UI::SetLabelText` and update your label with the text you created
Everytime you modify the text you need to call `UI::SetLabelText` again

Check exemples in the State_Play class with the PowerUp labels

### Timers

##### Create Timers
Use `Game::getInstance().timerManager.create` to create a new timer

Here is an exemple of a timer that last 3 seconds and calls a lambda function when ends
```cpp
uint64_t t = Game::getInstance().timerManager.create(
                                3.0f, false, "tag", {},
                                []() {
                                    // Code to execute when timer ends
                                }
                        );(void)t; // (void)t is to avoid unused variable warning
```
false stands for repeat indefinitely

##### Stop/Pause Timers
You have 2 ways to stop a timer:
1. the function `.end()` which triggers a onEnd function if one is implemeted
2. the function `.stop()` which just stops the timer without triggering the onEnd function

You can also stop timers by their tag with `stopByTag` or `endByTag`
Or you can also stop them all with `stopAll` or `endAll`

Here is an exemple of stopping a timer with the tag "tag"
```cpp
Game::getInstance().timerManager.stopByTag("tag"); // Stops all timers with the tag "tag"
Game::getInstance().timerManager.endAll(); // Ends all timers
```

If you simply want to pause a timer, use the function `.pauseByTag()` and `.resumeByTag()` to resume it
Or you can also pause all timers with `pauseAll()` and `resumeAll()`

Here is an exemple of pausing a timer
```cpp
Game::getInstance().timerManager.get(t).pauseByTag("tag"); // Pauses the timer with the tag "tag"
Game::getInstance().timerManager.get(t).resumeByTag("tag"); // Resumes the timer with the tag "tag"
```

### Physics and Collision
The physics system handles collision detection and response between game entities.

The collision system is based on Axis-Aligned Bounding Boxes (AABB) for simplicity and performance.

You can check for aabbVsAabb collision with the function `Physics::aabbVsAABB`
Or you can check for circleVsAabb collision with the function `Physics::circleVsAabb`

Here is an exemple of use (check ball and paddle as an exemple):
```cpp
Physics::circleVsAABB({position.x + radius, position.y + radius},
                                   radius, entity->getRect(), normal)
```

## Contributing

When contributing to the project:
1. Follow existing code style and conventions
2. Add appropriate comments and documentation
3. Test on multiple platforms when possible
4. Update this documentation for new features
5. Ensure proper memory management and error handling

---

This documentation covers the core aspects of the Breakout-Arkanoid game. 
For additional technical details, refer to the source code comments and header files.

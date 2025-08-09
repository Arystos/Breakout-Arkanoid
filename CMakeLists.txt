cmake_minimum_required(VERSION 3.28)
project(Breakout)

set(CMAKE_CXX_STANDARD 17)
set(BREAKOUT_SRC
        src/main.cpp
        src/Game.cpp
        src/State_MainMenu.cpp
        src/StatePlay.cpp
        include/StatePlay.hpp
        src/State_Pause.cpp
        include/State_Pause.hpp
        src/StateGameOver.cpp
        include/StateGameOver.hpp
)

add_executable(breakout ${BREAKOUT_SRC})

find_package(SDL2 REQUIRED)
# Link SDL2 to the executable

target_include_directories(breakout PRIVATE include)
target_link_libraries(breakout PRIVATE SDL2::SDL2)
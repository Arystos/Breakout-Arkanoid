cmake_minimum_required(VERSION 3.28)
project(Breakout)

set(ASSETS_DIR ${CMAKE_SOURCE_DIR}/assets)

set(CMAKE_CXX_STANDARD 17)
set(BREAKOUT_SRC
        src/main.cpp
        src/Game.cpp
        src/State_MainMenu.cpp
        src/State_Play.cpp
        src/State_Pause.cpp
        src/State_GameOver.cpp
        src/Entity_Paddle.cpp
        include/Entity_Paddle.hpp
        src/Entity_Ball.cpp
        include/Entity_Ball.hpp
        src/EntityBrick.cpp
        include/Entity_Brick.hpp
        src/Entity_PowerUp.cpp
        include/Entity_PowerUp.hpp
        src/Physics.cpp
        include/Physics.hpp
)

add_executable(breakout ${BREAKOUT_SRC})

add_custom_command(TARGET breakout POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:breakout>/assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ASSETS_DIR}
        $<TARGET_FILE_DIR:breakout>/assets
)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)   # <-- aggiunto
find_package(glm CONFIG REQUIRED)

target_include_directories(breakout PRIVATE include)

target_link_libraries(breakout PRIVATE
        SDL2::SDL2
        SDL2_ttf::SDL2_ttf   # <-- aggiunto
        glm::glm-header-only
)
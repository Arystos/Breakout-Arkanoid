cmake_minimum_required(VERSION 3.28)
project(Breakout)

set(ASSETS_DIR ${CMAKE_SOURCE_DIR}/assets)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
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
        src/UI.cpp
        include/UI.hpp
        src/Timer.cpp
        include/Timer.hpp
        src/TimerManager.cpp
        include/TimerManager.hpp
        src/State_Win.cpp
        include/State_Win.hpp
        include/Starfield.hpp
)

add_executable(breakout ${BREAKOUT_SRC})

add_custom_command(TARGET breakout POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:breakout>/assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ASSETS_DIR}
        $<TARGET_FILE_DIR:breakout>/assets
)


# === Non-Windows: mantieni la logica attuale ===
if(NOT WIN32)
    find_package(SDL2 REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(glm CONFIG REQUIRED)

    target_include_directories(breakout PRIVATE include)

    target_link_libraries(breakout PRIVATE
            SDL2::SDL2
            SDL2_ttf::SDL2_ttf
            glm::glm-header-only
    )
else()
    # === Windows: usa pacchetti vcpkg e aggiungi SDL2main ===
    # Richiede la toolchain passata da setup_deps.bat:
    #   -DCMAKE_TOOLCHAIN_FILE="%PROJECT_DIR%/vcpkg/scripts/buildsystems/vcpkg.cmake"
    add_definitions(-DNOMINMAX -D_CRT_SECURE_NO_WARNINGS)
    # Se nel tuo main usi int main(int, char**), spesso serve SDL2main su Windows
    find_package(SDL2 CONFIG REQUIRED)
    find_package(SDL2_ttf CONFIG REQUIRED)
    find_package(glm CONFIG REQUIRED)

    target_include_directories(breakout PRIVATE include)

    # glm su vcpkg espone sia glm::glm che glm::glm-header-only, usane uno qualsiasi
    target_link_libraries(breakout PRIVATE SDL2::SDL2 SDL2_ttf::SDL2_ttf glm::glm)

endif()
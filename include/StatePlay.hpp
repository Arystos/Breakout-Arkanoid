//
// Created by Aristide Sessa on 09/08/25.
//

#ifndef BREAKOUT_STATEPLAY_HPP
#define BREAKOUT_STATEPLAY_HPP


#include "State.hpp"
#include "Entity_Paddle.hpp"
#include "Entity_Ball.hpp"
#include "Entity_Brick.hpp"
#include "UI.hpp"
#include <memory>

class State_Play: public State {
public:
    void handleInput(Game &game, const SDL_Event &event) override;
    void update(Game &game, float deltaTime) override;
    void render(Game &game) override;
    void onEnter(Game &game) override;
    void onExit(Game &game) override;
    [[nodiscard]] const char* getName() const override { return "Play"; }
    
    [[nodiscard]] float getDeltaTime() const { return deltaTime; }
    Entity_Paddle& getPaddle() { return *paddle; }
    Entity_Ball& getBall() { return *ball; }
    std::vector<Entity*> getEntities() override;
    inline int getBrickCount() const { return static_cast<int>(bricks.size()); }
    bool destroyEntity(Entity* e); // remove entity from the state
    std::vector<Entity_Brick> loadLevel (const std::string& file, float offsetX, float offsetY); 

private:
    bool paused = false; // Flag to check if the game is paused
    int score = 0; // Player's score
    float deltaTime = 0.0f; // Time since last update, used for movement calculations
    // TODO: Add paddle, ball, bricks

    std::unique_ptr<Entity_Paddle> paddle = std::make_unique<Entity_Paddle>();
    std::unique_ptr<Entity_Ball> ball = std::make_unique<Entity_Ball>();
    std::vector<Entity_Brick> bricks;
    
    // Text rendering
    TTF_Font* font = nullptr;
    struct Label {
        std::string text;
        SDL_Texture *tex = nullptr;
        SDL_Rect dst{0, 0, 0, 0};
    };
    Label wonLabel{"You Win!"};
    
    // UI
    UI::Label winTitle;
    
    // Cache colors
    // white
    SDL_Color titleColor{240, 240, 240, 255};
    
};


#endif //BREAKOUT_STATEPLAY_HPP

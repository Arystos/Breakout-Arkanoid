//
// Created by Aristide Sessa on 09/08/25.
//

#ifndef BREAKOUT_STATEPLAY_HPP
#define BREAKOUT_STATEPLAY_HPP


#include "State.hpp"
#include "Entity_Paddle.hpp"
#include "Entity_Ball.hpp"
#include "Entity_Brick.hpp"
#include "Entity_PowerUp.hpp"
#include "UI.hpp"
#include "Starfield.hpp"
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
    std::vector<std::unique_ptr<Entity_Ball>>& getBalls() { return balls; }
    std::vector<Entity*> getEntities() override;
    inline int getBrickCount() const { return static_cast<int>(bricks.size()); }
    bool destroyEntity(Entity* e); // remove entity from the state
    static std::vector<std::unique_ptr<Entity_Brick>> loadLevel (const std::string& file, float offsetX, float offsetY);

    std::unique_ptr<Entity_PowerUp> spawnPowerUp(const glm::vec2 &position);
    std::unique_ptr<Entity_Ball> spawnBall();
    
    // get string of power-up label
    std::string PowerUpLabelText () const { return powerUpLabel.text; }
    void SetPowerUpLabelText (const std::string& text) { powerUpLabel.text = text; }
    void SetPowerUpLabelInfoText (const std::string& text) { powerUpLabelInfo.text = text; }
    bool PowerUpLabelVisible() const { return powerUpLabel.visible; }
    
    void SetPowerUpLabelVisible(bool v) { powerUpLabel.visible = v; }
    void SetPowerUpLabelInfoVisible(bool v) { powerUpLabelInfo.visible = v; }

private:
    bool paused = false; // Flag to check if the game is paused
    int score = 0; // Player's score
    float deltaTime = 0.0f; // Time since last update, used for movement calculations

    // Entities
    std::unique_ptr<Entity_Paddle> paddle = std::make_unique<Entity_Paddle>();
    std::vector<std::unique_ptr<Entity_Ball>> balls = {};
    std::vector<std::unique_ptr<Entity_Brick>> bricks = {};
    std::vector<std::unique_ptr<Entity_PowerUp>> powerUps = {};
    
    // Text rendering
    TTF_Font* font = nullptr;
    UI::Label wonLabel{"Level Complete!"};
    UI::Label powerUpLabel{"Power_Up!"};
    UI::Label powerUpLabelInfo{"Power-up collected!"};
    UI::Label messageLabel{"Loading next level..."};
    UI::Label livesLabel{"Lives: 3"};
    
    // UI
    UI::Label winTitle;
    UI::Label powerUpTitle;
    UI::Label powerUpInfoTitle;
    UI::Label messageTitle;
    UI::Label livesTitle;
    
    // Cache colors
    // white
    SDL_Color titleColor{240, 240, 240, 255}; // white
    SDL_Color textColor{200, 200, 200, 255}; // slightly grayer white
    SDL_Color livesColor{255, 255, 0, 255}; // yellow
    
    struct LevelInfo {
        std::string name;
        std::string file;
    };
    std::vector<LevelInfo> levels = {
            {"Grid Test", "assets/grid.txt"}, // dummy level 0
            {"Level 1", "assets/level1.txt"},
            {"Level 2", "assets/level2.txt"},
            {"Level 3", "assets/level3.txt"},
            {"Level 4", "assets/level4.txt"},
            {"Level 5", "assets/level5.txt"}
    };
    
    const int maxLevels = static_cast<int>(levels.size());
};


#endif //BREAKOUT_STATEPLAY_HPP

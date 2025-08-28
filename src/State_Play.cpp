//
// Created by Aristide Sessa on 09/08/25.
//

#include "StatePlay.hpp"
#include "State_Pause.hpp"
#include "StateGameOver.hpp"
#include "State_Win.hpp"
#include "State_MainMenu.hpp"
#include "Starfield.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>

Starfield::Params sp;
Starfield starfield(sp);

void State_Play::onEnter(Game &game) {
    State::onEnter(game);
    std::cout << "Playing State" << std::endl;

    // init paddle
    paddle->active = true;
    
    // init ball
    game.setBallSpeedModifier(1.0f); // reset ball speed modifier
    spawnBall();

    // load level
    game.IncrementLevelIndex();
    std::cout << "Loading level " << game.LevelIndex() << std::endl;
    if (game.LevelIndex() < (int)levels.size()) { // valid level index
        const auto &next = levels[game.LevelIndex()];
        bricks = loadLevel(next.file, 50.0f, 50.0f);
        SDL_SetWindowTitle(game.getWindow(), next.name.c_str()); // window title
    } else if (game.LevelIndex() >= (int)levels.size()) { // no more levels
        game.pushState(std::make_unique<State_Win>());
        game.SetLevelIndex(0); // reset level index for next playthrough
        return;
    } else {
        // error loading level
        std::cerr << "Error: Invalid level index " << game.LevelIndex() << std::endl;
        game.changeState(std::make_unique<State_MainMenu>());
        return;
    }
    
    // Old implementation: load a single level from a text file
    //bricks = loadLevel("assets/grid.txt", 50.0f, 50.0f);
    //std::cout << "Loaded " << bricks.size() << " bricks." << std::endl;
    
    // init UI font
    font = Game::getInstance().uiFont();
    if (!font) {
        SDL_Log("Failed to load font for State_Play");
    }
    
    // init UI labels
    auto* renderer = game.getRenderer();
    // win title
    UI::BuildLabel(renderer, winTitle, wonLabel.text, font, titleColor, UI::AlignH::Center);
    winTitle.dst.x = (game.Width() - winTitle.dst.w) / 2; // center horizontally
    winTitle.dst.y = game.Height() / 3;
    // power-up title
    UI::BuildLabel(renderer, powerUpTitle, powerUpLabel.text, font, titleColor, UI::AlignH::Center);
    powerUpTitle.dst.x = (game.Width() - powerUpTitle.dst.w) / 2; // center horizontally
    powerUpTitle.dst.y = game.Height() / 1.6f;
    // power-up info title
    UI::BuildLabel(renderer, powerUpInfoTitle, powerUpLabelInfo.text, font, titleColor, UI::AlignH::Center);
    powerUpInfoTitle.scale = 0.7f;
    powerUpInfoTitle.dst.x = (game.Width() - powerUpInfoTitle.dst.w) / 2; // center horizontally
    powerUpInfoTitle.dst.y = game.Height() / 1.6f + 20;
    // message title
    UI::BuildLabel(renderer, messageTitle, messageLabel.text, font, textColor, UI::AlignH::Left);
    messageTitle.scale = 0.7f;
    messageTitle.dst.x = (game.Width() - powerUpTitle.dst.w) / 2; // center horizontally
    messageTitle.dst.y = game.Height() / 1.6f;
    // lives title
    UI::BuildLabel(renderer, livesTitle, livesLabel.text, font, livesColor, UI::AlignH::Left);
    livesTitle.dst.x = 50;
    livesTitle.dst.y = 5;

    // init starfield
    starfield.setViewportSize(game.PlayableWidth(), game.PlayableHeight());
}

void State_Play::handleInput(Game &game, const SDL_Event &event) {
    if (event.type == SDL_KEYDOWN)  {
        switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_ESCAPE: // Pause the game
                game.pushState(std::make_unique<State_Pause>());
                break;
            case SDL_SCANCODE_SPACE: // release only one ball
                for (auto& ball : balls) {
                    if (ball->stuckToPaddle) {
                        ball->Release();
                        break;
                    }
                }
                break;
            default:
                break;
        }
    }
}

void State_Play::update(Game &game, float dt) {
    
    // --- Update entities ---
    if (paddle && paddle->active) paddle->update(dt);
    for (auto& ball : balls) {
        if (ball && ball->active) ball->update(dt);
    }
    for (auto& powerUp : powerUps) {
        if (powerUp && powerUp->active) powerUp->update(dt);
    }
    starfield.update(dt);

    // --- Check game conditions ---
    // Game Over
    if (game.PlayerLives() <= 0) {
        game.changeState(std::make_unique<State_GameOver>());
        return;
    }
    // Ball Lost
    if (game.getBallCount() <= 0) {
        // spawn a ball after a delay of 2 seconds
        static float respawnTimer = 0.0f;
        respawnTimer += dt;
        if (respawnTimer > 2.0f) {
            respawnTimer = 0.0f;
            // spawn a new ball
            spawnBall();
        }

    }
    // Win Condition: no bricks or only indestructible bricks left
    if (bricks.empty() ||
        std::all_of(bricks.begin(), bricks.end(), [](const auto& b){
            return b->type == BrickType::Indestructible; })) {
        // stop all balls
        for (auto& ball : balls) {
            // set ball velocity to zero and deactivate
            if (ball && ball->active)
                ball->velocity = {0.0f, 0.0f};
        }
        winTitle.visible = true; // show win title
        // delay a bit and then go to next level or restart
        uint64_t t = game.timerManager.create(
                3.0f, false, "next_level", {},
                [&game](auto &&tag) {
                    (void)tag;
                    // go to next level
                    game.changeState(std::make_unique<State_Play>());
                }
        ); (void)t;

        bricks.clear(); // clear any Remaining indestructible bricks for next level
    }

    // --- Clean up destroyed entities ---
    // balls
    for (auto& ball : balls) {
        if (ball && ball->toBeDestroyed) {
            ball.reset();
        }
    }
    balls.erase(std::remove_if(balls.begin(), balls.end(),
                               [](auto& b){
                                   return b == nullptr; }), balls.end());
    // paddle
    if (paddle && paddle->toBeDestroyed) paddle.reset();
    bricks.erase(std::remove_if(bricks.begin(), bricks.end(),
                                [](auto& b){
        return !b->active; }), bricks.end());
    // power-ups
    powerUps.erase(std::remove_if(powerUps.begin(), powerUps.end(),
                                  [](auto& p){
        return !p->active; }), powerUps.end());
}

void State_Play::render(Game &game) {
    SDL_Renderer* renderer = game.getRenderer();
    
    // Playable area box
    SDL_Rect box{40, 40, game.PlayableWidth(), game.PlayableHeight() };
    SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
    SDL_RenderDrawRect(renderer, &box);
    // starfield background
    starfield.render(renderer);

    // Win title (skip rendering other entities)
    if (winTitle.visible) {
        UI::DrawLabel(renderer, winTitle);
        UI::DrawLabel(renderer, messageTitle);
        return; // skip rendering other entities
    }
    
    // --- Render UI ---
    // Power-up title
    if (powerUpLabel.visible) {
        UI::SetLabelText(renderer, powerUpTitle, powerUpLabel.text, titleColor);
        powerUpTitle.dst.x = (game.Width() - powerUpTitle.dst.w) / 2;
        powerUpTitle.dst.y = game.Height() / 1.6f;
        UI::DrawLabel(renderer, powerUpTitle);
    }
    // Power-up info
    if (powerUpLabelInfo.visible) {
        UI::SetLabelText(renderer, powerUpInfoTitle, powerUpLabelInfo.text, textColor);
        powerUpInfoTitle.dst.x = (game.Width() - powerUpInfoTitle.dst.w) / 2;
        powerUpInfoTitle.dst.y = game.Height() / 1.6f + 40; // below power-up title
        UI::DrawLabel(renderer, powerUpInfoTitle);
    }
    // Lives
    if (game.PlayerLives() > 0) {
        UI::SetLabelText(renderer, livesTitle, "Lives: " + std::to_string(game.PlayerLives()), livesColor);
        livesTitle.dst.x = 50;
        livesTitle.dst.y = 5;
        UI::DrawLabel(renderer, livesTitle);
    }
    
    // --- Render entities ---
    // power-ups
    for (auto& powerUp : powerUps) {
        if (powerUp->active) powerUp->render(renderer);
    }
    // bricks
    for (auto& brick : bricks) {
        if (brick->active) brick->render(renderer);
    }
    // ball
    for (auto& ball : balls) {
        if (ball && ball->active) ball->render(renderer);
    }
    // paddle 
    if (paddle && paddle->active) paddle->render(renderer);
    
}

std::vector<Entity *> State_Play::getEntities() {
    std::vector<Entity*> entities;
    if (paddle && paddle->active) entities.push_back(paddle.get());
    for (auto& ball : balls) {
        if (ball && ball->active) entities.push_back(ball.get());
    }
    for (auto& brick : bricks) {
        if (brick->active) {
            entities.push_back(brick.get());
        }
    }
    for (auto& powerUp : powerUps) {
        if (powerUp->active) {
            entities.push_back(powerUp.get());
        }
    }
    
    return entities;
}

bool State_Play::destroyEntity(Entity *e) {
    if (e == (paddle ? paddle.get() : nullptr)) { paddle.reset(); return true; }
    for (auto it = balls.begin(); it != balls.end(); ++it) {
        if (e == it->get()) { balls.erase(it); return true; }
    }
    for (auto it = bricks.begin(); it != bricks.end(); ++it) {
        if (e == it->get()) { bricks.erase(it); return true; }
    }
    SDL_Log("State_Play::destroyEntity: Entity not found.");
    
    return false;
}

std::vector<std::unique_ptr<Entity_Brick>>
State_Play::loadLevel(const std::string &file, float offsetX, float offsetY) {
    // Open the level file
    std::vector<std::unique_ptr<Entity_Brick>> bricks;
    std::ifstream in(file);
    if (!in) {
        SDL_Log("Failed to open level file: %s", file.c_str());
        return {};
    }
    std::string line;
    int row = 0;
    while (std::getline(in, line)) {
        std::istringstream iss(line);
        int val, col = 0;
        while (iss >> val) {
            if (val == 0) { col++; continue; } // 0 means empty space
            // Emplace bricks
            auto& brick = *bricks.emplace_back(std::make_unique<Entity_Brick>()).get();
            brick.position = {
                    offsetX + float(col) * (brick.size.x + 10),
                    offsetY + float(row) * (brick.size.y + 10)
            };
            brick.type = (val == 1) ? BrickType::Normal :
                     (val == 2) ? BrickType::Strong :
                     (val == 3) ? BrickType::Indestructible :
                                  BrickType::Mystery;
            brick.health = (brick.type == BrickType::Strong) ? 2 : 1;
            brick.dropPowerUp = (brick.type == BrickType::Mystery);
            brick.setColor();
            
            col++;
        }
        row++;
    }
    
    return bricks;
}

void State_Play::onExit(Game &game) {
    State::onExit(game);
    // stop all timers
    game.timerManager.stopAll();
    // clear all entities
    paddle.reset();
    balls.clear();
    bricks.clear();
    powerUps.clear();
    // reset UI
    winTitle.visible = false;
    powerUpTitle.visible = false;
    powerUpLabel.visible = false;
    // reset ball count
    game.setBallCount(0);
}

std::unique_ptr<Entity_PowerUp> 
        State_Play::spawnPowerUp(const glm::vec2 &position) {
    // Create a new power-up entity at the given position
    auto powerUp = std::make_unique<Entity_PowerUp>();
    powerUp->position = position - powerUp->size / 2.0f; // center the power-up
    powerUps.push_back(std::move(powerUp));
    return powerUp;
}

std::unique_ptr<Entity_Ball> 
        State_Play::spawnBall() {
    auto ball = std::make_unique<Entity_Ball>();
    ball->position = {
            paddle->position.x + (paddle->size.x - ball->Size()) / 2.0f,
            paddle->position.y - ball->Size() - 1.0f
    };
    ball->attachToCentre(*paddle);
    // detach from paddle and set initial velocity
    ball->stuckToPaddle = false;
    // random initial direction (left or right)
    std::uniform_int_distribution<int> dist(0, 1);
    int dir = dist(Game::rng) == 0 ? -1 : 1;
    ball->velocity = {dir * 300.0f, -300.0f};
    
    ball->active = true;
    
    // add to balls vector
    balls.push_back(std::move(ball));
    Game::getInstance().setBallCount(static_cast<int>(balls.size()));
    return ball;
}

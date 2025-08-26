//
// Created by Aristide Sessa on 09/08/25.
//

#include "StatePlay.hpp"
#include "State_Pause.hpp"
#include "StateGameOver.hpp"
#include "State_Win.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

void State_Play::onEnter(Game &game) {
    // init level, paddle, ball etc. later
    std::cout << "Playing State" << std::endl;
    
    State::onEnter(game);

    // acttivate the paddle
    paddle->active = true;
    // spawn a ball
    spawnBall();

    // load level
    game.IncrementLevelIndex();
    std::cout << "Loading level " << game.LevelIndex() << std::endl;
    if (game.LevelIndex() < (int)levels.size()) {
        const auto &next = levels[game.LevelIndex()];
        bricks = loadLevel(next.file, 50.0f, 50.0f);
        // Set the window title to the level name
        SDL_SetWindowTitle(game.getWindow(), next.name.c_str());
    } else if (game.LevelIndex() >= (int)levels.size()) {
        game.pushState(std::make_unique<State_Win>());
        game.SetLevelIndex(0); // reset level index for next playthrough
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
    UI::BuildLabel(renderer, winTitle, wonLabel.text, font, titleColor, UI::AlignH::Center);
    winTitle.dst.w *= 2;
    winTitle.dst.h *= 2;
    winTitle.dst.x = (game.Width() - winTitle.dst.w) / 2; // center horizontally
    winTitle.dst.y = game.Height() / 3;
    
    UI::BuildLabel(renderer, powerUpTitle, powerUpLabel.text, font, titleColor, UI::AlignH::Center);
    powerUpTitle.dst.w *= 1.5;
    powerUpTitle.dst.h *= 1.5;
    powerUpTitle.dst.x = (game.Width() - powerUpTitle.dst.w) / 2; // center horizontally
    powerUpTitle.dst.y = game.Height() / 1.6f;
    
    UI::BuildLabel(renderer, messageTitle, messageLabel.text, font, textColor, UI::AlignH::Center);
    messageTitle.dst.w *= 1.2;
    messageTitle.dst.h *= 1.2;
    messageTitle.dst.x = (game.Width() - messageTitle.dst.w) / 2; // center horizontally
    messageTitle.dst.y = game.Height() / 1.2f;
}

void State_Play::handleInput(Game &game, const SDL_Event &event) {
    // Handle input events for the play state
    if (event.type == SDL_KEYDOWN)  {
        switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_ESCAPE:
                // Pause the game
                game.pushState(std::make_unique<State_Pause>());
                break;
            case SDL_SCANCODE_SPACE:
                for (auto& ball : balls) {
                    if (ball->stuckToPaddle) {
                        ball->Release();
                        break; // release only one ball
                    }
                }
                game.pushState(std::make_unique<State_Play>());
                break;
                // TODO: Improove paddle smothness
            default:
                break;
        }
    }
}

void State_Play::update(Game &game, float dt) {
    (void)game; (void)dt; // Avoid unused parameter warnings
    //deltaTime = dt;
    
    // Update paddle, ball etc.
    if (paddle && paddle->active) paddle->update(dt);
    for (auto& ball : balls) {
        if (ball && ball->active) ball->update(dt);
    }
    for (auto& powerUp : powerUps) {
        if (powerUp && powerUp->active) powerUp->update(dt);
    }

    // cleanup inactive entities
    for (auto& ball : balls) {
        if (ball && ball->toBeDestroyed) {
            ball.reset();
        }
    }
    // flush destroyed entities
    balls.erase(std::remove_if(balls.begin(), balls.end(),
                               [](auto& b){
                                   return b == nullptr; }), balls.end());
                                   
    if (paddle && paddle->toBeDestroyed) paddle.reset();
    bricks.erase(std::remove_if(bricks.begin(), bricks.end(),
                                [](auto& b){
        return !b->active; }), bricks.end());
    powerUps.erase(std::remove_if(powerUps.begin(), powerUps.end(),
                                  [](auto& p){
        return !p->active; }), powerUps.end());

    // Lost Condition
    if (game.getBallCount() <= 0) {
        // Game Over
        game.changeState(std::make_unique<State_GameOver>());
        return;
    }
    
    // Win Condition
    // if the bricks are empty or there are only indestructible bricks left
    if (bricks.empty() ||
        std::all_of(bricks.begin(), bricks.end(), [](const auto& b){ 
            return b->type == BrickType::Indestructible; })) 
    {
        for (auto& ball : balls) {
            // set ball velocity to zero and deactivate
            if (ball && ball->active)
                ball->velocity = {0.0f, 0.0f};
        }
        winTitle.visible = true;
        // delay a bit and then go to next level or restart
        static float winTimer = 0.0f;
        winTimer += dt;
        if (winTimer > 3.0f) {
            winTimer = 0.0f;
            // go to next level
            game.changeState(std::make_unique<State_Play>());
        }
        // 
        bricks.clear(); // clear any remaining indestructible bricks for next level
    }
    
    
    if (bricks.empty()) {
        
    }
}

void State_Play::render(Game &game) {
    SDL_Renderer* renderer = game.getRenderer();
    
    // TODO: placeholder: draw a playfield box and title
    SDL_Rect box{40, 40, game.Width() - 80, game.Height() - 80 };
    SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
    SDL_RenderDrawRect(renderer, &box);

    // win screen
    if (winTitle.visible) {
        UI::DrawLabel(renderer, winTitle);
        UI::DrawLabel(renderer, messageTitle);
        return; // skip rendering other entities
    }

    // Power-up title
    if (powerUpLabel.visible) {
    UI::BuildLabel(renderer, powerUpTitle, powerUpLabel.text, font, titleColor, UI::AlignH::Center);
    powerUpTitle.dst.w *= 1.5;
    powerUpTitle.dst.h *= 1.5;
    powerUpTitle.dst.x = (game.Width() - powerUpTitle.dst.w) / 2; // center horizontally
    powerUpTitle.dst.y = game.Height() / 1.6f;
    UI::DrawLabel(renderer, powerUpTitle);
    }

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
    // TODO check colors
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
    ball->stuckToPaddle = false;
    ball->active = true;

    balls.push_back(std::move(ball));
    Game::getInstance().setBallCount(static_cast<int>(balls.size()));
    return ball;
}
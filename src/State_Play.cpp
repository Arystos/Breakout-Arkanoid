//
// Created by Aristide Sessa on 09/08/25.
//

#include "StatePlay.hpp"
#include "State_Pause.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

void State_Play::onEnter(Game &game) {
    // init level, paddle, ball etc. later
    std::cout << "Playing State" << std::endl;
    State::onEnter(game);

    // load level
    bricks = loadLevel("assets/grid.txt", 50, 50);
    std::cout << "Loaded " << bricks.size() << " bricks." << std::endl;

    // acttivate the paddle
    paddle->active = true;

    // Place ball on top of the paddle
    //ball.radius = 10.0f;
    ball->attachTo(*paddle);
    ball->stuckToPaddle = false;
    ball->active = true;
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
                // Detach the ball from the paddle if it's stuck
                if (ball->stuckToPaddle) ball->stuckToPaddle = false;
                break;
                // TODO: Improove paddle smothness
            default:
                break;
        }
    }
}

void State_Play::update(Game &game, float dt) {
    (void)game; (void)dt; // Avoid unused parameter warnings
    deltaTime = dt;
    
    // Update paddle, ball etc.
    if (paddle && paddle->active) paddle->update(dt);
    if (ball && ball->active) ball->update(dt);
    for (auto& brick : bricks) {
        if (brick.active) brick.update(dt);
    }

    // cleanup inactive entities
    if (ball && ball->toBeDestroyed) ball.reset();
    if (paddle && paddle->toBeDestroyed) paddle.reset();
    for (auto it = bricks.begin(); it != bricks.end(); ) {
        if (it->toBeDestroyed) {
            it = bricks.erase(it);
        } else {
            ++it;
        }
    }
}

void State_Play::render(Game &game) {
    SDL_Renderer* r = game.getRenderer();
    // placeholder: draw a playfield box
    SDL_Rect box{40, 40, game.Width() - 80, game.Height() - 80 };
    SDL_SetRenderDrawColor(r, 60, 60, 60, 255);
    SDL_RenderDrawRect(r, &box);
    
    // render paddle, ball, bricks etc.
    if (paddle && paddle->active) paddle->render(r);
    if (ball && ball->active) ball->render(r);
    for (auto& brick : bricks) {
        if (brick.active) brick.render(r);
    }
    
}

std::vector<Entity *> State_Play::getEntities() {
    std::vector<Entity*> entities;
    if (paddle && paddle->active) entities.push_back(paddle.get());
    if (ball && ball->active) entities.push_back(ball.get());

    // TODO: Implement bricks
    /*
    for (auto& brick : bricks) {
        if (brick.active) {
            entities.push_back(&brick);
        }
    }
    */
    return entities;
}

bool State_Play::destroyEntity(Entity *e) {
    if (e == (paddle ? paddle.get() : nullptr)) { paddle.reset(); return true; }
    if (e == (ball   ? ball.get()   : nullptr)) { ball.reset();   return true; }
    SDL_Log("State_Play::destroyEntity: Entity not found.");
    return false;
}

std::vector<Entity_Brick>
State_Play::loadLevel(const std::string &file, float offsetX, float offsetY) {
    std::vector<Entity_Brick> bricks;
    std::ifstream in(file);
    // print error if file cannot be opened
    if (!in) {
        SDL_Log("Failed to open level file: %s", file.c_str());
        return bricks;
    }
    std::string line;
    int row = 0;
    while (std::getline(in, line)) {
        std::istringstream iss(line);
        int val, col = 0;
        while (iss >> val) {
            if (val != 0) { // 0 means empty space
                Entity_Brick brick;
                // Calculate position based on row and column
                // TODO: Implement proper spacing and sizing
                float x = offsetX + float(col) * (brick.size.x + 10);
                float y = offsetY + float(row) * (brick.size.y + 10);
                brick.position = {x, y};
                // switch case for different brick types
                switch (val) {
                    case 1:
                        brick.type = BrickType::Normal;
                        brick.health = 1;
                        break;
                    case 2:
                        brick.type = BrickType::Strong;
                        brick.health = 2;
                        break;
                    case 3:
                        brick.type = BrickType::Indestructible;
                        brick.health = -1; // Indestructible
                        break;
                    case 4:
                        brick.type = BrickType::Mystery;
                        brick.health = 1;
                        brick.dropPowerUp = true;
                        break;
                    default:
                        brick.type = BrickType::Normal;
                        brick.health = 1;
                        break;
                }
                brick.setColor();
                bricks.emplace_back(brick);
            }
            col++;
        }
        row++;
    }
    return bricks;
}

void State_Play::onExit(Game &game) {
    State::onExit(game);
}

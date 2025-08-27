//
// Created by Aristide Sessa on 09/08/25.
//

#ifndef BREAKOUT_STATEGAMEOVER_HPP
#define BREAKOUT_STATEGAMEOVER_HPP


#include "State.hpp"
#include "UI.hpp"
#include <iostream>

class State_GameOver : public State {
public:
    //explicit State_GameOver(int score) : score(score) {} // Constructor to initialize the score
    void handleInput(Game &game, const SDL_Event &event) override;
    void update(Game &game, float dt) override;
    void render(Game &game) override;
    void onEnter(Game& game) override;
    void onExit(Game& game) override {};
    const char* getName() const override { return "GameOver"; }
private:
    int score = 0; // Player's score
    
    // Text rendering
    TTF_Font* font = nullptr;
    struct Label {
        std::string text;
        SDL_Texture *tex = nullptr;
        SDL_Rect dst{0, 0, 0, 0};
        SDL_Rect container{0,0,0,0};
    };
    Label gameOverLabel{"Game Over"};
    //Label scoreLabel{"Score: 0"};
    Label instructionLabel{"Press Enter to return to Main Menu"};
    
    // UI
    UI::Label title;
    //UI::Label scoreText;
    UI::Label instruction;
    
    // Cache colors
    SDL_Color titleColor{240, 50, 50, 255}; // red
    SDL_Color textColor{240, 240, 240, 255}; // white
};


#endif //BREAKOUT_STATEGAMEOVER_HPP

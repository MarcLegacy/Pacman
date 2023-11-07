#pragma once

#include "Character.h"
#include "GlobalConstants.h"
#include "Object.h"

class Player : public Character
{
public:
    Player(const sf::Vector2f position, const float speed = 100.0f, const int lives = 3);
    void Move(const float deltaTime) override;
    void PlayerInput();

    void IncreaseScore(const int score) { mScore += score; }
    void LoseLife() { mLives--; }
    void ResetScore() { mScore = 0; }
    void ResetLives() { mLives = PLAYER_LIFE_AMOUNT; }

    int GetScore() const { return mScore; }
    int GetLives() const { return mLives; }



private:
    void LoadTextures();

    int mScore{ 0 };
    int mLives{ 0 };
};







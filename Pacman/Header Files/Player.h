#pragma once

#include "Character.h"
#include "Object.h"

class Player : public Character
{
public:
    Player(const sf::Vector2f position, const float speed = 100.0f);
    void Move(const float deltaTime) override;
    void PlayerInput();
    bool GetOnCellChanged() const { return mOnCellChanged; }

private:
    void CheckCellChanged();

    bool mOnCellChanged{ false };
    sf::Vector2i mCurrentGridPosition{};
};







#pragma once

#include "Object.h"

enum class Direction
{
    Up,
    Down,
    Left,
    Right
};

class Player : public Object
{
public:
    Player(const sf::Vector2f position);

    void Update(float deltaTime) override;
    void Move(float deltaTime);
    void PlayerInput();

private:
    sf::Vector2f GetMoveDirection(const Direction& direction);

    float mSpeed{ 50.0f };
    Direction mCurrentDirection{ Direction::Up };
};


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

    void Update(const float deltaTime) override;
    void Move(const float deltaTime);
    void Draw(sf::RenderTarget* target) override;
    bool MoveToDirection(const Direction& direction);
    void PlayerInput();

private:
    sf::Vector2f GetMoveDirection(const Direction& direction);
    float CalculateDistance(const sf::Vector2f fromPosition, const sf::Vector2f toPosition) const;
    bool IsGoingBack();
    // TODO: Write function that will reset the position of the character when it's position is at a wall, for safety.

    float mSpeed{ 100.0f };
    Direction mCurrentDirection{ Direction::Left }; // The pacman game I played, always made the player start left.
    Direction mDesiredDirection{ mCurrentDirection };
    sf::Vector2f mDestinationPosition{};
};







#pragma once

#include "Object.h"

enum class Direction
{
    Up,
    Down,
    Left,
    Right
};

class Character : public Object
{
public:
    Character(const sf::Vector2f position);

    void Update(const float deltaTime) override;
    void Draw(sf::RenderTarget* target) override;
    virtual void Move(const float deltaTime);



protected:
    float mSpeed{ 100.0f };
    Direction mCurrentDirection{ Direction::Left }; // The pacman game I played, always made the player start left.
    Direction mDesiredDirection{ mCurrentDirection };
    sf::Vector2f mDestinationPosition{};

private:
    bool MoveToDirection(const Direction& direction);
    sf::Vector2f GetMoveDirection(const Direction& direction);

    bool IsGoingBack() const;
    // TODO: Write function that will reset the position of the character when it's position is at a wall, for safety.


};


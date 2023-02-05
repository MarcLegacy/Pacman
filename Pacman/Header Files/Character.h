#pragma once

#include "GlobalEnums.h"
#include "Object.h"

class Character : public Object
{
public:
    Character(const sf::Vector2f position, const float speed = 100.0f);

    void Update(const float deltaTime) override;
    void Draw(sf::RenderTarget* target) override;

protected:
    virtual void Move(const float deltaTime) = 0;
    bool MoveToDirection(const Direction& direction);
    sf::Vector2f GetMoveDirection(const Direction& direction);
    bool IsGoingBack() const;

    const float CELL_REACHED_RADIUS{ 3.0f };

    float mSpeed{ 100.0f };
    Direction mCurrentDirection{ Direction::Left }; // The pacman game I played, always made the player start left.
    Direction mDesiredDirection{ mCurrentDirection };
    sf::Vector2f mDestinationPosition{};

    // TODO: Write function that will reset the position of the character when it's position is at a wall, for safety.
};


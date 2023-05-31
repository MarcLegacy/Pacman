#pragma once

#include "GlobalEnums.h"
#include "Object.h"

class Character : public Object
{
public:
    Character(const sf::Vector2f position, const float speed = 100.0f);

    void Update(const float deltaTime) override;
    void Draw(sf::RenderTarget* target) override;

    // Replacement for an event when the character's current grid position changed.
    bool GetOnCellChanged() const { return mOnCellChanged; }

    Direction GetCurrentDirection() const { return mCurrentDirection; }

protected:
    const float CELL_REACHED_RADIUS{ 3.0f };

    virtual void Move(const float deltaTime) = 0;

    // Check if the passed direction is valid and makes sure the variables for moving towards that direction are set.
    bool MoveToDirection(const Direction& direction);

    // Returns the direction in grid values
    sf::Vector2f GetMoveDirection(const Direction& direction);

    // Checks if the character current direction and desired direction are the exact opposite/
    bool IsGoingBack() const;

    float mSpeed{ 100.0f };
    Direction mCurrentDirection{ Direction::Left }; // The pacman game I played, always made the player start left.
    Direction mDesiredDirection{ mCurrentDirection };
    sf::Vector2f mDestinationWorldPosition{};
    sf::Vector2i mCurrentGridPosition{};
    bool mOnCellChanged{ false };

private:
    void CheckCellChanged();
    void CheckIfOnWall();
    void CheckIfOnTeleport();

    void ShowCurrentGridPosition(sf::RenderTarget* target, const bool show = true) const;
};


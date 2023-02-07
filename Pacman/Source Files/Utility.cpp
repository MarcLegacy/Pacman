#include "Utility.h"

#include "GlobalEnums.h"

Direction Utility::ConvertGridDirectionToDirection(const sf::Vector2i gridDirection)
{
    if (gridDirection == sf::Vector2i(0, -1))
    {
        return Direction::Up;
    }
    if (gridDirection == sf::Vector2i(0, 1))
    {
        return Direction::Down;
    }
    if (gridDirection == sf::Vector2i(-1, 0))
    {
        return Direction::Left;
    }
    if (gridDirection == sf::Vector2i(1, 0))
    {
        return Direction::Right;
    }

    return { Direction::Invalid };
}

sf::Vector2i Utility::ConvertDirectionToGridDirection(const Direction& direction)
{
    switch (direction)
    {
    case Direction::Up:
        return {0, -1};
    case Direction::Down:
        return { 0, 1 };
    case Direction::Left:
        return { -1, 0 };
    case Direction::Right:
        return { 1, 0 };
    default:
        return {};
    }
}

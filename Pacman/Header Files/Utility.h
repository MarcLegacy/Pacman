#pragma once

#include <cmath>
#include <SFML/System/Vector2.hpp>

class Utility
{
public:
    static float Distance(const sf::Vector2f fromPosition, const sf::Vector2f toPosition) { return sqrt(powf(toPosition.x - fromPosition.x, 2) + powf(toPosition.y - fromPosition.y, 2)); }
    static float ManhattanDistance(const sf::Vector2f fromPosition, const sf::Vector2f toPosition) { return abs(fromPosition.x - toPosition.x) + abs(fromPosition.y - toPosition.y); }
    static int ManhattanDistance(const sf::Vector2i fromPosition, const sf::Vector2i toPosition) { return abs(fromPosition.x - toPosition.x) + abs(fromPosition.y - toPosition.y); }
};


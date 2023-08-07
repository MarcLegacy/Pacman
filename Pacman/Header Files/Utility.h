#pragma once

#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <SFML/System/Vector2.hpp>

enum class Direction;

struct Vector2iHasher
{
    std::size_t operator()(const sf::Vector2i& key) const
    {
        // hashes the integers individually and then combines them.
        return std::hash<int>()(key.x) * std::hash<int>()(key.y);   // If something goes wrong, use the caret ('^') as operator.
    }
};

class Utility
{
public:
    static float CalculateDistance(const sf::Vector2f fromPosition, const sf::Vector2f toPosition) { return sqrt(powf(toPosition.x - fromPosition.x, 2) + powf(toPosition.y - fromPosition.y, 2)); }

    static float CalculateManhattanDistance(const sf::Vector2f fromPosition, const sf::Vector2f toPosition) { return abs(fromPosition.x - toPosition.x) + abs(fromPosition.y - toPosition.y); }
    static int CalculateManhattanDistance(const sf::Vector2i fromPosition, const sf::Vector2i toPosition) { return abs(fromPosition.x - toPosition.x) + abs(fromPosition.y - toPosition.y); }

    static Direction ConvertGridDirectionToDirection(const sf::Vector2i gridDirection);
    static sf::Vector2i ConvertDirectionToGridDirection(const Direction& direction);
};

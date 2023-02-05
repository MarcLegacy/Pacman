#pragma once
#include <unordered_map>
#include <vector>
#include <SFML/System/Vector2.hpp>

struct VectorHasher;

class Pathfinder
{
public:
    Pathfinder() = default;
    ~Pathfinder() = default;
    static std::vector<sf::Vector2i> AStar(const sf::Vector2i startGridPosition, const sf::Vector2i targetGridPosition);

private:
    static void DrawCellCosts(const std::unordered_map<sf::Vector2i, int, VectorHasher>& cellCostMap);
};


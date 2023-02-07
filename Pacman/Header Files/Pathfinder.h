#pragma once
#include <unordered_map>
#include <vector>
#include <SFML/System/Vector2.hpp>

#include "Utility.h"


class Pathfinder
{
public:
    Pathfinder();
    ~Pathfinder() = default;

    // Returns the shortest path, weighted means if the cells count other paths as weight.
    std::vector<sf::Vector2i> AStar(const int startX, const int startY, const int targetX, const int targetY, const bool weighted = false) const { return AStar(sf::Vector2i{ startX, startY }, sf::Vector2i{ targetX, targetY }, weighted); }
    // Returns the shortest path, weighted means if the cells count other paths as weight.
    std::vector<sf::Vector2i> AStar(const sf::Vector2i startGridPosition, const sf::Vector2i targetGridPosition, const bool weighted = false) const;
    // Returns the shortest path, weighted means if the cells count other paths as weight.
    std::vector<sf::Vector2i> AStar(const sf::Vector2f startWorldPosition, const sf::Vector2f targetWorldPosition, const bool weighted = false) const;

    // Performs a Breadth-First search and returns the first crossroad cells that are connected to the start grid position
    std::vector<sf::Vector2i> BreadthFirstSearchCrossroadCells(const sf::Vector2i startGridPosition, const std::vector<sf::Vector2i>& crossroadPositions) const;

    // Sets the costs of the cells that are then used as cost inside the AStar function.
    void SetCellCosts(const std::vector<sf::Vector2i>& paths);

private:
    void DrawCellCosts(const std::unordered_map<sf::Vector2i, int, Vector2iHasher>& cellCostMap) const;

    const int EXTRA_WEIGHT{ 50 };

    std::unordered_map<sf::Vector2i, int, Vector2iHasher> mCellCostMap{};
};


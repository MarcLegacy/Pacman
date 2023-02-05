#pragma once
#include <unordered_map>
#include <vector>
#include <SFML/System/Vector2.hpp>

struct VectorHasher;

class Pathfinder
{
public:
    Pathfinder();
    ~Pathfinder() = default;
    std::vector<sf::Vector2i> AStar(const int startX, const int startY, const int targetX, const int targetY) const { return AStar(sf::Vector2i{ startX, startY }, sf::Vector2i{ targetX, targetY }); }
    std::vector<sf::Vector2i> AStar(const sf::Vector2i startGridPosition, const sf::Vector2i targetGridPosition) const;
    std::vector<sf::Vector2i> AStar(const sf::Vector2f startWorldPosition, const sf::Vector2f targetWorldPosition) const;

private:
    void DrawCellCosts(const std::unordered_map<sf::Vector2i, int, VectorHasher>& cellCostMap);
    //std::unordered_map<sf::Vector2i, int, VectorHasher> mCellCostMap{};
};


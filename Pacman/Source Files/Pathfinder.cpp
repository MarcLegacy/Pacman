#include "Pathfinder.h"

#include <iostream>
#include <unordered_map>

#include "DrawDebug.h"
#include "Grid.h"
#include "Pacman.h"
#include "Utility.h"

struct VectorHasher
{
    std::size_t operator()(const sf::Vector2i& key) const
    {
        using std::size_t;
        using std::hash;
        using sf::Vector2i;

        return (hash<int>()(key.x) ^ hash<int>()(key.y) << 1);
    }
};

std::vector<sf::Vector2i> Pathfinder::AStar(const sf::Vector2i startGridPosition, const sf::Vector2i targetGridPosition)
{
    const auto& grid = Pacman::GetGrid();
    std::unordered_map<sf::Vector2i, int, VectorHasher> cellCostMap;
    std::unordered_map<sf::Vector2i, sf::Vector2i, VectorHasher> cameFromMap;
    std::unordered_map<sf::Vector2i, int, VectorHasher> priorityQueue;
    bool targetFound = false;

    cellCostMap[startGridPosition] = 0;
    priorityQueue[startGridPosition] = 0;

    while (!priorityQueue.empty())
    {
        // Get the current lowest cost cell
        int lowestCost = INT_MAX;
        sf::Vector2i lowestGridPosition;
        for (const auto& currentGridPosition : priorityQueue)
        {
            if (currentGridPosition.second < lowestCost)
            {
                lowestCost = currentGridPosition.second;
                lowestGridPosition = currentGridPosition.first;
            }
        }

        if (lowestGridPosition == targetGridPosition)
        {
            targetFound = true;
            break;    // Don't need to calculate the rest if the target has been found.
        }

        priorityQueue.erase(lowestGridPosition);

        // Search neighboring cells that can be travelled to.
        for (const auto& traversableCell : grid->GetTraversableCells(lowestGridPosition))
        {
            sf::Vector2i celGridPosition{ grid->GetCellGridPosition(traversableCell->GetPosition()) };
            if (cellCostMap.find(celGridPosition) == cellCostMap.end()) // avoid cells that are already in the map, this only works when the travel cost is even in cells
            {
                const int cost = lowestCost + 1 + Utility::ManhattanDistance(celGridPosition, targetGridPosition);    // add the cell cost (=1) and the distance to the target to the current cost.

                cellCostMap[celGridPosition] = cost;
                cameFromMap[celGridPosition] = lowestGridPosition;
                priorityQueue[celGridPosition] = cost;
            }
        }
    }

    std::vector<sf::Vector2i> path{};
    sf::Vector2i currentGridPosition{ targetGridPosition };

    if (targetFound)
    {
        // Trace the cells from the target back to the start, which gives us the path.
        while (currentGridPosition != startGridPosition)
        {
            path.push_back(currentGridPosition);
            currentGridPosition = cameFromMap[currentGridPosition];
        }
    }
    else
    {
        std::cout << "No path found!" << std::endl;
    }

    return path;
}

void Pathfinder::DrawCellCosts(const std::unordered_map<sf::Vector2i, int, VectorHasher>& cellCostMap)
{
    const auto& drawDebug = Pacman::GetDrawDebug();

    for (const auto& cellCost : cellCostMap)
    {
        drawDebug->DrawCellCostPersistant(cellCost.second, cellCost.first);
    }
}

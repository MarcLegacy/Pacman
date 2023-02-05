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
        return (std::hash<int>()(key.x) ^ std::hash<int>()(key.y) << 1);
    }
};

Pathfinder::Pathfinder()
{
    //const auto& grid = Pacman::GetGrid();
    //for (const auto& cell : grid->GetTraversableCellMap())
    //{
    //    mCellCostMap[grid->GetCellGridPosition(cell.first->GetPosition())] = 0;
    //}
}

std::vector<sf::Vector2i> Pathfinder::AStar(const sf::Vector2i startGridPosition, const sf::Vector2i targetGridPosition) const
{
    const auto& grid = Pacman::GetGrid();
    std::unordered_map<sf::Vector2i, int, VectorHasher> cellTotalCostMap;
    std::unordered_map<sf::Vector2i, sf::Vector2i, VectorHasher> cameFromMap;
    std::unordered_map<sf::Vector2i, int, VectorHasher> priorityQueue;
    bool targetFound = false;

    cellTotalCostMap[startGridPosition] = 0;
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
            const sf::Vector2i cellGridPosition{ grid->GetCellGridPosition(traversableCell->GetPosition()) };
            if (cellTotalCostMap.find(cellGridPosition) == cellTotalCostMap.end()) // avoid cells that are already in the map, this only works when the travel cost is even in cells
            {
                //const int cellCost = mCellCostMap.at(cellGridPosition);
                const int totalCost = lowestCost + 1 + Utility::ManhattanDistance(cellGridPosition, targetGridPosition);    // add the cell cost (=1) and the distance to the target to the current cost.

                cellTotalCostMap[cellGridPosition] = totalCost;
                cameFromMap[cellGridPosition] = lowestGridPosition;
                priorityQueue[cellGridPosition] = totalCost;
            }
        }
    }

    std::vector<sf::Vector2i> path{};
    sf::Vector2i currentGridPosition{ targetGridPosition };

    if (targetFound)
    {
        // Trace the cells from the target back to the start. Which gives the path backwards, so we reverse it.

        while (currentGridPosition != startGridPosition)
        {
            currentGridPosition = cameFromMap[currentGridPosition];
            path.push_back(currentGridPosition);
        }

        std::reverse(path.begin(), path.end());
    }
    else
    {
        std::cout << "No path found!" << std::endl;
    }

    return path;
}

std::vector<sf::Vector2i> Pathfinder::AStar(const sf::Vector2f startWorldPosition,
    const sf::Vector2f targetWorldPosition) const
{
    const auto& grid = Pacman::GetGrid();
    return AStar(grid->GetCellGridPosition(startWorldPosition), grid->GetCellGridPosition(targetWorldPosition));
}

void Pathfinder::DrawCellCosts(const std::unordered_map<sf::Vector2i, int, VectorHasher>& cellCostMap)
{
    const auto& drawDebug = Pacman::GetDrawDebug();

    for (const auto& cellCost : cellCostMap)
    {
        drawDebug->DrawCellCostPersistant(cellCost.second, cellCost.first);
    }
}

#include "Pathfinder.h"

#include <iostream>
#include <unordered_map>

#include "DrawDebug.h"
#include "Grid.h"
#include "Pacman.h"
#include "Utility.h"



Pathfinder::Pathfinder()
{
    const auto& grid = Pacman::GetGrid();
    for (const auto& cell : grid->GetTraversableCellMap())
    {
        mCellCostMap[grid->GetCellGridPosition(cell.first->GetPosition())] = 1;
    }
}

std::vector<sf::Vector2i> Pathfinder::AStar(const sf::Vector2i startGridPosition, const sf::Vector2i targetGridPosition, const bool weighted) const
{
    const auto& grid = Pacman::GetGrid();
    std::unordered_map<sf::Vector2i, int, Vector2iHasher> cellTotalCostMap;
    std::unordered_map<sf::Vector2i, sf::Vector2i, Vector2iHasher> cameFromMap;
    std::unordered_map<sf::Vector2i, int, Vector2iHasher> priorityQueue;    // TODO: change this to a std::priority_queue.
    bool targetFound = false;

    cellTotalCostMap[startGridPosition] = 0;
    priorityQueue[startGridPosition] = 0;

    while (!priorityQueue.empty())
    {
        // Get the current lowest cost cell
        int currentCost = INT_MAX;
        sf::Vector2i currentGridPosition;
        for (const auto& gridPosition : priorityQueue)
        {
            if (gridPosition.second < currentCost)
            {
                currentCost = gridPosition.second;
                currentGridPosition = gridPosition.first;
            }
        }

        if (currentGridPosition == targetGridPosition)
        {
            targetFound = true;
            break;    // Don't need to calculate the rest if the target has been found.
        }

        priorityQueue.erase(currentGridPosition);

        // Search neighboring cells that can be travelled to.
        for (const auto& traversableCell : grid->GetTraversableCells(currentGridPosition))
        {
            const sf::Vector2i traversableCellGridPosition{ grid->GetCellGridPosition(traversableCell->GetPosition()) };
            if (cellTotalCostMap.find(traversableCellGridPosition) == cellTotalCostMap.end()) // avoid cells that are already in the map, this only works when the travel cost is even in cells
            {
                const int cellCost = weighted ? mCellCostMap.at(traversableCellGridPosition) : 1; // if the function is weighted. then cell cost is based on how many enemies already pathed over the cell.
                const int totalCost = currentCost + cellCost + Utility::ManhattanDistance(traversableCellGridPosition, targetGridPosition); 

                cellTotalCostMap[traversableCellGridPosition] = totalCost;
                cameFromMap[traversableCellGridPosition] = currentGridPosition;
                priorityQueue[traversableCellGridPosition] = totalCost;
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
    const sf::Vector2f targetWorldPosition, const bool weighted) const
{
    const auto& grid = Pacman::GetGrid();
    return AStar(grid->GetCellGridPosition(startWorldPosition), grid->GetCellGridPosition(targetWorldPosition), weighted);
}

void Pathfinder::SetCellCosts(const std::vector<sf::Vector2i>& paths)
{
    for (auto& cellCost : mCellCostMap)
    {
        cellCost.second = 1;

        for (const auto& cell : paths)
        {
            if (cellCost.first == cell)
            {
                cellCost.second = cellCost.second + 20;
            }
        }
    }
}

void Pathfinder::DrawCellCosts(const std::unordered_map<sf::Vector2i, int, Vector2iHasher>& cellCostMap) const
{
    const auto& drawDebug = Pacman::GetDrawDebug();

    for (const auto& cellCost : cellCostMap)
    {
        drawDebug->DrawCellCostPersistant(cellCost.second, cellCost.first);
    }
}

#include "EnemyManager.h"

#include <iostream>
#include <SFML/Graphics/RenderTarget.hpp>

#include "DrawDebug.h"
#include "Enemy.h"
#include "Grid.h"
#include "Pacman.h"

EnemyManager::EnemyManager(std::unique_ptr<Player>& target)
    : mTarget(target)
{
    const auto& grid = Pacman::GetGrid();

    auto enemyBlue = std::make_unique<Enemy>(grid->GetEnemySpawnPosition(0), SkinColor::Blue);
    mEnemies.push_back(std::move(enemyBlue));
    auto enemyRed = std::make_unique<Enemy>(grid->GetEnemySpawnPosition(1), SkinColor::Red);
    mEnemies.push_back(std::move(enemyRed));
    auto enemyOrange = std::make_unique<Enemy>(grid->GetEnemySpawnPosition(2), SkinColor::Orange);
    mEnemies.push_back(std::move(enemyOrange));

    for (const auto& gridPosition : grid->GetCrossroadPositions())
    {
        mTargetDistanceToCrossroadMap[gridPosition] = 0;
    }
}

void EnemyManager::Update(const float deltaTime)
{
    const auto& grid = Pacman::GetGrid();

    for (const auto& enemy : mEnemies)
    {
        enemy->Update(deltaTime);

        // If the enemies are already on their tactical position or the pathing didn't end up at the player's position, find a new path.
        if (grid->GetCellGridPosition(enemy->GetCenterPosition()) == enemy->GetTargetGridPosition() || enemy->GetPath().empty())
        {
            enemy->FindPath(grid->GetCellGridPosition(mTarget->GetCenterPosition()), true);
        }
    }

    if (mTarget->GetOnCellChanged())
    {
        UpdateTargetDistanceToCrossroadMap();
        SurroundTactic();
    }
}

void EnemyManager::Draw(sf::RenderTarget* target)
{
    for (const auto& enemy : mEnemies)
    {
        enemy->Draw(target);
    }

    ShowCrossroadDistanceToTargetCost(target, false);
    ShowClosestCrossroadsToTarget(target, false);
    ShowCrossroads(target, false);
}

void EnemyManager::SurroundTactic()
{
    const auto& pathfinder = Pacman::GetPathfinder();
    const auto& grid = Pacman::GetGrid();

    // This makes sure that we don't try to repath when on a crossroad, as we will get an new target position to go to once the player's left the crossroad immediatelly after.
    if (std::find(grid->GetCrossroadPositions().begin(), grid->GetCrossroadPositions().end(), grid->GetCellGridPosition(mTarget->GetCenterPosition())) != grid->GetCrossroadPositions().end())
    {
        return;
    }


    auto closestCrossroadGridPositions = pathfinder->BreadthFirstSearchCrossroadCells(grid->GetCellGridPosition(mTarget->GetCenterPosition()), grid->GetCrossroadPositions());

    // First check if we need to find a new crossroad to path to
    if (!AreCrossroadsEqual(closestCrossroadGridPositions, mCurrentClosestCrossroadGridPositions))
    {
        mCurrentClosestCrossroadGridPositions = closestCrossroadGridPositions;
        std::vector<float> distancesToTarget{};

        for (const auto& enemy : mEnemies)
        {
            distancesToTarget.push_back(static_cast<float>(pathfinder->AStar(enemy->GetCenterPosition(), mTarget->GetCenterPosition()).size()));
            enemy->ClearPath();
        }

        // Closest enemy is allowed first to go to their closest crossroad.
        const unsigned int closestEnemyIndex = std::distance(std::begin(distancesToTarget), std::min_element(std::begin(distancesToTarget), std::end(distancesToTarget)));
        const unsigned int furthestEnemyIndex = std::distance(std::begin(distancesToTarget), std::max_element(std::begin(distancesToTarget), std::end(distancesToTarget)));
        const unsigned int middleEnemyIndex = mEnemies.size() - closestEnemyIndex - furthestEnemyIndex;

        SetEnemyTargetPosition(closestEnemyIndex, closestCrossroadGridPositions);
        SetEnemyTargetPosition(middleEnemyIndex, closestCrossroadGridPositions);
        SetEnemyTargetPosition(furthestEnemyIndex, closestCrossroadGridPositions);
    }
}

std::vector<sf::Vector2i> EnemyManager::CollectCalculatedPaths() const
{
    std::vector<sf::Vector2i> paths;

    for (const auto& enemy : mEnemies)
    {
        paths.insert(paths.end(), enemy->GetPath().begin(), enemy->GetPath().end());
    }

    return paths;
}

void EnemyManager::UpdateTargetDistanceToCrossroadMap()
{
    const auto& pathfinder = Pacman::GetPathfinder();
    const auto& grid = Pacman::GetGrid();

    for (auto& crossroadDistance : mTargetDistanceToCrossroadMap)
    {
        crossroadDistance.second = static_cast<int>(pathfinder->AStar(crossroadDistance.first, grid->GetCellGridPosition(mTarget->GetPosition())).size());
    }
}

std::vector<sf::Vector2i> EnemyManager::GetShortestGridDistances(const int amount) const
{
    std::vector<sf::Vector2i> lowestDistances{};

    for (int i = 0; i < amount; ++i)
    {
        int lowestDistance = INT_MAX;
        sf::Vector2i lowestGridPosition{};

        for (const auto& crossroadDistance : mTargetDistanceToCrossroadMap)
        {
            if (crossroadDistance.second < lowestDistance && std::find(lowestDistances.begin(), lowestDistances.end(), crossroadDistance.first) == lowestDistances.end())
            {
                lowestDistance = crossroadDistance.second;
                lowestGridPosition = crossroadDistance.first;
            }
        }

        lowestDistances.push_back(lowestGridPosition);
    }

    return lowestDistances;
}

sf::Vector2i EnemyManager::GetClosestGridPosition(const sf::Vector2i currentGridPosition,
    const std::vector<sf::Vector2i>& gridPositions)
{
    int closestDistance{ INT_MAX };
    sf::Vector2i closestGridPosition{};

    for (const auto& gridPosition : gridPositions)
    {
        const int distance = Pacman::GetPathfinder()->AStar(currentGridPosition, gridPosition).size();
        if (distance < closestDistance)
        {
            closestDistance = distance;
            closestGridPosition = gridPosition;
        }
    }

    return closestGridPosition;
}

void EnemyManager::SetEnemyTargetPosition(const unsigned int enemyIndex, std::vector<sf::Vector2i>& closestCrossroadGridPositions)
{
    if (enemyIndex >= mEnemies.size())
    {
        std::cout << "Wrong index given!" << std::endl;
        return;
    }

    const auto& grid = Pacman::GetGrid();

    // If no crossroad is found, then just move to the player position. This will happen mostly to the furthers enemy and when only 2 crossroads are available.
    auto closestGridPosition = GetClosestGridPosition(grid->GetCellGridPosition(mEnemies[enemyIndex]->GetCenterPosition()), closestCrossroadGridPositions);
    if (closestGridPosition == sf::Vector2i{ 0, 0 })
    {
        closestGridPosition = grid->GetCellGridPosition(mTarget->GetCenterPosition());
    }

    // Sets the cell cost to make sure enemies try to avoid taking the same path.
    Pacman::GetPathfinder()->SetCellCosts(CollectCalculatedPaths());

    mEnemies[enemyIndex]->FindPath(closestGridPosition, true);

    // Remove crossroad option so that other enemies will not go to that crossroad.
    auto iterator = std::find(closestCrossroadGridPositions.begin(), closestCrossroadGridPositions.end(), mEnemies[enemyIndex]->GetTargetGridPosition());
    if (iterator != closestCrossroadGridPositions.end())
    {
        closestCrossroadGridPositions.erase(iterator);
    }
}

bool EnemyManager::AreCrossroadsEqual(const std::vector<sf::Vector2i>& crossRoadsPositionsA,
    const std::vector<sf::Vector2i>& crossRoadsPositionsB)
{
    if (crossRoadsPositionsA.size() != crossRoadsPositionsB.size()) return false;

    unsigned int equalAmount{ 0 };
    for (const auto& crossRoadPositionA : crossRoadsPositionsA)
    {
        for (const auto& crossRoadPositionB : crossRoadsPositionsB)
        {
            if (crossRoadPositionA == crossRoadPositionB)
            {
                equalAmount++;
            }
        }
    }

    if (equalAmount == crossRoadsPositionsA.size())
    {
        return true;
    }

    return false;
}

void EnemyManager::ShowCrossroads(sf::RenderTarget* target, const bool show) const
{
    if (!show) return;

    const auto& grid = Pacman::GetGrid();

    for (const auto& gridPosition : grid->GetCrossroadPositions())
    {
        target->draw(Pacman::GetDrawDebug()->DrawCell(grid->GetCellWorldPosition(gridPosition), 32.0f, sf::Color::Red));
    }
}

void EnemyManager::ShowCrossroadDistanceToTargetCost(sf::RenderTarget* target, const bool show) const
{
    if (!show) return;

    for (const auto& crossroadDistance : mTargetDistanceToCrossroadMap)
    {
        target->draw(Pacman::GetDrawDebug()->DrawCellCost(crossroadDistance.second, crossroadDistance.first));
    }
}

void EnemyManager::ShowClosestCrossroadsToTarget(sf::RenderTarget* target, const bool show) const
{
    if (!show) return;

    for (const auto& gridPosition : Pacman::GetPathfinder()->BreadthFirstSearchCrossroadCells(Pacman::GetGrid()->GetCellGridPosition(mTarget->GetCenterPosition()), Pacman::GetGrid()->GetCrossroadPositions()))
    {
        target->draw(Pacman::GetDrawDebug()->DrawCell(Pacman::GetGrid()->GetCellWorldPosition(gridPosition), 32.0f, sf::Color::Green));
    }
}

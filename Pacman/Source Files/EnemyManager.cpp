#include "EnemyManager.h"

#include <iostream>
#include <SFML/Graphics/RenderTarget.hpp>

#include "DrawDebug.h"
#include "Enemy.h"
#include "Grid.h"
#include "Pacman.h"
#include "Utility.h"

EnemyManager::EnemyManager(std::shared_ptr<Player> target)
    : mTarget(std::move(target))
{
    const auto& grid = Pacman::GetGrid();

    const auto enemyBlue = std::make_shared<Enemy>(grid->GetEnemySpawnPosition(0), SkinColor::Blue);
    mEnemies.push_back(enemyBlue);
    const auto enemyRed = std::make_shared<Enemy>(grid->GetEnemySpawnPosition(1), SkinColor::Red);
    mEnemies.push_back(enemyRed);
    const auto enemyOrange = std::make_shared<Enemy>(grid->GetEnemySpawnPosition(2), SkinColor::Orange);
    mEnemies.push_back(enemyOrange);

    for (const auto& gridPosition : grid->GetCrossroadPositions())
    {
        //Pacman::GetDrawDebug()->DrawCellPersistant(grid->GetCellWorldPosition(gridPosition), 32.0f, sf::Color::Red);
        mTargetDistanceToCrossroadMap[gridPosition] = 0;
    }
}

void EnemyManager::Update(float deltaTime)
{
    const auto& grid = Pacman::GetGrid();

    for (const auto& enemy : mEnemies)
    {
        enemy->Update(deltaTime);
    }

    if (mTarget->GetOnCellChanged())
    {
        //OldTactic();
        UpdateTargetDistanceToCrossroadMap();
        NewTactic();
    }

    for (const auto& enemy : mEnemies)
    {
        if (grid->GetCellGridPosition(enemy->GetCenterPosition()) == enemy->GetTargetGridPosition() || enemy->GetPath().empty())
        {
            //const auto& traversableCells = grid->GetTraversableCells(mTarget->GetCenterPosition());
            //const auto& newTargetGridDirection = grid->GetCellGridPosition(mTarget->GetCenterPosition()) + Utility::ConvertDirectionToGridDirection(mTarget->GetCurrentDirection());
            //if (std::find(traversableCells.begin(), traversableCells.end(), grid->GetCell(newTargetGridDirection)) != traversableCells.end())
            //{
            //    enemy->FindPath(newTargetGridDirection, true);
            //}
            //else
            {
                enemy->FindPath(grid->GetCellGridPosition(mTarget->GetCenterPosition()), true);
            }

        }
    }
}

void EnemyManager::Draw(sf::RenderTarget* target)
{
    for (const auto& enemy : mEnemies)
    {
        enemy->Draw(target);
    }

    //for (const auto& crossroadDistance : mTargetDistanceToCrossroadMap)
    //{
    //    target->draw(Pacman::GetDrawDebug()->DrawCellCost(crossroadDistance.second, crossroadDistance.first));
    //}

    //for (const auto& gridPosition : Pacman::GetPathfinder()->BreadthFirstSearchCrossroadCells(Pacman::GetGrid()->GetCellGridPosition(mTarget->GetCenterPosition()), Pacman::GetGrid()->GetCrossroadPositions()))
    //{
    //    target->draw(Pacman::GetDrawDebug()->DrawCell(Pacman::GetGrid()->GetCellWorldPosition(gridPosition), 32.0f, sf::Color::Green));
    //}
}

void EnemyManager::OldTactic() const
{
    const auto& grid = Pacman::GetGrid();

    std::vector<unsigned int> orderRank;
    for (const auto& enemy : mEnemies)
    {
        int rank{};
        for (const auto& enemyToCheck : mEnemies)
        {
            if (enemy == enemyToCheck) continue;

            if (enemy->GetPath().size() > enemyToCheck->GetPath().size())
            {
                rank++;
            }
        }
        orderRank.push_back(rank);
    }

    for (const auto& enemy : mEnemies)
    {
        enemy->ClearPath();
    }

    int totalIndex{};

    for (unsigned int i = 0; i < orderRank.size(); ++i)
    {
        unsigned int index = orderRank[i];
        totalIndex += index;

        mEnemies[index]->FindPath(grid->GetCellGridPosition(mTarget->GetCenterPosition()));

        const int distance = Utility::ManhattanDistance(grid->GetCellGridPosition(mTarget->GetCenterPosition()), grid->GetCellGridPosition(mEnemies[index]->GetCenterPosition()));
        if (distance < 30)
        {
            //std::cout << "Operation: 'Surround Mouse' activated!" << std::endl;
            Pacman::GetPathfinder()->SetCellCosts(CollectCalculatedPaths());
            mEnemies[index]->mIsDoingTactic = true;
        }
        //else if (distance > 15)
        //{
        //    mEnemies[index]->mIsDoingTactic = false;
        //}
    }

    //std::cout << totalIndex << std::endl;
}

void EnemyManager::NewTactic()
{
    const auto& pathfinder = Pacman::GetPathfinder();
    const auto& grid = Pacman::GetGrid();

    if (std::find(grid->GetCrossroadPositions().begin(), grid->GetCrossroadPositions().end(), grid->GetCellGridPosition(mTarget->GetCenterPosition())) != grid->GetCrossroadPositions().end())
    {
        return;
    }

    // Decide which enemy goes to which crossroad.
    auto closestCrossroadGridPositions = pathfinder->BreadthFirstSearchCrossroadCells(grid->GetCellGridPosition(mTarget->GetCenterPosition()), grid->GetCrossroadPositions());

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

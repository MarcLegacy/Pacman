#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include <SFML/System/Vector2.hpp>

#include "Cell.h"

class Cell;

class Grid
{
public:
    Grid(const int width, const int height, const float cellSize, const std::vector<std::string>& levelLayout, const sf::Vector2f originPosition = {});

    void Draw(sf::RenderTarget* target);

    int GetGridWidth() const { return mWidth; }
    int GetGridHeight() const { return mHeight; }
    sf::Vector2i GetGridSize() { return { mWidth, mHeight }; }

    float GetCellSize() const { return mCellSize; }

    std::shared_ptr<Cell> GetCell(const int x, const int y) { return IsCellValid(x, y) ? mGridCells[y][x] : nullptr; }
    std::shared_ptr<Cell> GetCell(const sf::Vector2i gridPosition) { return GetCell(gridPosition.x, gridPosition.y); }
    std::shared_ptr<Cell> GetCell(const sf::Vector2f worldPosition) { return GetCell(GetCellGridPosition(worldPosition)); }

    sf::Vector2i GetCellGridPosition(const int x, const int y) { return { x, y }; }
    sf::Vector2i GetCellGridPosition(const sf::Vector2f worldPosition) const;

    sf::Vector2f GetCellWorldPosition(const int x, const int y) const { return sf::Vector2f(static_cast<float>(x), static_cast<float>(y)) * mCellSize + mOriginPosition; }
    sf::Vector2f GetCellWorldPosition(const sf::Vector2i gridPosition) const { return GetCellWorldPosition(gridPosition.x, gridPosition.y); }
    // Function clams the given worldPosition to the cell's worldPosition.
    sf::Vector2f GetCellWorldPosition(const sf::Vector2f worldPosition) const { return GetCellWorldPosition(GetCellGridPosition(worldPosition)); }

    sf::Vector2f GetCellCenterPosition(const int x, const int y) const { return GetCellCenterPosition(GetCellWorldPosition(x, y)); }
    sf::Vector2f GetCellCenterPosition(const sf::Vector2i gridPosition) const { return GetCellCenterPosition(gridPosition.x, gridPosition.y); }
    sf::Vector2f GetCellCenterPosition(const sf::Vector2f worldPosition) const { return GetCellWorldPosition(worldPosition) + GetCellCenter(); }

    std::vector<std::shared_ptr<Cell>> GetNeighboringCells(const int x, const int y);
    std::vector<std::shared_ptr<Cell>> GetNeighboringCells(const sf::Vector2i gridPosition) { return GetNeighboringCells(gridPosition.x, gridPosition.y); }
    std::vector<std::shared_ptr<Cell>> GetNeighboringCells(const sf::Vector2f worldPosition) { return GetNeighboringCells(GetCellGridPosition(worldPosition)); }

    const std::unordered_map<std::shared_ptr<Cell>, std::vector<std::shared_ptr<Cell>>>& GetTraversableCellMap() const { return mTraversableCellMap; }
    std::vector<std::shared_ptr<Cell>> GetTraversableCells(const int x, const int y) { return IsCellValid(x, y) ? mTraversableCellMap[GetCell(x, y)] : std::vector<std::shared_ptr<Cell>>{}; }
    std::vector<std::shared_ptr<Cell>> GetTraversableCells(const sf::Vector2i gridPosition) { return GetTraversableCells(gridPosition.x, gridPosition.y); }
    std::vector<std::shared_ptr<Cell>> GetTraversableCells(const sf::Vector2f worldPosition) { return GetTraversableCells(GetCellGridPosition(worldPosition)); }

    const std::unordered_multimap<int, std::shared_ptr<Cell>>& GetTeleportCellMap() const { return mTeleportCellMap; }
    std::shared_ptr<Cell> GetTeleportToCell(const int x, const int y);
    std::shared_ptr<Cell> GetTeleportToCell(const sf::Vector2i gridPosition) { return GetTeleportToCell(gridPosition.x, gridPosition.y); }
    std::shared_ptr<Cell> GetTeleportToCell(const sf::Vector2f worldPosition) { return GetTeleportToCell(GetCellGridPosition(worldPosition)); }

    // Return the index of it is a teleport cell, return 0 if not.
    int CheckIsTeleportCell(const int x, const int y);
    // Return the index of it is a teleport cell, return 0 if not.
    int CheckIsTeleportCell(const sf::Vector2i gridPosition) { return CheckIsTeleportCell(gridPosition.x, gridPosition.y); }
    // Return the index of it is a teleport cell, return 0 if not.
    int CheckIsTeleportCell(const sf::Vector2f worldPosition) { return CheckIsTeleportCell(GetCellGridPosition(worldPosition)); }

    sf::Vector2f GetPlayerSpawnPosition() const { return mPlayerSpawnPosition; }
    sf::Vector2f GetEnemySpawnPosition(const int number) const;
    const std::vector<sf::Vector2i>& GetCrossroadPositions() const { return mCrossroadPositions; }

private:
    void SetupGridFromFile(const std::vector<std::string>& levelLayout);
    void SetupTraversableCellMap();
    void SetupCrossroadPositions();

    sf::Vector2f GetCellCenter() const { return sf::Vector2f(mCellSize, mCellSize) * 0.5f; }

    bool IsCellValid(const int x, const int y) const;
    bool IsCellValid(const sf::Vector2i gridPosition) const { return IsCellValid(gridPosition.x, gridPosition.y); }

    int mWidth{};
    int mHeight{};
    float mCellSize{};

    sf::Vector2f mOriginPosition{};
    sf::Vector2f mPlayerSpawnPosition{};
    std::vector<sf::Vector2f> mEnemySpawnPositions{};

    std::vector<std::vector<std::shared_ptr<Cell>>> mGridCells{};
    std::unordered_map<std::shared_ptr<Cell>, std::vector<std::shared_ptr<Cell>>> mTraversableCellMap{};
    std::unordered_multimap<int, std::shared_ptr<Cell>> mTeleportCellMap{};
    std::vector<sf::Vector2i> mCrossroadPositions{};
};


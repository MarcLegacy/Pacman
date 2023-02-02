#pragma once

#include <array>
#include <memory>
#include <unordered_map>
#include <vector>
#include <SFML/System/Vector2.hpp>

#include "Cell.h"

class Cell;

class Grid : public Object
{
public:
    Grid(const int width, const int height, const float cellSize, const sf::Vector2f originPosition = {});

    void Update(float deltaTime) override;
    void Draw(sf::RenderTarget* target) override;

    int GetGridWidth() const { return mWidth; }
    int GetGridHeight() const { return mHeight; }
    sf::Vector2i GetGridSize() { return {mWidth, mHeight}; }

    float GetCellSize() const { return mCellSize; }

    std::shared_ptr<Cell> GetCell(const int x, const int y) { return mGridCells[x][y]; }
    std::shared_ptr<Cell> GetCell(const sf::Vector2i gridPosition) { return GetCell(gridPosition.x, gridPosition.y); }
    std::shared_ptr<Cell> GetCell(const sf::Vector2f worldPosition) { return GetCell(GetCellGridPosition(worldPosition)); }

    sf::Vector2i GetCellGridPosition(const int x, const int y) { return {x, y}; }
    sf::Vector2i GetCellGridPosition(const sf::Vector2f worldPosition) const;

    sf::Vector2f GetCellWorldPosition(const int x, const int y) const { return sf::Vector2f(static_cast<float>(x), static_cast<float>(y)) * mCellSize + mOriginPosition; }
    sf::Vector2f GetCellWorldPosition(const sf::Vector2i gridPosition) const { return GetCellWorldPosition(gridPosition.x, gridPosition.y); }

    sf::Vector2f GetCellCenterPosition(const int x, const int y) const { return GetCellWorldPosition(x, y) + GetCellCenter(); }
    sf::Vector2f GetCellCenterPosition(const sf::Vector2i gridPosition) const { return GetCellCenterPosition(gridPosition.x, gridPosition.y); }
    sf::Vector2f GetCellCenterPosition(const sf::Vector2f worldPosition) const { return worldPosition + GetCellCenter(); }

    std::vector<std::shared_ptr<Cell>> GetNeighbouringCells(const int x, const int y);
    std::vector<std::shared_ptr<Cell>> GetNeighbouringCells(const sf::Vector2i gridPosition) { return GetNeighbouringCells(gridPosition.x, gridPosition.y); }
    std::vector<std::shared_ptr<Cell>> GetNeighbouringCells(const sf::Vector2f worldPosition) { return GetNeighbouringCells(GetCellGridPosition(worldPosition)); }

    std::unordered_map<std::shared_ptr<Cell>, std::vector<std::shared_ptr<Cell>>> GetTraversableCellMap() const { return mTraversableCellMap; }

private:
    void SetupLevelLayout();
    void SetupGrid();
    void SetupTraversableCellMap();

    int RetrieveNumberFromLayout(const int x, const int y) const { return IsCellValid(x, y) ? mLevelLayout[y].at(x) : 0; };
    int RetrieveNumberFromLayout(const sf::Vector2i gridPosition) const { return RetrieveNumberFromLayout(gridPosition.x, gridPosition.y); };

    sf::Vector2f GetCellCenter() const { return sf::Vector2f(mCellSize, mCellSize) * 0.5f; }
    bool IsCellValid(const int x, const int y) const;
    bool IsCellValid(const sf::Vector2i gridPosition) const { return IsCellValid(gridPosition.x, gridPosition.y); }

    int mWidth{};
    int mHeight{};
    float mCellSize{};
    sf::Vector2f mOriginPosition{};
    std::vector<std::vector<std::shared_ptr<Cell>>> mGridCells{};
    std::array<std::array<int, 28>, 31> mLevelLayout{};
    std::unordered_map<std::shared_ptr<Cell>, std::vector<std::shared_ptr<Cell>>> mTraversableCellMap{};
};


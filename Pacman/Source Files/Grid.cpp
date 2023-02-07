#include "Grid.h"

#include <array>
#include <iostream>
#include <memory>

Grid::Grid(const int width, const int height, const float cellSize, const std::vector<std::string>& levelLayout, const sf::Vector2f originPosition)
: mWidth(width), mHeight(height), mCellSize(cellSize), mOriginPosition(originPosition)
{
    SetupGridFromFile(levelLayout);
    SetupTraversableCellMap();
    SetupCrossroadPositions();
}

void Grid::Update(float deltaTime) {}

void Grid::Draw(sf::RenderTarget* target)
{
    for (const auto& row : mGridCells)
    {
        for (const auto& cell : row)
        {
            cell->Draw(target);
        }
    }
}

bool Grid::IsCellValid(const int x, const int y) const
{
    if (y > mHeight - 1 || y < 0)
    {
        std::cout << "Trying to get gridPosition.y: {" << y << "} which is outside of the grid's height[" << mHeight - 1 << "]" << std::endl;
        return false;
    }

    if (x > mWidth - 1|| x < 0)
    {
        std::cout << "Trying to get gridPosition.x: {" << x << "} which is outside of the grid's width[" << mWidth - 1 << "]" << std::endl;
        return false;
    }

    return true;
}

sf::Vector2i Grid::GetCellGridPosition(const sf::Vector2f worldPosition) const
{
    int x = static_cast<int>((worldPosition - mOriginPosition).x / mCellSize);
    int y = static_cast<int>((worldPosition - mOriginPosition).y / mCellSize);
    return {x, y};
}

std::vector<std::shared_ptr<Cell>> Grid::GetNeighboringCells(const int x, const int y)
{
    if (!IsCellValid(x, y)) return {};

    std::vector<std::shared_ptr<Cell>> neighbouringCells{};

    if (x > 0)
    {
        neighbouringCells.push_back(GetCell(x - 1, y));
    }

    if (x < mWidth - 1)
    {
        neighbouringCells.push_back(GetCell(x + 1, y));
    }

    if (y > 0)
    {
        neighbouringCells.push_back(GetCell(x, y - 1));
    }

    if (y < mHeight - 1)
    {
        neighbouringCells.push_back(GetCell(x, y + 1));
    }

    return neighbouringCells;
}

std::shared_ptr<Cell> Grid::GetTeleportToCell(const int x, const int y)
{
    const int teleportIndex{ CheckIsTeleportCell(x, y) };

    if (!teleportIndex)
    {
        std::cout << "Teleport Cell not found!" << std::endl;
        return {};
    }

    for (auto iterator = mTeleportCellMap.find(teleportIndex); iterator != mTeleportCellMap.end(); ++iterator)
    {
        if (iterator->second != GetCell(x, y)) return iterator->second;
    }

    std::cout << "No linked teleport found!?" << std::endl;

    return {};
}

int Grid::CheckIsTeleportCell(const int x, const int y)
{
    const auto currentCell{ GetCell(x, y) };
    for (const auto& teleportCell : mTeleportCellMap)
    {
        if (teleportCell.second == currentCell)
        {
            return teleportCell.first;
        }
    }

    return 0;
}

sf::Vector2f Grid::GetEnemySpawnPosition(const int number) const
{
    if (number >= 0 && number < static_cast<int>(mEnemySpawnPositions.size()))
    {
        return mEnemySpawnPositions[number];
    }
        
    return sf::Vector2f{};
}

void Grid::SetupGridFromFile(const std::vector<std::string>& levelLayout)
{
    for (unsigned int column = 0; column < levelLayout.size(); ++column)
    {
        const std::string& layoutString = levelLayout[column];
        std::vector<std::shared_ptr<Cell>> cellRow{};

        for (unsigned int row = 0; row < layoutString.size(); ++row)
        {
            const auto& character = layoutString[row];
            // Calculates position and retrieves the cell type to create a cell.
            sf::Vector2f position{ GetCellWorldPosition(row, column) };

            CellType type = character == '*' ? CellType::Wall : CellType::Empty;
            bool isTeleportCell{ false };

            switch (character)
            {
            case 'P':
                mPlayerSpawnPosition = position;
                break;
            case 'G':
                mEnemySpawnPositions.push_back(position);
                break;
            case '1':
                isTeleportCell = true;
                break;
            default:
                break;
            }

            auto cell = std::make_shared<Cell>(position, type);
            if (isTeleportCell)
            {
                mTeleportCellMap.insert(mTeleportCellMap.begin(), std::pair<int, std::shared_ptr<Cell>>(1, cell));    // 0 should eventually be the number of the teleport cell.
            }
            cellRow.push_back(std::move(cell)); // using std::move here, as to avoid having to increment the reference count, which is cheaper.
        }

        mGridCells.push_back(std::move(cellRow));
    }
}

void Grid::SetupTraversableCellMap()
{
    for (const auto& row : mGridCells)
    {
        for (const auto& cell : row)
        {
            if (cell->GetCellType() == CellType::Wall) continue;    // Don't want to add wall's to the list

            std::vector<std::shared_ptr<Cell>> traversableCells{};

            for (const auto& neighbouringCell : GetNeighboringCells(cell->GetPosition()))
            {
                if (neighbouringCell->GetCellType() != CellType::Wall)
                {
                    traversableCells.push_back(neighbouringCell);
                }
            }

            mTraversableCellMap[cell] = traversableCells;
        }
    }

    // Also making sure that any teleport cell is connected.
    for (const auto& teleportFromCell : mTeleportCellMap)
    {
        for (const auto& teleportToCell : mTeleportCellMap)
        {
            if (teleportFromCell.first == teleportToCell.first && teleportFromCell.second != teleportToCell.second)
            {
                auto& vector = mTraversableCellMap[teleportFromCell.second];
                vector.push_back(teleportToCell.second);
            }
        }
    }
}

void Grid::SetupCrossroadPositions()
{
    for (const auto& row : mGridCells)
    {
        for (const auto& cell : row)
        {
            if (cell->GetCellType() == CellType::Wall) continue;    // Can avoid walls.

            std::vector<std::shared_ptr<Cell>> traversableCells{};

            if (GetTraversableCells(cell->GetPosition()).size() > 2)    // Every cell with more than 2 traversable cells is a crossroad.
            {
                mCrossroadPositions.push_back(GetCellGridPosition(cell->GetPosition()));
            }
        }
    }
}




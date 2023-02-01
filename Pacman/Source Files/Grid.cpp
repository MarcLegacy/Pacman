#include "Grid.h"

#include <iostream>
#include <memory>

#include "Pacman.h"


Grid::Grid(const int width, const int height, const float cellSize, const sf::Vector2f originPosition) : mWidth(width), mHeight(height), mCellSize(cellSize), mOriginPosition(originPosition)
{
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            auto position = sf::Vector2f(static_cast<float>(x) * cellSize, static_cast<float>(y) * cellSize);
            auto cell = std::make_unique<Cell>(position, CellType::Wall);
            //Pacman::AddObject(std::move(cell));
        }
    }
}

void Grid::SetupLevel(std::vector<std::vector<int>> levelLayout)
{
    if (levelLayout.empty())
    {
        std::cout << "Invalid Layout!" << std::endl;
        return;
    }

    for (int x = 0; x < levelLayout.size(); x++)
    {
        for (int y = 0; y < levelLayout[0].size(); y++)
        {
        }
    }
}

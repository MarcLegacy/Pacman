#pragma once

#include <vector>
#include <SFML/System/Vector2.hpp>

#include "Cell.h"

class Cell;

class Grid
{
public:
    Grid() = default;
    Grid(const int width, const int height, const float cellSize, const sf::Vector2f originPosition = {});
    ~Grid() = default;

    void SetupLevel(std::vector<std::vector<int>> levelLayout);

private:
    int mWidth{};
    int mHeight{};
    float mCellSize{};
    sf::Vector2f mOriginPosition{};
    std::vector<std::vector<Cell>> mGridCells;
};


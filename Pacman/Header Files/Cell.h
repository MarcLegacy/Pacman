#pragma once

#include "Object.h"

enum class CellType
{
    Empty,
    Wall,
};

class Cell : public Object
{
public:
    Cell() = default;
    Cell(const sf::Vector2f position) : Object(position) {}
    Cell(const sf::Vector2f position, const CellType cellType);

    void Update(float deltaTime) override;

    void SetCellType(CellType cellType);

private:
    CellType mCellType{ CellType::Empty };
    std::vector<Cell> mTraversableCells;
};


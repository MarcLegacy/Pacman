#pragma once

#include "Object.h"

enum class CellType
{
    Empty,
    Wall
};

class Cell : public Object
{
public:
    Cell(const sf::Vector2f position);
    Cell(const sf::Vector2f position, const CellType cellType);

    void Update(float deltaTime) override;

    CellType GetCellType() const { return mCellType; }
    void SetCellType(CellType cellType);

private:
    CellType mCellType{};
};


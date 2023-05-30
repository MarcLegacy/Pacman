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
    Cell(const sf::Vector2f position, const CellType cellType, const sf::Texture& texture) : Object(position, texture), mCellType(cellType) {}

    void Update(float deltaTime) override;

    CellType GetCellType() const { return mCellType; }

private:
    CellType mCellType{};
};


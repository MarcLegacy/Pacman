#pragma once

#include "Object.h"

enum class CellType
{
    Empty,
    Wall,
    Pill,
    PowerPill
};

class Cell : public Object
{
public:
    Cell(const sf::Vector2f position);
    Cell(const sf::Vector2f position, const CellType cellType);
    Cell(const sf::Vector2f position, const CellType cellType, const sf::Texture& texture) : Object(position, texture), mCellType(cellType) {}

    void Update(float deltaTime) override;
    void Draw(sf::RenderTarget* target) override;

    CellType GetCellType() const { return mCellType; }
    void RemovePill();
    void ResetPill();
    bool ContainsPill() const { return mContainsPill; }

private:
    CellType mCellType{};
    bool mContainsPill{ false };
};


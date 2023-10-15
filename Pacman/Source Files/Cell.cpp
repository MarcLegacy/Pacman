#include "Cell.h"

#include <iostream>

#include "GlobalConstants.h"


Cell::Cell(const sf::Vector2f position) : Object(position) {}

Cell::Cell(const sf::Vector2f position, const CellType cellType) : Object(position), mCellType(cellType)
{
    switch (cellType)
    {
    case CellType::Empty:
        break;
    case CellType::Wall:
        if (mTexture.loadFromFile("Resource Files/Wall.png"))
        {
            mSprite.setTexture(mTexture);
        }
        break;
    case CellType::Pill:
        if (mTexture.loadFromFile("Resource Files/Pill.png"))
        {
            mSprite.setTexture(mTexture);
            mSprite.setPosition(position.x + (CELL_SIZE * 0.5f), position.y + (CELL_SIZE * 0.5f));
        }
        break;
    default:
        std::cout << "Error: Invalid CellType" << std::endl;
    }
}

void Cell::Update(float deltaTime) {}

void Cell::RemovePill()
{
    if (mCellType != CellType::Pill) return;

    mCellType = CellType::Empty;

    // set to empty sprite
    const sf::Sprite sprite;
    mSprite = sprite;
}


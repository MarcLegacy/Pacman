#include "Cell.h"

#include <iostream>

#include "GlobalConstants.h"


Cell::Cell(const sf::Vector2f position) : Object(position) {}

Cell::Cell(const sf::Vector2f position, const CellType cellType) : Object(position), mCellType(cellType)
{
    static sf::Texture wallTexture;
    static sf::Texture pillTexture;

    if (wallTexture.getSize().x == 0 && wallTexture.getSize().y == 0)
    {
        wallTexture.loadFromFile("Resource Files/Wall.png");
    }
    if (pillTexture.getSize().x == 0 && pillTexture.getSize().y == 0)
    {
        pillTexture.loadFromFile("Resource Files/Pill.png");
    }

    switch (cellType)
    {
    case CellType::Empty:
        break;
    case CellType::Wall:
        if (wallTexture.getSize().x != 0 || wallTexture.getSize().y != 0)
        {
            mSprite.setTexture(wallTexture);
        }
        break;
    case CellType::Pill:
        if (pillTexture.getSize().x != 0 || pillTexture.getSize().y != 0)
        {
            mSprite.setTexture(pillTexture);
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


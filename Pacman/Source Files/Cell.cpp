#include "Cell.h"

#include <iostream>

#include "GlobalConstants.h"


Cell::Cell(const sf::Vector2f position) : Object(position) {}

Cell::Cell(const sf::Vector2f position, const CellType cellType) : Object(position), mCellType(cellType)
{
    static sf::Texture wallTexture;
    static sf::Texture pillTexture;
    static sf::Texture powerPillTexture;
    constexpr float halfCellSize = CELL_SIZE * 0.5f;

    if (wallTexture.getSize().x == 0 && wallTexture.getSize().y == 0)
    {
        wallTexture.loadFromFile("Resource Files/Wall.png");
    }
    if (pillTexture.getSize().x == 0 && pillTexture.getSize().y == 0)
    {
        pillTexture.loadFromFile("Resource Files/Pac-Man_Sprite_Sheet.png", sf::IntRect(99, 16, 32, 32));
    }
    if (powerPillTexture.getSize().x == 0 && powerPillTexture.getSize().y == 0)
    {
        powerPillTexture.loadFromFile("Resource Files/Pac-Man_Sprite_Sheet.png", sf::IntRect(99, 16, 32, 32));
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
            mSprite.setScale(0.125f, 0.125f);

            const float halfPillWidth = static_cast<float>(pillTexture.getSize().x) * 0.5f * mSprite.getScale().x;
            const float halfPillHeight = static_cast<float>(pillTexture.getSize().y) * 0.5f * mSprite.getScale().y;
            mSprite.setPosition(position.x + halfCellSize - halfPillWidth, position.y + halfCellSize - halfPillHeight);

            mContainsPill = true;
        }
        break;
    case CellType::PowerPill:
        if (powerPillTexture.getSize().x != 0 || powerPillTexture.getSize().y != 0)
        {
            mSprite.setTexture(powerPillTexture);
            mSprite.setScale(0.5f, 0.5f);

            const float halfPillWidth = static_cast<float>(powerPillTexture.getSize().x) * 0.5f * mSprite.getScale().x;
            const float halfPillHeight = static_cast<float>(powerPillTexture.getSize().y) * 0.5f * mSprite.getScale().y;
            mSprite.setPosition(position.x + halfCellSize - halfPillWidth, position.y + halfCellSize - halfPillHeight);
        }
        break;
    default:
        std::cout << "Error: Invalid CellType" << std::endl;
    }
}

void Cell::Update(float deltaTime) {}

void Cell::Draw(sf::RenderTarget* target)
{
    if (mCellType == CellType::Pill && !mContainsPill) return;

    Object::Draw(target);
}

void Cell::RemovePill()
{
    if (mCellType == CellType::Pill && mContainsPill)
    {
        mContainsPill = false;
    }
}

void Cell::ResetPill()
{
    if (mCellType != CellType::Pill) return;

    mContainsPill = true;
}


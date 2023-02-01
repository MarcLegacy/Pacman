#include "Cell.h"


Cell::Cell(const sf::Vector2f position, const CellType cellType) : Object(position), mCellType(cellType)
{
    if (cellType == CellType::Wall)
    {
        if (mTexture.loadFromFile("Resource Files/Wall.png"))
        {
            mSprite.setTexture(mTexture);
        }
    }
}

void Cell::Update(float deltaTime)
{

}

void Cell::SetCellType(CellType cellType)
{
    mCellType = cellType;

    if (cellType == CellType::Wall)
    {
        if (mTexture.loadFromFile("Resource Files/Wall.png"))
        {
            mSprite.setTexture(mTexture);
        }
    }
}

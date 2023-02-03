#include "Enemy.h"

#include <iostream>

#include "Utility.h"

Enemy::Enemy(const sf::Vector2f position, SkinColor skinColor)
    : Character(position)
{
    std::string fileName{};
    switch (skinColor)
    {
    case SkinColor::Blue:
        fileName = "Resource Files/EnemyBlue.png";
        break;
    case SkinColor::Orange:
        fileName = "Resource Files/EnemyOrange.png";
        break;
    case SkinColor::Pink:
        fileName = "Resource Files/EnemyPink.png";
        break;
    case SkinColor::Red:
        fileName = "Resource Files/EnemyRed.png";
        break;
    default:
        std::cout << "Skincolor given that hasn't been set yet!" << std::endl;
        break;
    }

    if (mTexture.loadFromFile(fileName))
    {
        mSprite.setTexture(mTexture);
    }

    srand(time(0));
}

void Enemy::Move(const float deltaTime)
{
    Character::Move(deltaTime);

    if (Utility::Distance(mPosition, mDestinationPosition) < 1.0f)
    {
        const int randomNumber = rand() / (RAND_MAX / (4));

        switch (randomNumber)
        {
        case 0:
            mDesiredDirection = Direction::Up;
            break;
        case 1:
            mDesiredDirection = Direction::Down;
            break;
        case 2:
            mDesiredDirection = Direction::Left;
            break;
        case 3:
            mDesiredDirection = Direction::Right;
            break;
        }
    }

}

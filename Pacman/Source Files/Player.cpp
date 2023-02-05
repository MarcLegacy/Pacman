#include "Player.h"

#include <SFML/Window/Keyboard.hpp>

#include "Pacman.h"
#include "Grid.h"
#include "Utility.h"

Player::Player(const sf::Vector2f position, const float speed)
    : Character(position, speed)
{
    if (mTexture.loadFromFile("Resource Files/Player.png"))
    {
        mSprite.setTexture(mTexture);
    }
}

void Player::Move(const float deltaTime)
{
    PlayerInput();

    // Checks if the destination has been reached before setting the next destination.
    // Also added immediate changing of direction when the character tries to move the other way around.
    if (Utility::Distance(mPosition, mDestinationPosition) < CELL_REACHED_RADIUS || IsGoingBack())
    {
        if (!MoveToDirection(mDesiredDirection))
        {
            // This makes sure that the character will move back to the center of the cell instead of stopping immediately when the character tries to move back while a wall is blocking that direction.
            // TODO: Change the movement so that this bandage is automatically fixed.
            if (MoveToDirection(mCurrentDirection) && IsGoingBack())
            {
                mCurrentDirection = mDesiredDirection;
                mDestinationPosition = Pacman::GetGrid()->GetCellWorldPosition(GetCenterPosition());
            }
        }
    }
    else
    {
        mPosition += GetMoveDirection(mCurrentDirection) * mSpeed * deltaTime;
    }
}


void Player::PlayerInput()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        mDesiredDirection = Direction::Up;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        mDesiredDirection = Direction::Down;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        mDesiredDirection = Direction::Left;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        mDesiredDirection = Direction::Right;
    }
}



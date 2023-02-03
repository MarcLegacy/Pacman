#include "Player.h"

#include <iostream>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "DrawDebug.h"
#include "Grid.h"
#include "Pacman.h"

Player::Player(const sf::Vector2f position)
: Object(position)
{
    if (mTexture.loadFromFile("Resource Files/Player.png"))
    {
        mSprite.setTexture(mTexture);
    }

    mDestinationPosition = mPosition;
}

void Player::Update(const float deltaTime)
{
    Move(deltaTime);

    mSprite.setPosition(mPosition);
}

void Player::Move(const float deltaTime)
{
    PlayerInput();

    //std::cout << CalculateDistance(mPosition, mDestinationPosition) << std::endl;

    // Checks if the destination has been reached before setting the next destination.
    // Also added immediate changing of direction when the character tries to move the other way around.
    if (CalculateDistance(mPosition, mDestinationPosition) < 1.0f || IsGoingBack())
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

void Player::Draw(sf::RenderTarget* target)
{
    Object::Draw(target);

    //target->draw(DrawDebug::DrawCell(Pacman::GetGrid()->GetCellWorldPosition(mPosition), 32.0f, sf::Color::Red));
}

bool Player::MoveToDirection(const Direction& direction)
{
    const auto& grid = Pacman::GetGrid();

    // We don't want the grid position of the character's actual position, we want to get the position's of the center of a cell so that the position is anchored to cell positions.
    const sf::Vector2i currentGridPosition = grid->GetCellGridPosition(GetCenterPosition());

    sf::Vector2i destinationGridPosition{};
    destinationGridPosition.x = currentGridPosition.x + static_cast<int>(GetMoveDirection(direction).x);
    destinationGridPosition.y = currentGridPosition.y + static_cast<int>(GetMoveDirection(direction).y);

    // We check if the destination cell is actually traversable, only then will we set the new destination position and current direction.
    const auto& traversableCells = grid->GetTraversableCells(GetCenterPosition());
    if (std::find(traversableCells.begin(), traversableCells.end(), grid->GetCell(destinationGridPosition)) != traversableCells.end())
    {
        mDestinationPosition = grid->GetCellWorldPosition(destinationGridPosition);
        mCurrentDirection = direction;
        return true;
    }

    return false;
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

sf::Vector2f Player::GetMoveDirection(const Direction& direction)
{
    switch (direction)
    {
    case Direction::Up:
        return { 0.0f, -1.0f };
    case Direction::Down:
        return { 0.0f, 1.0f };
    case Direction::Left:
        return { -1.0f, 0.0f };
    case Direction::Right:
        return { 1.0f, 0.0f };
    default:
        return {};
    }
}

float Player::CalculateDistance(const sf::Vector2f fromPosition, const sf::Vector2f toPosition) const
{
    return sqrt(powf(toPosition.x - fromPosition.x, 2) + powf(toPosition.y - fromPosition.y, 2));
}

bool Player::IsGoingBack()
{
    if (mCurrentDirection == Direction::Up && mDesiredDirection == Direction::Down) return true;
    if (mCurrentDirection == Direction::Down && mDesiredDirection == Direction::Up) return true;
    if (mCurrentDirection == Direction::Left && mDesiredDirection == Direction::Right) return true;
    if (mCurrentDirection == Direction::Right && mDesiredDirection == Direction::Left) return true;

    return false;
}

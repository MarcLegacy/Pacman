#include "Character.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include "DrawDebug.h"
#include "Grid.h"
#include "Pacman.h"


Character::Character(const sf::Vector2f position, const float speed)
    : Object(position), mSpeed(speed), mDestinationPosition(mPosition)
{
}

void Character::Update(const float deltaTime)
{
    Move(deltaTime);

    mSprite.setPosition(mPosition);
}

void Character::Draw(sf::RenderTarget* target)
{
    Object::Draw(target);

    target->draw(DrawDebug::DrawCell(Pacman::GetGrid()->GetCellWorldPosition(GetCenterPosition()), 32.0f, sf::Color::Red));
}

bool Character::MoveToDirection(const Direction& direction)
{
    const auto& grid = Pacman::GetGrid();

    // We don't want the grid position of the character's actual position, we want to get the position's of the center of a cell so that the position is anchored to cell positions.
    const sf::Vector2i currentGridPosition = grid->GetCellGridPosition(GetCenterPosition());

    sf::Vector2i destinationGridPosition;
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

sf::Vector2f Character::GetMoveDirection(const Direction& direction)
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

bool Character::IsGoingBack() const
{
    if (mCurrentDirection == Direction::Up && mDesiredDirection == Direction::Down) return true;
    if (mCurrentDirection == Direction::Down && mDesiredDirection == Direction::Up) return true;
    if (mCurrentDirection == Direction::Left && mDesiredDirection == Direction::Right) return true;
    if (mCurrentDirection == Direction::Right && mDesiredDirection == Direction::Left) return true;

    return false;
}

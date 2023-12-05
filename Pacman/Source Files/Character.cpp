#include "Character.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include "DrawDebug.h"
#include "Grid.h"
#include "Pacman.h"
#include "Utility.h"


Character::Character(const sf::Vector2f position, const float speed)
    : Object(position), mSpeed(speed), mDestinationWorldPosition(mPosition), mStartingPosition(position) {}

void Character::Update(const float deltaTime)
{
    UpdateCellPositionStatus();

    CharacterTeleportOrCentering();
    CharacterPositionCorrection();

    Move(deltaTime);

    Animation(deltaTime);

    mSprite.setPosition(mPosition);
}

void Character::Draw(sf::RenderTarget* target)
{
    Object::Draw(target);

    ShowCurrentGridPosition(target, false);
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
        mDestinationWorldPosition = grid->GetCellWorldPosition(destinationGridPosition);
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

void Character::UpdateCellPositionStatus()
{
    const auto& gridPosition = Pacman::GetGrid()->GetCellGridPosition(GetCenterPosition());

    // This replaces an event
    if (Utility::CalculateDistance(mPosition, mDestinationWorldPosition) < CELL_REACHED_RADIUS && mCurrentGridPosition != gridPosition)
    {
        mOnCellChanged = true;
        mCurrentGridPosition = gridPosition;
        mCellCostMap = Pacman::GetPathfinder()->BreadthFirstSearch(Pacman::GetGrid()->GetCellGridPosition(GetCenterPosition()));
    }
    else
    {
        mOnCellChanged = false;
    }
}

void Character::CharacterPositionCorrection()
{
    const auto& grid = Pacman::GetGrid();

    // Bandage to make sure the position of the character is reset should they ever get stuck inside a wall.
    if (grid->GetCell(GetCenterPosition())->GetCellType() == CellType::Wall)
    {
        mPosition = grid->GetCellWorldPosition(grid->GetCellGridPosition(GetCenterPosition()) - sf::Vector2i{ static_cast<int>(GetMoveDirection(mCurrentDirection).x), static_cast<int>(GetMoveDirection(mCurrentDirection).y) });
        mDestinationWorldPosition = mPosition;
    }
}

void Character::CharacterTeleportOrCentering()
{
    const auto& grid = Pacman::GetGrid();

    if (mOnCellChanged)
    {
        if (grid->CheckIsTeleportCell(mCurrentGridPosition))
        {
            mPosition = grid->GetTeleportToCell(mCurrentGridPosition)->GetCenterPosition(); // Teleports the character to the connected cell.
            mDestinationWorldPosition = mPosition;
        }
        else
        {
            mPosition = grid->GetCellWorldPosition(mCurrentGridPosition);   // This makes sure we stay in the middle of the cell
        }
    }
}

void Character::ShowCurrentGridPosition(sf::RenderTarget* target, const bool show) const
{
    if (!show) return;

    target->draw(DrawDebug::DrawCell(Pacman::GetGrid()->GetCellWorldPosition(GetCenterPosition()), 32.0f, sf::Color::Red));
}

void Character::WalkingAnimation(const float deltaTime)
{
    //if (mCurrentDirection == Direction::Invalid || mTextures.empty()) return;

    animationTimer -= deltaTime;

    if (animationTimer > 0.0f) return;

    animationTimer = ANIMATION_SPEED;
    auto it = mTextures.equal_range(mCurrentDirection);

    if (firstTexture)
    {
        firstTexture = false;
        ++it.first;
    }
    else
    {
        firstTexture = true;
    }

    mSprite.setTexture(it.first->second);
}

#include "Enemy.h"

#include <iostream>
#include <SFML/Graphics/RenderTarget.hpp>

#include "DrawDebug.h"
#include "Grid.h"
#include "Pacman.h"

#include "Pathfinder.h"
#include "Utility.h"

Enemy::Enemy(const sf::Vector2f position, SkinColor skinColor, const float speed)
    : Character(position)
{
    std::string fileName{};
    switch (skinColor)
    {
    case SkinColor::Blue:
        fileName = "Resource Files/EnemyBlue.png";
        mPathColor = sf::Color::Blue;
        break;
    case SkinColor::Orange:
        fileName = "Resource Files/EnemyOrange.png";
        mPathColor = { 230, 126, 34 };
        break;
    case SkinColor::Pink:
        fileName = "Resource Files/EnemyPink.png";
        mPathColor = { 245, 183, 177 };
        break;
    case SkinColor::Red:
        fileName = "Resource Files/EnemyRed.png";
        mPathColor = sf::Color::Red;
        break;
    default:
        std::cout << "Skincolor given that hasn't been set yet!" << std::endl;
        break;
    }

    if (mTexture.loadFromFile(fileName))
    {
        mSprite.setTexture(mTexture);
    }
}

void Enemy::Move(const float deltaTime)
{
    if (Utility::Distance(mPosition, mDestinationWorldPosition) < CELL_REACHED_RADIUS)
    {
        FollowPath();

        MoveToDirection(mDesiredDirection);
    }
    else
    {
        mPosition += GetMoveDirection(mCurrentDirection) * mSpeed * deltaTime;
    }
}

void Enemy::Draw(sf::RenderTarget* target)
{
    Character::Draw(target);

    ShowDebugPathArrows(target, false);
    ShowTargetGridPosition(target, false);
}

void Enemy::FindPath(const sf::Vector2i targetGridPosition, const bool withWeight)
{
    mTargetGridPosition = targetGridPosition;

    mPath = Pacman::GetPathfinder()->AStar(Pacman::GetGrid()->GetCellGridPosition(GetCenterPosition()), targetGridPosition, withWeight);
    mCurrentPathIndex = 0;
}

void Enemy::FollowPath()
{
    const auto& grid = Pacman::GetGrid();

    if (mPath.empty()) return;

    if (mCurrentPathIndex >= mPath.size())
    {
        std::cout << "End of path reached!" << std::endl;
        return;
    }

    // TODO: Make sure the pathing is done when they are on the cell they're currently traveling to.
    if (mPath.size() > 1 && mPath[1] == grid->GetCellGridPosition(GetCenterPosition()) && mCurrentPathIndex == 0)   // Bandage fix to battle them moving back because they repathed too soon.
    {
        mCurrentPathIndex = 1;
    }

    mDesiredDirection = Utility::ConvertGridDirectionToDirection(mPath[mCurrentPathIndex++] - grid->GetCellGridPosition(GetCenterPosition()));

    if (mCurrentPathIndex == mPath.size())
    {
        mPath.clear();
    }
}

void Enemy::ShowDebugPathArrows(sf::RenderTarget* target, const bool show) const
{
    if (!show) return;

    for (const auto& pathArrow : DrawDebug::DrawPathArrows(mPath, 24.0f, mPathColor))
    {
        target->draw(&pathArrow[0], pathArrow.size(), sf::LineStrip);
    }
}

void Enemy::ShowTargetGridPosition(sf::RenderTarget* target, const bool show) const
{
    if (!show) return;

    target->draw(Pacman::GetDrawDebug()->DrawCell(Pacman::GetGrid()->GetCellWorldPosition(mTargetGridPosition), CELL_SIZE, mPathColor));
}



#include "Enemy.h"

#include <iostream>
#include <SFML/Graphics/RenderTarget.hpp>

#include "DrawDebug.h"
#include "EnemyManager.h"
#include "Grid.h"
#include "Pacman.h"

#include "Pathfinder.h"
#include "Utility.h"

Enemy::Enemy(const sf::Vector2f position, const SkinColor skinColor, const float speed)
    : Character(position, speed)
{
    int spriteYPos{};

    switch (skinColor)
    {
    case SkinColor::Blue:
        spriteYPos = 97;
        mPathColor = sf::Color::Blue;
        break;
    case SkinColor::Orange:
        spriteYPos = 113;
        mPathColor = { 230, 126, 34 };
        break;
    case SkinColor::Pink:
        spriteYPos = 81;
        mPathColor = { 245, 183, 177 };
        break;
    case SkinColor::Red:
        spriteYPos = 65;
        mPathColor = sf::Color::Red;
        break;
    default:
        std::cout << "Skincolor given that hasn't been set yet!" << std::endl;
        break;
    }

    if (mTexture.loadFromFile("Resource Files/Pac-Man_Sprite_Sheet.png", sf::IntRect(68, spriteYPos, ENEMY_SIZE, ENEMY_SIZE)))
    {
        mSprite.setTexture(mTexture);
        mSprite.setScale(1.0f / ENEMY_SIZE * CELL_SIZE, 1.0f / ENEMY_SIZE * CELL_SIZE);
    }

    LoadTextures(spriteYPos);
}

void Enemy::Move(const float deltaTime)
{
    if (Utility::CalculateDistance(mPosition, mDestinationWorldPosition) < CELL_REACHED_RADIUS)
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

void Enemy::Animation(const float deltaTime)
{
    switch (Pacman::GetEnemyManager()->GetEnemyMode())
    {
        case EnemyMode::Chase:
            WalkingAnimation(deltaTime);
        break;

        case EnemyMode::Scatter:
            ScaredAnimation(deltaTime);
        break;
    }
    
}

void Enemy::FindPath(const sf::Vector2i targetGridPosition, const bool withWeight)
{
    mTargetGridPosition = targetGridPosition;

    mPath = Pacman::GetPathfinder()->AStar(Pacman::GetGrid()->GetCellGridPosition(GetCenterPosition()), targetGridPosition, withWeight);
    mCurrentPathIndex = 0;
}

void Enemy::ScaredAnimation(const float deltaTime)
{
    bool isTurning = false;

    animationTimer -= deltaTime;

    if (animationTimer > 0.0f) return;

    animationTimer = ANIMATION_SPEED;

    if (Pacman::GetEnemyManager()->GetScarredTimer() < (POWER_PILL_DURATION * 0.25f))
    {
        // This should make sure the enemy is flashing every 0.5 seconds.
        static_cast<int>(Pacman::GetEnemyManager()->GetScarredTimer() * 10) % 5 == 0 ? isTurning = false : isTurning = true;
    }

    auto it = mScaredTextures.equal_range(isTurning);

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

void Enemy::LoadTextures(const int spriteYPos)
{
    LoadNormalTextures(spriteYPos);
    LoadScaredTextures();
}

void Enemy::LoadNormalTextures(int spriteYPos)
{
    sf::Texture texture;

    if (texture.loadFromFile("Resource Files/Pac-Man_Sprite_Sheet.png", sf::IntRect(68, spriteYPos, ENEMY_SIZE, ENEMY_SIZE)))
    {
        mTextures.emplace(Direction::Up, texture);
    }
    if (texture.loadFromFile("Resource Files/Pac-Man_Sprite_Sheet.png", sf::IntRect(84, spriteYPos, ENEMY_SIZE, ENEMY_SIZE)))
    {
        mTextures.emplace(Direction::Up, texture);
    }
    if (texture.loadFromFile("Resource Files/Pac-Man_Sprite_Sheet.png", sf::IntRect(100, spriteYPos, ENEMY_SIZE, ENEMY_SIZE)))
    {
        mTextures.emplace(Direction::Down, texture);
    }
    if (texture.loadFromFile("Resource Files/Pac-Man_Sprite_Sheet.png", sf::IntRect(116, spriteYPos, ENEMY_SIZE, ENEMY_SIZE)))
    {
        mTextures.emplace(Direction::Down, texture);
    }
    if (texture.loadFromFile("Resource Files/Pac-Man_Sprite_Sheet.png", sf::IntRect(36, spriteYPos, ENEMY_SIZE, ENEMY_SIZE)))
    {
        mTextures.emplace(Direction::Left, texture);
    }
    if (texture.loadFromFile("Resource Files/Pac-Man_Sprite_Sheet.png", sf::IntRect(52, spriteYPos, ENEMY_SIZE, ENEMY_SIZE)))
    {
        mTextures.emplace(Direction::Left, texture);
    }
    if (texture.loadFromFile("Resource Files/Pac-Man_Sprite_Sheet.png", sf::IntRect(4, spriteYPos, ENEMY_SIZE, ENEMY_SIZE)))
    {
        mTextures.emplace(Direction::Right, texture);
    }
    if (texture.loadFromFile("Resource Files/Pac-Man_Sprite_Sheet.png", sf::IntRect(20, spriteYPos, ENEMY_SIZE, ENEMY_SIZE)))
    {
        mTextures.emplace(Direction::Right, texture);
    }
}

void Enemy::LoadScaredTextures()
{
    sf::Texture texture;

    if (texture.loadFromFile("Resource Files/Pac-Man_Sprite_Sheet.png", sf::IntRect(132, 65, ENEMY_SIZE, ENEMY_SIZE)))
    {
        mScaredTextures.emplace(false, texture);
    }
    if (texture.loadFromFile("Resource Files/Pac-Man_Sprite_Sheet.png", sf::IntRect(148, 65, ENEMY_SIZE, ENEMY_SIZE)))
    {
        mScaredTextures.emplace(false, texture);
    }
    if (texture.loadFromFile("Resource Files/Pac-Man_Sprite_Sheet.png", sf::IntRect(164, 65, ENEMY_SIZE, ENEMY_SIZE)))
    {
        mScaredTextures.emplace(true, texture);
    }
    if (texture.loadFromFile("Resource Files/Pac-Man_Sprite_Sheet.png", sf::IntRect(180, 65, ENEMY_SIZE, ENEMY_SIZE)))
    {
        mScaredTextures.emplace(true, texture);
    }
}



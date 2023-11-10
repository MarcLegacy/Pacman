#include "Player.h"

#include <SFML/Window/Keyboard.hpp>

#include "EnemyManager.h"
#include "GlobalConstants.h"
#include "Pacman.h"
#include "Grid.h"
#include "Utility.h"

Player::Player(const sf::Vector2f position, const float speed, const int lives)
    : Character(position, speed), mLives(lives)
{
    if (mTexture.loadFromFile("Resource Files/Pac-Man_Sprite_Sheet.png", sf::IntRect(36, 1, PACMAN_SIZE, PACMAN_SIZE)))
    {
        mSprite.setTexture(mTexture);
        mSprite.setScale(1.0f / PACMAN_SIZE * CELL_SIZE, 1.0f / PACMAN_SIZE * CELL_SIZE);
    }

    LoadTextures();
}

void Player::Move(const float deltaTime)
{
    PlayerInput();

    // Checks if the destination has been reached before setting the next destination.
    // Also added immediate changing of direction when the character tries to move the other way around.
    if (Utility::CalculateDistance(mPosition, mDestinationWorldPosition) < CELL_REACHED_RADIUS || IsGoingBack())
    {
        if (!MoveToDirection(mDesiredDirection))
        {
            // This makes sure that the character will move back to the center of the cell instead of stopping immediately when the character tries to move back while a wall is blocking that direction.
            // TODO: Change the movement so that this bandage is automatically fixed.
            if (MoveToDirection(mCurrentDirection) && IsGoingBack())
            {
                mCurrentDirection = mDesiredDirection;
                mDestinationWorldPosition = Pacman::GetGrid()->GetCellWorldPosition(GetCenterPosition());
            }
        }

        // Check if on cell with Pill
        if (Pacman::GetGrid()->IsCellWithPill(GetCenterPosition()))
        {
            Pacman::GetGrid()->GetCell(GetCenterPosition())->RemovePill();

            if (Pacman::GetGrid()->GetCell(GetCenterPosition())->GetCellType() == CellType::Pill)
            {
                IncreaseScore(PILL_SCORE);
                mTotalPillsCaught++;
            }
            else
            {
                Pacman::GetEnemyManager()->SwitchEnemyMode();
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

void Player::LoadTextures()
{
    sf::Texture texture;

    if (texture.loadFromFile("Resource Files/Pac-Man_Sprite_Sheet.png", sf::IntRect(20, 33, PACMAN_SIZE, PACMAN_SIZE)))
    {
        mTextures.emplace(Direction::Up, texture);
    }
    if (texture.loadFromFile("Resource Files/Pac-Man_Sprite_Sheet.png", sf::IntRect(4, 33, PACMAN_SIZE, PACMAN_SIZE)))
    {
        mTextures.emplace(Direction::Up, texture);
    }
    if (texture.loadFromFile("Resource Files/Pac-Man_Sprite_Sheet.png", sf::IntRect(20, 49, PACMAN_SIZE, PACMAN_SIZE)))
    {
        mTextures.emplace(Direction::Down, texture);
    }
    if (texture.loadFromFile("Resource Files/Pac-Man_Sprite_Sheet.png", sf::IntRect(4, 49, PACMAN_SIZE, PACMAN_SIZE)))
    {
        mTextures.emplace(Direction::Down, texture);
    }
    if (texture.loadFromFile("Resource Files/Pac-Man_Sprite_Sheet.png", sf::IntRect(20, 17, PACMAN_SIZE, PACMAN_SIZE)))
    {
        mTextures.emplace(Direction::Left, texture);
    }
    if (texture.loadFromFile("Resource Files/Pac-Man_Sprite_Sheet.png", sf::IntRect(4, 17, PACMAN_SIZE, PACMAN_SIZE)))
    {
        mTextures.emplace(Direction::Left, texture);
    }
    if (texture.loadFromFile("Resource Files/Pac-Man_Sprite_Sheet.png", sf::IntRect(20, 1, PACMAN_SIZE, PACMAN_SIZE)))
    {
        mTextures.emplace(Direction::Right, texture);
    }
    if (texture.loadFromFile("Resource Files/Pac-Man_Sprite_Sheet.png", sf::IntRect(4, 1, PACMAN_SIZE, PACMAN_SIZE)))
    {
        mTextures.emplace(Direction::Right, texture);
    }
}






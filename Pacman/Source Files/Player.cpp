#include "Player.h"

#include <SFML/Window/Keyboard.hpp>

Player::Player(const sf::Vector2f position)
: Object(position)
{
    if (mTexture.loadFromFile("Resource Files/Player.png"))
    {
        mSprite.setTexture(mTexture);
    }
}

void Player::Update(float deltaTime)
{
    Move(deltaTime);

    mSprite.setPosition(mPosition);
}

void Player::Move(float deltaTime)
{
    PlayerInput();
    mPosition += GetMoveDirection(mCurrentDirection) * mSpeed * deltaTime;
}

void Player::PlayerInput()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        mCurrentDirection = Direction::Up;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        mCurrentDirection = Direction::Down;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        mCurrentDirection = Direction::Left;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        mCurrentDirection = Direction::Right;
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

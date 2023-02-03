#include "Player.h"

#include <SFML/Window/Keyboard.hpp>

Player::Player(const sf::Vector2f position)
    : Character(position)
{
    if (mTexture.loadFromFile("Resource Files/Player.png"))
    {
        mSprite.setTexture(mTexture);
    }
}

void Player::Move(const float deltaTime)
{
    Character::Move(deltaTime);

    PlayerInput();
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



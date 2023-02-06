#pragma once

#include "Character.h"
#include "Object.h"

class Player : public Character
{
public:
    Player(const sf::Vector2f position, const float speed = 100.0f);
    void Move(const float deltaTime) override;
    void PlayerInput();
};







#pragma once

#include "Character.h"

enum class SkinColor
{
    Blue,
    Orange,
    Pink,
    Red
};

class Enemy : public Character
{
public:
    Enemy(const sf::Vector2f position, SkinColor skinColor);

    void Move(const float deltaTime) override;
};


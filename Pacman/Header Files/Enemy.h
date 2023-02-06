#pragma once

#include "Character.h"
#include "Player.h"

class Enemy : public Character
{
public:
    Enemy(const sf::Vector2f position, SkinColor skinColor, const float speed = 100.0f);

    void Move(const float deltaTime) override;
    void Draw(sf::RenderTarget* target) override;

    void FindPath(const sf::Vector2i targetGridPosition);
    const std::vector<sf::Vector2i>& GetPath() const { return mPath; }
    void ClearPath() { mPath.clear(); }

    bool mIsDoingTactic{ false };

private:
    void FollowPath();

    std::vector<sf::Vector2i> mPath{};
    unsigned int mCurrentPathIndex{};
    sf::Color mPathColor{};

};


#pragma once

#include "Character.h"
#include "Player.h"

class Enemy : public Character
{
public:
    Enemy(const sf::Vector2f position, SkinColor skinColor, const float speed = 100.0f);

    void Move(const float deltaTime) override;
    void Draw(sf::RenderTarget* target) override;

    void FindPath(const sf::Vector2i targetGridPosition, const bool withWeight = false);
    const std::vector<sf::Vector2i>& GetPath() const { return mPath; }
    void ClearPath() { mPath.clear(); }
    sf::Vector2i GetTargetGridPosition() { return mTargetGridPosition; }

    bool mIsDoingTactic{ false };

private:
    void FollowPath();
    void ShowDebugPathArrows(sf::RenderTarget* target, const bool show = true) const;
    void ShowTargetGridPosition(sf::RenderTarget* target, const bool show = true) const;

    std::vector<sf::Vector2i> mPath{};
    unsigned int mCurrentPathIndex{};
    sf::Color mPathColor{};
    sf::Vector2i mTargetGridPosition{};

};


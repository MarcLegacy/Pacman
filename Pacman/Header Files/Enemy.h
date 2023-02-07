#pragma once

#include "Character.h"
#include "Player.h"

class Enemy : public Character
{
public:
    Enemy(const sf::Vector2f position, SkinColor skinColor, const float speed = 100.0f);

    void Move(const float deltaTime) override;
    void Draw(sf::RenderTarget* target) override;

    // Find a path using the A* algorithm.
    void FindPath(const sf::Vector2i targetGridPosition, const bool withWeight = false);

    void ClearPath() { mPath.clear(); }

    const std::vector<sf::Vector2i>& GetPath() const { return mPath; }
    sf::Vector2i GetTargetGridPosition() const { return mTargetGridPosition; }

private:
    // Takes care of setting the correct direction and destination following the current path it is on.
    void FollowPath();

    void ShowDebugPathArrows(sf::RenderTarget* target, const bool show = true) const;
    void ShowTargetGridPosition(sf::RenderTarget* target, const bool show = true) const;

    std::vector<sf::Vector2i> mPath{};
    unsigned int mCurrentPathIndex{};
    sf::Color mPathColor{};
    sf::Vector2i mTargetGridPosition{};

};


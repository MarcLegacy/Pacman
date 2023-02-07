#pragma once

#include <memory>
#include <unordered_map>

#include "Object.h"
#include "Utility.h"

class Player;
class Enemy;



class EnemyManager : public Object
{
public:
    EnemyManager(std::shared_ptr<Player> target);

    void Update(float deltaTime) override;
    void Draw(sf::RenderTarget* target) override;

    void OldTactic() const;
    void NewTactic();
    const std::vector<std::shared_ptr<Enemy>>& GetEnemies() { return mEnemies; }

private:
    std::vector<sf::Vector2i> CollectCalculatedPaths() const;
    void UpdateTargetDistanceToCrossroadMap();
    // Returns a vector of the shortest grid positions to the player, amount sets the vector size.
    std::vector<sf::Vector2i> GetShortestGridDistances(const int amount) const;
    sf::Vector2i GetClosestGridPosition(const sf::Vector2i currentGridPosition, const std::vector<sf::Vector2i>& gridPositions);
    void SetEnemyTargetPosition(const unsigned int enemyIndex, std::vector<sf::Vector2i>& closestCrossroadGridPositions);
    // Checks vectors if they contain the same grid positions, even when they're not in the same index.
    bool AreCrossroadsEqual(const std::vector<sf::Vector2i>& crossRoadsPositionsA, const std::vector<sf::Vector2i>& crossRoadsPositionsB);

    std::vector<std::shared_ptr<Enemy>> mEnemies{};
    std::shared_ptr<Player> mTarget{};
    std::unordered_map<sf::Vector2i, int, Vector2iHasher> mTargetDistanceToCrossroadMap{};
    std::vector<sf::Vector2i> mCurrentClosestCrossroadGridPositions{};
};




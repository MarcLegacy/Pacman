#pragma once

#include <map>
#include <memory>
#include <unordered_map>

#include "Object.h"
#include "Utility.h"

class Player;
class Enemy;

enum class EnemyMode
{
    Chase,
    Scatter
};


class EnemyManager
{
public:
    EnemyManager(std::unique_ptr<Player>& target);

    void Update(float deltaTime);
    void Draw(sf::RenderTarget* target);

    // Makes the enemies move to different crossroads to trap the player.
    void SurroundTactic();
    void ResetPositions() const;
    void SwitchEnemyMode();
    void LoadTextures();

    const std::vector<std::unique_ptr<Enemy>>& GetEnemies() { return mEnemies; }
    EnemyMode GetEnemyMode() const { return mEnemyMode; }

private:
    std::vector<sf::Vector2i> CollectCalculatedPaths() const;
    void UpdateTargetDistanceToCrossroadMap();

    // Returns a vector of the shortest grid positions to the player, amount sets the vector size.
    std::vector<sf::Vector2i> GetShortestGridDistances(const int amount) const;

    // Returns the closest grid position towards the passed currentGridPosition.
    sf::Vector2i GetClosestGridPosition(const sf::Vector2i currentGridPosition, const std::vector<sf::Vector2i>& gridPositions);

    // Sets the target position of the enemy to the closest crossroad position in the list
    void SetEnemyTargetPosition(const unsigned int enemyIndex, std::vector<sf::Vector2i>& closestCrossroadGridPositions);

    // Checks vectors if they contain the same grid positions, even when they're not in the same index.
    static bool AreCrossroadsEqual(const std::vector<sf::Vector2i>& crossRoadsPositionsA, const std::vector<sf::Vector2i>& crossRoadsPositionsB);

    void ShowCrossroads(sf::RenderTarget* target, const bool show = true) const;
    void ShowCrossroadDistanceToTargetCost(sf::RenderTarget* target, const bool show = true) const;
    void ShowClosestCrossroadsToTarget(sf::RenderTarget* target, const bool show = true) const;

    static sf::Vector2i FindTacticalRetreatGridPosition(const sf::Vector2i currentGridPosition, const std::vector<sf::Vector2i>& avoidPositions);
    sf::Vector2i FindTacticalRetreatGridPosition2(const std::unique_ptr<Enemy>& currentEnemy);

    std::vector<std::unique_ptr<Enemy>> mEnemies{};
    std::unique_ptr<Player>& mTarget;
    std::unordered_map<sf::Vector2i, int, Vector2iHasher> mTargetDistanceToCrossroadMap{};
    std::vector<sf::Vector2i> mCurrentClosestCrossroadGridPositions{};

    EnemyMode mEnemyMode{ EnemyMode::Chase };

    std::multimap<bool, sf::Texture> mScaredTextures{};
};





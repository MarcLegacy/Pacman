#pragma once

#include <memory>
#include <vector>
#include <SFML/Graphics/Font.hpp>

#include "Pathfinder.h"

class EnemyManager;
class Pathfinder;
class Enemy;
class Object;
class Grid;
class Player;
class DrawDebug;

namespace sf
{
    class Sprite;
    class Event;
    class RenderWindow;
}

enum class GameState
{
    Start,
    Paused,
    Playing,
    GameOver
};

class Pacman
{
public:
    Pacman();
    ~Pacman();

    bool IsRunning() const;
    void Update(const float deltaTime);
    void Render();

    static const std::unique_ptr<Grid>& GetGrid() { return mGrid; }
    static const std::unique_ptr<DrawDebug>& GetDrawDebug() { return mDrawDebug; }
    static const std::unique_ptr<Pathfinder>& GetPathfinder() { return mPathfinder; }
    static const std::unique_ptr<EnemyManager>& GetEnemyManager() { return mEnemyManager; }

private:
    void Draw();
    void PollEvents();
    void FPSTimer(const float deltaTime, const float logTimer) const;
    void DrawTraversableMapPersistant() const;
    void InitializeObjects();
    void CheckCharacterContact();
    void ShowGameText() const;
    void ShowScoreText() const;
    void ReadLayoutFromFile();
    void LoadFiles();
    void ResetPositions() const;
    void ResetGame();

    std::unique_ptr<sf::RenderWindow> mWindow{};
    std::unique_ptr<sf::Event> mEvent{};

    GameState mGameState{ GameState::Start };
    sf::Font mFont{};
    sf::Vector2i mCurrentScreenPosition{};
    std::vector<std::string> mLevelLayout{};
    sf::Vector2i mGridSize{};
    sf::Texture mLifeTexture{};
    std::vector<sf::Sprite> mLifeSprites{};

    std::unique_ptr<Player> mPlayer{};

    static std::unique_ptr<Grid> mGrid;
    static std::unique_ptr<DrawDebug> mDrawDebug;
    static std::unique_ptr<Pathfinder> mPathfinder;
    static std::unique_ptr<EnemyManager> mEnemyManager;
};

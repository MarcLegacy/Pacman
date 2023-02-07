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

    static std::shared_ptr<Grid> GetGrid() { return mGrid; }
    static std::shared_ptr<DrawDebug> GetDrawDebug() { return mDrawDebug; }
    static std::shared_ptr<Pathfinder> GetPathfinder() { return mPathfinder; }
    static std::shared_ptr<EnemyManager> GetEnemyManager() { return mEnemyManager; }

    static constexpr float CELL_SIZE{ 32.0f };  // constexpr specifies that the value of an object or a function can be evaluated at compile-time and the expression can be used in other constant expressions. Need this to set in other classes.

private:
    void Draw();
    void PollEvents();
    void FPSTimer(const float deltaTime);
    void DrawTraversableMapPersistant() const;
    void InitializeObjects();
    void CheckCharacterContact();
    void ShowGameText();
    void ReadLayoutFromFile();

    std::unique_ptr<sf::RenderWindow> mWindow{};
    std::unique_ptr<sf::Event> mEvent{};

    float mFpsTimer{};
    GameState mGameState{ GameState::Start };
    sf::Font mFont{};
    sf::Vector2i mCurrentScreenPosition{};
    std::vector<std::string> mLevelLayout{};
    sf::Vector2i mGridSize{};

    // Objects
    std::vector<std::shared_ptr<Object>> mObjects{};
    std::shared_ptr<Player> mPlayer{};

    static std::shared_ptr<Grid> mGrid;
    static std::shared_ptr<DrawDebug> mDrawDebug;
    static std::shared_ptr<Pathfinder> mPathfinder;
    static std::shared_ptr<EnemyManager> mEnemyManager;
};

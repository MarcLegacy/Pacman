#pragma once

#include <memory>
#include <vector>

#include "Pathfinder.h"

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

    static constexpr float CELL_SIZE{ 32.0f };  // constexpr specifies that the value of an object or a function can be evaluated at compile-time and the expression can be used in other constant expressions. Need this to make set in other classes.

private:
    void Draw();
    void PollEvents() const;
    void FPSTimer(const float deltaTime);
    void DrawTraversableMap() const;
    void InitializeObjects();

    const int SCREEN_WIDTH{ 896 };
    const int SCREEN_HEIGHT{ 992 };


    std::unique_ptr<sf::RenderWindow> mWindow{};
    std::unique_ptr<sf::Event> mEvent{};

    float mFpsTimer{};

    // Objects
    std::vector<std::shared_ptr<Object>> mObjects{};
    static std::shared_ptr<Grid> mGrid;
    static std::shared_ptr<DrawDebug> mDrawDebug;
    static std::shared_ptr<Pathfinder> mPathfinder;
    std::shared_ptr<Player> mPlayer{};
    std::vector<std::shared_ptr<Enemy>> mEnemies{};
};

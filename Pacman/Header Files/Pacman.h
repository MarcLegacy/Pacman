#pragma once

#include <memory>
#include <vector>

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
    std::shared_ptr<Player> mPlayer{};
};

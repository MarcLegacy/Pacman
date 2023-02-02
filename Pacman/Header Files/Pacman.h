#pragma once

#include <memory>
#include <vector>

#include "Grid.h"

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
    ~Pacman() = default;

    bool IsRunning() const;
    void Update(float deltaTime);
    void Render();

private:
    void Draw();
    void PollEvents();
    void FPSTimer(float deltaTime);
    void DrawTraversableMap();
    void InitializeObjects();

    const int SCREEN_WIDTH{ 896 };
    const int SCREEN_HEIGHT{ 992 };

    std::unique_ptr<sf::RenderWindow> mWindow{};
    std::unique_ptr<sf::Event> mEvent{};

    float mFpsTimer{};

    // Objects
    std::vector<std::shared_ptr<Object>> mObjects{};
    std::shared_ptr<Grid> mGrid{};
    std::shared_ptr<DrawDebug> mDrawDebug{};
    std::shared_ptr<Player> mPlayer{};

};

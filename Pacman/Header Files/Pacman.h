#pragma once

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

    void Update(float deltaTime);
    void Draw();
    void Render();
    void PollEvents();

private:
    sf::RenderWindow* mWindow;
    sf::Event* mEvent;
};

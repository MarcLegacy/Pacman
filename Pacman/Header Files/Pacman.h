#pragma once
#include <memory>
#include <vector>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class Object;

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
    void Render();

private:
    void Draw();
    void PollEvents();
    void SetupGrid(int width, int height, float cellSize, sf::Vector2f originPosition);
    int RetrieveNumberFromLayout(sf::Vector2i gridPosition);
    void FPSTimer(float deltaTime);

    const int SCREEN_WIDTH{ 896 };
    const int SCREEN_HEIGHT{ 992 };

    sf::RenderWindow* mWindow{};
    sf::Event* mEvent{};

    sf::Texture texture{};
    sf::Sprite sprite{};

    std::vector<std::unique_ptr<Object>> mObjects{};

    float mFpsTimer{};
};

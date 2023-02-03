#pragma once

#include <memory>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Shape.hpp>

#include "Object.h"

namespace sf
{
    class RenderWindow;
}

class DrawDebug : public Object
{
public:
    void Update(float deltaTime) override;
    void Draw(sf::RenderTarget* target) override;

    void DrawLinePersistant(const sf::Vector2f fromPosition, const sf::Vector2f toPosition, const sf::Color& color = sf::Color::White);
    std::array<sf::Vertex, 2> DrawLine(const sf::Vector2f fromPosition, const sf::Vector2f toPosition, const sf::Color& color = sf::Color::White);

    void DrawCellPersistant(const sf::Vector2f cellWorldPosition, const float cellSize = 32.0f, const sf::Color& color = sf::Color::White);
    static sf::RectangleShape DrawCell(const sf::Vector2f cellWorldPosition, const float cellSize = 32.0f, const sf::Color& color = sf::Color::White);

private:
    std::vector<std::array<sf::Vertex, 2>> mDrawLines{};
    std::vector<sf::RectangleShape> mDrawRectangleShapes{}; // Can't use the 'Shape' class as it is an abstract class.

    std::shared_ptr<sf::RenderWindow> mWindow{};
};


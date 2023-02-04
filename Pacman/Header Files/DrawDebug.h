#pragma once

#include <array>
#include <memory>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Shape.hpp>

#include "GlobalEnums.h"
#include "Object.h"
#include "Pacman.h"

namespace sf
{
    class RenderWindow;
}

class DrawDebug : public Object
{
public:
    void Update(float deltaTime) override;
    void Draw(sf::RenderTarget* target) override;

    static std::array<sf::Vertex, 2> DrawLine(const sf::Vector2f fromPosition, const sf::Vector2f toPosition, const sf::Color& color = sf::Color::White);
    void DrawLinePersistant(const sf::Vector2f fromPosition, const sf::Vector2f toPosition, const sf::Color& color = sf::Color::White) { mDrawLines.push_back(DrawLine(fromPosition, toPosition, color)); }

    static sf::RectangleShape DrawCell(const sf::Vector2f cellWorldPosition, const float cellSize = Pacman::CELL_SIZE, const sf::Color& color = sf::Color::White);
    void DrawCellPersistant(const sf::Vector2f cellWorldPosition, const float cellSize = Pacman::CELL_SIZE, const sf::Color& color = sf::Color::White) { mDrawRectangleShapes.push_back(DrawCell(cellWorldPosition, cellSize, color)); }

    static std::array<sf::Vertex, 5> DrawArrow(const sf::Vector2f position, const Direction direction, const float size, const sf::Color& color = sf::Color::White);
    void DrawArrowPersistant(const sf::Vector2f position, const Direction direction, const float size, const sf::Color& color = sf::Color::White) { mDrawArrows.push_back(DrawArrow(position, direction, size, color)); }

private:
    std::vector<std::array<sf::Vertex, 2>> mDrawLines{};
    std::vector<sf::RectangleShape> mDrawRectangleShapes{}; // Can't use the 'Shape' class as it is an abstract class.
    std::vector<std::array<sf::Vertex, 5>> mDrawArrows{};

    std::shared_ptr<sf::RenderWindow> mWindow{};
};


#include "DrawDebug.h"

#include <array>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

void DrawDebug::Update(float deltaTime)
{
}

void DrawDebug::Draw(sf::RenderTarget* target)
{
    for (const auto& drawLine : mDrawLines)
    {
        target->draw(&drawLine[0], 2, sf::Lines);
    }

    for (const auto& drawShape : mDrawRectangleShapes)
    {
        target->draw(drawShape);
    }
}

void DrawDebug::DrawLine(const sf::Vector2f fromPosition, const sf::Vector2f toPosition, const sf::Color& color)
{
    const std::array<sf::Vertex, 2> line =
    {
        sf::Vertex(fromPosition, color),
        sf::Vertex(toPosition, color)
    };

    mDrawLines.push_back(line);
}

void DrawDebug::DrawCell(const sf::Vector2f cellWorldPosition, const float cellSize, const sf::Color& color)
{
    sf::RectangleShape shape{};

    shape.setPosition(cellWorldPosition);
    shape.setSize({ cellSize, cellSize });
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(color);
    shape.setOutlineThickness(1.0f);

    mDrawRectangleShapes.push_back(shape);
}



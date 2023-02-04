#include "DrawDebug.h"

#include <array>
#include <iostream>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

void DrawDebug::Update(float deltaTime)
{
}

void DrawDebug::Draw(sf::RenderTarget* target)
{
    for (const auto& drawLine : mDrawLines)
    {
        target->draw(&drawLine[0], drawLine.size(), sf::Lines);
    }

    for (const auto& drawShape : mDrawRectangleShapes)
    {
        target->draw(drawShape);
    }

    for (const auto& drawLine : mDrawArrows)
    {
        target->draw(&drawLine[0], drawLine.size(), sf::LineStrip);
    }
}

std::array<sf::Vertex, 2> DrawDebug::DrawLine(const sf::Vector2f fromPosition, const sf::Vector2f toPosition, 
    const sf::Color& color)
{
    const std::array<sf::Vertex, 2> line =
    {
        sf::Vertex(fromPosition, color),
        sf::Vertex(toPosition, color)
    };

    return line;
}

sf::RectangleShape DrawDebug::DrawCell(const sf::Vector2f cellWorldPosition, const float cellSize, 
    const sf::Color& color)
{
    sf::RectangleShape shape{};

    shape.setPosition(cellWorldPosition);
    shape.setSize({ cellSize, cellSize });
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(color);
    shape.setOutlineThickness(1.0f);

    return shape;
}

std::array<sf::Vertex, 5> DrawDebug::DrawArrow(const sf::Vector2f position, const Direction direction, const float size,
    const sf::Color& color)
{
    sf::Vector2f floatDirection;
    switch (direction)
    {
    case Direction::Up:
        floatDirection = { 0, -1 };
        break;
    case Direction::Down:
        floatDirection = { 0, 1 };
        break;
    case Direction::Left:
        floatDirection = { -1, 0 };
        break;
    case Direction::Right:
        floatDirection = { 1, 0 };
        break;
    }

    const auto startPosition = sf::Vector2f(position.x - floatDirection.x * size * 0.5f, position.y - floatDirection.y * size * 0.5f);
    const auto endPosition = sf::Vector2f(position.x + floatDirection.x * size * 0.5f, position.y + floatDirection.y * size * 0.5f);
    const auto leftPosition = sf::Vector2f(position.x - floatDirection.y * size * 0.5f, position.y - floatDirection.x * size * 0.5f);
    const auto rightPosition = sf::Vector2f(position.x + floatDirection.y * size * 0.5f, position.y + floatDirection.x * size * 0.5f);

    const std::array<sf::Vertex, 5> arrow =
    {
        sf::Vertex(startPosition, color),
        sf::Vertex(endPosition, color),
        sf::Vertex(leftPosition, color),
        sf::Vertex(endPosition, color),
        sf::Vertex(rightPosition, color)
    };

    return arrow;
}





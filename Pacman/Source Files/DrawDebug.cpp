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
}

void DrawDebug::DrawLine(const sf::Vector2f fromPosition, const sf::Vector2f toPosition, const sf::Color color)
{
    const std::array<sf::Vertex, 2> line =
    {
        sf::Vertex(fromPosition, color),
        sf::Vertex(toPosition, color)
    };

    mDrawLines.push_back(line);
}

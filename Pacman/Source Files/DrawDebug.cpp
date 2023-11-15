#include "DrawDebug.h"

#include <array>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Grid.h"
#include "Pacman.h"
#include "Utility.h"

DrawDebug::DrawDebug()
{
    mFont.loadFromFile("Resource Files/Font.ttf");
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

    for (const auto& drawArrow : mDrawArrows)
    {
        target->draw(&drawArrow[0], drawArrow.size(), sf::LineStrip);
    }

    for (const auto& drawText : mDrawTexts)
    {
        target->draw(drawText);
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

    // Points are calculated in a 45 degrees square.
    const auto startPosition = sf::Vector2f(position.x - floatDirection.x * size * 0.5f, position.y - floatDirection.y * size * 0.5f);
    const auto endPosition = sf::Vector2f(position.x + floatDirection.x * size * 0.5f, position.y + floatDirection.y * size * 0.5f);
    const auto leftPosition = sf::Vector2f(position.x - floatDirection.y * size * 0.5f, position.y - floatDirection.x * size * 0.5f);
    const auto rightPosition = sf::Vector2f(position.x + floatDirection.y * size * 0.5f, position.y + floatDirection.x * size * 0.5f);

    // The vertices are then set in order so that the lines that are drawn form an arrow.
    const std::array<sf::Vertex, 5> arrow =
    {
        sf::Vertex{startPosition, color},
        sf::Vertex{endPosition, color},
        sf::Vertex{leftPosition, color},
        sf::Vertex{endPosition, color},
        sf::Vertex{rightPosition, color}
    };

    return arrow;
}



std::vector<std::array<sf::Vertex, 5>> DrawDebug::DrawPathArrows(const std::vector<sf::Vector2f>& positions, 
    const float size, const sf::Color& color)
{
    const auto& grid = Pacman::GetGrid();
    std::vector<std::array<sf::Vertex, 5>> pathArrows;

    for (unsigned int i = 1; i < positions.size(); ++i)
    {
        pathArrows.push_back(DrawArrow(grid->GetCellCenterPosition(positions[i]), Utility::ConvertGridDirectionToDirection(grid->GetCellGridPosition(positions[i]) - grid->GetCellGridPosition(positions[i - 1])), size, color));
    }

    return pathArrows;
}

void DrawDebug::DrawPathArrowsPersistant(const std::vector<sf::Vector2f>& positions, const float size,
    const sf::Color& color)
{
    for (const auto& pathArrow: DrawPathArrows(positions, size, color))
    {
        mDrawArrows.push_back(pathArrow);
    }
}

std::vector<std::array<sf::Vertex, 5>> DrawDebug::DrawPathArrows(const std::vector<sf::Vector2i>& positions, const float size, const sf::Color& color)
{
    const auto& grid = Pacman::GetGrid();
    std::vector<std::array<sf::Vertex, 5>> pathArrows;

    for (unsigned int i = 1; i < positions.size(); ++i)
    {
        pathArrows.push_back(DrawArrow(grid->GetCellCenterPosition(positions[i]), Utility::ConvertGridDirectionToDirection(positions[i] - positions[i - 1]), size, color));
    }

    return pathArrows;
}

void DrawDebug::DrawPathArrowsPersistant(const std::vector<sf::Vector2i>& positions, const float size,
    const sf::Color& color)
{
    for (const auto& pathArrow : DrawPathArrows(positions, size, color))
    {
        mDrawArrows.push_back(pathArrow);
    }
}

sf::Text DrawDebug::DrawText(const std::string& text, const sf::Vector2f position, const sf::Color& color) const
{
    sf::Text textToDraw;

    textToDraw.setFont(mFont);
    textToDraw.setString(text);
    textToDraw.setPosition(position);
    textToDraw.setFillColor(color);

    return textToDraw;
}

sf::Text DrawDebug::DrawCellCost(const int cost, const sf::Vector2i gridPosition, const sf::Color& color) const
{
    sf::Text text{ DrawText(std::to_string(cost), Pacman::GetGrid()->GetCellWorldPosition(gridPosition), color) };
    // This is needed to be able to read different sizes of numbers in a cell.
    if (cost < 10)
    {
        text.setPosition(text.getPosition() + sf::Vector2f{ 10.0f, -6.0f });
    }
    else if (cost < 100)
    {
        text.setLetterSpacing(0.1f);
        text.setCharacterSize(24);
        text.setPosition(text.getPosition() + sf::Vector2f{ 6.0f, -1.0f });
    }
    else if (cost < 1000)
    {
        text.setLetterSpacing(0.1f);
        text.setCharacterSize(16);
        text.setPosition(text.getPosition() + sf::Vector2f{ 5.0f, 4.0f });
    }
    else
    {
        text.setLetterSpacing(0.1f);
        text.setCharacterSize(8);
    }

    return text;
}





#pragma once

#include <array>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

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
    DrawDebug();

    void Update(float deltaTime) override;
    void Draw(sf::RenderTarget* target) override;

    static std::array<sf::Vertex, 2> DrawLine(const sf::Vector2f fromPosition, const sf::Vector2f toPosition, const sf::Color& color = sf::Color::White);
    void DrawLinePersistant(const sf::Vector2f fromPosition, const sf::Vector2f toPosition, const sf::Color& color = sf::Color::White) { mDrawLines.push_back(DrawLine(fromPosition, toPosition, color)); }

    static sf::RectangleShape DrawCell(const sf::Vector2f cellWorldPosition, const float cellSize = Pacman::CELL_SIZE, const sf::Color& color = sf::Color::White);
    void DrawCellPersistant(const sf::Vector2f cellWorldPosition, const float cellSize = Pacman::CELL_SIZE, const sf::Color& color = sf::Color::White) { mDrawRectangleShapes.push_back(DrawCell(cellWorldPosition, cellSize, color)); }

    static std::array<sf::Vertex, 5> DrawArrow(const sf::Vector2f position, const Direction direction, const float size, const sf::Color& color = sf::Color::White);
    void DrawArrowPersistant(const sf::Vector2f position, const Direction direction, const float size, const sf::Color& color = sf::Color::White) { mDrawArrows.push_back(DrawArrow(position, direction, size, color)); }

    static std::vector<std::array<sf::Vertex, 5>> DrawPathArrows(const std::vector<sf::Vector2f>& positions, const float size, const sf::Color& color = sf::Color::White);
    void DrawPathArrowsPersistant(const std::vector<sf::Vector2f>& positions, const float size, const sf::Color& color = sf::Color::White);
    static std::vector<std::array<sf::Vertex, 5>> DrawPathArrows(const std::vector<sf::Vector2i>& positions, const float size, const sf::Color& color = sf::Color::White);
    void DrawPathArrowsPersistant(const std::vector<sf::Vector2i>& positions, const float size, const sf::Color& color = sf::Color::White);

    sf::Text DrawText(const std::string& text, const sf::Vector2f position, const sf::Color& color = sf::Color::White) const;
    void DrawTextPersistant(const std::string& text, const sf::Vector2f position, const sf::Color& color = sf::Color::White) { mDrawTexts.push_back(DrawText(text, position, color)); }

    // Draw's the passed cost in a cell. Won't space well will negative numbers
    sf::Text DrawCellCost(const int cost, const sf::Vector2i gridPosition, const sf::Color& color = sf::Color::White) const;
    // Draw's the passed cost in a cell. Won't space well will negative numbers
    void DrawCellCostPersistant(const int cost, const sf::Vector2i gridPosition, const sf::Color& color = sf::Color::White) { mDrawTexts.push_back(DrawCellCost(cost, gridPosition, color)); }

private:
    std::vector<std::array<sf::Vertex, 2>> mDrawLines{};
    std::vector<sf::RectangleShape> mDrawRectangleShapes{}; // Can't use the 'Shape' class as it is an abstract class.
    std::vector<std::array<sf::Vertex, 5>> mDrawArrows{};
    std::vector<sf::Text> mDrawTexts{};

    sf::Font mFont;
};


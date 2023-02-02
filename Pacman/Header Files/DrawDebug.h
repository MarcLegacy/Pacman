#pragma once

#include "Object.h"

class DrawDebug : public Object
{
public:
    void Update(float deltaTime) override;
    void Draw(sf::RenderTarget* target) override;

    void DrawLine(const sf::Vector2f fromPosition, const sf::Vector2f toPosition, const sf::Color color = sf::Color::White);

private:
    std::vector<std::array<sf::Vertex, 2>> mDrawLines;
};


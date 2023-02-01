#pragma once

#include <string>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

class Object
{
public:
    Object() = default;
    Object(const sf::Vector2f position);
    Object(const sf::Vector2f position, const std::string& textureFileName);
    virtual ~Object() = default;

    virtual void Update(float deltaTime) = 0;
    virtual void Draw(sf::RenderTarget* target);

protected:
    sf::Vector2f mPosition{};
    sf::Sprite mSprite{};
    sf::Texture mTexture{};
};


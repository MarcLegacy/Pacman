#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

class Object
{
public:
    Object() = default;
    Object(const sf::Vector2f position);
    Object(const sf::Vector2f position, const sf::Texture& texture);
    virtual ~Object() = default;

    virtual void Update(float deltaTime) = 0;
    virtual void Draw(sf::RenderTarget* target);

    sf::Vector2f GetPosition() const { return mPosition; }

    // Returns the position + center of the texture.
    sf::Vector2f GetCenterPosition() const { return mPosition + sf::Vector2f(static_cast<float>(mTexture.getSize().x), static_cast<float>(mTexture.getSize().y)) * 0.5f; }

protected:
    sf::Vector2f mPosition{};
    sf::Sprite mSprite{};
    sf::Texture mTexture{};
};


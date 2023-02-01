#include "Object.h"

#include <iostream>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

Object::Object(const sf::Vector2f position) : mPosition(position)
{
    mSprite.setPosition(mPosition);
}

Object::Object(const sf::Vector2f position, const std::string& textureFileName) : Object(position)
{
    if (mTexture.loadFromFile(textureFileName))
    {
        mSprite.setTexture(mTexture);
    }
}

void Object::Draw(sf::RenderTarget* target)
{
    if (mSprite.getTexture() == nullptr) return;

    target->draw(mSprite);
}

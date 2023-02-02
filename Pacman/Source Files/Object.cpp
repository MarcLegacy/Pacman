#include "Object.h"

#include <SFML/Graphics/RenderTarget.hpp>

Object::Object(const sf::Vector2f position) : mPosition(position)
{
    mSprite.setPosition(mPosition);
}

void Object::Draw(sf::RenderTarget* target)
{
    if (mSprite.getTexture() == nullptr) return;

    target->draw(mSprite);
}

#include "Pacman.h"

#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>

Pacman::Pacman() : mEvent(new sf::Event)
{
    const sf::VideoMode videoMode = sf::VideoMode(800, 600);
    mWindow = new sf::RenderWindow(sf::VideoMode(videoMode), "Pacman", sf::Style::Titlebar | sf::Style::Close);
    mWindow->setFramerateLimit(60);
}

Pacman::~Pacman()
{
    delete mWindow;
}

bool Pacman::IsRunning() const
{
    return mWindow->isOpen();
}

void Pacman::Update(float deltaTime)
{
    PollEvents();
}

void Pacman::Draw()
{

}

void Pacman::Render()
{
    mWindow->clear(sf::Color(0, 0, 0, 255));
    Draw();
    mWindow->display();
}

void Pacman::PollEvents()
{
    while (mWindow->pollEvent(*mEvent))
    {
        switch (mEvent->type)
        {
        case sf::Event::Closed:
            mWindow->close();
            break;
        case sf::Event::KeyPressed:
            if (mEvent->key.code == sf::Keyboard::Escape)
                mWindow->close();
            break;
        default:
            break;
        }
    }
}
#include <SFML/Graphics.hpp>
#include "Pacman.h"

int main()
{
    Pacman pacman;

    sf::Clock clock;

    while (pacman.IsRunning())
    {
        pacman.Update(clock.restart().asSeconds());
        pacman.Render();
    }

    return 0;
}
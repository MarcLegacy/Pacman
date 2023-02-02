#include "Pacman.h"

#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "DrawDebug.h"
#include "Grid.h"
#include "Object.h"
#include "Player.h"

Pacman::Pacman()
{
    mEvent = std::make_unique<sf::Event>();
    const sf::VideoMode videoMode = sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT);
    mWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(videoMode), "Pacman", sf::Style::Close);
    mWindow->setFramerateLimit(60);

    InitializeObjects();
}


bool Pacman::IsRunning() const
{
    return mWindow->isOpen();
}

void Pacman::Update(float deltaTime)
{
    PollEvents();

    for (const auto& object : mObjects)
    {
        object->Update(deltaTime);
    }

    FPSTimer(deltaTime);
}

void Pacman::Draw()
{
    for (const auto& object : mObjects)
    {
        object->Draw(mWindow.get());
    }
}

void Pacman::Render()
{
    mWindow->clear();
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

void Pacman::FPSTimer(float deltaTime)
{
    if (mFpsTimer < 1.0f)
    {
        mFpsTimer += deltaTime;
    }
    else
    {
        mFpsTimer = 0.0f;
        std::cout << "FPS: " << 1 / deltaTime << std::endl;
    }
}

void Pacman::DrawTraversableMap()
{
    for (const auto& traversableCells : mGrid->GetTraversableCellMap())
    {
        for (const auto& cell : traversableCells.second)
        {
            mDrawDebug->DrawLine(mGrid->GetCellCenterPosition(traversableCells.first->GetPosition()), mGrid->GetCellCenterPosition(cell->GetPosition()));
        }
    }
}

void Pacman::InitializeObjects()
{
    mGrid = std::make_shared<Grid>(28, 31, 32.0f);
    mObjects.push_back(mGrid);
    mDrawDebug = std::make_shared<DrawDebug>();
    mObjects.push_back(mDrawDebug);
    mPlayer = std::make_shared<Player>(mGrid->GetPlayerSpawnPosition());
    mObjects.push_back(mPlayer);

    mDrawDebug->DrawCell(mPlayer->GetPosition(), 32.0f, sf::Color::Red);
}




#include "Pacman.h"

#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "DrawDebug.h"
#include "Enemy.h"
#include "Grid.h"
#include "Object.h"
#include "Pathfinder.h"
#include "Player.h"

std::shared_ptr<Grid> Pacman::mGrid{};
std::shared_ptr<DrawDebug> Pacman::mDrawDebug{};
std::shared_ptr<Pathfinder> Pacman::mPathfinder{};

Pacman::Pacman()
{
    mEvent = std::make_unique<sf::Event>();
    const sf::VideoMode videoMode = sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT);
    mWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(videoMode), "Pacman", sf::Style::Close);
    mWindow->setFramerateLimit(60);

    InitializeObjects();

    //for (const auto& cellGridPosition: Pathfinder::AStar(mGrid->GetCellGridPosition(mPlayer->GetPosition()), mGrid->GetCellGridPosition(mEnemies[0]->GetPosition())))
    //{
    //    mDrawDebug->DrawCellCostPersistant(0, cellGridPosition);
    //}

    //mDrawDebug->DrawPathArrowsPersistant(Pathfinder::AStar(mGrid->GetCellGridPosition(mPlayer->GetCenterPosition()), mGrid->GetCellGridPosition(mEnemies[0]->GetCenterPosition())), 24.0f);

    //mDrawDebug->DrawPathArrowsPersistant(Pathfinder::AStar(sf::Vector2i{ 1, 1 }, sf::Vector2i{ 1, 5 }), 24.0f);
}

Pacman::~Pacman()
{
    if (mGrid != nullptr)
    {
        mGrid = nullptr;
    }

    if (mDrawDebug != nullptr)
    {
        mDrawDebug = nullptr;
    }
}


bool Pacman::IsRunning() const
{
    return mWindow->isOpen();
}

void Pacman::Update(const float deltaTime)
{
    PollEvents();

    for (const auto& object : mObjects)
    {
        object->Update(deltaTime);
    }

    FPSTimer(deltaTime);

    if (mPlayer->GetOnCellChanged())
    {
        for (const auto& enemy : mEnemies)
        {
            enemy->FindPath(mGrid->GetCellGridPosition(mPlayer->GetCenterPosition()));
        }
    }
}

void Pacman::Draw()
{
    for (const auto& object : mObjects)
    {
        object->Draw(mWindow.get());
    }

    //const auto& arrow = mDrawDebug->DrawArrow({ 500, 500 }, Direction::Right, 500, sf::Color::Red);
    //mWindow->draw(&arrow[0], arrow.size(), sf::LineStrip);

    //mWindow->draw(mDrawDebug->DrawText("something!", mGrid->GetCellWorldPosition(mPlayer->GetCenterPosition())));
}

void Pacman::Render()
{
    mWindow->clear();
    Draw();
    mWindow->display();
}

void Pacman::PollEvents() const
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

void Pacman::FPSTimer(const float deltaTime)
{
    if (mFpsTimer < 1.0f)
    {
        mFpsTimer += deltaTime;
    }
    else
    {
        mFpsTimer = 0.0f;
        std::cout << "FPS: " << floorf(1 / deltaTime * 100) / 100 << std::endl;
    }
}

void Pacman::DrawTraversableMap() const
{
    for (const auto& traversableCells : mGrid->GetTraversableCellMap())
    {
        for (const auto& cell : traversableCells.second)
        {
            mDrawDebug->DrawLinePersistant(mGrid->GetCellCenterPosition(traversableCells.first->GetPosition()), mGrid->GetCellCenterPosition(cell->GetPosition()));
        }
    }
}

void Pacman::InitializeObjects()
{
    mGrid = std::make_shared<Grid>(28, 31, CELL_SIZE);
    mObjects.push_back(mGrid);
    mDrawDebug = std::make_shared<DrawDebug>();
    mObjects.push_back(mDrawDebug);
    mPlayer = std::make_shared<Player>(mGrid->GetPlayerSpawnPosition());
    mObjects.push_back(mPlayer);
    const auto enemyBlue = std::make_shared<Enemy>(mGrid->GetEnemySpawnPosition(0), SkinColor::Blue);
    mEnemies.push_back(enemyBlue);
    const auto enemyRed = std::make_shared<Enemy>(mGrid->GetEnemySpawnPosition(1), SkinColor::Red);
    mEnemies.push_back(enemyRed);
    const auto enemyOrange = std::make_shared<Enemy>(mGrid->GetEnemySpawnPosition(2), SkinColor::Orange);
    mEnemies.push_back(enemyOrange);
    mObjects.insert(mObjects.end(), mEnemies.begin(), mEnemies.end());

}






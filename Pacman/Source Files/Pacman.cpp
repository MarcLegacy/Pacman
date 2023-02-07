#include "Pacman.h"

#include <fstream>
#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "DrawDebug.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "Grid.h"
#include "Object.h"
#include "Pathfinder.h"
#include "Player.h"

std::shared_ptr<Grid> Pacman::mGrid{};
std::shared_ptr<DrawDebug> Pacman::mDrawDebug{};
std::shared_ptr<Pathfinder> Pacman::mPathfinder{};
std::shared_ptr<EnemyManager> Pacman::mEnemyManager{};

Pacman::Pacman()
{
    ReadLayoutFromFile();
    if (mLevelLayout.empty()) return;

    mEvent = std::make_unique<sf::Event>();
    const auto videoMode = sf::VideoMode(mLevelLayout[0].size() * static_cast<int>(CELL_SIZE), mLevelLayout.size() * static_cast<int>(CELL_SIZE));
    mWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(videoMode), "Pacman", sf::Style::Close);
    mWindow->setFramerateLimit(60);
    mWindow->setPosition({ mWindow->getPosition().x, 0 });
    mCurrentScreenPosition = mWindow->getPosition();

    mGridSize = { static_cast<int>(mLevelLayout[0].size()), static_cast<int>(mLevelLayout.size()) };
    mFont.loadFromFile("Resource Files/Font.ttf");

    InitializeObjects();

    mPathfinder = std::make_shared<Pathfinder>();
}

Pacman::~Pacman()
{
    // This is needed, otherwise the default deconstructor cannot properly remove these objects.
    if (mGrid != nullptr)
    {
        mGrid = nullptr;
    }

    if (mDrawDebug != nullptr)
    {
        mDrawDebug = nullptr;
    }

    if (mPathfinder != nullptr)
    {
        mPathfinder = nullptr;
    }

    if (mEnemyManager != nullptr)
    {
        mEnemyManager = nullptr;
    }
}


bool Pacman::IsRunning() const
{
    return mWindow->isOpen();
}

void Pacman::Update(const float deltaTime)
{
    PollEvents();
    //FPSTimer(deltaTime);

    // Makes sure that when the screen is dragged, that the game is paused to avoid creating a big difference in delta time and giving the player time to prepare for the game.
    if (mCurrentScreenPosition != mWindow->getPosition())
    {
        mCurrentScreenPosition = mWindow->getPosition();
        mGameState = GameState::Paused;
    }

    if (mGameState != GameState::Playing) return;

    for (const auto& object : mObjects)
    {
        object->Update(deltaTime);
    }

    CheckCharacterContact();
}

void Pacman::Draw()
{
    for (const auto& object : mObjects)
    {
        object->Draw(mWindow.get());
    }

    ShowGameText();
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
            if (mEvent->key.code == sf::Keyboard::Space)
            {
                if (mGameState == GameState::Start || mGameState == GameState::Paused)
                {
                    mGameState = GameState::Playing;
                }
                else if (mGameState != GameState::GameOver)
                {
                    mGameState = GameState::Paused;
                }
            }
            break;
        default:
            break;
        }
    }

}

void Pacman::FPSTimer(const float deltaTime)
{
    // TODO: Get the average over time instead of one frame, because this is not accurate.
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

void Pacman::DrawTraversableMapPersistant() const
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
    mGrid = std::make_shared<Grid>(mGridSize.x, mGridSize.y, CELL_SIZE, mLevelLayout);
    mObjects.push_back(mGrid);
    mDrawDebug = std::make_shared<DrawDebug>();
    mObjects.push_back(mDrawDebug);
    mPlayer = std::make_shared<Player>(mGrid->GetPlayerSpawnPosition());
    mObjects.push_back(mPlayer);
    mEnemyManager = std::make_shared<EnemyManager>(mPlayer);
    mObjects.push_back(mEnemyManager);
}

void Pacman::CheckCharacterContact()
{
    for (const auto& enemy : mEnemyManager->GetEnemies())
    {
        if (mGrid->GetCellGridPosition(mPlayer->GetCenterPosition()) == mGrid->GetCellGridPosition(enemy->GetCenterPosition()))
        {
            mGameState = GameState::GameOver;
        }
    }
}

void Pacman::ShowGameText()
{
    // TODO: Have the positions set dynamically depending on the screen size.
    sf::Text text{};
    text.setFont(mFont);

    switch (mGameState)
    {
    case GameState::Start:
        text.setString("Press 'Space' to start.");
        text.setPosition(200.0f, 440.0f);
        break;
    case GameState::Playing:
        break;
    case GameState::Paused:
        text.setString("Press 'Space' to continue.");
        text.setPosition(180.0f, 440.0f);
        break;
    case GameState::GameOver:
        text.setString("Game Over!");
        text.setPosition(350.0f, 440.0f);
        break;
    }

    mWindow->draw(text);
}

void Pacman::ReadLayoutFromFile()
{
    std::vector<std::string> levelLayout;
    std::string line{};
    std::ifstream file("Resource Files/LevelLayout.txt");

    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            mLevelLayout.push_back(line);
        }

        file.close();
    }
    else
    {
        std::cout << "Could not open file!" << std::endl;
        std::cout << "make sure the txt file is inside the 'Resource Files' folder and called 'LevelLayout'!" << std::endl;
    }
}








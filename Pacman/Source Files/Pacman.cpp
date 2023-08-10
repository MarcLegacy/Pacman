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
#include "GlobalConstants.h"
#include "Grid.h"
#include "Object.h"
#include "Pathfinder.h"
#include "Player.h"

std::unique_ptr<Grid> Pacman::mGrid{};
std::unique_ptr<DrawDebug> Pacman::mDrawDebug{};
std::unique_ptr<Pathfinder> Pacman::mPathfinder{};
std::unique_ptr<EnemyManager> Pacman::mEnemyManager{};

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

    mPathfinder = std::make_unique<Pathfinder>();
}

Pacman::~Pacman()
{
    // This is needed, otherwise the default destructor cannot properly remove these objects.
    if (mGrid != nullptr)
    {
        mGrid = nullptr;
    }

    //if (mDrawDebug != nullptr)
    //{
    //    mDrawDebug = nullptr;
    //}

    //if (mPathfinder != nullptr)
    //{
    //    mPathfinder = nullptr;
    //}

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
    FPSTimer(deltaTime, 1.0f);

    // Makes sure that when the screen is dragged, that the game is paused to avoid creating a big difference in delta time and giving the player time to prepare for the game.
    if (mCurrentScreenPosition != mWindow->getPosition())
    {
        mCurrentScreenPosition = mWindow->getPosition();
        mGameState = GameState::Paused;
    }

    if (mGameState != GameState::Playing) return;

    mPlayer->Update(deltaTime);
    mEnemyManager->Update(deltaTime);

    CheckCharacterContact();
}

void Pacman::Draw()
{
    mPlayer->Draw(mWindow.get());
    mGrid->Draw(mWindow.get());
    mDrawDebug->Draw(mWindow.get());
    mEnemyManager->Draw(mWindow.get());

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

void Pacman::FPSTimer(const float deltaTime, const float logTimer) const
{
    static float fpsTimer = 0.0f;
    static int frameCount = 0;
    static float totalFPS = 0.0f;

    fpsTimer += deltaTime;
    frameCount++;
    totalFPS += 1.0f / deltaTime;

    if (fpsTimer >= logTimer)
    {
        const float averageFPS = totalFPS / static_cast<float>(frameCount);
        std::cout << "Average FPS: " << floorf(averageFPS * 100) / 100 << std::endl;

        fpsTimer = 0.0f;
        frameCount = 0;
        totalFPS = 0.0f;
    }
}

void Pacman::DrawTraversableMapPersistant() const
{
    for (const auto& traversableCells : mGrid->GetTraversableCellMap())
    {
        for (const auto& cellGridPosition : traversableCells.second)
        {
            mDrawDebug->DrawLinePersistant(mGrid->GetCellCenterPosition(traversableCells.first), mGrid->GetCellCenterPosition(cellGridPosition));
        }
    }
}

void Pacman::InitializeObjects()
{
    mGrid = std::make_unique<Grid>(mGridSize.x, mGridSize.y, CELL_SIZE, mLevelLayout);
    mDrawDebug = std::make_unique<DrawDebug>();
    mPlayer = std::make_unique<Player>(mGrid->GetPlayerSpawnPosition());
    mEnemyManager = std::make_unique<EnemyManager>(mPlayer);
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








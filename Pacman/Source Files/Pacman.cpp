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

    LoadFiles();
    InitializeObjects();
}

Pacman::~Pacman()
{
    // This is needed, otherwise the default destructor cannot properly remove these objects.
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
    FPSTimer(deltaTime, 1.0f);

    // Makes sure that when the screen is dragged, that the game is paused to avoid creating a big difference in delta time and giving the player time to prepare for the game.
    if (mCurrentScreenPosition != mWindow->getPosition())
    {
        mCurrentScreenPosition = mWindow->getPosition();
        mGameState = GameState::Paused;
    }

    if (mGameState != GameState::Playing) return;

    CheckCharacterContact();

    mPlayer->Update(deltaTime);
    mEnemyManager->Update(deltaTime);

    if (mPlayer->GetTotalPillsCaught() >= mGrid->GetTotalPillCount())
    {
        mGameState = GameState::Victory;
    }
}

void Pacman::Draw()
{
    mPlayer->Draw(mWindow.get());
    mGrid->Draw(mWindow.get());
    mDrawDebug->Draw(mWindow.get());
    mEnemyManager->Draw(mWindow.get());
    for (int i = 0; i < mPlayer->GetLives(); i++)
    {
        mWindow->draw(mLifeSprites[i]);
    }

    ShowGameText();
    ShowScoreText();
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
                else if (mGameState == GameState::GameOver || mGameState == GameState::Victory)
                {
                    ResetGame();
                }
                else
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
    mPlayer = std::make_unique<Player>(mGrid->GetPlayerSpawnPosition(), CHARACTER_SPEED, PLAYER_LIFE_AMOUNT);
    mEnemyManager = std::make_unique<EnemyManager>(mPlayer);
    mPathfinder = std::make_unique<Pathfinder>();
}

void Pacman::CheckCharacterContact()
{
    for (const auto& enemy : mEnemyManager->GetEnemies())
    {
        //if (mGrid->GetCellGridPosition(mPlayer->GetCenterPosition()) == mGrid->GetCellGridPosition(enemy->GetCenterPosition()))
        if (Utility::CalculateDistance(mPlayer->GetCenterPosition(), enemy->GetCenterPosition()) < CELL_SIZE)
        {
            switch (mEnemyManager->GetEnemyMode())
            {
            case EnemyMode::Chase:
                mPlayer->LoseLife();

                if (mPlayer->GetLives() <= 0)
                {
                    mGameState = GameState::GameOver;
                    return;
                }

                ResetPositions();
                mGameState = GameState::Paused;
                return;
            case EnemyMode::Scatter:
                enemy->ResetPosition();
                mPlayer->IncreaseScore(ENEMY_KILLED_SCORE);
                break;
            }
        }
    }
}

void Pacman::ShowGameText() const
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
        text.setString("        Game Over! \n    Your Score: " + std::to_string(mPlayer->GetScore()) + "\n Press 'Space' to restart.");
        text.setPosition(180.0f, 440.0f);
        break;
    case GameState::Victory:
        text.setString("        You've Won! \n    Your Score: " + std::to_string(mPlayer->GetScore()) + "\n Press 'Space' to restart.");
        text.setPosition(180.0f, 440.0f);
        break;
    }

    mWindow->draw(text);
}

void Pacman::ShowScoreText() const
{
    sf::Text text{};
    text.setFont(mFont);

    text.setString(std::to_string(mPlayer->GetScore()));
    text.setPosition(0.0f, 0.0f);

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

void Pacman::LoadFiles()
{
    if (!mFont.loadFromFile("Resource Files/Font.ttf"))
    {
        std::cout << "The program will be closed." << std::endl;
        exit(0);
    }

    if (!mLifeTexture.loadFromFile("Resource Files/Pac-Man_Sprite_Sheet.png", sf::IntRect(133, 18, 11, 11)))
    {
        std::cout << "The program will be closed." << std::endl;
        exit(0);
    }

    for (int i = 0; i < PLAYER_LIFE_AMOUNT; i++)
    {
        sf::Sprite lifeSprite{};
        lifeSprite.setTexture(mLifeTexture);
        // Left bottom positioning with middle of cell and middle of life sprite added.
        sf::Vector2f position{ CELL_SIZE * 0.5f - static_cast<float>(mLifeTexture.getSize().x) * 0.5f + CELL_SIZE * static_cast<float>(i), static_cast<float>(mWindow->getSize().y) - CELL_SIZE * 0.5f - static_cast<float>(mLifeTexture.getSize().y) * 0.5f};
        lifeSprite.setPosition(position);
        mLifeSprites.push_back(lifeSprite);
    }
}

void Pacman::ResetPositions() const
{
    mPlayer->ResetPosition();
    mEnemyManager->ResetPositions();
}

void Pacman::ResetGame()
{
    ResetPositions();
    mPlayer->ResetStats();
    mGrid->ResetPills();
    mGameState = GameState::Start;
}








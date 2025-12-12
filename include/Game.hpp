#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> // Indispensable para la música de Los Cafres
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include "Snake.hpp"

struct ScoreEntry {
    std::string name;
    int score;
};

enum GameState {
    MENU,
    ENTER_NAME,
    PLAYING,
    GAME_OVER
};

class Game {
public:
    Game();
    void run();

private:
    // Funciones principales
    void processEvents();
    void update(sf::Time deltaTime);
    void render();
    
    // Funciones del sistema de puntuación y comida
    void loadScores();
    void saveScore();
    void updateHighScores();
    void spawnFood();

    // Variables de control
    sf::RenderWindow mWindow;
    Snake mSnake;
    GameState mState;
    sf::Time mTimePerFrame;
    int mScore;
    std::string mPlayerName;

    // Recursos de Audio
    sf::Music mMusic; 

    // Recursos de Texto
    sf::Font mFont;
    sf::Text mTitleText;
    sf::Text mScoreText;
    sf::Text mInstructionsText;
    sf::Text mHighScoreText;

    // Recursos de Imagen (Texturas y Sprites)
    sf::Texture mMenuBackgroundTexture;
    sf::Sprite mMenuBackgroundSprite;

    sf::Texture mGameBackgroundTexture;
    sf::Sprite mGameBackgroundSprite;

    sf::Texture mEnterNameTexture;
    sf::Sprite mEnterNameSprite;

    sf::Texture mGameOverTexture;
    sf::Sprite mGameOverSprite;

    sf::RectangleShape mFood;
    std::vector<ScoreEntry> mHighScores;
};
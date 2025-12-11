#pragma once
#include <SFML/Graphics.hpp>
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
    void processEvents();
    void update(sf::Time deltaTime);
    void render();
    
    // Funciones del sistema
    void loadScores();
    void saveScore();
    void updateHighScores();
    void spawnFood();

    sf::RenderWindow mWindow;
    Snake mSnake;
    
    GameState mState;
    sf::Time mTimePerFrame;
    
    // Recursos
    sf::Font mFont;
    sf::Text mTitleText;
    sf::Text mScoreText;
    sf::Text mInstructionsText;
    sf::Text mHighScoreText;

    // --- NUEVO: Variables para el fondo del menú ---
    sf::Texture mMenuBackgroundTexture;
    sf::Sprite mMenuBackgroundSprite;
    
    // Datos juego
    int mScore;
    std::string mPlayerName;
    std::vector<ScoreEntry> mHighScores;
    
    sf::RectangleShape mFood;
};
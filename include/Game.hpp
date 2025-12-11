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
    
    // Recursos de Texto
    sf::Font mFont;
    sf::Text mTitleText;
    sf::Text mScoreText;
    sf::Text mInstructionsText;
    sf::Text mHighScoreText;

    // --- IMÁGENES DE FONDO ---
    sf::Texture mMenuBackgroundTexture;  // Menú Principal
    sf::Sprite mMenuBackgroundSprite;

    sf::Texture mGameBackgroundTexture;  // Juego
    sf::Sprite mGameBackgroundSprite;

    sf::Texture mEnterNameTexture;       // NUEVO: Pantalla Ingresar Nombre (Imagen1.png)
    sf::Sprite mEnterNameSprite;

    sf::Texture mGameOverTexture;        // NUEVO: Pantalla Game Over (Game_Over.png)
    sf::Sprite mGameOverSprite;
    
    // Datos juego
    int mScore;
    std::string mPlayerName;
    std::vector<ScoreEntry> mHighScores;
    
    sf::RectangleShape mFood;
};
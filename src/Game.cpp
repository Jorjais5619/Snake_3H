#include "../include/Game.hpp"
#include <iostream>

// Constructor
Game::Game() 
: mWindow(sf::VideoMode(800, 600), "Snake Game - C++ SFML")
, mSnake(20) // Tamaño del bloque 20px
, mState(MENU)
, mScore(0)
, mPlayerName("")
{
    mWindow.setFramerateLimit(60);
    
    // --- 1. CARGA DE FUENTE ---
    if (!mFont.loadFromFile("assets/fonts/Arial.ttf")) {
        std::cerr << "ERROR CRITICO: No se encontro assets/fonts/Arial.ttf" << std::endl;
        exit(1);
    }

    // --- 2. CARGA DE FONDO DEL MENÚ ---
    if (!mMenuBackgroundTexture.loadFromFile("assets/Foto_Menu.png")) {
        std::cerr << "ADVERTENCIA: No se encontro assets/Foto_Menu.png. El menu sera negro." << std::endl;
    } else {
        mMenuBackgroundSprite.setTexture(mMenuBackgroundTexture);
        
        // Ajustar imagen al tamaño de la ventana (800x600)
        sf::Vector2u textureSize = mMenuBackgroundTexture.getSize();
        float scaleX = 800.0f / textureSize.x;
        float scaleY = 600.0f / textureSize.y;
        mMenuBackgroundSprite.setScale(scaleX, scaleY);

        // Opcional: Oscurecer un poco la imagen para que se lea el texto
        mMenuBackgroundSprite.setColor(sf::Color(220, 220, 220, 255));
    }

    // Configuración de textos
    mTitleText.setFont(mFont);
    mTitleText.setCharacterSize(50);
    mTitleText.setFillColor(sf::Color::White); // Blanco para resaltar sobre la imagen
    mTitleText.setOutlineColor(sf::Color::Black);
    mTitleText.setOutlineThickness(2);
    mTitleText.setPosition(220, 50);

    mInstructionsText.setFont(mFont);
    mInstructionsText.setCharacterSize(22);
    mInstructionsText.setFillColor(sf::Color::White);
    mInstructionsText.setOutlineColor(sf::Color::Black);
    mInstructionsText.setOutlineThickness(1);
    mInstructionsText.setPosition(200, 450);

    mScoreText.setFont(mFont);
    mScoreText.setCharacterSize(20);
    mScoreText.setPosition(10, 10);

    mHighScoreText.setFont(mFont);
    mHighScoreText.setCharacterSize(24);
    mHighScoreText.setFillColor(sf::Color::Yellow);
    mHighScoreText.setOutlineColor(sf::Color::Black);
    mHighScoreText.setOutlineThickness(1);
    mHighScoreText.setPosition(250, 150);

    mTimePerFrame = sf::seconds(0.15f);

    mFood.setSize(sf::Vector2f(20, 20));
    mFood.setFillColor(sf::Color::Red);
    spawnFood();

    loadScores();
}

void Game::run() {
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (mWindow.isOpen()) {
        processEvents();
        timeSinceLastUpdate += clock.restart();
        while (timeSinceLastUpdate > mTimePerFrame) {
            timeSinceLastUpdate -= mTimePerFrame;
            processEvents();
            if (mState == PLAYING) {
                update(mTimePerFrame);
            }
        }
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (mWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            mWindow.close();

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
             mWindow.close();

        if (mState == ENTER_NAME && event.type == sf::Event::TextEntered) {
            if (event.text.unicode == 8) { // Backspace
                if (!mPlayerName.empty()) mPlayerName.pop_back();
            }
            else if (event.text.unicode == 13) { // Enter
                if (!mPlayerName.empty()) mState = PLAYING;
            }
            else if (event.text.unicode < 128 && event.text.unicode > 31) {
                if (mPlayerName.size() < 10) mPlayerName += static_cast<char>(event.text.unicode);
            }
        }

        if (event.type == sf::Event::KeyPressed) {
            if (mState == MENU) {
                if (event.key.code == sf::Keyboard::Enter) {
                    mState = ENTER_NAME; 
                    mPlayerName = "";
                }
            }
            else if (mState == GAME_OVER) {
                if (event.key.code == sf::Keyboard::R) {
                    mSnake = Snake(20); 
                    mScore = 0;
                    mTimePerFrame = sf::seconds(0.15f); 
                    spawnFood();
                    mState = MENU;
                }
            }
        }
    }
    
    if (mState == PLAYING) {
        mSnake.handleInput();
    }
}

void Game::update(sf::Time deltaTime) {
    mSnake.update();

    if (mSnake.checkCollision()) {
        mState = GAME_OVER;
        updateHighScores();
        saveScore();
    }

    sf::Vector2i headPos = mSnake.getHeadPosition(); 
    if (headPos.x * 20 == mFood.getPosition().x && headPos.y * 20 == mFood.getPosition().y) {
        mSnake.grow();
        spawnFood();
        mScore += 10;
        
        if (mTimePerFrame.asSeconds() > 0.05f) {
            mTimePerFrame -= sf::seconds(0.005f); 
        }
    }
}

void Game::render() {
    mWindow.clear();

    if (mState == MENU) {
        // --- DIBUJAR FONDO PRIMERO ---
        if (mMenuBackgroundSprite.getTexture()) {
            mWindow.draw(mMenuBackgroundSprite);
        }

        mTitleText.setString("SNAKE GAME");
        mWindow.draw(mTitleText);

        std::string scoresStr = "TOP 3 JUGADORES:\n";
        if(mHighScores.empty()) scoresStr += "Sin registros aun.\n";
        for (const auto& entry : mHighScores) {
            scoresStr += entry.name + ": " + std::to_string(entry.score) + "\n";
        }
        mHighScoreText.setString(scoresStr);
        mWindow.draw(mHighScoreText);

        mInstructionsText.setString("Presiona ENTER para Jugar\nESC para Salir");
        mWindow.draw(mInstructionsText);
    }
    else if (mState == ENTER_NAME) {
        // En esta pantalla dejamos fondo negro para claridad, o puedes copiar el código del sprite si quieres fondo aquí también
        mTitleText.setString("Ingresa tu Nombre:");
        mWindow.draw(mTitleText);
        
        mInstructionsText.setString(mPlayerName + "_\n\n(ENTER para confirmar)");
        mWindow.draw(mInstructionsText);
    }
    else if (mState == PLAYING) {
        mSnake.render(mWindow);
        mWindow.draw(mFood);
        
        mScoreText.setString("Puntos: " + std::to_string(mScore) + " | " + mPlayerName);
        mWindow.draw(mScoreText);
    }
    else if (mState == GAME_OVER) {
        mTitleText.setString("GAME OVER");
        mWindow.draw(mTitleText);

        mInstructionsText.setString("Puntaje Final: " + std::to_string(mScore) + 
                                    "\n\n'R' para Menu\nESC para Salir");
        mWindow.draw(mInstructionsText);
    }

    mWindow.display();
}

void Game::spawnFood() {
    int cols = 800 / 20;
    int rows = 600 / 20;
    int x = rand() % cols;
    int y = rand() % rows;
    mFood.setPosition(x * 20, y * 20);
}

void Game::loadScores() {
    mHighScores.clear();
    std::ifstream file("scores.txt");
    if (file.is_open()) {
        std::string name;
        int score;
        while (file >> name >> score) {
            mHighScores.push_back({name, score});
        }
        file.close();
    }
    std::sort(mHighScores.begin(), mHighScores.end(), [](const ScoreEntry& a, const ScoreEntry& b) {
        return a.score > b.score;
    });
}

void Game::updateHighScores() {
    mHighScores.push_back({mPlayerName, mScore});
    std::sort(mHighScores.begin(), mHighScores.end(), [](const ScoreEntry& a, const ScoreEntry& b) {
        return a.score > b.score;
    });
    if (mHighScores.size() > 3) mHighScores.resize(3);
}

void Game::saveScore() {
    std::ofstream file("scores.txt"); 
    if (file.is_open()) {
        for (const auto& entry : mHighScores) {
            file << entry.name << " " << entry.score << "\n";
        }
        file.close();
    }
}
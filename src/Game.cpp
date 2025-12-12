#include "../include/Game.hpp"
#include <iostream>
#include <SFML/Audio.hpp>
#include <algorithm>
#include <fstream>

Game::Game()
: mWindow(sf::VideoMode(800, 600), "SNAKE MASTER - Arcade Edition")
, mSnake(20), mState(MENU), mScore(0), mPlayerName("")
{
    mWindow.setFramerateLimit(60);

    // --- 1. FUENTE ---
    if (!mFont.loadFromFile("assets/fonts/Arcade.ttf")) {
        mFont.loadFromFile("C:/Windows/Fonts/arial.ttf");
    }

    // --- 2. MÚSICA ---
    if (mMusic.openFromFile("assets/music/rolita.ogg")) {
        mMusic.setLoop(true);
        mMusic.setVolume(40);
        mMusic.play();
    }

    // --- 3. CARGA DE IMÁGENES (ASEGÚRATE QUE EL FORMATO .jpeg O .png SEA EL CORRECTO) ---
    // Si tus archivos son .png, cambia ".jpeg" por ".png" en las líneas de abajo:
    mMenuBackgroundTexture.loadFromFile("assets/images/Foto_Menu.jpeg");
    mMenuBackgroundSprite.setTexture(mMenuBackgroundTexture);
    mMenuBackgroundSprite.setScale(800.0f / mMenuBackgroundTexture.getSize().x, 600.0f / mMenuBackgroundTexture.getSize().y);

    mGameBackgroundTexture.loadFromFile("assets/images/Fondo_Juego.jpeg");
    mGameBackgroundSprite.setTexture(mGameBackgroundTexture);
    mGameBackgroundSprite.setScale(800.0f / mGameBackgroundTexture.getSize().x, 600.0f / mGameBackgroundTexture.getSize().y);

    mEnterNameTexture.loadFromFile("assets/images/Imagen1.jpeg");
    mEnterNameSprite.setTexture(mEnterNameTexture);
    mEnterNameSprite.setScale(800.0f / mEnterNameTexture.getSize().x, 600.0f / mEnterNameTexture.getSize().y);

    mGameOverTexture.loadFromFile("assets/images/Game_Over.jpeg");
    mGameOverSprite.setTexture(mGameOverTexture);
    mGameOverSprite.setScale(800.0f / mGameOverTexture.getSize().x, 600.0f / mGameOverTexture.getSize().y);

    // --- 4. CONFIGURACIÓN DE TEXTOS ---
    mTitleText.setFont(mFont);
    mTitleText.setOutlineColor(sf::Color::Black);
    mTitleText.setOutlineThickness(4);

    mHighScoreText.setFont(mFont);
    mHighScoreText.setOutlineColor(sf::Color::Black);
    mHighScoreText.setOutlineThickness(2);

    mInstructionsText.setFont(mFont);
    mInstructionsText.setOutlineColor(sf::Color::Black);
    mInstructionsText.setOutlineThickness(3);

    mScoreText.setFont(mFont);
    mScoreText.setFillColor(sf::Color::Cyan);
    mScoreText.setOutlineColor(sf::Color::Black);
    mScoreText.setOutlineThickness(2);
    mScoreText.setPosition(15, 15);

    mTimePerFrame = sf::seconds(0.15f);
    mFood.setSize(sf::Vector2f(20, 20));
    mFood.setFillColor(sf::Color::Cyan); 
    mFood.setOutlineColor(sf::Color::Black);
    mFood.setOutlineThickness(2);

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
            if (mState == PLAYING) update(mTimePerFrame);
        }
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (mWindow.pollEvent(event)) {
        // 1. CERRAR VENTANA (Con la X o con ESC)
        if (event.type == sf::Event::Closed) {
            mWindow.close();
        }
        
        if (event.type == sf::Event::KeyPressed) {
            // Tecla ESC funciona en CUALQUIER momento para salir
            if (event.key.code == sf::Keyboard::Escape) {
                mWindow.close();
            }

            // Controles del Menú y Game Over
            if (mState == MENU && event.key.code == sf::Keyboard::Enter) {
                mState = ENTER_NAME;
                mPlayerName = "";
            } 
            else if (mState == GAME_OVER && event.key.code == sf::Keyboard::R) {
                mSnake = Snake(20);
                mScore = 0;
                mTimePerFrame = sf::seconds(0.15f);
                spawnFood();
                mState = MENU;
                if (mMusic.getStatus() != sf::Music::Playing) mMusic.play();
            }
        }

        // 2. ESCRIBIR NOMBRE (Solo en estado ENTER_NAME)
        if (mState == ENTER_NAME && event.type == sf::Event::TextEntered) {
            if (event.text.unicode == 8) { // Borrar (Backspace)
                if (!mPlayerName.empty()) mPlayerName.pop_back();
            } else if (event.text.unicode == 13) { // Confirmar (Enter)
                if (!mPlayerName.empty()) mState = PLAYING;
            } else if (event.text.unicode < 128 && event.text.unicode > 31) {
                if (mPlayerName.size() < 10) mPlayerName += static_cast<char>(event.text.unicode);
            }
        }
    }

    // 3. MOVIMIENTO (Solo mientras juegas)
    if (mState == PLAYING) {
        mSnake.handleInput();
    }
}

void Game::update(sf::Time deltaTime) {
    mSnake.update();
    if (mSnake.checkCollision()) { mState = GAME_OVER; updateHighScores(); saveScore(); }
    if (mSnake.getHeadPosition().x * 20 == mFood.getPosition().x && mSnake.getHeadPosition().y * 20 == mFood.getPosition().y) {
        mSnake.grow(); spawnFood(); mScore += 10;
    }
}

void Game::render() {
    mWindow.clear();
    if (mState == MENU) {
        mWindow.draw(mMenuBackgroundSprite);
        mTitleText.setString("SNAKE GAME");
        mTitleText.setCharacterSize(35);
        mTitleText.setPosition(245, 215); // Centrado en pantalla verde
        mWindow.draw(mTitleText);

        std::string s = "TOP 3 JUGADORES:\n";
        for (const auto& e : mHighScores) s += e.name + ": " + std::to_string(e.score) + "\n";
        mHighScoreText.setString(s);
        mHighScoreText.setCharacterSize(18);
        mHighScoreText.setFillColor(sf::Color::Yellow);
        mHighScoreText.setPosition(275, 290);
        mWindow.draw(mHighScoreText);

        mInstructionsText.setString("Presiona ENTER para Jugar\n       ESC para Salir");
        mInstructionsText.setCharacterSize(16);
        mInstructionsText.setPosition(200, 520);
        mWindow.draw(mInstructionsText);
    } else if (mState == ENTER_NAME) {
        mWindow.draw(mEnterNameSprite);
        mTitleText.setString("Ingresa tu Nombre:");
        mTitleText.setCharacterSize(25);
        mTitleText.setPosition(190, 70);
        mWindow.draw(mTitleText);
        mInstructionsText.setString(mPlayerName + "_\n\n(ENTER para confirmar)");
        mInstructionsText.setPosition(220, 380);
        mWindow.draw(mInstructionsText);
    } else if (mState == PLAYING) {
        mWindow.draw(mGameBackgroundSprite);
        mSnake.render(mWindow);
        mWindow.draw(mFood);
        mScoreText.setString("Puntos: " + std::to_string(mScore) + " | " + mPlayerName);
        mWindow.draw(mScoreText);
    } else if (mState == GAME_OVER) {
        mWindow.draw(mGameOverSprite);
        mTitleText.setString("GAME OVER");
        mTitleText.setCharacterSize(55);
        mTitleText.setFillColor(sf::Color::Red);
        mTitleText.setPosition(220, 280);
        mWindow.draw(mTitleText);
        mInstructionsText.setString("Puntaje Final: " + std::to_string(mScore) + "\n\n'R' para Menu - ESC para Salir");
        mInstructionsText.setPosition(180, 420);
        mWindow.draw(mInstructionsText);
    }
    mWindow.display();
}

void Game::spawnFood() { mFood.setPosition((rand() % 40) * 20, (rand() % 30) * 20); }
void Game::loadScores() { 
    mHighScores.clear();
    std::ifstream f("scores.txt");
    if (f.is_open()) {
        std::string n; int sc;
        while (f >> n >> sc) mHighScores.push_back({n, sc});
        f.close();
    }
    std::sort(mHighScores.begin(), mHighScores.end(), [](const ScoreEntry& a, const ScoreEntry& b) { return a.score > b.score; });
}
void Game::updateHighScores() {
    mHighScores.push_back({mPlayerName, mScore});
    std::sort(mHighScores.begin(), mHighScores.end(), [](const ScoreEntry& a, const ScoreEntry& b) { return a.score > b.score; });
    if (mHighScores.size() > 3) mHighScores.resize(3);
}
void Game::saveScore() {
    std::ofstream f("scores.txt"); 
    if (f.is_open()) {
        for (const auto& e : mHighScores) f << e.name << " " << e.score << "\n";
        f.close();
    }
}
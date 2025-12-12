#include "../include/Game.hpp"
#include <iostream>

// Constructor
Game::Game()
: mWindow(sf::VideoMode(800, 600), "Snake Game - C++ SFML")
, mSnake(20)
, mState(MENU)
, mScore(0)
, mPlayerName("")
{
    mWindow.setFramerateLimit(60);

    // --- 1. CARGA DE FUENTE ARCADE ---
    if (!mFont.loadFromFile("assets/fonts/Arcade.ttf")) {
        std::cerr << "ADVERTENCIA: No se encontro Arcade.ttf, usando Arial." << std::endl;
        mFont.loadFromFile("assets/fonts/Arial.ttf");
    }

    // --- AJUSTE DE TÍTULO ---
    mTitleText.setFont(mFont);
    mTitleText.setCharacterSize(45);
    mTitleText.setFillColor(sf::Color::White);
    mTitleText.setOutlineColor(sf::Color::Black);
    mTitleText.setOutlineThickness(4);
    mTitleText.setPosition(235, 215); // Posición en la pantalla verde [cite: 4, 5]

    // --- 2. CARGA DE FONDOS ---
    if (mMenuBackgroundTexture.loadFromFile("assets/Foto_Menu.jpeg")) {
        mMenuBackgroundSprite.setTexture(mMenuBackgroundTexture);
        sf::Vector2u size = mMenuBackgroundTexture.getSize();
        mMenuBackgroundSprite.setScale(800.0f / size.x, 600.0f / size.y);
        mMenuBackgroundSprite.setColor(sf::Color(220, 220, 220, 255));
    }
    if (mGameBackgroundTexture.loadFromFile("assets/Fondo_Juego.jpeg")) {
        mGameBackgroundSprite.setTexture(mGameBackgroundTexture);
        sf::Vector2u gameBgSize = mGameBackgroundTexture.getSize();
        mGameBackgroundSprite.setScale(800.0f / gameBgSize.x, 600.0f / gameBgSize.y);
    }
    if (mEnterNameTexture.loadFromFile("assets/Imagen1.jpeg")) {
        mEnterNameSprite.setTexture(mEnterNameTexture);
        sf::Vector2u nameBgSize = mEnterNameTexture.getSize();
        mEnterNameSprite.setScale(800.0f / nameBgSize.x, 600.0f / nameBgSize.y);
        mEnterNameSprite.setColor(sf::Color(200, 200, 200, 255));
    }
    if (mGameOverTexture.loadFromFile("assets/Game_Over.jpeg")) {
        mGameOverSprite.setTexture(mGameOverTexture);
        sf::Vector2u overBgSize = mGameOverTexture.getSize();
        mGameOverSprite.setScale(800.0f / overBgSize.x, 600.0f / overBgSize.y);
    }

    // --- CONFIGURACIÓN DE TEXTOS SECUNDARIOS ---
    mInstructionsText.setFont(mFont);
    mInstructionsText.setCharacterSize(18);
    mInstructionsText.setFillColor(sf::Color::White);
    mInstructionsText.setOutlineColor(sf::Color::Black);
    mInstructionsText.setOutlineThickness(2);
    mInstructionsText.setPosition(240, 520); // Zona de controles [cite: 19]

    mScoreText.setFont(mFont);
    mScoreText.setCharacterSize(22);
    mScoreText.setFillColor(sf::Color(0, 255, 255));
    mScoreText.setOutlineColor(sf::Color::Black);
    mScoreText.setOutlineThickness(2);
    mScoreText.setPosition(15, 15);

    mHighScoreText.setFont(mFont);
    mHighScoreText.setCharacterSize(22);
    mHighScoreText.setFillColor(sf::Color(255, 215, 0));
    mHighScoreText.setOutlineColor(sf::Color::Black);
    mHighScoreText.setOutlineThickness(2);
    mHighScoreText.setPosition(280, 290); // Ranking en pantalla verde [cite: 20, 21]

    mTimePerFrame = sf::seconds(0.15f);

    // --- CONFIGURACIÓN DE COMIDA ---
    mFood.setSize(sf::Vector2f(20, 20));
    mFood.setFillColor(sf::Color::Cyan); // Color cian para contraste [cite: 22]
    mFood.setOutlineColor(sf::Color(255, 255, 255));
    mFood.setOutlineThickness(1);
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
        if (event.type == sf::Event::Closed) mWindow.close();
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) mWindow.close();
        if (mState == ENTER_NAME && event.type == sf::Event::TextEntered) {
            if (event.text.unicode == 8) {
                if (!mPlayerName.empty()) mPlayerName.pop_back();
            } else if (event.text.unicode == 13) {
                if (!mPlayerName.empty()) mState = PLAYING;
            } else if (event.text.unicode < 128 && event.text.unicode > 31) {
                if (mPlayerName.size() < 10) mPlayerName += static_cast<char>(event.text.unicode);
            }
        }
        if (event.type == sf::Event::KeyPressed) {
            if (mState == MENU && event.key.code == sf::Keyboard::Enter) {
                mState = ENTER_NAME;
                mPlayerName = "";
            } else if (mState == GAME_OVER && event.key.code == sf::Keyboard::R) {
                mSnake = Snake(20);
                mScore = 0;
                mTimePerFrame = sf::seconds(0.15f);
                spawnFood();
                mState = MENU;
            }
        }
    }
    if (mState == PLAYING) mSnake.handleInput();
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
        if (mTimePerFrame.asSeconds() > 0.05f) mTimePerFrame -= sf::seconds(0.005f);
    }
}

void Game::render() {
    mWindow.clear();
    if (mState == MENU) {
        if (mMenuBackgroundSprite.getTexture()) mWindow.draw(mMenuBackgroundSprite);
        mTitleText.setString("  SNAKE  GAME");
        mTitleText.setCharacterSize(30);
        mTitleText.setOutlineThickness(4);
        mTitleText.setPosition(190, 215); // Forzar posición menú [cite: 44]
        mWindow.draw(mTitleText);
        std::string scoresStr = "TOP 3 JUGADORES:\n";
        if(mHighScores.empty()) scoresStr += "Sin registros aun.\n";
        for (const auto& entry : mHighScores) scoresStr += entry.name + ": " + std::to_string(entry.score) + "\n";
        mHighScoreText.setString(scoresStr);
        mHighScoreText.setCharacterSize(18);
        mHighScoreText.setPosition(270, 280);
        mWindow.draw(mHighScoreText);
        mInstructionsText.setString("Presiona ENTER para Jugar\n       ESC para Salir");
        mInstructionsText.setCharacterSize(16);
        mInstructionsText.setPosition(150, 520);
        mWindow.draw(mInstructionsText);
    } else if (mState == ENTER_NAME) {
        if (mEnterNameSprite.getTexture()) mWindow.draw(mEnterNameSprite);
        mTitleText.setString("Ingresa tu Nombre:");
        mTitleText.setCharacterSize(25);
        mTitleText.setPosition(140, 80); // Arriba de las cabezas [cite: 54]
        mWindow.draw(mTitleText);
        mInstructionsText.setString(mPlayerName + "_\n\n(ENTER para confirmar)");
        mInstructionsText.setCharacterSize(18);
        mInstructionsText.setPosition(200, 380); // Escudo Spider-Man [cite: 57]
        mWindow.draw(mInstructionsText);
    } else if (mState == PLAYING) {
        if (mGameBackgroundSprite.getTexture()) mWindow.draw(mGameBackgroundSprite);
        mSnake.render(mWindow);
        mWindow.draw(mFood);
        mScoreText.setString("Puntos: " + std::to_string(mScore) + " | " + mPlayerName);
        mWindow.draw(mScoreText);
    } else if (mState == GAME_OVER) {
        if (mGameOverSprite.getTexture()) mWindow.draw(mGameOverSprite);
        mTitleText.setString("GAME OVER");
        mTitleText.setCharacterSize(50);
        mTitleText.setFillColor(sf::Color::Red);
        mTitleText.setPosition(220, 280); // Centro visual [cite: 65]
        mWindow.draw(mTitleText);
        mInstructionsText.setString("Puntaje Final: " + std::to_string(mScore) + "\n\n'R' para Menu\nESC para Salir");
        mWindow.draw(mInstructionsText);
    }
    mWindow.display();
}

void Game::spawnFood() {
    int x = rand() % (800 / 20);
    int y = rand() % (600 / 20);
    mFood.setPosition(x * 20, y * 20);
}

void Game::loadScores() {
    mHighScores.clear();
    std::ifstream file("scores.txt");
    if (file.is_open()) {
        std::string name;
        int score;
        while (file >> name >> score) mHighScores.push_back({name, score});
        file.close();
    }
    std::sort(mHighScores.begin(), mHighScores.end(), [](const ScoreEntry& a, const ScoreEntry& b) { return a.score > b.score; });
}

void Game::updateHighScores() {
    mHighScores.push_back({mPlayerName, mScore});
    std::sort(mHighScores.begin(), mHighScores.end(), [](const ScoreEntry& a, const ScoreEntry& b) { return a.score > b.score; });
    if (mHighScores.size() > 3) mHighScores.resize(3);
}

void Game::saveScore() {
    std::ofstream file("scores.txt");
    if (file.is_open()) {
        for (const auto& entry : mHighScores) file << entry.name << " " << entry.score << "\n";
        file.close();
    }
}
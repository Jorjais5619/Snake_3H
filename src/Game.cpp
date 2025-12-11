#include "../include/Game.hpp"
#include <cstdlib>
#include <ctime>

Game::Game() 
    : m_window(sf::VideoMode(800, 600), "Snake Tuneado"), 
      m_snake(20), 
      m_blockSize(20) 
{
    m_window.setFramerateLimit(60);
    m_timePerFrame = sf::seconds(0.1f);
    m_timeSinceLastMove = sf::Time::Zero;
    
    srand(static_cast<unsigned>(time(nullptr)));
    spawnFood();
}

void Game::run() {
    sf::Clock clock;
    while (m_window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            m_window.close();
    }
    m_snake.handleInput();
}

void Game::update(sf::Time deltaTime) {
    m_timeSinceLastMove += deltaTime;
    
    if (m_timeSinceLastMove > m_timePerFrame) {
        m_snake.update();
        
        // Colisión con comida
        if (m_snake.getHeadPosition() == m_food) {
            m_snake.grow();
            spawnFood();
            // Aumentar velocidad ligeramente
            if (m_timePerFrame.asSeconds() > 0.05f) 
                 m_timePerFrame -= sf::seconds(0.002f);
        }

        // Colisión con paredes o cuerpo
        sf::Vector2i head = m_snake.getHeadPosition();
        if (head.x < 0 || head.x >= 800/m_blockSize || 
            head.y < 0 || head.y >= 600/m_blockSize || 
            m_snake.checkCollision()) {
            
            // Reiniciar juego
            m_window.close(); // Por ahora cerramos, luego podemos poner menú
        }

        m_timeSinceLastMove = sf::Time::Zero;
    }
}

void Game::spawnFood() {
    int cols = 800 / m_blockSize;
    int rows = 600 / m_blockSize;
    m_food.x = rand() % cols;
    m_food.y = rand() % rows;
}

void Game::render() {
    m_window.clear(sf::Color::Black);
    m_snake.render(m_window);
    
    // Dibujar Comida
    sf::RectangleShape foodRect(sf::Vector2f(m_blockSize - 1, m_blockSize - 1));
    foodRect.setPosition(m_food.x * m_blockSize, m_food.y * m_blockSize);
    foodRect.setFillColor(sf::Color::Red);
    m_window.draw(foodRect);

    m_window.display();
}
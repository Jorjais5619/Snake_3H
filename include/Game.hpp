#pragma once
#include <SFML/Graphics.hpp>
#include "Snake.hpp"

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(sf::Time deltaTime);
    void render();
    void spawnFood();

    sf::RenderWindow m_window;
    Snake m_snake;
    sf::Vector2i m_food;
    int m_blockSize;
    
    sf::Time m_timePerFrame;
    sf::Time m_timeSinceLastMove;
};
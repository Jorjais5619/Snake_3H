#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Snake {
public:
    Snake(int blockSize);
    ~Snake();

    void handleInput();
    void update();
    void render(sf::RenderWindow& window);
    
    void grow();
    bool checkCollision();
    sf::Vector2i getHeadPosition() const { return m_body[0]; }

private:
    int m_blockSize;
    std::vector<sf::Vector2i> m_body; // El cuerpo
    sf::Vector2i m_direction;         // Dirección actual
    bool m_hasMoved;                  // Control de input
};
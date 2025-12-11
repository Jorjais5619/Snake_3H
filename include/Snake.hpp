#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Snake {
public:
    Snake(int blocksize);
    ~Snake();

    void handleInput();
    void update();
    void render(sf::RenderWindow& window);

    void grow();
    bool checkCollision();
    sf::Vector2i getHeadPosition() const { return m_body.empty() ? sf::Vector2i(0,0) : m_body[0]; }

private:
    int m_blockSize;
    std::vector<sf::Vector2i> m_body; 
    
    // Variables de movimiento
    sf::Vector2i m_direction; 
    bool m_hasMoved;
};
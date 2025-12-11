#include "../include/Snake.hpp"
#include <algorithm> 

Snake::Snake(int blockSize) : m_blockSize(blockSize) {
    // Inicializamos con cuerpo de 3 piezas
    m_body.push_back(sf::Vector2i(5, 5));
    m_body.push_back(sf::Vector2i(4, 5));
    m_body.push_back(sf::Vector2i(3, 5));
    
    m_direction = sf::Vector2i(1, 0);
    m_hasMoved = false;
}

Snake::~Snake() {}

void Snake::handleInput() {
    // Evitar giro de 180 grados y múltiples giros en un frame
    if (m_hasMoved) return; 

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && m_direction.y == 0) {
        m_direction = sf::Vector2i(0, -1);
        m_hasMoved = true;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && m_direction.y == 0) {
        m_direction = sf::Vector2i(0, 1);
        m_hasMoved = true;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && m_direction.x == 0) {
        m_direction = sf::Vector2i(-1, 0);
        m_hasMoved = true;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && m_direction.x == 0) {
        m_direction = sf::Vector2i(1, 0);
        m_hasMoved = true;
    }
}

void Snake::update() {
    if (m_body.empty()) return;

    // Mover cuerpo
    for (int i = m_body.size() - 1; i > 0; --i) {
        m_body[i] = m_body[i - 1];
    }
    // Mover cabeza
    m_body[0] += m_direction;
    m_hasMoved = false; // Permitir nuevo input para el siguiente frame
}

void Snake::grow() {
    if (!m_body.empty()) {
        m_body.push_back(m_body.back());
    }
}

bool Snake::checkCollision() {
    if (m_body.empty()) return false;

    // 1. Chocar con su propio cuerpo
    for (size_t i = 1; i < m_body.size(); ++i) {
        if (m_body[0] == m_body[i]) return true;
    }

    // 2. Chocar con PAREDES (Asumiendo ventana 800x600 y bloque 20)
    // 800/20 = 40 columnas (0 a 39)
    // 600/20 = 30 filas (0 a 29)
    if (m_body[0].x < 0 || m_body[0].x >= 40 || 
        m_body[0].y < 0 || m_body[0].y >= 30) {
        return true;
    }

    return false;
}

void Snake::render(sf::RenderWindow& window) {
    sf::RectangleShape rect(sf::Vector2f(m_blockSize - 1, m_blockSize - 1));
    
    for (size_t i = 0; i < m_body.size(); ++i) {
        rect.setPosition(m_body[i].x * m_blockSize, m_body[i].y * m_blockSize);
        
        if (i == 0) rect.setFillColor(sf::Color::Green); // Cabeza
        else {
            // Degradado visual
            int alpha = 255 - (std::min((int)i * 10, 200));
            rect.setFillColor(sf::Color(0, alpha, 0));
        }
        window.draw(rect);
    }
}
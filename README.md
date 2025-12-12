# 🐍 Snake Master: Arcade Edition

**Snake Master** es una reinterpretación del clásico juego de la serpiente, desarrollada en **C++ con SFML**. El juego está diseñado bajo una estética de cabina "Arcade" clásica, integrando elementos visuales de superhéroes y una banda sonora relajada de reggae.

## 🎯 Objetivo del Juego
Recolectar la mayor cantidad de manzanas (Cian) para crecer y aumentar tu puntaje, evitando chocar con las paredes o con tu propio cuerpo. ¡La velocidad aumenta conforme avanzas!

## 🎮 Controles
* **Flechas / WASD:** Movimiento de la serpiente.
* **ENTER:** Iniciar el juego / Confirmar nombre en la pantalla de inicio.
* **ESC:** Salir del juego en cualquier momento.
* **R:** Reiniciar el juego desde la pantalla de Game Over.

## ⚙️ Mecánicas
* **Dificultad Progresiva:** Cada vez que comes, el intervalo de tiempo disminuye, haciendo al juego más rápido.
* **Sistema de Ranking:** Registro de los 3 mejores puntajes guardados localmente en `scores.txt`.
* **Máquina de Estados:** Transiciones fluidas entre Menú, Registro de Nombre, Juego y Fin del Juego.

## 🏆 Características
* **Visuales Arcade:** Interfaz ajustada para encajar en un monitor de cabina arcade verde.
* **Banda Sonora:** Música de fondo: *"Tus Ojos"* de Los Cafres.
* **Contraste de Color:** Manzana en color Cian Neón para máxima visibilidad sobre fondos de Batman y Spider-Man.

## 👥 Equipo
* **Líder:** Emilio (@tu-usuario-github)

## 🛠️ Tecnologías
* **Lenguaje:** C++17
* **Librería:** SFML 2.5.1 (Graphics, Window, System, Audio)
* **Compilador:** MinGW / g++

## 📜 Créditos
* **Música:** Los Cafres - Tus Ojos.
* **Fuentes:** Arcade Font (TTF).
* **Inspiración:** Juegos de consola de los años 80.

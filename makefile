# Makefile pensado para MSYS2/MinGW

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude

# Librerías (Incluimos Box2D por si lo usas en el futuro, aunque Snake no lo usa hoy)
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

SRC_DIR = src
OBJ_DIR = bin/obj
BIN_DIR = bin

# Busca todos los .cpp dentro de src/
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

TARGET = $(BIN_DIR)/SnakeGame.exe

all: directories $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run : $(TARGET)
	./$(TARGET)

directories:
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(BIN_DIR)
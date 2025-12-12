# Makefile corregido y optimizado
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude

# LIBRERÍAS: -lsfml-audio debe ir antes de -lsfml-system
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system

SRC_DIR = src
OBJ_DIR = bin/obj
BIN_DIR = bin

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

TARGET = $(BIN_DIR)/SnakeGame.exe

all: directories $(TARGET)

# El ejecutable depende de los objetos
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# CADA objeto depende de que la carpeta exista
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | directories
	$(CXX) $(CXXFLAGS) -c $< -o $@

run : all
	./$(TARGET)

directories:
	@mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(BIN_DIR)
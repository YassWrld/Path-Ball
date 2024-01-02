# Compiler
CC = gcc

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BIN_DIR = bin

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SRC_FILES))

# Compiler flags
CFLAGS = -I$(INCLUDE_DIR) -g -Wall `sdl2-config --cflags` -lSDL2_mixer -lSDL2_ttf -lSDL2_image  -lSDL2_gfx
LDFLAGS = `sdl2-config --libs`

# Target binary
TARGET = $(BIN_DIR)/game

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BIN_DIR)/*.o $(TARGET)

.PHONY: all run clean
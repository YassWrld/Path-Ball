ifeq ($(OS),Windows_NT) 
all: clean build run

current_date := $(shell powershell Get-Date -Format "-dd-MM-HH-mm-ss")

 
sdlPath := C:\SDL2
src := src/*.c
linkers= -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer  
includes= -I include -I $(sdlPath)\include -I $(sdlPath)\include\SDL2
libs= -L $(sdlPath)\lib
flags:=-Wall  -std=c17 -Wall   -Werror  -pedantic #-mwindows
resources:= assets\windows\resources.res 


build:
	gcc $(src) $(resources) $(includes) $(libs) $(linkers) $(flags) -o "bin/game.exe"
run :
	@echo Run started...
	.\bin\game.exe
	
testBuild: 
	gcc $(src) $(resources) $(includes) $(libs) $(linkers) $(flags) -o "bin/game${current_date}.exe"


testRun:
	@echo Run started...
	./bin/game${current_date}.exe

test : testBuild testRun
exes= $(wildcard bin/*.exe)

runAll:
	@echo Run started...
	cmd /c "for %x in ($(exes)) do start %x"


	
	



clean:
	@echo Clean started...
		del bin\*.exe

commands:
	@echo "make build - compiles the game"
	@echo "make run - runs the game"
	@echo "make clean - removes the executable"

files:= $(wildcard src/*.c)

webs:
	emcc $(files) -o web/static/game.html -I include -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2 -sINITIAL_MEMORY=83886080 --preload-file assets

else 
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

endif





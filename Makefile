all: clean build run

current_date := $(shell powershell Get-Date -Format "-dd-MM-HH-mm-ss")

 
sdlPath := C:\SDL2
src := src/*.c
linkers= -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer #-mwindows 
includes= -I include -I $(sdlPath)\include -I $(sdlPath)\include\SDL2
libs= -L $(sdlPath)\lib
flags:=-Wall  -std=c17  -g  -Wall   -Werror  -pedantic 
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

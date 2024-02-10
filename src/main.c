#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "logic.h"
#include "draw.h"
#include "util.h"
#include "renderers.h"
#include "handlers.h"

SDL_Window *window = NULL; // The window we'll be rendering to

SDL_Renderer *renderer = NULL; // The window renderer
screen Screen = MainMenu;      // The current screen
game Game;                     // The game structure
int fps = 60;                  // The frames per second

int main(int argc, char *argv[])
{

    InitializeSDL(&window, &renderer, GAME_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, ICON_PATH); // Initialize SDL

    int quit = 0; // Main loop flag
    SDL_Event e;  // Event structure

    loadAllSounds(&Game);         // Load all sounds (music and effects) into the game structure
    playMusic(Game.sounds.music); // Play the background music (infinite loop)

    while (!quit) // While application is running
    {

        while (SDL_PollEvent(&e) != 0) // Handle events on queue
        {
            handleEvents(e, renderer, &Screen, &Game, &quit); // Handle all events
        }
        renderScreens(renderer, Screen, &Game); // Render all screens

        graycefulDelay(1000 / fps); // Cap the frame rate
    }

    return QuitSDL(&window, &renderer); // Quit SDL
}

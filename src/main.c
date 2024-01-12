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

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
void initializeSDL();
int Quit();

game Game;
screen Screen = PlayerGameMode;
int fps = 60;
int main(int argc, char *argv[])
{

    initializeSDL();
    int quit = 0;
    SDL_Event e;

    initGame(&Game, false);
    printMatrix(Game.level + 5, Game.matrix);

    // ca
    while (!quit)
    {

        while (SDL_PollEvent(&e) != 0)
        {
            handleEvents(e, Screen, renderer, &Game, &quit);
        }
        renderScreens(renderer, Screen, &Game);

        graycefulDelay(10);
    }

    return Quit();
}

void initializeSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "SDL could not be initialized! SDL Error: %s\n", SDL_GetError());
        // Handle the SDL error and exit or return an error code.
    }

    // Initialize SDL_ttf
    if (TTF_Init() < 0)
    {
        fprintf(stderr, "SDL_ttf could not be initialized! SDL_ttf Error: %s\n", TTF_GetError());
        // Handle the SDL_ttf error and exit or return an error code.
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        Mix_AllocateChannels(16);
        fprintf(stderr, "SDL_mixer could not be initialized! SDL_mixer Error: %s\n", Mix_GetError());
        // Handle the SDL_mixer error and exit or return an error code.
    }

    if (IMG_Init(IMG_INIT_PNG) < 0)
    {
        fprintf(stderr, "SDL_image could not be initialized! SDL_image Error: %s\n", IMG_GetError());
        // Handle the SDL_image error and exit or return an error code.
    }

    window = SDL_CreateWindow("Pinball Recall", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0); // Create a window
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);                                           // Create a renderer

    SDL_Surface *icon = IMG_Load("assets/images/icon.png"); // Load the icon

    icon = SDL_ConvertSurfaceFormat(icon, SDL_PIXELFORMAT_RGBA8888, 0); // Convert the icon to the right format
    SDL_SetWindowIcon(window, icon);                                    // Set the icon
    SDL_FreeSurface(icon);                                              // Free the icon surface

    // SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RegisterEvents(1); // Register a user event (for machine mode)
    SDL_RenderSetVSync(renderer, true);
}

int Quit()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    Mix_Quit();

    SDL_Quit();
    return 0;
}
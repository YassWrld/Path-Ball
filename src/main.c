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

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
void initialize();
void Quit();

game Game;

int main(int argc, char *argv[])
{
    initialize();
    bool quit = false;
    SDL_Event e;
    Game.level = 1;
    Game.solution = setupMatrix(Game.level + 5, Game.matrix);
    Game.state = Memorizing;

    printMatrix(Game.level + 5, Game.matrix);
    while (!quit)
    {

        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }
        SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR);
        SDL_RenderClear(renderer);

        switch (Game.state)
        {
        case Memorizing:
            drawGrid(renderer, Game, 0, 0);
            Game.state = Selecting;
            break;
        case Selecting:
            drawGrid(renderer, Game, 0, 0);
            break;
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(Game.state==0?10*1000:10);
    }
    Quit();
}

void initialize()
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
        fprintf(stderr, "SDL_mixer could not be initialized! SDL_mixer Error: %s\n", Mix_GetError());
        // Handle the SDL_mixer error and exit or return an error code.
    }

    window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

void Quit()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}
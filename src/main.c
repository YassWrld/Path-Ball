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
int matrix[20][20];
int winStreak = 0;
int loseStreak = 0;
int level = 1;
int hoverI = 0;
int hoverJ = 0;

int main(int argc, char *argv[])
{

    initialize();

    int n = 6;

    solution *s = setupMatrix(n, matrix);

    printMatrix(n, matrix);

    printf("start: %d, end: %d, hit: %d, noHit: %d\n", s->start, s->end,s->hit,s->noHit);

    int cellSize = GRID_SIZE / n;

    SDL_Event e;
    int quit = 0;
    while (!quit)
    {

        SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR);
        SDL_RenderClear(renderer);
        drawGrid(renderer, n, cellSize, matrix, hoverI, hoverJ);
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = 1;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                int key = e.key.keysym.sym;
                // key x
                if (key == 120)
                {
                    printf("x pressed\n");
                    quit = 1;
                }
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                drawPath(renderer, cellSize, s->path);
                SDL_PollEvent(&(SDL_Event){0});
                SDL_Delay(2000);
                level++;
                if (level % 3 == 0)
                {
                    n++;
                    cellSize = GRID_SIZE / n;
                }

                s = setupMatrix(n, matrix);
                printMatrix(n, matrix);
                printf("start: %d, end: %d, hit: %d, noHit: %d\n", s->start, s->end, s->hit, s->noHit);

                // SDL_Delay(20000);
            }
            else if (e.type == SDL_MOUSEMOTION)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);
                getMatrixClick(renderer, x, y, n, &hoverI, &hoverJ);
            }
        };

        SDL_RenderPresent(renderer);
    }

    // Clean up and exit
    Quit();
    return 0;
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
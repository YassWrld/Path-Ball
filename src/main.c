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
int Quit();

game Game;

int main(int argc, char *argv[])
{

    initialize();
    int quit = 0;
    SDL_Event e;

    Game.level = 1;

    Game.solution = setupMatrix(Game.level + 5, Game.matrix);
    Game.state = Memorizing;
    
    printf("start: %d,i=%d,j=%d\n", Game.solution->start, Game.solution->startI, Game.solution->startJ);
    printMatrix(Game.level + 5, Game.matrix);
    while (!quit)
    {
        int won = 0;
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = 1;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                printf("Mouse click detected\n");
                if (e.button.button != SDL_BUTTON_LEFT)
                    continue;

                int x, y;

                SDL_GetMouseState(&x, &y);
                int i, j;
                getMatrixClick(renderer, x, y, Game.level + 5, &i, &j);
                printf("i=%d,j=%d\n", i, j);

                if (Game.state == Selecting)
                {
                    if (Game.solution->endI == i && Game.solution->endJ == j)
                    {
                        Game.state = Result;
                        printf("You won\n");
                        won = 1;
                    }
                    else
                    {
                        printf("You lost\n");
                        Game.state = Result;
                        won = -1;
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR);
        SDL_RenderClear(renderer);

        if (Game.level == 0)
        {
            SDL_SetRenderDrawColor(renderer, PATH_COLOR);
            drawFilledCircle(renderer, WIDTH / 2, HEIGHT / 2, 100);
        }
        else
        {
            switch (Game.state)
            {
            case Memorizing:
            case Selecting:
                drawGrid(renderer, &Game);
                drawSideBar(renderer, &Game);
                break;
            case Result:
                drawGrid(renderer, &Game);
                drawSideBar(renderer, &Game);
                drawPath(renderer, Game.level + 5, Game.solution->path);
                if (won == 1)
                {
                    Game.winStreak++;
                }
                else
                {
                    Game.loseStreak++;
                }
                updateLevel(&Game);
                if (Game.level != 0)
                    Game.solution = setupMatrix(Game.level + 5, Game.matrix);
                break;
            }
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(Game.state == 0 ? 3 * 1000 : 10);
        if (Game.state == 0)
        {
            Game.state = Selecting;
        }
        else if (Game.state == 2)
        {
            Game.state = Memorizing;
        }
    }
    return Quit();
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

int Quit()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
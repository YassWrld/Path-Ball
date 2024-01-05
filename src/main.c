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

    // str concatenation

    Game.level = 1;
    Game.maxLevel = 1;

    Game.solution = setupMatrix(Game.level + 5, Game.matrix);
    Game.state = TextInput;

    printf("start: %d,i=%d,j=%d\n", Game.solution->start, Game.solution->startI, Game.solution->startJ);
    printMatrix(Game.level + 5, Game.matrix);
    // playMusic("assets/sounds/undertale.mp3");

    while (!quit)
    {
        int win = 0;
        Uint32 delay = 10;
        while (SDL_PollEvent(&e) != 0)
        {

            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
                quit = 1;

            if (e.type == SDL_MOUSEBUTTONDOWN && Game.state == Selecting)
            {
                if (e.button.button != SDL_BUTTON_LEFT)
                    continue;
                // continue;
                int x, y;

                SDL_GetMouseState(&x, &y);
                int i, j;
                getMatrixClick(renderer, x, y, Game.level + 5, &i, &j);

                if (i == -1 && j == -1)
                {
                    continue;
                }

                if (Game.solution->endI == i && Game.solution->endJ == j)
                {
                    Game.state = Result;
                    win = 1;
                }
                else if (Game.solution->startI != i || Game.solution->startJ != j)
                {
                    Game.state = Result;
                    win = -1;
                }
            }

            if (e.type == SDL_TEXTINPUT && Game.state == TextInput)
            {
                // check if the name is too long
                if (strlen(Game.player.name) >= 20)
                    continue;

                strcat(Game.player.name, e.text.text);
            }

            if (e.type == SDL_KEYDOWN && Game.state == TextInput)
            {
                if (e.key.keysym.sym == SDLK_BACKSPACE && strlen(Game.player.name) > 0)
                {
                    Game.player.name[strlen(Game.player.name) - 1] = '\0';
                }
                if (e.key.keysym.sym == SDLK_RETURN)
                {
                    if (strlen(Game.player.name) == 0)
                        continue;

                    SDL_StopTextInput();
                    Game.state = Memorizing;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR);
        SDL_RenderClear(renderer);

        switch (Game.state)
        {
        case TextInput:

            drawTextInput(renderer, &Game);

            break;
        case Memorizing:
        case Selecting:
            drawGrid(renderer, &Game);
            drawSideBar(renderer, &Game);
            if (Game.state == Memorizing)
                delay = 3 * 1000;
            Game.state = Selecting;
            break;

        case Result:
            drawGrid(renderer, &Game);
            drawSideBar(renderer, &Game);
            drawPath(renderer, Game.level + 5, Game.solution->path);

            updateLevel(&Game, win);

            Game.state = Game.level == 0 ? GameOver : Memorizing;
            break;
        case GameOver:
            break;
        }

        SDL_RenderPresent(renderer);

        if (graycefulDelay(delay))
            quit = 1;
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
        Mix_AllocateChannels(16);
        fprintf(stderr, "SDL_mixer could not be initialized! SDL_mixer Error: %s\n", Mix_GetError());
        // Handle the SDL_mixer error and exit or return an error code.
    }

    window = SDL_CreateWindow("Pineball Recall", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
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
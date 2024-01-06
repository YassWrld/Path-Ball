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
int mat[20][20];
int main(int argc, char *argv[])
{

    initialize();
    int quit = 0;
    SDL_Event e;

    initGame(&Game, false);
    // take the start time

    Uint32 startTime = 0;
    Uint32 pauseTime = 0;
    game_state oldState = Game.state;

    while (!quit)
    {
        int win = 0;

        while (SDL_PollEvent(&e) != 0)
        {

            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) // quit
                quit = 1;
            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_p && SDL_GetModState() & KMOD_CTRL) // pause
            {
                if (Game.state == TextInput || Game.state == Result || Game.state == GameOver)
                    continue;

                if (Game.state != Pause)
                {
                    oldState = Game.state;
                    pauseTime = SDL_GetTicks();
                }
                else
                    startTime += SDL_GetTicks() - pauseTime;

                Game.state = Game.state == Pause ? oldState : Pause;

                // change the state to pause
            }

            else if (e.type == SDL_TEXTINPUT && Game.state == TextInput)
            {
                // check if the name is too long
                if (strlen(Game.player.name) >= 20)
                    continue;

                strcat(Game.player.name, e.text.text); // add the character to the name
            }
            else if (e.type == SDL_KEYDOWN && Game.state == TextInput) // text input
            {
                if (strlen(Game.player.name) == 0) // if the name is empty
                    continue;
                if (e.key.keysym.sym == SDLK_BACKSPACE)
                {
                    Game.player.name[strlen(Game.player.name) - 1] = '\0'; // remove the last character
                }

                if (e.key.keysym.sym == SDLK_RETURN)
                {
                    SDL_StopTextInput();     // stop text input mode
                    Game.state = Memorizing; // change the state to memorizing
                }
            }
            else if (e.type == SDL_MOUSEBUTTONUP && Game.state == Selecting && !Game.machineMode)
            {
                if (e.button.button != SDL_BUTTON_LEFT)
                    continue; // only left click

                int x = e.button.x; // get the click position
                int y = e.button.y;
                int i, j;
                getMatrixClick(renderer, x, y, Game.level + 5, &i, &j); // get the matrix position

                if ((i == -1 && j == -1) || (Game.solution->startI == i && Game.solution->startJ == j)) // if the click is outside the matrix
                    continue;

                if (Game.solution->endI == i && Game.solution->endJ == j)
                {

                    win = 1;
                }
                else
                {
                    win = -1;
                }
                Game.state = Result;
            }
            else if (e.type == SDL_USEREVENT && Game.state == Selecting && Game.machineMode)

            {
                int x = e.button.x; // get the click position
                int y = e.button.y;
                int i, j;
                getMatrixClick(renderer, x, y, Game.level + 5, &i, &j); // get the matrix position

                if (i == -1 && j == -1) // if the click is outside the matrix
                    continue;

                if (Game.solution->endI == i && Game.solution->endJ == j)
                {

                    win = 1;
                }
                else if (Game.solution->startI != i || Game.solution->startJ != j)
                {
                    win = -1;
                }
                Game.state = Result;
            }

            // ctrl + p
        }

        SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR);
        SDL_RenderClear(renderer);

        switch (Game.state)
        {
        case TextInput:

            drawTextInput(renderer, &Game);

            break;
        case Memorizing:

            drawGrid(renderer, &Game);
            drawSideBar(renderer, &Game);
            if (Game.machineMode)
                machineModeMemorize(renderer, Game.level + 5, mat);
            // delay = 3 * 1000;
            if (startTime == 0)
                startTime = SDL_GetTicks();
            // Game.state = Selecting;
            if (SDL_GetTicks() - startTime > MEMORIZING_TIME)
            {
                Game.state = Selecting;
                startTime = 0;
            }
            break;
        case Selecting:
            drawGrid(renderer, &Game);
            drawSideBar(renderer, &Game);
            if (Game.machineMode)
                machineModeSelecting(renderer, Game.level + 5, mat);
            break;

        case Result:
            drawGrid(renderer, &Game);
            drawSideBar(renderer, &Game);
            drawPath(renderer, Game.level + 5, Game.solution->path);
            updateLevelAndScore(&Game, win);

            Game.state = Game.level == 0 || Game.level == MAX_LEVEL ? GameOver : Memorizing;

            break;
        case Pause:
            drawPause(renderer, &Game);
            break;
        case GameOver:
            drawGameOver(renderer, &Game);
            break;
        }

        SDL_RenderPresent(renderer);

        if (graycefulDelay(10))
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

    // SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RegisterEvents(1);
}

int Quit()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
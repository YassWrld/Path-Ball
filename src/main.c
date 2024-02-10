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


game Game;
screen Screen = MainMenu;
int fps = 60;

int main(int argc, char *argv[])
{

    InitializeSDL(&window, &renderer, GAME_TITLE, WIDTH, HEIGHT, ICON_PATH);

    int quit = 0;
    SDL_Event e;

    loadAllSounds(&Game);
    playMusic(Game.sounds.music);

    while (!quit)
    {

        while (SDL_PollEvent(&e) != 0)
        {
            handleEvents(e, renderer, &Screen, &Game, &quit);
        }
        renderScreens(renderer, Screen, &Game);

        quit = graycefulDelay(1000 / fps);
    }

    return QuitSDL(&window, &renderer);
}

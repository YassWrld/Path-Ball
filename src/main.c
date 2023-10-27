#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Initialization failed!");
    }
    else
    {
        printf("SDL Initialized succesfully!");
    }
}

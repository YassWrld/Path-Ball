
#ifndef HANDLERS_H
#define HANDLERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>
#include "logic.h"
#include "draw.h"
void handleGlobal(SDL_Event event, int *quit);

void handleGameMode(SDL_Event event, game *Game, SDL_Renderer *renderer);

#endif // HANDLERS_H

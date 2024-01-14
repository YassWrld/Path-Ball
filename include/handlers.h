
#ifndef HANDLERS_H
#define HANDLERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>
#include "logic.h"
#include "draw.h"
#include "util.h"

void handleEvents(SDL_Event event, SDL_Renderer *renderer, screen *Screen,  game *Game, int *quit);
void handleGlobal(SDL_Event event, int *quit);
void handleMainMenu(SDL_Event event, SDL_Renderer *renderer, screen *Screen, game *Game);
void handleGameMode(SDL_Event event,SDL_Renderer *renderer,screen *Screen, game *Game);


#endif // HANDLERS_H

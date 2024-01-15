#pragma once


#include "draw.h"
#include "logic.h"

void renderScreens(SDL_Renderer *renderer, screen Screen, game *Game);
void renderGameModeScreen(SDL_Renderer *renderer, game *game);
void renderTopPlayersScreen(SDL_Renderer *renderer, game *game);


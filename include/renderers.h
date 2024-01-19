#pragma once


#include "draw.h"
#include "logic.h"

void renderScreens(SDL_Renderer *renderer, screen Screen, game *Game);
void renderMainMenuScreen(SDL_Renderer *renderer, game *game);
void renderGameModeScreen(SDL_Renderer *renderer, game *game);
void renderTopPlayersScreen(SDL_Renderer *renderer, game *game);
void renderChooseMachineGameModeScreen(SDL_Renderer *renderer, game *game);
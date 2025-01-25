#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>
#include "logic.h"
#include "draw.h"
#include "util.h"

/**
 * @brief Handle All Events
 *
 * (Handles all events including global events, main menu screen events, game mode events,
 * choose machine screen game mode events, and top players screen events)
 *
 * @param event: SDL_Event containing the event data
 * @param renderer: SDL_Renderer pointer
 * @param Screen: pointer to the screen structure
 * @param Game: pointer to the game structure
 * @param quit: pointer to the flag indicating program termination
 * @return void
 */
void handleEvents(SDL_Event event, SDL_Renderer *renderer, screen *Screen, game *Game, int *quit);

/**
 * @brief Handle Global Events
 *
 * (Handles global events such as quit)
 *
 * @param event: SDL_Event containing the event data
 * @param quit: pointer to the flag indicating program termination
 * @return void
 */
void handleGlobal(SDL_Event event, int *quit);

/**
 * @brief Handle Main Menu Screen Events
 *
 * (Handles events specific to the main menu screen)
 *
 * @param event: SDL_Event containing the event data
 * @param renderer: SDL_Renderer pointer
 * @param Screen: pointer to the screen structure
 * @param Game: pointer to the game structure
 * @return void
 */
void handleMainMenu(SDL_Event event, SDL_Renderer *renderer, screen *Screen, game *Game);

/**
 * @brief Handle Game Mode Events
 *
 * (Handles events specific to the game mode)
 *
 * @param event: SDL_Event containing the event data
 * @param renderer: SDL_Renderer pointer
 * @param Screen: pointer to the screen structure
 * @param Game: pointer to the game structure
 * @return void
 */
void handleGameMode(SDL_Event event, SDL_Renderer *renderer, screen *Screen, game *Game);

/**
 * @brief Handle Credits Screen Events
 *
 * (Handles events specific to the credits screen)
 *
 * @param event: SDL_Event containing the event data
 * @param renderer: SDL_Renderer pointer
 * @param Screen: pointer to the screen structure
 * @param Game: pointer to the game structure
 * @return void
 */
void handleCredits(SDL_Event event, SDL_Renderer *renderer, screen *Screen, game *Game);
/**
 * @brief Handle Choose Machine Screen Game Mode Events
 *
 * (Handles events specific to the choose machine screen game mode)
 *
 * @param event: SDL_Event containing the event data
 * @param renderer: SDL_Renderer pointer
 * @param Screen: pointer to the screen structure
 * @param Game: pointer to the game structure
 * @return void
 */
void handleChooseMachineGameMode(SDL_Event event, SDL_Renderer *renderer, screen *Screen, game *Game);

/**
 * @brief Handle Top Players Screen Events
 *
 * (Handles events specific to the top players screen)
 *
 * @param event: SDL_Event containing the event data
 * @param renderer: SDL_Renderer pointer
 * @param Screen: pointer to the screen structure
 * @param Game: pointer to the game structure
 * @return void
 */
void handleTopPlayers(SDL_Event event, SDL_Renderer *renderer, screen *Screen, game *Game);

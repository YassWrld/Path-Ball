#pragma once

#include "draw.h"
#include "logic.h"

/**
 * @brief Renders all screens
 *
 * (Renders all screens according to the current state of the game)
 *
 * @param renderer: SDL_Renderer pointer
 * @param Screen: current screen state
 * @param Game: pointer to the game structure
 * @return void
 */
void renderScreens(SDL_Renderer *renderer, screen Screen, game *Game);

/**
 * @brief Renders the main menu screen
 *
 * (Renders the main menu screen with appropriate graphics and text)
 *
 * @param renderer: SDL_Renderer pointer
 * @param Game: pointer to the game structure
 * @return void
 */
void renderMainMenuScreen(SDL_Renderer *renderer, game *Game);

/**
 * @brief Renders the game mode screen
 *
 * (Renders the game mode screen with appropriate graphics and text)
 *
 * @param renderer: SDL_Renderer pointer
 * @param Game: pointer to the game structure
 * @return void
 */
void renderGameModeScreen(SDL_Renderer *renderer, game *Game);

/**
 * @brief Renders the top players screen
 *
 * (Renders the top players screen with appropriate graphics and text)
 *
 * @param renderer: SDL_Renderer pointer
 * @param Game: pointer to the game structure
 * @return void
 */
void renderTopPlayersScreen(SDL_Renderer *renderer, game *Game);

/**
 * @brief Renders the choose machine game mode screen
 *
 * (Renders the choose machine game mode screen with appropriate graphics and text)
 *
 * @param renderer: SDL_Renderer pointer
 * @param Game: pointer to the game structure
 * @return void
 */
void renderChooseMachineGameModeScreen(SDL_Renderer *renderer, game *Game);

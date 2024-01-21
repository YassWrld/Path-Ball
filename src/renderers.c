#include "renderers.h"

void renderScreens(SDL_Renderer *renderer, screen Screen, game *Game)
{

    SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR);
    SDL_RenderClear(renderer);

    switch (Screen)
    {
    case MainMenu:
        renderMainMenuScreen(renderer, Game);
        break;

    case ChooseMachineGameMode:
        renderChooseMachineGameModeScreen(renderer, Game);
        break;
    case PlayerGameMode:
    case MachineGameMode:
        renderGameModeScreen(renderer, Game);
        break;
    case TopPlayers:
        renderTopPlayersScreen(renderer, Game);
        break;
    case Credits:
        // renderCreditsScreen(renderer, Game);
        break;
    }
    SDL_RenderPresent(renderer);
}
void renderGameModeScreen(SDL_Renderer *renderer, game *Game)
{

    switch (Game->state)
    {
    case TextInput:

        drawTextInput(renderer, Game);

        break;
    case Filling:
        drawGrid(renderer, Game);
        drawSideBar(renderer, Game);
        break;
    case Memorizing:

        drawGrid(renderer, Game);
        drawSideBar(renderer, Game);
        if (Game->machineMode && !Game->helpers.filledMachineMatrix)
        {
            machineModeMemorize(renderer, Game->level + 5, Game->machineMatrix);
            Game->helpers.filledMachineMatrix = true;
        }

        if (Game->helpers.memorizingStartTime == 0)
            Game->helpers.memorizingStartTime = SDL_GetTicks();

        if (SDL_GetTicks() - Game->helpers.memorizingStartTime > MEMORIZING_TIME)
        {
            Game->state = Selecting;
            Game->helpers.memorizingStartTime = 0;
        }
        break;
    case Selecting:
        drawGrid(renderer, Game);
        drawSideBar(renderer, Game);
        if (Game->machineMode && Game->helpers.filledMachineMatrix)
            machineModeSelecting(renderer, Game->level + 5, Game->machineMatrix);
        break;

    case Result:
        drawGrid(renderer, Game);
        drawSideBar(renderer, Game);
        drawPath(renderer, Game);
        updateLevelAndScore(Game);

        Game->state = (Game->level == 0 || Game->level == MAX_LEVEL)
                          ? GameOver
                          : (Game->manualFill ? Filling : Memorizing);

        if (Game->state == GameOver && !Game->helpers.savedScore && !Game->machineMode && Game->player.score != 0)
        {
            insertScore(Game->player);
            Game->helpers.updatedTopPlayers = true;
            Game->helpers.savedScore = true;
        }

        break;
    case Pause:
        drawPause(renderer, Game);
        break;
    case GameOver:
        drawGameOver(renderer, Game);
        break;
    }
}
/*
void renderCreditsScreen(SDL_Renderer *renderer, game *game)
{
    drawCredits(renderer, game);
}

*/

void renderMainMenuScreen(SDL_Renderer *renderer, game *game)
{
    drawMainMenu(renderer, game);
}
void renderTopPlayersScreen(SDL_Renderer *renderer, game *game)
{
    drawTopPlayers(renderer, game);
}

void renderChooseMachineGameModeScreen(SDL_Renderer *renderer, game *game)
{
    drawChooseMachineGameMode(renderer, game);
}
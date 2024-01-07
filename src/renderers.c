#include "renderers.h"

void renderGameMode(SDL_Renderer *renderer, game *Game)
{
    // Clear screen
    SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR);
    SDL_RenderClear(renderer);

    switch (Game->state)
    {
    case TextInput:

        drawTextInput(renderer, Game);

        break;
    case Memorizing:

        drawGrid(renderer, Game);
        drawSideBar(renderer, Game);
        if (Game->machineMode)
            machineModeMemorize(renderer, Game->level + 5, Game->machineMatrix);

        if (Game->helpers.startTime == 0)
            Game->helpers.startTime = SDL_GetTicks();

        if (SDL_GetTicks() - Game->helpers.startTime > MEMORIZING_TIME)
        {
            Game->state = Selecting;
            Game->helpers.startTime = 0;
        }
        break;
    case Selecting:
        drawGrid(renderer, Game);
        drawSideBar(renderer, Game);
        if (Game->machineMode)
            machineModeSelecting(renderer, Game->level + 5, Game->machineMatrix);
        break;

    case Result:
        drawGrid(renderer, Game);
        drawSideBar(renderer, Game);
        drawPath(renderer, Game->level + 5, Game->solution->path);
        updateLevelAndScore(Game);

        Game->state = Game->level == 0 || Game->level == MAX_LEVEL ? GameOver : Memorizing;

        break;
    case Pause:
        drawPause(renderer, Game);
        break;
    case GameOver:
        drawGameOver(renderer, Game);
        break;
    }

    // Update screen
    SDL_RenderPresent(renderer);
}
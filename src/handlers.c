#include "handlers.h"

void handleEvents(SDL_Event event, screen Screen, SDL_Renderer *renderer, game *Game, int *quit)
{
    handleGlobal(event, quit);
    switch (Screen)
    {
    case MainMenu:
        // handleMainMenu(event,renderer, Game);
        break;
    case PlayerGameMode:
    case MachineGameMode:
        handleGameMode(event, renderer, Game);
        break;
    case TopPlayers:
        // handleTopPlayers(event, renderer, Game);
        break;
    case Credits:
        // handleCredits(event, renderer, Game);
        break;
    }
}

void handleGlobal(SDL_Event event, int *quit)
{
    if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) // quit
    {
        *quit = 1;
        return;
    }
}

void handleGameMode(SDL_Event event, SDL_Renderer *renderer, game *Game)
{

    if (Game->state == TextInput) // Text input mode
    {
        if (event.type == SDL_KEYDOWN) // text input
        {
            if (strlen(Game->player.name) == 0) // if the name is empty
                return;
            if (event.key.keysym.sym == SDLK_BACKSPACE || event.key.keysym.sym == SDLK_DELETE) // backspace
            {
                Game->player.name[strlen(Game->player.name) - 1] = '\0'; // remove the last character
            }

            if (event.key.keysym.sym == SDLK_RETURN)
            {
                SDL_StopTextInput();      // stop text input mode
                Game->state = Memorizing; // change the state to memorizing
            }
            return;
        }

        if (event.type == SDL_TEXTINPUT)
        {
            // check if the name is too long
            if (!checkAllowedString(event.text.text))
                return;
            if (strlen(Game->player.name) >= 20)
                return;

            strcat(Game->player.name, event.text.text); // add the character to the name
            return;
        }
    }

    // Pause mode

    if ((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p && SDL_GetModState() & KMOD_CTRL) || isClickInButton(event, &Game->buttons.pause)) // pause
    {
        if (Game->state == TextInput || Game->state == Result || Game->state == GameOver)
            return;

        if (Game->state != Pause)
        {
            Game->helpers.prevState = Game->state;
            Game->helpers.pauseTime = SDL_GetTicks();
        }
        else
        {
            Game->helpers.memorizingStartTime += SDL_GetTicks() - Game->helpers.pauseTime;
            Game->helpers.gameStartTime += SDL_GetTicks() - Game->helpers.pauseTime;

            Game->helpers.pauseTime = 0;
        }
        Game->state = Game->state == Pause ? Game->helpers.prevState : Pause;

        return;
    }

    if (Game->state == Pause)
    {
    }

    // Selecting mode (keyboard)
    if (Game->state == Selecting)
    {

        if (event.type == SDL_KEYDOWN)
        {
            if (Game->machineMode) // if machine mode is on, return (no user input)
                return;
            int key = event.key.keysym.sym;
            if (key != SDLK_RIGHT && key != SDLK_LEFT && key != SDLK_UP && key != SDLK_DOWN && key != SDLK_RETURN)
                return;

            int n = Game->level + 5;

            if (Game->helpers.selectedI == -1 && Game->helpers.selectedJ == -1)
            {
                Game->helpers.selected = Game->solution->start + 1;

                if (Game->helpers.selected > 10 + 4 * (n - 2))
                    Game->helpers.selected = 11;
                findStart(n, Game->matrix, Game->helpers.selected, &Game->helpers.selectedI, &Game->helpers.selectedJ);

                return;
            }
            if (key == SDLK_RETURN)
            {
                if (Game->solution->endI == Game->helpers.selectedI && Game->solution->endJ == Game->helpers.selectedJ)
                    Game->helpers.win = 1;
                else
                    Game->helpers.win = -1;

                Game->state = Result;
                return;
            }

            if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_DOWN)
            {
                Game->helpers.selected++;
                if (Game->helpers.selected == Game->solution->start)
                    Game->helpers.selected++;
            }
            else if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_UP)
            {
                Game->helpers.selected--;
                if (Game->helpers.selected == Game->solution->start)
                    Game->helpers.selected--;
            }

            if (Game->helpers.selected < 11)
                Game->helpers.selected = 10 + 4 * (n - 2);
            else if (Game->helpers.selected > 10 + 4 * (n - 2))
                Game->helpers.selected = 11;

            if (Game->helpers.selected == Game->solution->start)
                key == SDLK_RIGHT ? Game->helpers.selected++ : Game->helpers.selected--;

            findStart(n, Game->matrix, Game->helpers.selected, &Game->helpers.selectedI, &Game->helpers.selectedJ);
            return;
        }

        if (event.type == SDL_MOUSEBUTTONUP)
        {
            if (Game->machineMode) // if machine mode is on, return (no user input)
                return;
            if (event.button.button != SDL_BUTTON_LEFT)
                return; // only left click

            int x = event.button.x; // get the click position
            int y = event.button.y;
            int i, j;
            getMatrixClick(renderer, x, y, Game->level + 5, &i, &j); // get the matrix position

            if ((i == -1 && j == -1) || (Game->solution->startI == i && Game->solution->startJ == j)) // if the click is outside the matrix
                return;

            if (Game->solution->endI == i && Game->solution->endJ == j)
            {

                Game->helpers.win = 1;
            }
            else
            {
                Game->helpers.win = -1;
            }
            Game->state = Result;
            return;
        }

        if (event.type == SDL_USEREVENT)

        {
            if (!Game->machineMode) // if machine mode is off, return (no machine input)
                return;
            int x = event.button.x; // get the click position
            int y = event.button.y;
            int i, j;
            getMatrixClick(renderer, x, y, Game->level + 5, &i, &j); // get the matrix position

            if (i == -1 && j == -1) // if the click is outside the matrix
                return;

            if (Game->solution->endI == i && Game->solution->endJ == j)
            {

                Game->helpers.win = 1;
            }
            else if (Game->solution->startI != i || Game->solution->startJ != j)
            {
                Game->helpers.win = -1;
            }
            Game->state = Result;
            return;
        }
    }
}
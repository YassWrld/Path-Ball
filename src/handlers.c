#include "handlers.h"
void handleEvents(SDL_Event event, SDL_Renderer *renderer, screen *Screen, game *Game, int *quit)
{
    handleGlobal(event, quit);
    switch (*Screen)
    {
    case MainMenu:
        handleMainMenu(event, renderer, Screen, Game);
        break;
    case ChooseMachineGameMode:
        handleChooseMachineGameMode(event, renderer, Screen, Game);
        break;
    case PlayerGameMode:
    case MachineGameMode:
        handleGameMode(event, renderer, Screen, Game);
        break;
    case TopPlayers:
        handleTopPlayers(event, renderer, Screen, Game);
        break;
    }
}

void handleGlobal(SDL_Event event, int *quit)
{
    if (event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)) // quit
    {
        *quit = 1;
        return;
    }
}

void handleGameMode(SDL_Event event, SDL_Renderer *renderer, screen *Secreen, game *Game)
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
                SDL_StopTextInput();                          // stop text input mode
                Game->helpers.gameStartTime = SDL_GetTicks(); // start the game
                Game->state = Memorizing;                     // change the state to memorizing
            }
            return;
        }

        if (event.type == SDL_TEXTINPUT)
        {
            // check if the name is too long
            if (!checkAllowedString(event.text.text))
                return;
            if (strlen(Game->player.name) >= MAX_NAME_LENGTH - 1)
                return;

            strcat(Game->player.name, event.text.text); // add the character to the name
            return;
        }
    }

    // Pause mode

    if ((event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_p) || isClickInButton(event, Game, Game->buttons.pause)) // pause
    {
        if (Game->state == TextInput || Game->state == GameOver)
            return;

        if (Game->state != Pause)
        {
            Game->helpers.prevState = Game->state;
            Game->helpers.pauseTime = SDL_GetTicks();
        }
        else
        {
            int addTime = SDL_GetTicks() - Game->helpers.pauseTime;

            Game->helpers.memorizingStartTime += addTime;
            Game->helpers.gameStartTime += addTime;
            if (Game->helpers.pathDrawStartTime != 0)
                Game->helpers.pathDrawStartTime += addTime;

            if (Game->helpers.pathEndCircleTime != 0)
                Game->helpers.pathEndCircleTime += addTime;

            if (Game->helpers.selectedMachineStart != 0)
                Game->helpers.machineModeSelectingTime += addTime;

            Game->helpers.pauseTime = 0;
        }
        Game->state = Game->state == Pause ? Game->helpers.prevState : Pause;

        return;
    }

    if (isClickInButton(event, Game, Game->buttons.MainMenu))
    {
        if (Game->player.score != 0 && !Game->machineMode && !Game->helpers.savedScore)
        {
            insertScore(Game->player);
            Game->helpers.updatedTopPlayers = true;
        }

        *Secreen = MainMenu;
        return;
    }
    if (Game->state == GameOver)
    {
        if (isClickInButton(event, Game, Game->buttons.playAgain))
        {

            initGame(Game, Game->machineMode, Game->manualFill, true);

            return;
        }
    }

    if (Game->state == Result)
    {
        if (isClickInButton(event, Game, Game->buttons.skip) || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_s)) // skip
        {
            Game->helpers.skipPath = true;
            return;
        }
    }

    if (Game->state == Filling)
    {

        if (event.type == SDL_MOUSEBUTTONUP)
        {
            if (event.button.button != SDL_BUTTON_LEFT)
                return; // only left click

            int x = event.button.x; // get the click position
            int y = event.button.y;
            int i, j;
            bool isOutside;
            getMatrixClick(renderer, x, y, Game->level + 5, &i, &j, &isOutside);
            // get the matrix position

            if (i == -1 && j == -1) // if the click is outside the matrix
                return;
            int n = Game->level + 5;
            if (isOutside)
            {
                Game->solution = solveMatrix(Game->matrix[i][j], Game->helpers.filledObstacles, n, Game->matrix);
                Game->state = Memorizing;
                return;
            }

            if (Game->matrix[i][j] == 0)
            {
                Game->matrix[i][j] = 1;
                Game->helpers.filledObstacles++;
            }
            else if (Game->matrix[i][j] == 1)
                Game->matrix[i][j] = -1;
            else if (Game->matrix[i][j] == -1)
            {
                Game->matrix[i][j] = 0;

                Game->helpers.filledObstacles--;
            }
            return;
        }

        if (event.type == SDL_KEYUP)
        {
            if (!Game->machineMode && Game->manualFill) // if machine mode is on, return (no user input)
                return;
            int key = event.key.keysym.sym;
            if (key != SDLK_RIGHT && key != SDLK_LEFT && key != SDLK_UP && key != SDLK_DOWN && key != SDLK_RETURN)
                return;

            int n = Game->level + 5;

            if (Game->helpers.selectedI == -1 && Game->helpers.selectedJ == -1)
            {
                Game->helpers.selected = 11;

                Game->helpers.selectedI = 0;
                Game->helpers.selectedJ = 1;

                return;
            }
            if (key == SDLK_RETURN)
            {

                Game->solution = solveMatrix(Game->helpers.selected, Game->helpers.filledObstacles, n, Game->matrix);
                Game->state = Memorizing;
                return;
            }

            if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_DOWN)
            {
                Game->helpers.selected++;
            }
            else if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_UP)
            {
                Game->helpers.selected--;
            }

            if (Game->helpers.selected < 11)
                Game->helpers.selected = 10 + 4 * (n - 2);
            else if (Game->helpers.selected > 10 + 4 * (n - 2))
                Game->helpers.selected = 11;

            findStart(n, Game->matrix, Game->helpers.selected, &Game->helpers.selectedI, &Game->helpers.selectedJ);
            return;
        }
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

                Game->helpers.win == -1 ? playSoundEffect(Game->sounds.lose) : playSoundEffect(Game->sounds.win);
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
            bool isOutside;
            getMatrixClick(renderer, x, y, Game->level + 5, &i, &j, &isOutside);
            // get the matrix position

            if (!isOutside)
                return;

            if ((i == -1 && j == -1) || (Game->solution->startI == i && Game->solution->startJ == j)) // if the click is outside the matrix
                return;

            if (Game->solution->endI == i && Game->solution->endJ == j)
            {

                Game->helpers.win = 1;
                playSoundEffect(Game->sounds.win);
            }
            else
            {
                Game->helpers.win = -1;
                playSoundEffect(Game->sounds.lose);
            }
            Game->helpers.selectedI = i;
            Game->helpers.selectedJ = j;
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
            bool isOutside;
            getMatrixClick(renderer, x, y, Game->level + 5, &i, &j, &isOutside); // get the matrix position
            if (!isOutside)
                return;

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

void handleMainMenu(SDL_Event event, SDL_Renderer *renderer, screen *Secreen, game *Game)
{
    if (event.type != SDL_MOUSEBUTTONUP)
        return;
    if (event.button.button != SDL_BUTTON_LEFT)
        return;

    if (isClickInButton(event, Game, Game->buttons.PlayerGameMode))
    {
        *Secreen = PlayerGameMode;
        initGame(Game, false, false, false); // player mode
        return;
    }
    if (isClickInButton(event, Game, Game->buttons.MachineGameMode))
    {
        *Secreen = ChooseMachineGameMode;
        return;
    }
    if (isClickInButton(event, Game, Game->buttons.TopPlayers))
    {

        *Secreen = TopPlayers;
        return;
    }
}

void handleChooseMachineGameMode(SDL_Event event, SDL_Renderer *renderer, screen *Secreen, game *Game)
{
    if (event.type != SDL_MOUSEBUTTONUP)
        return;
    if (event.button.button != SDL_BUTTON_LEFT)
        return;

    if (isClickInButton(event, Game, Game->buttons.MachineGameAutoMode))
    {
        *Secreen = MachineGameMode;
        initGame(Game, true, false, false);
        return;
    }
    if (isClickInButton(event, Game, Game->buttons.MachineGameManualMode))
    {
        *Secreen = MachineGameMode;
        initGame(Game, true, true, false);
        return;
    }
    if (isClickInButton(event, Game, Game->buttons.MainMenu))
    {
        *Secreen = MainMenu;
        return;
    }
}

void handleTopPlayers(SDL_Event event, SDL_Renderer *renderer, screen *Secreen, game *Game)
{
    if (event.type != SDL_MOUSEBUTTONUP)
        return;
    if (event.button.button != SDL_BUTTON_LEFT)
        return;

    if (isClickInButton(event, Game, Game->buttons.MainMenu))
    {

        *Secreen = MainMenu;
        return;
    }
}
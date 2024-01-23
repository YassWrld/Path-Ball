#include <logic.h>
/*
matrix logic part
*/

void initializeMatrix(int n, int matrix[n][n])
{
    matrix[0][0] = matrix[0][n - 1] = matrix[n - 1][0] = matrix[n - 1][n - 1] = -9;

    int borderNumber = 11;

    // Top border
    for (int j = 1; j < n - 1; j++)
    {
        matrix[0][j] = borderNumber++;
    }

    // Right border
    for (int i = 1; i < n - 1; i++)
    {
        matrix[i][n - 1] = borderNumber++;
    }

    // Bottom border
    for (int j = n - 2; j > 0; j--)
    {
        matrix[n - 1][j] = borderNumber++;
    }

    // Left border
    for (int i = n - 2; i > 0; i--)
    {
        matrix[i][0] = borderNumber++;
    }

    for (int i = 1; i < n - 1; i++)
    {
        for (int j = 1; j < n - 1; j++)
        {
            matrix[i][j] = 0;
        }
    }
}

int setObstacles(int n, int matrix[n][n])
{
    // Calculate the maximum number of obstacles in the interior
    int maxObstacles = (n - 2) * (n - 2) / 2 - 1;

    // Calculate the minimum number of obstacles (1/6 of the interior size)
    int minObstacles = ((n - 2) * (n - 2) / 6);

    // Generate a random number of obstacles from minObstacles to maxObstacles
    int numObstacles = randomInt(minObstacles, maxObstacles);
    int k = numObstacles;
    // Print the total number of obstacles

    // Place random obstacles (either 1 or -1) at random positions in the interior

    while (k > 0)
    {
        // Generate random coordinates within the interior
        int x = randomInt(0, n - 2);
        int y = randomInt(0, n - 2);

        if (matrix[x][y] == 0)
        {
            // Set a random obstacle (either 1 or -1)
            int obstacle = (randomInt(-1, 0) == 0) ? 1 : -1;
            matrix[x][y] = obstacle;

            k--;
        }
    }

    return numObstacles;
}

void findStart(int n, int matrix[n][n], int start, int *x, int *y)
{
    int i = 0;
    int j = 0;
    /*if (start < 11 || start > 10 + 4 * (n - 2))
    {
        *x = -1;
        *y = -1;
        return;
    }
*/
    while (matrix[i][j] != start)
    {
        if (i == 0 && j < n - 1)
        {
            j++;
        }
        else if (j == n - 1 && i < n - 1)
        {
            i++;
        }
        else if (i == n - 1 && j > 0)
        {
            j--;
        }
        else if (j == 0 && i > 0)
        {
            i--;
        }
    }

    *x = i;
    *y = j;
}

direction getNextDirection(direction d, int current)
{
    // Checking if there is not a diagonal
    if (current != -1 && current != 1)
        return d;

    // Checking colision with diagonals.
    switch (d)
    {
    case Up:
        d = current == 1 ? Right : Left;
        break;
    case Left:
        d = current == 1 ? Down : Up;
        break;
    case Down:
        d = current == 1 ? Left : Right;
        break;
    case Right:
        d = current == 1 ? Up : Down;
        break;
    }

    return d;
}

solution *solveMatrix(int start, int obs, int n, int matrix[n][n])
{
    solution *s = malloc(sizeof(solution));
    s->start = start;

    direction d;
    int i = 0, j = 0;
    findStart(n, matrix, start, &i, &j);
    s->startI = i;
    s->startJ = j;
    start = start - 10;
    int m = n - 2;

    if (0 < start && start <= m)
    {
        d = Down;
    }
    if (m < start && start <= 2 * m)
    {
        d = Left;
    }
    if (2 * m < start && start <= 3 * m)
    {
        d = Up;
    }
    if (3 * m < start && start <= 4 * m)
    {
        d = Right;
    }
    path *T = (path *)malloc(sizeof(path));
    T->x = i;
    T->y = j;
    T->next = NULL;
    s->path = T;
    s->hit = 0;

    path *temp = T;

    ball b = {d, i, j};
    while (matrix[b.x][b.y] < 2 || matrix[b.x][b.y] == start + 10)
    {
        b.dir = getNextDirection(b.dir, matrix[b.x][b.y]);
        if (matrix[b.x][b.y] == 1 || matrix[b.x][b.y] == -1)
            s->hit++;

        switch (b.dir)
        {
        case Up:
            b.x--;
            break;
        case Right:
            b.y++;
            break;
        case Down:
            b.x++;
            break;
        case Left:
            b.y--;
            break;
        }
        path *new = (path *)malloc(sizeof(path));
        new->x = b.x;
        new->y = b.y;
        new->next = NULL;
        temp->next = new;
        temp = new;
    }

    s->end = matrix[b.x][b.y];
    s->noHit = obs - s->hit;
    s->endI = b.x;
    s->endJ = b.y;

    return s;
}

solution *setupMatrix(int n, int matrix[n][n])
{
    initializeMatrix(n, matrix);
    int obs = setObstacles(n, matrix);
    // int start = randomInt(11, 4 * (n - 2));
    const int start = 11;
    solution *s = solveMatrix(start, obs, n, matrix);
    for (int i = start + 1; i <= 4 * (n - 2); i++)
    {
        solution *tmp = solveMatrix(i, obs, n, matrix);
        if (tmp->hit > s->hit)
        {
            freePath(s->path);
            s = tmp;
        }
    }

    return s;
}

void freePath(path *p)
{
    while (p != NULL)
    {
        path *tmp = p;
        p = p->next;
        free(tmp);
    }
}

/*
Score Managment Functions
*/

void sortTopPlayers(player arr[])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4 - i; j++)
        {
            // If the element found is greater than the next element, swap them
            if (arr[j].score < arr[j + 1].score)
            {
                // Swap elements
                player temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void insertScore(player current)
{
    FILE *file = fopen(SCORES_FILE_PATH, "a");

    // Check if the file was opened successfully
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return; // Return an error code
    }

    // Write the player's information to the file
    fprintf(file, "%d,%d-%d-%d,%s\n", current.score, current.date.day, current.date.month, current.date.year, current.name);

    // Close the file
    fclose(file);
}

void getTopPlayers(player players[])
{
    FILE *file = fopen(SCORES_FILE_PATH, "r");
    for (int i = 0; i < 5; i++)
    {
        players[i].score = 0;
        strcpy(players[i].name, "empty");
    }

    player current;
    player last;
    last.score = 0;
    last.date.day = 0;
    last.date.month = 0;
    last.date.year = 0;
    strcpy(last.name, "empty");
    int c;
    while (!(feof(file)))
    {
        fscanf(file, "%d,%d-%d-%d,%s", &current.score, &current.date.day, &current.date.month, &current.date.year, current.name);

        bool duplicate = current.score == last.score && strcmp(current.name, last.name) == 0 && current.date.day == last.date.day && current.date.month == last.date.month && current.date.year == last.date.year;
        if (current.score > players[4].score && !duplicate)
        {
            last = current;
            players[4] = current;
            sortTopPlayers(players);
        }

        while ((c = fgetc(file)) != '\n' && c != EOF)
        {
            // Do nothing, just advance the file position
        }
    }
}

/*
game part
*/

void initGame(game *Game, bool machineMode, bool manualFill)
{

    Game->level = 1;
    Game->maxLevel = Game->level;

    Game->winStreak = 0;
    Game->loseStreak = 0;
    Game->player.score = 0;
    Game->machineMode = machineMode;
    Game->manualFill = manualFill;
    if (manualFill && !machineMode)
        exit(1);

    // random name
    if (!machineMode)
    {
        sprintf(Game->player.name, "Player%d", randomInt(1, 1000));
        SDL_StartTextInput();
    }
    else
        sprintf(Game->player.name, "Machine");

    Game->player.date = getCurrentDate();

    Game->state = machineMode ? manualFill ? Filling : Memorizing : TextInput;

    Game->helpers.memorizingStartTime = 0;
    Game->helpers.pauseTime = 0;
    Game->helpers.selectedI = -1;
    Game->helpers.selectedJ = -1;
    Game->helpers.selected = -1;
    Game->helpers.prevState = Game->state;
    Game->helpers.win = 0;
    Game->helpers.filledObstacles = 0;
    Game->helpers.savedScore = false;
    Game->helpers.filledMachineMatrix = false;
    Game->helpers.updatedTopPlayers = false;
    Game->helpers.currentPath = NULL;
    Game->helpers.pathDrawStartTime = 0;
    Game->helpers.pathEndCircleTime = 0;
    Game->helpers.machineModeSelectingTime = 0;
    Game->helpers.selectedMachineStart = false;

    if (!Game->manualFill)
        Game->solution = setupMatrix(Game->level + 5, Game->matrix);
    else
    {
        initializeMatrix(Game->level + 5, Game->matrix);
        Game->solution = NULL;
    }
    Game->helpers.gameStartTime = SDL_GetTicks();
}

void updateLevelAndScore(game *Game)
{
    if (Game->helpers.win == 0)
        return;

    if (Game->helpers.win == 1)
    {
        int addedScore = Game->solution->noHit * 5 + Game->solution->hit * 10 + 100 * Game->level + Game->winStreak * 10;
        if (Game->level == Game->maxLevel)
            Game->player.score += addedScore;
        Game->winStreak++;
        Game->loseStreak = 0;
    }
    else if (Game->helpers.win == -1)
    {
        Game->loseStreak++;
        Game->winStreak = 0;
    }

    if (Game->winStreak % 3 == 0 && Game->winStreak != 0)
    {
        Game->level++;
        Game->loseStreak = 0;

        if (Game->level > Game->maxLevel)
            Game->maxLevel = Game->level;
    }

    if (Game->loseStreak == 3)
    {
        Game->level--;
        Game->winStreak = 0;
        Game->loseStreak = 0;
    }
    Game->helpers.win = 0;
    Game->helpers.filledMachineMatrix = false;
    Game->helpers.selectedI = -1;
    Game->helpers.selectedJ = -1;
    Game->helpers.selected = -1;
    Game->helpers.filledObstacles = 0;
    Game->helpers.memorizingStartTime = 0;
    Game->helpers.pathDrawStartTime = 0;
    Game->helpers.pathEndCircleTime = 0;
    Game->helpers.machineModeSelectingTime = 0;
    Game->helpers.currentPath = NULL;
    Game->helpers.selectedMachineStart = false;

    freePath(Game->solution->path);
    if (Game->level != 0 && Game->level != MAX_LEVEL)
    {
        if (!Game->manualFill)
            Game->solution = setupMatrix(Game->level + 5, Game->matrix);
        else
        {
            initializeMatrix(Game->level + 5, Game->matrix);
            Game->solution = NULL;
            Game->helpers.filledObstacles = 0;
        }
    }
}

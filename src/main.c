#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#define HEIGHT 700
#define WIDTH 1000

typedef struct path
{
    int x;
    int y;
    struct path *next;
} path;

typedef struct solution
{
    int start;
    int end;
    path *path;
} solution;

typedef struct player
{
    char name[20];
    int score;
} player;

typedef enum Directions
{
    Up,
    Right,
    Down,
    Left
} direction;

typedef struct ball
{
    direction dir;
    int x;
    int y;
} ball;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

void initialize();
void insertScore(player current);
void sortTopPlayers(player arr[]);
void getTopPlayers(player players[]);
int randomInt(int min, int max);
void initializeMatrix(int n, int matrix[n][n]);
void setObstacles(int n, int matrix[n][n]);
void findStart(int n, int matrix[n][n], int start, int *x, int *y);
direction getNextDirection(direction d, int current);
solution *solveMatrix(int start, int n, int matrix[n][n]);
void printMatrix(int n, int matrix[n][n]);
void drawFilledCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius);
void drawGrid(SDL_Renderer* renderer, int n, int cellSize);

int main()
{
    initialize();
    int n = 7;
    int matrix[n][n];
    initializeMatrix(n, matrix);
    setObstacles(n, matrix);
    int start = randomInt(11, 4 * (n - 2));
    solution *s = solveMatrix(start, n, matrix);
    printMatrix(n, matrix);
    printf("start: %d, end: %d\n", s->start, s->end);
    int cellSize = 500 / n;

    // Set the draw color to brown
    SDL_SetRenderDrawColor(renderer, 112, 66, 20, 255);

    // Clear the renderer with the brown color
    SDL_RenderClear(renderer);

    // Draw the grid
    drawGrid(renderer, n, cellSize);

    // Update the screen
    SDL_RenderPresent(renderer);

    SDL_Event e;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }
    }

    // Clean up and exit
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

}

void drawFilledCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    int x = radius - 1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);

    while (x >= y) {
        SDL_RenderDrawLine(renderer, centerX + x, centerY - y, centerX - x, centerY - y);
        SDL_RenderDrawLine(renderer, centerX + x, centerY + y, centerX - x, centerY + y);
        SDL_RenderDrawLine(renderer, centerX + y, centerY - x, centerX - y, centerY - x);
        SDL_RenderDrawLine(renderer, centerX + y, centerY + x, centerX - y, centerY + x);

        if (err <= 0) {
            y++;
            err += dy;
            dy += 2;
        }
        if (err > 0) {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
}

void drawGrid(SDL_Renderer* renderer, int n, int cellSize) {
    int offset = 100;
    
     
    SDL_SetRenderDrawColor(renderer, 180, 134, 88, 255);

    // Draw vertical lines
    int x = (n % 2 != 0) ? 1 : 0;
    int thickness = 4;
    for (int i = 0; i <= n - 2; ++i) {

        for (int s = 0; s < thickness + x; s++)
        {
            SDL_RenderDrawLine(renderer, cellSize + offset + i * cellSize + s, cellSize + offset, cellSize + offset + i * cellSize + s, offset + 500 - cellSize);
            SDL_RenderDrawLine(renderer, cellSize + offset, cellSize + offset + i * cellSize + s, offset + 500 - cellSize , cellSize + offset + i * cellSize + s);
        }
    }

    for (int i = 1; i < n - 1; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (j > 0 && j < n - 1)
                continue;

            int x = offset + i * cellSize + cellSize / 2 + thickness / 2;
            int y = offset + j * cellSize + cellSize / 2 + thickness / 2;

            // Hortizontal
            SDL_SetRenderDrawColor(renderer, 58, 32, 100, 255);
            drawFilledCircle(renderer, x, y, cellSize / 3);
            SDL_SetRenderDrawColor(renderer, 128, 80, 40, 255);
            drawFilledCircle(renderer, x, y, cellSize / 4);

            // Vertical
            SDL_SetRenderDrawColor(renderer, 58, 32, 100, 255);
            drawFilledCircle(renderer, y, x, cellSize / 3);
            SDL_SetRenderDrawColor(renderer, 128, 80, 40, 255);
            drawFilledCircle(renderer, y, x, cellSize / 4); 
        }
    }
}

void initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "SDL could not be initialized! SDL Error: %s\n", SDL_GetError());
        // Handle the SDL error and exit or return an error code.
    }

    // Initialize SDL_ttf
    if (TTF_Init() < 0)
    {
        fprintf(stderr, "SDL_ttf could not be initialized! SDL_ttf Error: %s\n", TTF_GetError());
        // Handle the SDL_ttf error and exit or return an error code.
    }

    window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void printPath(path *p)
{
    int i = 1;
    while (p != NULL)
    {
        printf("x%d: %d, y%d: %d\n", i, p->x, i, p->y);
        p = p->next;
    }
    
}

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
    FILE *file = fopen("scores.txt", "a");

    // Check if the file was opened successfully
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return 1; // Return an error code
    }

    // Write the player's information to the file
    fprintf(file, "%d,%s\n", current.score, current.name);

    // Close the file
    fclose(file);

    printf("Score saved successfully!\n");
}

void getTopPlayers(player players[])
{
    FILE *file = fopen("scores.txt", "r");
    for (int i = 0; i < 5; i++)
    {
        players[i].score = 0;
        strcpy(players[i].name, "empty");
    }

    player current;

    while (!(feof(file)))
    {
        fscanf(file, "%d,%s", &current.score, current.name);
        printf("current: %d, %s\n", current.score, current.name);
        int c;
        while ((c = fgetc(file)) != '\n' && c != EOF)
        {
            // Do nothing, just advance the file position
        }
        if (current.score > players[4].score)
        {
            players[4] = current;
            sortTopPlayers(players);
        }
    }

    return players;
}

int randomInt(int min, int max)
{
    static bool first = true;

    if (first)
    {
        srand(time(NULL));
        first = false;
    }
    return rand() % (max - min + 1) + min;
}

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

    // Filling the interior with zeros
    for (int i = 1; i < n - 1; i++)
    {
        for (int j = 1; j < n - 1; j++)
        {
            matrix[i][j] = 0;
        }
    }
}

void setObstacles(int n, int matrix[n][n])
{
    // Calculate the maximum number of obstacles in the interior
    int maxObstacles = (n - 2) * (n - 2) / 2 - 1;

    // Calculate the minimum number of obstacles (1/8 of the interior size)
    int minObstacles = ((n - 2) * (n - 2) / 8);

    // Generate a random number of obstacles from minObstacles to maxObstacles
    int numObstacles = randomInt(minObstacles, maxObstacles);

    // Print the total number of obstacles
    printf("Obstacles: %d\n", numObstacles);

    // Place random obstacles (either 1 or -1) at random positions in the interior
    while (numObstacles > 0)
    {
        // Generate random coordinates within the interior
        int x = randomInt(0, n - 2);
        int y = randomInt(0, n - 2);

        if (matrix[x][y] == 0)
        {
            // Set a random obstacle (either 1 or -1)
            int obstacle = (randomInt(-1, 0) == 0) ? 1 : -1;

            matrix[x][y] = obstacle;

            // Print the coordinates of the generated obstacle
            printf("Obstacle at (x, y) = (%d, %d)\n", x, y);

            numObstacles--;
        }
    }
}

void findStart(int n, int matrix[n][n], int start, int *x, int *y)
{
    int i = 0;
    int j = 0;

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

solution *solveMatrix(int start, int n, int matrix[n][n])
{
    solution *s = malloc(sizeof(solution));
    s->start = start;

    direction d;
    int i = 0, j = 0;
    findStart(n, matrix, start, &i, &j);
    path *p = malloc(sizeof(path));
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

    s->path = p;
    p->x = i;
    p->y = j;
    p->next = NULL;

    ball b = {d, i, j};
    while (matrix[b.x][b.y] < 2 || matrix[b.x][b.y] == start + 10)
    {
        b.dir = getNextDirection(b.dir, matrix[b.x][b.y]);
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
        path *new = malloc(sizeof(path));
        new->x = b.x;
        new->x = b.y;
        p->next = new;
        new->next = NULL;
        p = new;
    }

    s->end = matrix[b.x][b.y];
    return s;
}

void printMatrix(int n, int matrix[n][n])
{
    printf("+---");
    for (int j = 0; j < n; j++)
    {
        printf("----");
    }
    printf("--+\n");

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (j == 0)
            {
                printf("| ");
            }
            printf("%2d | ", matrix[i][j]);
        }

        printf("\n+---");
        for (int j = 0; j < n; j++)
        {
            printf("----");
        }
        printf("--+\n");
    }
}

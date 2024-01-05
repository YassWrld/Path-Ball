#include <stdio.h>

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "logic.h"
#include "utils.h"

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

direction nextDirection(direction d, int c)
{
    if (c != -1 && c != 1)
        return d;

    switch (d)
    {
    case Up:
        d = c == -1 ? Left : Right;
        break;
    case Left:
        d = c == -1 ? Up : Down;
        break;
    case Down:
        d = c == -1 ? Right : Left;
        break;
    case Right:
        d = c == -1 ? Down : Up;
        break;
    }

    return d;
}

int zerosMatrix(int n, int matrix[n][n])
{

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            matrix[i][j] = 0;
        }
    }
    return 0;
}

void findE(int n, int matrix[n][n], int e, int *x, int *y)
{
    int i = *x;
    int j = *y;

    while (matrix[i][j] != e)
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

int setupMatrix(int n, int matrix[n][n])
{
    int i = 0, j = 0;
    int count = 1;
    bool stop = false;

    while (1)
    {

        bool corner = (i == 0 && j == 0) || (i == 0 && j == n - 1) || (i == n - 1 && j == 0) || (i == n - 1 && j == n - 1);
        if (corner)
        {

            matrix[i][j] = -10;
        }
        else
        {
            matrix[i][j] = count + 10;
            count++;
        }

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

        if (i == 0 && j == 0 && count > 1)
        {
            break;
        }
    }
    int cells = (n - 2) * (n - 2);

    int k = randomInt(cells / 4, cells / 2);
    /*if (k == 0)
        k = 1;*/

    while (k > 0)
    {
        int x = 0, y = 0;

        do
        {

            x = randomInt(1, n - 2);
            y = randomInt(1, n - 2);
        } while (matrix[x][y] != 0);

        // printf("x=%d,y=%d\n", x, y);

        matrix[x][y] = randomInt(0, 1) == 0 ? -1 : 1;

        k--;
    }

    return 0;
}

solution *solveMatrix(int n, int matrix[n][n], int start)
{
    solution *s = malloc(sizeof(solution));
    s->start = start;

    direction d;

    int a = n - 2;
    if (0 < start && start <= a)
    {
        d = Down;
    }
    if (a < start && start <= 2 * a)
    {

        d = Left;
    }
    if (2 * a < start && start <= 3 * a)
    {

        d = Up;
    }
    if (3 * a < start && start <= 4 * a)
    {

        d = Right;
    }
    int i = 0, j = 0;
    findE(n, matrix, start + 10, &i, &j);
    path *p = malloc(sizeof(path));
    s->path = p;

    p->x = i;
    p->y = j;
    p->next = NULL;

    while (matrix[i][j] < 2 || matrix[i][j] == start + 10)
    {

        d = nextDirection(d, matrix[i][j]);
        switch (d)
        {
        case Up:
            i--;
            break;
        case Right:
            j++;
            break;
        case Down:
            i++;
            break;
        case Left:
            j--;
            break;
        }
        path *new = malloc(sizeof(path));
        new->x = i;
        new->y = j;
        p->next = new;
        new->next = NULL;
        p = new;
    }
    s->end = matrix[i][j] - 10;
    return s;
}

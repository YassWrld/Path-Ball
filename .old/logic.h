#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

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
typedef enum Directions
{

    Up,
    Right,
    Down,
    Left
} direction;
void printMatrix(int n, int matrix[n][n]);
direction nextDirection(direction d, int c);
int zerosMatrix(int n, int matrix[n][n]);
void findE(int n, int matrix[n][n], int e, int *x, int *y);
int setupMatrix(int n, int matrix[n][n]);
solution* solveMatrix(int n, int matrix[n][n], int start);
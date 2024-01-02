

#ifndef LOGIC_H
#define LOGIC_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util.h"

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
    int noHit;
    int hit;
    path *path;

} solution;

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

typedef struct player
{
    char name[20];
    int score;
    date_t date;

} player;
solution *setupMatrix(int n, int matrix[n][n]);
int updateLevel(int level, int *winStreak, int *loseStreak);
void initializeMatrix(int n, int matrix[n][n]);
void findStart(int n, int matrix[n][n], int start, int *x, int *y);
int setObstacles(int n, int matrix[n][n]);
direction getNextDirection(direction d, int current);
solution *solveMatrix(int start, int obs, int n, int matrix[n][n]);
void printPath(path *p);

void insertScore(player current);
void sortTopPlayers(player arr[]);
void getTopPlayers(player players[]);

#endif // LOGIC_H
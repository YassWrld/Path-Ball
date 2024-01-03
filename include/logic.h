#ifndef LOGIC_H
#define LOGIC_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util.h"

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

typedef struct path
{
    int x;
    int y;
    struct path *next;
} path;

typedef struct solution
{
    int start;
    int startI;
    int startJ;
    int end;
    int endI;
    int endJ;
    int noHit;
    int hit;
    path *path;

} solution;
typedef struct player
{
    char name[20];
    int score;
    date_t date;

} player;

typedef enum game_state
{
    Memorizing,
    Selecting,
    Result,
} game_state;

typedef struct game
{
    game_state state;
    player player;
    int matrix[20][20];
    int level;
    int maxLevel;
    int winStreak;
    int loseStreak;
    solution *solution;
} game;

solution *setupMatrix(int n, int matrix[n][n]);
void updateLevel(game *game);
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
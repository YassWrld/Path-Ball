#ifndef LOGIC_H
#define LOGIC_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util.h"

#define MAX_NAME_LENGTH 20
#define MAX_MATRIX_SIZE 20
#define MAX_LEVEL 11
#define MEMORIZING_TIME 3 * 1000

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
    char name[MAX_NAME_LENGTH];
    int score;
    date_t date;

} player;

typedef enum game_state
{
    TextInput,
    Memorizing,
    Selecting,
    Result,
    Pause,
    GameOver,
} game_state;

typedef struct helpers
{
    int selected;
    int selectedI;
    int selectedJ;
    int startTime;
    int pauseTime;
    int win;
    game_state prevState;

} helpers_t;

typedef struct game
{
    game_state state;
    player player;
    int matrix[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];
    int level;
    int maxLevel;
    int winStreak;
    int loseStreak;
    helpers_t helpers;
    bool machineMode;
    solution *solution;
} game;

solution *setupMatrix(int n, int matrix[n][n]);
void updateLevelAndScore(game *Game);
void initializeMatrix(int n, int matrix[n][n]);
void findStart(int n, int matrix[n][n], int start, int *x, int *y);
int setObstacles(int n, int matrix[n][n]);
direction getNextDirection(direction d, int current);
solution *solveMatrix(int start, int obs, int n, int matrix[n][n]);
void printPath(path *p);
void initGame(game *Game, bool machineMode);

void insertScore(player current);
void sortTopPlayers(player arr[]);
void getTopPlayers(player players[]);

#endif // LOGIC_H
#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util.h"

#define MAX_NAME_LENGTH 20       // max length of player name
#define MAX_MATRIX_SIZE 20       // max size of matrix
#define MAX_LEVEL 11             // the real max level is MAX_LEVEL - 1
#define MEMORIZING_TIME 3 * 1000 // memorizing time in milliseconds

typedef enum Directions
{
    Up,    // 0
    Right, // 1
    Down,  // 2
    Left   // 3

} direction;

typedef struct ball
{
    direction dir;
    int x; // current point i
    int y; // current point j
} ball;

typedef struct path
{
    int x; // point i
    int y; // point j
    struct path *next;

} path;

typedef struct solution
{
    int start;  // start circle number
    int startI; // start circle i
    int startJ; // start circle j

    int end;  // end circle number
    int endI; // end circle i
    int endJ; // end circle j

    int noHit; // obs
    int hit;

    path *path;

} solution;
typedef struct player
{
    char name[MAX_NAME_LENGTH]; // player name
    int score;                  // player score
    date_t date;                // date of the game

} player;
typedef struct Button
{
    int centerX, centerY;
    int width, height;
    SDL_Color color;
    SDL_Color hoverColor;
    SDL_Color textColor;
    SDL_Color outlineColor;
    int outlineThickness;

    char label[20];
} button;

typedef enum game_state
{
    TextInput,  // text input mode (for player name)
    Filling,    // filling the matrix with obstacles
    Memorizing, // memorizing state
    Selecting,  // selecting state
    Result,     // result state
    Pause,      // pause state
    GameOver,   // game over state (win/lose)
} game_state;

typedef struct helpers
{
    int selected;             // selected circle (keyboard)
    int selectedI;            // selected i
    int selectedJ;            // selected j
    int memorizingStartTime;  // time when the memorizing state started
    int gameStartTime;        // time when the game started
    int resultTime;           // time when the result state started
    int pauseTime;            // time when the pause state started
    int filledObstacles;      // number of filled obstacles
    int win;                  // 1 if win, -1 if lose, 0 if not finished
    bool savedScore;          // true if the score is saved in the file
    bool filledMachineMatrix; // true if the machine matrix is filled
    bool updatedTopPlayers;   // true if the top players are updated
    player topPlayers[5];     // top players

    // path *currentPath;       // current path of the ball
    // int currentPathLength;   // current path length
    game_state prevState; // previous state before pause

} helpers_t;

typedef struct buttonsHandle
{

    button pause;
    button playAgain;

    button MainMenu;
    button PlayerGameMode;
    button MachineGameMode;
    button MachineGameAutoMode;
    button MachineGameManualMode;
    button TopPlayers;
} buttonsHandle;

typedef struct game
{
    game_state state; // game state

    player player; // player info

    int matrix[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE]; // for human mode

    int machineMatrix[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE]; // for machine mode

    int level;      // current level
    int maxLevel;   // max level reached
    int winStreak;  // win streak
    int loseStreak; // lose streak

    bool machineMode; // machine mode on/off
    bool manualFill;  // manual fill on/off

    solution *solution; // solution for current level

    helpers_t helpers; // control variables

    buttonsHandle buttons; // buttons
                           // Mix_Chunk *clickSound;

} game;

typedef enum Screen // screen enum
{
    MainMenu,              // main menu screen
    ChooseMachineGameMode, // choose game mode screen
    PlayerGameMode,        // player game mode screen
    MachineGameMode,       // machine game mode screen
    TopPlayers,            // top players screen
    Credits,               // credits screen
} screen;

solution *setupMatrix(int n, int matrix[n][n]); // setup the matrix for the current level (initialize, set obstacles,choose start, solve the matrix) and return the solution
void initializeMatrix(int n, int matrix[n][n]); // initialize outer matrix and fill inner matrix with 0s
int setObstacles(int n, int matrix[n][n]);      // set obstacles in the matrix

void findStart(int n, int matrix[n][n], int start, int *x, int *y); // find the start circle i, j
direction getNextDirection(direction d, int current);               // get the next direction of the ball
solution *solveMatrix(int start, int obs, int n, int matrix[n][n]); // solve the matrix and return the solution

void freePath(path *p);

void initGame(game *Game, bool machineMode, bool manualFill);
// initialize the game
void updateLevelAndScore(game *Game); // update the level and score of the player and the game
void insertScore(player current);     // insert the score of the current player in the file
void sortTopPlayers(player arr[]);    // sort the top players by score
void getTopPlayers(player players[]); // get the top players from the file (5 players)

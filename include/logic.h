#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util.h"

#define MAX_NAME_LENGTH 21          // max length of player name
#define MAX_MATRIX_SIZE 20          // max size of matrix
#define MAX_N MAX_MATRIX_SIZE       // just an alias for MAX_MATRIX_SIZE
#define MAX_LEVEL 11                // the real max level is MAX_LEVEL - 1
#define MEMORIZING_TIME 3 * 1000    // memorizing time in milliseconds
#define CIRCLE_DRAW_TIME 500        // circle draw time in milliseconds
#define FULL_PATH_SHOW_TIME 1500    // full path draw time in milliseconds
#define MACHINE_SELECTING_TIME 2500 // machine selecting time in milliseconds

#define SCORES_FILE_PATH "assets/data/scores.txt" // scores file path

#define MUSIC_PATH "assets/sounds/music.mp3"       // background music path
#define STEP_SOUND_PATH "assets/sounds/step.wav"   // step sound path
#define CLICK_SOUND_PATH "assets/sounds/click.wav" // click sound path
#define WIN_SOUND_PATH "assets/sounds/right.wav"   // win sound path
#define LOSE_SOUND_PATH "assets/sounds/wrong.wav"  // lose sound path

typedef enum Directions // direction enum
{
    Up,    // 0
    Right, // 1
    Down,  // 2
    Left   // 3
} direction;

typedef struct ball // ball struct
{
    direction dir; // direction of the ball
    int x;         // current point i
    int y;         // current point j
} ball;

typedef struct path // path struct (linked list)
{
    int x;         // point i
    int y;         // point j
    direction dir; // direction of the ball
    struct path *next;

} path;

typedef struct solution // solution struct
{
    int start;  // start circle number
    int startI; // start circle i
    int startJ; // start circle j

    int end;  // end circle number
    int endI; // end circle i
    int endJ; // end circle j

    int noHit; // obstacles not hit
    int hit;   // obs

    path *path; // path of the ball

} solution;
typedef struct player // player struct
{
    char name[MAX_NAME_LENGTH]; // player name
    int score;                  // player score
    date_t date;                // date of the game
} player;

typedef struct button // button struct
{
    int centerX, centerY;   // center of the button
    int width, height;      // width and height of the button
    SDL_Color color;        // color of the button
    SDL_Color hoverColor;   // hover color of the button
    SDL_Color textColor;    // text color
    SDL_Color outlineColor; // outline color
    int outlineThickness;   // outline thickness
    char label[20];         // label of the button
    char iconPath[50];      // icon path of the button
} button;

typedef enum game_state // game state enum
{
    TextInput,  // text input mode (for player name)
    Filling,    // filling the matrix with obstacles
    Memorizing, // memorizing state
    Selecting,  // selecting state
    Result,     // result state
    Pause,      // pause state
    GameOver,   // game over state (win/lose)
} game_state;

typedef struct helpers // control variables
{
    int selected;                 // selected circle (keyboard)
    int selectedI;                // selected i
    int selectedJ;                // selected j
    int memorizingStartTime;      // time when the memorizing state started
    int gameStartTime;            // time when the game started
    int pathDrawStartTime;        // time when the path started to be drawn
    int pathEndCircleTime;        // time when the path ended to be drawn
    int resultTime;               // time when the result state started
    int pauseTime;                // time when the pause state started
    int machineModeSelectingTime; // time when the machine mode selecting state started
    int filledObstacles;          // number of filled obstacles
    int win;                      // 1 if win, -1 if lose, 0 if not finished

    bool savedScore;           // true if the score is saved in the file
    bool filledMachineMatrix;  // true if the machine matrix is filled
    bool selectedMachineStart; // true if the start circle is selected
    bool updatedTopPlayers;    // true if the top players are updated
    bool skipPath;             // true if the path is skipped

    player topPlayers[5]; // top players
    path *currentPath;    // current path of the ball
    game_state prevState; // previous state before pause

} helpers_t;

typedef struct buttonsHandle
{
    button pause;                 // pause button (in game)
    button playAgain;             // play again button (in game)
    button skip;                  // skip button (in game)
    button MainMenu;              // main menu button
    button PlayerGameMode;        // player game mode button
    button MachineGameMode;       // machine game mode button (choose mode)
    button MachineGameAutoMode;   // machine game auto mode button
    button MachineGameManualMode; // machine game manual mode button
    button TopPlayers;            // top players button
} buttonsHandle;

typedef struct sounds_t // sounds struct (sound effects and background music)
{
    Mix_Chunk *click; // click sound (button click)
    Mix_Chunk *win;   // win sound (win state)
    Mix_Chunk *lose;  // lose sound (lose state)
    Mix_Chunk *step;  // step sound (path draw)
    Mix_Music *music; // background music

} sounds_t;

typedef struct game // game struct (main game structure)
{
    game_state state; // game state

    player player; // player info

    int matrix[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE]; // for human mode

    int machineMatrix[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE]; // for machine mode

    int level;             // current level
    int maxLevel;          // max level reached
    int winStreak;         // win streak
    int loseStreak;        // lose streak
    bool machineMode;      // machine mode on/off
    bool manualFill;       // manual fill on/off
    solution *solution;    // solution for current level
    helpers_t helpers;     // control variables
    buttonsHandle buttons; // buttons
    sounds_t sounds;       // sounds

} game;

typedef enum Screen // screen enum
{
    MainMenu,              // main menu screen
    ChooseMachineGameMode, // choose game mode screen
    PlayerGameMode,        // player game mode screen
    MachineGameMode,       // machine game mode screen
    TopPlayers,            // top players screen
} screen;

/**
 * @brief Initialize the matrix
 *
 * (Initializes the outer matrix and fills the inner matrix with 0s)
 *
 * @param n: size of the matrix
 * @param matrix: the matrix to be initialized
 * @return void
 */
void initializeMatrix(int n, int matrix[MAX_N][MAX_N]);

/**
 * @brief Set obstacles in the matrix
 *
 * (Sets obstacles randomly in the matrix)
 *
 * @param n: size of the matrix
 * @param matrix: the matrix to set obstacles in
 * @return int: number of obstacles set
 */
int setObstacles(int n, int matrix[MAX_N][MAX_N]);

/**
 * @brief Find the start circle i, j,
 *
 * (Finds the position of the start circle in the matrix)
 *
 * @param n: size of the matrix
 * @param matrix: the matrix to search in
 * @param start: value representing the start circle
 * @param x: pointer to store the row index of the start circle
 * @param y: pointer to store the column index of the start circle
 * @return void
 */
void findStart(int n, int matrix[MAX_N][MAX_N], int start, int *x, int *y);

/**
 * @brief Get the next direction of the ball
 *
 * (Determines the next direction of the ball based on its current direction and the current cell)
 *
 * @param d: current direction of the ball
 * @param current: value of the current cell
 * @return direction: next direction of the ball
 */
direction getNextDirection(direction d, int current);

/**
 * @brief Solve the matrix
 *
 * (Solves the matrix and returns the solution)
 *
 * @param start: value representing the start circle
 * @param obs: value representing obstacles
 * @param n: size of the matrix
 * @param matrix: the matrix to be solved
 * @return solution*: pointer to the solution
 */
solution *solveMatrix(int start, int obs, int n, int matrix[MAX_N][MAX_N]);

/**
 * @brief Setup the matrix for the current level
 *
 * (Initializes, sets obstacles, chooses start, and solves the matrix)
 *
 * @param n: size of the matrix
 * @param matrix: the matrix to be setup
 * @return solution*: pointer to the solution
 */
solution *setupMatrix(int n, int matrix[MAX_N][MAX_N]);
/**
 * @brief Load all the sounds
 *
 * (Loads all the sound effects and background music)
 *
 * @param Game: pointer to the game structure
 * @return void
 */
void loadAllSounds(game *Game);

/**
 * @brief Initialize the game
 *
 * (Initializes the game structure)
 *
 * @param Game: pointer to the game structure
 * @param machineMode: flag indicating machine mode
 * @param manualFill: flag indicating manual fill mode
 * @param playAgain: flag indicating play again mode
 * @return void
 */
void initGame(game *Game, bool machineMode, bool manualFill, bool playAgain);

/**
 * @brief Update the level and score of the player and the game
 *
 * (Updates the level and score of the player and the game)
 *
 * @param Game: pointer to the game structure
 * @return void
 */
void updateLevelAndScore(game *Game);

/**
 * @brief Insert the score of the current player in the file
 *
 * (Inserts the score of the current player in the file)
 *
 * @param current: current player's score
 * @return void
 */
void insertScore(player current);

/**
 * @brief Sort the top players by score
 *
 * (Sorts the top players by their scores in descending order)
 *
 * @param arr: array of players
 * @return void
 */
void sortTopPlayers(player arr[]);

/**
 * @brief Get the top players from the file (5 players)
 *
 * (Retrieves the top players from the file)
 *
 * @param players: array to store the top players
 * @return void
 */
void getTopPlayers(player players[]);

/**
 * @brief Free the path (linked list)
 *
 * (Frees the memory occupied by the path linked list)
 *
 * @param p: pointer to the path
 * @return void
 */
void freePath(path *p);

/**
 * @brief Print the matrix to the console.
 * @param n: size of the matrix
 * @param matrix: the matrix to be printed
 * @return void
 */
void printMatrix(int n, int matrix[MAX_N][MAX_N]);

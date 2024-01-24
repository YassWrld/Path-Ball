
#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "util.h"
#include "logic.h"

#define GAME_TITLE "Pinball Recall"


#define HEIGHT 700    // window height
#define WIDTH 1100    // window width
#define GRID_SIZE 580 // grid size (square)
#define OFFSET 60     // offset from the top left corner of the window
#define THICKNESS 4   // thickness of the lines

#define BACKGROUND_COLOR 94, 26, 32, 255  // wood brown
#define BLUR_BACKGROUND_COLOR 180, 134, 88, 30 // wood brown
#define DIAGONAL_COLOR 227, 180, 122, 255 // white
#define BORDER_COLOR 190, 121, 79, 255    // beige

#define SMALL_CIRCLE_COLOR 141, 64, 47, 255  // light brown
#define HOVER_CIRCLE_COLOR 227 , 180, 122, 255 // red

#define SELECTED_CIRCLE_COLOR 40, 80, 128, 255 // yellow
#define PATH_COLOR 0, 169, 157, 255            // blue

#define CIRCLE_OUTLINE_COLOR 241, 237, 232, 255 // white
#define WRONG_CIRCLE_COLOR 255, 0, 0, 255       // red
#define START_CIRCLE_COLOR PATH_COLOR
#define END_CIRCLE_COLOR 0, 255, 0, 255 // green

#define SIDE_BAR_COLOR 227, 180, 122, 255          // 192, 192, 192, 255 // beige
#define SIDE_BAR_OUTLINE_COLOR 141, 64, 47, 255   // 112, 128, 144, 255 // black

#define DISPLAY_BOX_COLOR 190, 121, 79, 255 // white

#define FONT_COLOR 0, 0, 0, 255 // black
#define FONT_SIZE 30
// monocraft
// #define MONOCRAFT_FONT "assets/fonts/monocraft.ttf"// black
#define GAMEPAUSED_FONT "assets/fonts/gamepaused.otf" // black

#define MUSIC_PATH "assets/sounds/music.mp3"
#define STEPS_SOUND_PATH "assets/sounds/step.wav"
#define CLICK_SOUND_PATH "assets/sounds/click.wav"
#define RIGHT_SOUND_PATH "assets/sounds/right.wav"
#define WRONG_SOUND_PATH "assets/sounds/wrong.wav"

#define ICON_PATH "assets/images/icon.png"
#define NOBG_ICON_PATH "assets/images/nobg-icon.png"
#define ENTER_ICON_PATH "assets/images/enter.png"
#define BACK_ICON_PATH "assets/images/back.png"
#define PAUSE_ICON_PATH "assets/images/pause.png"
#define RESUME_ICON_PATH "assets/images/resume.png"
#define ROBOT_ICON_PATH "assets/images/robot.png"
#define HUMAN_ICON_PATH "assets/images/human.png"
#define CROWN_ICON_PATH "assets/images/crown.png"
#define SAVE_ICON_PATH "assets/images/save.png"
#define AUTO_ICON_PATH "assets/images/auto.png"
#define MANUAL_ICON_PATH "assets/images/manual.png"
#define HEART_ICON_PATH "assets/images/heart.png"
#define EXIT_ICON_PATH "assets/images/exit.png"
#define AGAIN_ICON_PATH "assets/images/again.png"

#define FIRST_ICON_PATH "assets/images/first.png"
#define SECOND_ICON_PATH "assets/images/second.png"
#define THIRD_ICON_PATH "assets/images/third.png"

void drawFilledCircle(SDL_Renderer *renderer, int centerX, int centerY, int radius);
void drawImage(SDL_Renderer *renderer, char *path, int x, int y, int w, int h);
void writeText(SDL_Renderer *renderer, char *fontPath, char *text, int x, int y, int size, int r, int g, int b, int a);
void drawButton(SDL_Renderer *renderer, button *Button);

// Text input
void drawTextInput(SDL_Renderer *renderer, game *Game);
// Matrix
void drawGrid(SDL_Renderer *renderer, game *Game);
void drawDiagonal(SDL_Renderer *renderer, int n, int direction, int centerX, int centerY);
// void drawPath(SDL_Renderer *renderer, game *Game);
bool drawPath(SDL_Renderer *renderer, game *Game);
int mesurePathLength(path *path);
void drawSideBar(SDL_Renderer *renderer, game *Game);  // Side bar
void drawGameOver(SDL_Renderer *renderer, game *Game); // Game over
void drawPause(SDL_Renderer *renderer, game *Game);    // Pause

void drawMainMenu(SDL_Renderer *renderer, game *Game);
void drawTopPlayers(SDL_Renderer *renderer, game *Game);
void machineModeMemorize(SDL_Renderer *renderer, int n, int matrix[n][n]);
void machineModeSelecting(SDL_Renderer *renderer, int n, int matrix[n][n], int *selectI, int *selectJ);
void drawChooseMachineGameMode(SDL_Renderer *renderer, game *Game);
void machineModeChoosing(SDL_Renderer *renderer, game *Game);

void getMatrixClick(SDL_Renderer *renderer, int clickX, int clickY, int n, int *i, int *j, bool *isOutside); // Get matrix click
SDL_Color getPixelColor(SDL_Renderer *renderer, int pixel_X, int pixel_Y);
bool compareColor(SDL_Color color, int r, int g, int b, int a);
int mesureTextWidth(char *fontPath, char *text, int size);
bool isClickInButton(SDL_Event event, button *Button);

void drawBlurredBackground(SDL_Renderer *renderer);

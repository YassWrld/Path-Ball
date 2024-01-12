

#ifndef DRAW_H
#define DRAW_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#ifndef UTIL_H

#include "util.h"
#endif // UTIL_H

#ifndef LOGIC_H

#include "logic.h"
#endif // LOGIC_H



#define HEIGHT 700    // window height
#define WIDTH 1100    // window width
#define GRID_SIZE 580 // grid size (square)
#define OFFSET 60     // offset from the top left corner of the window
#define THICKNESS 4   // thickness of the lines

#define BACKGROUND_COLOR 112, 66, 20, 255      // wood brown
#define DIAGONAL_COLOR 241, 237, 232, 255      // white
#define BORDER_COLOR 180, 134, 88, 255         // beige
#define SMALL_CIRCLE_COLOR 128, 80, 40, 255    // light brown
#define HOVER_CIRCLE_COLOR 150, 100, 60, 255   // red
#define SELECTED_CIRCLE_COLOR 40, 80, 128, 255 // yellow
#define PATH_COLOR 0, 169, 157, 255            // blue

#define CIRCLE_OUTLINE_COLOR 241, 237, 232, 255 // white
#define WRONG_CIRCLE_COLOR 255, 0, 0, 255       // red
#define START_CIRCLE_COLOR PATH_COLOR
#define END_CIRCLE_COLOR 0, 255, 0, 255 // green

#define SIDE_BAR_COLOR 139, 161, 175, 255       // 192, 192, 192, 255 // beige
#define SIDE_BAR_OUTLINE_COLOR 81, 96, 124, 255 // 112, 128, 144, 255 // black

#define FONT_COLOR 0, 0, 0, 255 // black
#define FONT_SIZE 30
// monocraft
// #define MONOCRAFT_FONT "assets/fonts/monocraft.ttf"// black
#define GAMEPAUSED_FONT "assets/fonts/gamepaused.otf" // black


void drawFilledCircle(SDL_Renderer *renderer, int centerX, int centerY, int radius);

// Text input
void drawTextInput(SDL_Renderer *renderer, game *Game);

// Matrix
void drawGrid(SDL_Renderer *renderer, game *Game);
void drawDiagonal(SDL_Renderer *renderer, int n, int direction, int centerX, int centerY);
void drawPath(SDL_Renderer *renderer, game *Game);

void getMatrixClick(SDL_Renderer *renderer, int clickX, int clickY, int n, int *i, int *j);

// Side bar
void drawSideBar(SDL_Renderer *renderer, game *Game);

void drawGameOver(SDL_Renderer *renderer, game *Game);
void drawPause(SDL_Renderer *renderer, game *Game);

void writeText(SDL_Renderer *renderer, char *fontPath, char *text, int x, int y, int size, int r, int g, int b, int a);
int mesureTextWidth(char *fontPath, char *text, int size);

void drawSVG(SDL_Renderer *renderer, char *path, int x, int y, int w, int h);

Mix_Music *playMusic(char *path);
void playSoundEffect(char *path);
void drawFilledArc(SDL_Renderer *renderer, int centerX, int centerY, int radius, int startAngle, int endAngle);
SDL_Color getPixelColor(SDL_Renderer *renderer, int pixel_X, int pixel_Y);
bool compareColor(SDL_Color color, int r, int g, int b, int a);

void machineModeMemorize(SDL_Renderer *renderer, int n, int matrix[n][n]);
void machineModeSelecting(SDL_Renderer *renderer, int n, int matrix[n][n]);
#endif // DRAW_H


#ifndef DRAW_H
#define DRAW_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "logic.h"

#define HEIGHT 700    // window height
#define WIDTH 1100    // window width
#define GRID_SIZE 580 // grid size (square)
#define OFFSET 60     // offset from the top left corner of the window
#define THICKNESS 4   // thickness of the lines

#define BACKGROUND_COLOR 112, 66, 20, 255   // wood brown
#define DIAGONAL_COLOR 241, 237, 232, 255   // white
#define BORDER_COLOR 180, 134, 88, 255      // beige
#define SMALL_CIRCLE_COLOR 128, 80, 40, 255 // light brown
#define HOVER_CIRCLE_COLOR 255, 0, 0, 255   // red
#define PATH_COLOR 0, 169, 157, 255         // blue

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
#define MONOCRAFT_FONT "assets/fonts/gamepaused-final.otf" // black

void drawFilledCircle(SDL_Renderer *renderer, int centerX, int centerY, int radius);

// Text input
void drawTextInput(SDL_Renderer *renderer, game *Game);

// Matrix
void drawGrid(SDL_Renderer *renderer, game *Game);
void drawDiagonal(SDL_Renderer *renderer, int n, int direction, int centerX, int centerY);
void drawPath(SDL_Renderer *renderer, int n, path *sPath);

// Side bar
void drawSideBar(SDL_Renderer *renderer, game *Game);

SDL_Color getPixelColor(SDL_Renderer *renderer, int pixel_X, int pixel_Y);
void getMatrixClick(SDL_Renderer *renderer, int clickX, int clickY, int n, int *i, int *j);
void writeText(SDL_Renderer *renderer, char *fontPath, char *text, int x, int y, int size, int r, int g, int b, int a);
int mesureTextWidth(char *fontPath, char *text, int size);

Mix_Music *playMusic(char *path);
void playSoundEffect(char *path);
#endif // DRAW_H
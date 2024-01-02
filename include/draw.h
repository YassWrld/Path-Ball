

#ifndef DRAW_H
#define DRAW_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#include <SDL2/SDL_mixer.h>
#include "logic.h"

#define HEIGHT 700
#define WIDTH 1100
#define GRID_SIZE 580
#define OFFSET 60
#define THICKNESS 4

#define BACKGROUND_COLOR 112, 66, 20, 255
#define DIAGONAL_COLOR 241, 237, 232, 255
#define BORDER_COLOR 180, 134, 88, 255
#define BIG_CIRCLE_COLOR 180, 134, 88, 255
#define SMALL_CIRCLE_COLOR 128, 80, 40, 255
#define HOVER_CIRCLE_COLOR 255, 0, 0, 255
#define PATH_COLOR 0, 169, 157, 255

void drawFilledCircle(SDL_Renderer *renderer, int centerX, int centerY, int radius);
void drawGrid(SDL_Renderer *renderer, int n, int cellSize, int matrix[n][n], int hoverX, int hoverY);
void drawDiagonal(SDL_Renderer *renderer, int cellsize, int direction, int centerX, int centerY);
void drawPath(SDL_Renderer *renderer, int cellSize, path *sPath);

SDL_Color getPixelColor(SDL_Renderer *renderer, int pixel_X, int pixel_Y);
void getMatrixClick(SDL_Renderer *renderer, int clickX, int clickY, int n, int *i, int *j);




#endif // DRAW_H
#ifndef FUNCTIONS_H
#include <SDL2/SDL.h>
#include "SDL2/SDL_mixer.h"
#define FUNCTIONS_H


typedef struct dimensions dimensions;
typedef struct point point;
typedef struct color color;
struct dimensions
{
    int width;
    int height;
};

struct point
{
    int x;
    int y;
};

struct color
{
    int r;
    int g;
    int b;
    int a;
};

void waitUntil(SDL_EventType type);

dimensions getDimensions(SDL_Window *window);

void drawLine(SDL_Renderer *renderer, point start, point end, color c);

void drawPoint(SDL_Renderer *renderer, point p, color c);

void drawLine(SDL_Renderer *renderer, point start, point end, color c);

void writeText(SDL_Renderer *renderer, char *text, point p, int size, color c);

void drawImage(SDL_Renderer *renderer, char *path, point p, dimensions d);
void drawCircle(SDL_Renderer *renderer, int centerX, int centerY, int radius , color c);
void drawFilledCircle(SDL_Renderer *renderer, int centerX, int centerY, int radius, color c);
Mix_Music *playMusic(char *path);

#endif
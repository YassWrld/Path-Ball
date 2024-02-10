#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "functions.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"

void waitUntil(SDL_EventType type)

{
    SDL_Event event;
    while (1)
    {
        SDL_PollEvent(&event);
        if (event.type == type)
        {
            // printf("Event type: %d\n", event.type);
            break;
        }
    }
}

dimensions getDimensions(SDL_Window *window)
{
    dimensions d;
    SDL_GetWindowSize(window, &d.width, &d.height);
    return d;
    // return (dimensions){600,600};
}

void drawLine(SDL_Renderer *renderer, point start, point end, color c)
{
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y);

    // SDL_RenderPresent(renderer);
}

void drawPoint(SDL_Renderer *renderer, point p, color c)
{
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);

    SDL_RenderDrawPoint(renderer, p.x, p.y);
    // SDL_RenderPresent(renderer);
}

void writeText(SDL_Renderer *renderer, char *text, point p, int size, color c)
{
    TTF_Font *font = TTF_OpenFont("assets/fonts/Roboto-Regular.ttf", size);
    if (!font)
    {
        printf("TTF_OpenFont Error: %s\n", TTF_GetError());
        return;
    }

    SDL_Color color = {c.r, c.g, c.b, c.a};
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {p.x, p.y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    TTF_CloseFont(font);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void drawImage(SDL_Renderer *renderer, char *path, point p, dimensions d)
{
    SDL_Surface *surface = IMG_Load(path);
    if (!surface)
    {
        printf("IMG_Load Error: %s\n", IMG_GetError());
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {p.x, p.y, d.width, d.height};
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

Mix_Music *playMusic(char *path)
{
    Mix_Music *music = Mix_LoadMUS(path);
    if (!music)
    {
        printf("Mix_LoadMUS Error: %s\n", Mix_GetError());
        return;
    }
    Mix_PlayMusic(music, -1);

    return music;
}
void drawFilledCircle(SDL_Renderer *renderer, int centerX, int centerY, int radius, color c)
{
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);

    for (int y = -radius; y <= radius; ++y)
    {
        int maxX = (int)sqrt(radius * radius - y * y);

        for (int x = -maxX; x <= maxX; ++x)
        {
            SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
        }
    }
}
void drawCircle(SDL_Renderer *renderer, int centerX, int centerY, int radius, color c)
{
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);

    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;

    while (y >= x)
    {
        // Draw the eight octants of the circle
        SDL_RenderDrawPoint(renderer, centerX + x, centerY - y);
        SDL_RenderDrawPoint(renderer, centerX - x, centerY - y);
        SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
        SDL_RenderDrawPoint(renderer, centerX - x, centerY + y);
        SDL_RenderDrawPoint(renderer, centerX + y, centerY - x);
        SDL_RenderDrawPoint(renderer, centerX - y, centerY - x);
        SDL_RenderDrawPoint(renderer, centerX + y, centerY + x);
        SDL_RenderDrawPoint(renderer, centerX - y, centerY + x);

        x++;

        if (d > 0)
        {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
        {
            d = d + 4 * x + 6;
        }

        // If the generated point is on the line x = y, the perimeter points have already been printed
        if (x != y)
        {
            // Draw the mirror point of the generated point
            SDL_RenderDrawPoint(renderer, centerX + y, centerY - x);
            SDL_RenderDrawPoint(renderer, centerX - y, centerY - x);
            SDL_RenderDrawPoint(renderer, centerX + y, centerY + x);
            SDL_RenderDrawPoint(renderer, centerX - y, centerY + x);
        }
    }
}

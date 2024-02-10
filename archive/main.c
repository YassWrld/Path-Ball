#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL_gfxPrimitives.h"

#include "functions.h"
#include "utils.h"
#include "logic.h"

void exitGame()
{
    printf("Game Exited\n");
    exit(0);
}

void HandleHover(SDL_Renderer *renderer, point click, int n, dimensions grid, dimensions offset, color elementColor, color hoverColor)
{
    static point toUnhover = {0, 0};
    if (toUnhover.x + toUnhover.y > 0)
        drawFilledCircle(renderer, toUnhover.x, toUnhover.y, (grid.width / n) / 4, elementColor);
    if (click.x <= offset.width || click.x >= offset.width + grid.width || click.y <= offset.height || click.y >= offset.height + grid.height)
        return;
    int j = (click.x - offset.width) * n / grid.width;
    int i = (click.y - offset.height) * n / grid.height;
    bool isInsideMatrix = j != 0 && j != n - 1 && i != 0 && i != n - 1;
    bool isItCorner = (j == 0 && i == 0) || (j == 0 && i == n - 1) || (j == n - 1 && i == 0) || (j == n - 1 && i == n - 1);
    if (isInsideMatrix || isItCorner)
        return;
    int x = (2 * j + 1) * grid.width / n / 2 + offset.width;
    int y = (2 * i + 1) * grid.height / n / 2 + offset.height;

    drawFilledCircle(renderer, x, y, (grid.width / n) / 4, hoverColor);
    toUnhover = (point){x, y};
}

int main(int argc, char *argv[])
{
    printf("Game Started\n");

    // get screen resolution

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() < 0)
    {
        printf("TTF_Init Error: %s\n", TTF_GetError());
        return 1;
    }
    if (IMG_Init(IMG_INIT_PNG) < 0)
        
        {
            printf("IMG_Init Error: %s\n", IMG_GetError());
            return 1;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("Mix_OpenAudio Error: %s\n", Mix_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 700, SDL_WINDOW_FOREIGN);

    if (!window)
    {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // SDL_Surface *icon = IMG_Load(path1);

    // SDL_SetWindowIcon(window, icon);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    int p = 7;
    int n = p + 2;

    int matrix[n][n];

    zerosMatrix(n, matrix);
    setupMatrix(n, matrix);
    // printMatrix(n, matrix);

    int start = randomInt(1, 4 * p);

    solution *s = solveMatrix(n, matrix, start);
    printf("start: %d, end: %d\n", s->start, s->end);
    // waitUntil(SDL_QUIT);
    dimensions grid = (dimensions){500, 500};
    dimensions offset = {100, 100};
    color elementColor = {112, 80, 40, 255};
    color backgroundColor = {80, 48, 16, 5};

    int cellSize = (grid.width) / n;

    // get rgb values from pix

    // dimensions dw = getDimensions(window);

    // SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    ;
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    for (int s = 0; s < 4; s++)
    {
        SDL_Rect rect = {offset.width - s + cellSize, offset.height - s + cellSize, grid.width + 2 * s - 2 * cellSize, grid.height + 2 * s - 2 * cellSize};
        SDL_RenderDrawRect(renderer, &rect);
    }

    for (int i = 1; i < n; i++)
    {
        point Hstart = {offset.width + cellSize, i * grid.height / n + offset.height};
        point Hend = {grid.width + offset.width - cellSize, i * grid.height / n + offset.height};

        point Vstart = {i * grid.width / n + offset.width, offset.height + cellSize};
        point Vend = {i * grid.width / n + offset.width, grid.height + offset.height - cellSize};
        for (int s = 0; s < 3; ++s)
        {

            drawLine(renderer, (point){Hstart.x, Hstart.y + s}, (point){Hend.x, Hend.y + s}, elementColor);
            drawLine(renderer, (point){Hstart.x, Hstart.y - s}, (point){Hend.x, Hend.y - s}, elementColor);
            drawLine(renderer, (point){Vstart.x + s, Vstart.y}, (point){Vend.x + s, Vend.y}, elementColor);
            drawLine(renderer, (point){Vstart.x - s, Vstart.y}, (point){Vend.x - s, Vend.y}, elementColor);
        }
    }

    for (int j = 1; j < 2 * (n); j += 2)
    {

        for (int i = 1; i < 2 * (n); i += 2)
        {
            int x = i * grid.width / n / 2 + offset.width;
            int y = j * grid.height / n / 2 + offset.height;

            int of = (grid.width / n) / 4;

            if (i == 1 || j == 1 || i == 2 * n - 1 || j == 2 * n - 1)
            {
                bool corner = (i == 1 && j == 1) || (i == 1 && j == 2 * n - 1) || (i == 2 * n - 1 && j == 1) || (i == 2 * n - 1 && j == 2 * n - 1);
                if (corner)
                    continue;
                //filledCircleRGBA(renderer, x, y, (grid.width / n) / 3, 58, 32, 10, 255);
                //filledCircleRGBA(renderer, x, y, (grid.width / n) / 4, 128, 80, 40, 255);
                
                    drawFilledCircle(renderer, x, y, (grid.width / n) / 3, (color){58, 32, 10, 255});
                drawFilledCircle(renderer, x, y, (grid.width / n) / 4, (color){128, 80, 40, 255});

                continue;
            }

            int bold = (grid.width / n) / 25;

            int value = matrix[j / 2][i / 2];

            if (value == 0)
                continue;

            point dS = value > 0 ? (point){x - of, y + of} : (point){x - of, y - of};
            point dE = value > 0 ? (point){x + of, y - of} : (point){x + of, y + of};
            for (int s = 0; s < bold; ++s)
            {

                drawLine(renderer, (point){dS.x, dS.y + s}, (point){dE.x, dE.y + s}, elementColor);
                drawLine(renderer, (point){dS.x, dS.y - s}, (point){dE.x, dE.y - s}, elementColor);
            }
        }
    }

    SDL_RenderPresent(renderer);
    bool quit = false;

    while (!quit)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            SDL_RenderPresent(renderer);

            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;

            case SDL_MOUSEMOTION:
                // printf("mouse motion\n");
                /*if (event.button.button != SDL_BUTTON_LEFT)
                    break;*/
                point click = {event.motion.x, event.motion.y};
                HandleHover(renderer, click, n, grid, offset, elementColor, (color){255, 255, 255, 255});

                break;
            }
        }
    }

    // return 0;

    SDL_Delay(2 * 1000);
    //  draw path as lines
    printf("\n");
    path *pth = s->path;
    while (pth->next != NULL && pth != NULL)
    {
        path *start = pth;
        path *end = pth->next;

        point startP = {(2 * start->y + 1) * grid.width / n / 2 + offset.width, (2 * start->x + 1) * grid.height / n / 2 + offset.height};
        point endP = {(2 * end->y + 1) * grid.width / n / 2 + offset.width, (2 * end->x + 1) * grid.height / n / 2 + offset.height};
        // drawLine(renderer, startP, endP, (color){0, 0, 0, 255});
        //  SDL_RenderPresent(renderer);

        int circlesNumber = 2;
        int circleRadius = (grid.width / n) / 8;
        color circleColor = (color){255, 255, 255, 255};

        for (int i = 0; i <= circlesNumber; i++)
        {
            if (i != circlesNumber && i != 0)
            {
                circleRadius = ((grid.width / n)) / 12;
            }
            else
            {
                circleRadius = (grid.width / n) / 8;
            }

            if (end->next == NULL && i == circlesNumber)
            {
                circleRadius = (grid.width / n) / 4;
                circleColor = (color){0, 255, 0, 255};
            }
            int c1 = startP.x + (endP.x - startP.x) * i / circlesNumber;
            int c2 = startP.y + (endP.y - startP.y) * i / circlesNumber;
            SDL_PollEvent(&(SDL_Event){0});
            drawFilledCircle(renderer, c1, c2, circleRadius, circleColor);
            SDL_RenderPresent(renderer);
            SDL_Delay(100);
        }

        // SDL_Delay(100);
        pth = pth->next;
    }
    SDL_RenderPresent(renderer);
    waitUntil(SDL_QUIT);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

// drawGrid(reder, hover ,solution,matrix n,user input,draw_diagonals)
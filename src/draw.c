#include <draw.h>

void getMatrixClick(SDL_Renderer *renderer, int clickX, int clickY, int n, int *i, int *j)
{
    if (clickX <= OFFSET || clickX >= OFFSET + GRID_SIZE || clickY <= OFFSET || clickY >= OFFSET + GRID_SIZE)
    {
        *i = -1;
        *j = -1;
        return;
    }

    *j = (clickX - OFFSET) * n / GRID_SIZE;
    *i = (clickY - OFFSET) * n / GRID_SIZE;

    // check if the click in the circle

    bool isOuterMatrix = *i == 0 || *i == n - 1 || *j == 0 || *j == n - 1;
    if (!isOuterMatrix)
    {
        return;
    }

    int cellSize = GRID_SIZE / n;
    int x = OFFSET + *j * cellSize + cellSize / 2 + THICKNESS / 2;
    int y = OFFSET + *i * cellSize + cellSize / 2 + THICKNESS / 2;
    int radius = cellSize / 3; // radius of the small circle

    if (clickX < x - radius || clickX > x + radius || clickY < y - radius || clickY > y + radius)
    {
        *i = -1;
        *j = -1;
        return;
    }
}

// void getClickCenter()

void drawFilledCircle(SDL_Renderer *renderer, int centerX, int centerY, int radius)
{
    int x = radius - 1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);

    while (x >= y)
    {
        SDL_RenderDrawLine(renderer, centerX + x, centerY - y, centerX - x, centerY - y);
        SDL_RenderDrawLine(renderer, centerX + x, centerY + y, centerX - x, centerY + y);
        SDL_RenderDrawLine(renderer, centerX + y, centerY - x, centerX - y, centerY - x);
        SDL_RenderDrawLine(renderer, centerX + y, centerY + x, centerX - y, centerY + x);

        if (err <= 0)
        {
            y++;
            err += dy;
            dy += 2;
        }
        if (err > 0)
        {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
}

void drawDiagonal(SDL_Renderer *renderer, int n, int direction, int centerX, int centerY)
{
    // Set the length of the diagonal line

    // Calculate the end points of the diagonal based on the direction
    int cellSize = GRID_SIZE / n;
    int of = cellSize / 4;    // OFFSET
    int bold = cellSize / 10; // boldness

    int Sx = centerX - of; // Start x

    int Ex = centerX + of; // End x

    int Sy = direction > 0 ? centerY + of : centerY - of; // Start y depending on the direction
    int Ey = direction > 0 ? centerY - of : centerY + of; // End y depending on the direction

    SDL_SetRenderDrawColor(renderer, DIAGONAL_COLOR); // Set the color of the diagonal
    if (direction < 0)
    {
        SDL_SetRenderDrawColor(renderer, DIAGONAL_COLOR);
    }
    for (int s = 0; s < bold; ++s)
    {
        SDL_RenderDrawLine(renderer, Sx, Sy + s, Ex, Ey + s);
        SDL_RenderDrawLine(renderer, Sx, Sy - s, Ex, Ey - s);
    }
}

void drawGrid(SDL_Renderer *renderer, game *Game)
{
    int n = Game->level + 5;
    int cellSize = GRID_SIZE / n;

    int matrix[n][n];

    // cast the matrix using mmset
    memcpy(matrix, Game->matrix, sizeof(matrix));
    // copy the matrix

    SDL_SetRenderDrawColor(renderer, BORDER_COLOR);

    SDL_Rect matrixRect = {OFFSET, OFFSET, GRID_SIZE, GRID_SIZE};
    SDL_RenderDrawRect(renderer, &matrixRect);
    // Draw vertical lines
    int x = n % 2;
    for (int i = 0; i < n - 1; i++)
    {

        int startX = cellSize + OFFSET + i * cellSize;
        int startY = cellSize + OFFSET;
        int endX = cellSize + OFFSET + i * cellSize;
        int endY = OFFSET + GRID_SIZE - cellSize;

        for (int s = 0; s < THICKNESS + x; s++)
        {

            SDL_RenderDrawLine(renderer, startX + s, startY, endX + s, endY);
            SDL_RenderDrawLine(renderer, startY, startX + s, endY, endX + s);

            if (i == n - 2)
            {
                // SDL_RenderDrawLine(renderer, endY, endX + s, endY , endX + s);
            }
        }
    }

    for (int i = 1; i < n - 1; i++)
    {
        for (int j = 0; j < n; j++)
        {

            int x = OFFSET + j * cellSize + cellSize / 2 + THICKNESS / 2;
            int y = OFFSET + i * cellSize + cellSize / 2 + THICKNESS / 2;
            if (j > 0 && j < n - 1)
            {
                int current = matrix[i][j];
                if (current != 0 && Game->state != 1)
                {
                    drawDiagonal(renderer, n, current, x, y);
                    // drawDiagonal(renderer, n, current, x, y);
                }

                continue;
            }

            SDL_SetRenderDrawColor(renderer, BORDER_COLOR); // Color of the big circle

            drawFilledCircle(renderer, x, y, cellSize / 3); // Draw the big circle vertically

            drawFilledCircle(renderer, y, x, cellSize / 3); // Draw the big circle horizontally
            int startI = Game->solution->startI, startJ = Game->solution->startJ;

            int hoverX, hoverY;
            SDL_GetMouseState(&hoverX, &hoverY);

            int hoverI, hoverJ;
            getMatrixClick(renderer, hoverX, hoverY, n, &hoverI, &hoverJ);

            SDL_SetRenderDrawColor(renderer, SMALL_CIRCLE_COLOR);

            if (Game->state == 1)
            {
                if (startI == i && startJ == j)
                {
                    SDL_SetRenderDrawColor(renderer, START_CIRCLE_COLOR);
                }
                else if (hoverI == i && hoverJ == j)
                {
                    SDL_SetRenderDrawColor(renderer, HOVER_CIRCLE_COLOR);
                }
                else
                {
                    SDL_SetRenderDrawColor(renderer, SMALL_CIRCLE_COLOR);
                }
            }

            drawFilledCircle(renderer, x, y, cellSize / 4); // Draw the small circle vertically

            if (Game->state == 1)
            {
                if (startI == j && startJ == i)
                {
                    SDL_SetRenderDrawColor(renderer, START_CIRCLE_COLOR);
                }
                else if (hoverI == j && hoverJ == i)
                {
                    SDL_SetRenderDrawColor(renderer, HOVER_CIRCLE_COLOR);
                }
                else
                {
                    SDL_SetRenderDrawColor(renderer, SMALL_CIRCLE_COLOR);
                }
            }
            drawFilledCircle(renderer, y, x, cellSize / 4); // Draw the small circle horizontally
        }
    }
}

void drawPath(SDL_Renderer *renderer, int n, path *sPath)
{
    int cellSize = GRID_SIZE / n;
    SDL_SetRenderDrawColor(renderer, PATH_COLOR);
    while (sPath->next != NULL && sPath != NULL)
    {
        path *start = sPath;
        path *end = sPath->next;
        int startX = OFFSET + start->y * cellSize + cellSize / 2 + THICKNESS / 2;
        int startY = OFFSET + start->x * cellSize + cellSize / 2 + THICKNESS / 2;
        int endX = OFFSET + end->y * cellSize + cellSize / 2 + THICKNESS / 2;
        int endY = OFFSET + end->x * cellSize + cellSize / 2 + THICKNESS / 2;

        int circlesNumber = 2;
        int circleRadius = cellSize / 8;
        for (int i = 0; i <= circlesNumber; i++)
        {
            if (i != circlesNumber && i != 0)
            {
                circleRadius = cellSize / 12;
            }
            else
            {
                circleRadius = cellSize / 8;
            }

            if (end->next == NULL && i == circlesNumber)
            {
                circleRadius = cellSize / 4;
            }
            int c1 = startX + (endX - startX) * i / circlesNumber;
            int c2 = startY + (endY - startY) * i / circlesNumber;

            SDL_PollEvent(&(SDL_Event){0});

            drawFilledCircle(renderer, c1, c2, circleRadius);
            SDL_RenderPresent(renderer);
            SDL_Delay(100);
        }

        // SDL_Delay(100);
        sPath = sPath->next;
    }
}

void drawSideBar(SDL_Renderer *renderer, game *Game)
{
    // side bar
    int outline = 10;
    int sideBarWidth = WIDTH - GRID_SIZE - OFFSET * 2;
    
    SDL_Rect outerSideBar = {GRID_SIZE + OFFSET * 2, 0, sideBarWidth, HEIGHT};
    SDL_Rect innerSideBar = {GRID_SIZE + OFFSET * 2+ outline, outline,sideBarWidth - 2 * outline, HEIGHT - 2 * outline};

    SDL_SetRenderDrawColor(renderer, SIDE_BAR_OUTLINE_COLOR);
    SDL_RenderFillRect(renderer, &outerSideBar);

    SDL_SetRenderDrawColor(renderer, SIDE_BAR_COLOR);
    SDL_RenderFillRect(renderer, &innerSideBar);

    // 3 displays
        // Name
        // Level
        // Score

    
    // 2 buttons
        // Pause
        // Save & Quit
    
}

SDL_Color getPixelColor(SDL_Renderer *renderer, int pixel_X, int pixel_Y)
{
    SDL_Surface *getPixel_Surface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, getPixel_Surface->pixels, getPixel_Surface->pitch);
    const Uint8 getPixel_bpp = getPixel_Surface->format->BytesPerPixel;

    SDL_Color pixelColor;
    Uint8 *pPixel = (Uint8 *)getPixel_Surface->pixels + pixel_Y * getPixel_Surface->pitch + pixel_X * getPixel_bpp;

    Uint32 pixelData;

    switch (getPixel_bpp)
    {
    case 1:
        pixelData = *pPixel;
        break;
    case 2:
        pixelData = *(Uint16 *)pPixel;
        break;
    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            pixelData = pPixel[0] << 16 | pPixel[1] << 8 | pPixel[2];
        else
            pixelData = pPixel[0] | pPixel[1] << 8 | pPixel[2] << 16;
        break;
    case 4:
        pixelData = *(Uint32 *)pPixel;
        break;
    }

    SDL_GetRGBA(pixelData, getPixel_Surface->format, &pixelColor.r, &pixelColor.g, &pixelColor.b, &pixelColor.a);

    return pixelColor;
}



Mix_Music *
playMusic(char *path)
{
    Mix_Music *music = Mix_LoadMUS(path);
    if (!music)
    {
        printf("Mix_LoadMUS Error: %s\n", Mix_GetError());
        return NULL;
    }
    Mix_PlayMusic(music, -1);

    return music;
}


void writeText(SDL_Renderer *renderer,char *fontPath, char *text,int x,int y, int size, int r, int g, int b, int a)

{
    TTF_Font *font = TTF_OpenFont(fontPath, size);
    if (!font)
    {
        printf("TTF_OpenFont Error: %s\n", TTF_GetError());
        return;
    }

    SDL_Color color = {r, g, b, a};
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    TTF_CloseFont(font);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
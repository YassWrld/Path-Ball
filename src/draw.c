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
        *i = -1;
        *j = -1;
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
                if (current != 0 && Game->state != Selecting)
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

            SDL_SetRenderDrawColor(renderer, SMALL_CIRCLE_COLOR); // Color of the small circle
            if (Game->state == Selecting)
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

            if (Game->state == Selecting)
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

    SDL_SetRenderDrawColor(renderer, HOVER_CIRCLE_COLOR);
    int endx = Game->solution->endJ * cellSize + OFFSET + cellSize / 2 + THICKNESS / 2;
    int endy = Game->solution->endI * cellSize + OFFSET + cellSize / 2 + THICKNESS / 2;
    SDL_RenderDrawLine(renderer, endx, endy, endx + 15, endy + 15);
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
        for (int i = 0; i < circlesNumber; i++)
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
                SDL_SetRenderDrawColor(renderer, END_CIRCLE_COLOR);
                circleRadius = cellSize / 4;
            }

            int c1 = startX + (endX - startX) * i / circlesNumber;
            int c2 = startY + (endY - startY) * i / circlesNumber;

            drawFilledCircle(renderer, c1, c2, circleRadius);
            SDL_RenderPresent(renderer);
            playSoundEffect("assets/sounds/step.wav");
            graycefulDelay(250);
        }

        // SDL_Delay(100);
        sPath = sPath->next;
    }
    if (sPath->next == NULL)
    {
        SDL_SetRenderDrawColor(renderer, END_CIRCLE_COLOR);
        int endx = sPath->y * cellSize + OFFSET + cellSize / 2 + THICKNESS / 2;
        int endy = sPath->x * cellSize + OFFSET + cellSize / 2 + THICKNESS / 2;
        drawFilledCircle(renderer, endx, endy, cellSize / 4);
        // playSoundEffect("assets/sounds/step.wav");
        SDL_RenderPresent(renderer);
    }

    graycefulDelay(1000);
}

void drawSideBar(SDL_Renderer *renderer, game *Game)
{
    // side bar
    int outline = 10;
    int sideBarWidth = WIDTH - GRID_SIZE - OFFSET * 2;

    SDL_Rect outerSideBar = {GRID_SIZE + OFFSET * 2, 0, sideBarWidth, HEIGHT};
    SDL_Rect innerSideBar = {GRID_SIZE + OFFSET * 2 + outline, outline, sideBarWidth - 2 * outline, HEIGHT - 2 * outline};

    SDL_SetRenderDrawColor(renderer, SIDE_BAR_OUTLINE_COLOR);
    SDL_RenderFillRect(renderer, &outerSideBar);

    SDL_SetRenderDrawColor(renderer, SIDE_BAR_COLOR);
    SDL_RenderFillRect(renderer, &innerSideBar);

    int centerX = outline + GRID_SIZE + OFFSET * 2 + sideBarWidth / 2;

    int centerY = outline + HEIGHT / 2;
    char text[100];
    sprintf(text, "Score:%d", Game->player.score);
    int w = mesureTextWidth(GAMEPAUSED_FONT, text, 24);
    writeText(renderer, GAMEPAUSED_FONT, text, centerX - w / 2, centerY - 100, 24, FONT_COLOR);

    // write player name at the top of the side bar
    sprintf(text, "Name:%s", Game->player.name);
    int fontSize = 24;
    do
    {

        w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);
        fontSize--;
    } while (w >= sideBarWidth - 2 * outline - w / 2);
    writeText(renderer, GAMEPAUSED_FONT, text, centerX - w / 2, centerY - 150, fontSize, FONT_COLOR);

    // 3 displays
    // Name
    // Level
    // Score

    // 2 buttons
    // Pause
    // Save & Quit
}

void drawTextInput(SDL_Renderer *renderer, game *Game)
{

    char text[100];
    int fontSize = 24;
    sprintf(text, "Enter Your Name:");
    int w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);

    writeText(renderer, GAMEPAUSED_FONT, text, WIDTH / 2 - w / 2, HEIGHT / 2 - 100, fontSize, FONT_COLOR);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    sprintf(text, "%s", Game->player.name);
    w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);
    writeText(renderer, GAMEPAUSED_FONT, text, WIDTH / 2 - w / 2, HEIGHT / 2 - 50, fontSize, 255, 0, 0, 255);

    // draw wrting line
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    if (strlen(text))
        SDL_RenderDrawLine(renderer, WIDTH / 2 - w / 2, HEIGHT / 2 - 50 + fontSize, WIDTH / 2 + w / 2, HEIGHT / 2 - 50 + fontSize);

    sprintf(text, "Press Enter to Start");
    w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);
    writeText(renderer, GAMEPAUSED_FONT, text, WIDTH / 2 - w / 2, HEIGHT / 2 + 50, fontSize, FONT_COLOR);

    drawSVG(renderer, "assets/images/enter.png", WIDTH / 2 - 50, HEIGHT / 2 + 100, 100, 100);
}

void drawGameOver(SDL_Renderer *renderer, game *Game)
{
    char text[100];
    int fontSize = 24;
    sprintf(text, "Game Over");
    int w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);

    // you won if game->level = maxLevel or you lost if game->level = 0
    sprintf(text, "You %s", Game->level == 0 ? "Lost" : "Won");
    w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);
    SDL_Color color = Game->level == 0 ? (SDL_Color){255, 0, 0, 255} : (SDL_Color){0, 255, 0, 255};
    writeText(renderer, GAMEPAUSED_FONT, text, WIDTH / 2 - w / 2, HEIGHT / 2 - 100, fontSize, color.r, color.g, color.b, color.a);

    sprintf(text, "Your Score:%d", Game->player.score);
    w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);
    writeText(renderer, GAMEPAUSED_FONT, text, WIDTH / 2 - w / 2, HEIGHT / 2 - 50, fontSize, FONT_COLOR);
}

void drawPause(SDL_Renderer *renderer, game *Game)
{
    char text[100];
    int fontSize = 24;
    sprintf(text, "Game Paused");
    int w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);

    writeText(renderer, GAMEPAUSED_FONT, text, WIDTH / 2 - w / 2, HEIGHT / 2 - 100, fontSize, FONT_COLOR);

    sprintf(text, "Press Ctrl + P to Resume");
    w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);
    writeText(renderer, GAMEPAUSED_FONT, text, WIDTH / 2 - w / 2, HEIGHT / 2 + 50, fontSize, FONT_COLOR);
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
    Mix_PlayMusic(music, 0);

    return music;
}

void playSoundEffect(char *path)
{
    Mix_Chunk *soundEffect = Mix_LoadWAV(path);
    if (!soundEffect)
    {
        printf("Mix_LoadWAV Error: %s\n", Mix_GetError());
        return;
    }
    Mix_PlayChannel(-1, soundEffect, 0);
}

void writeText(SDL_Renderer *renderer, char *fontPath, char *text, int x, int y, int size, int r, int g, int b, int a)
{
    if (!text || text[0] == '\0')
    {
        return; // Don't render anything if the text is empty
    }
    TTF_Font *font = TTF_OpenFont(fontPath, size);
    if (!font)
    {
        printf("TTF_OpenFont Error: %s\n", TTF_GetError());
        return;
    }

    // TTF_SetFontWrappedAlign(font, TTF_WRAPPED_ALIGN_CENTER);

    SDL_Color color = {r, g, b, a};
    // TTF_SetFontStyle(font, TTF_STYLE_BOLD);
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    TTF_CloseFont(font);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

int mesureTextWidth(char *fontPath, char *text, int size)
{
    TTF_Font *font = TTF_OpenFont(fontPath, size);
    if (!font)
    {
        printf("TTF_OpenFont Error: %s\n", TTF_GetError());
        return 0;
    }

    int w, h;
    TTF_SizeText(font, text, &w, &h);

    TTF_CloseFont(font);
    return w;
}

void drawSVG(SDL_Renderer *renderer, char *path, int x, int y, int w, int h)
{
    SDL_Surface *surface = IMG_Load(path);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
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

bool compareColor(SDL_Color color, int r, int g, int b, int a)
{
    return (r == color.r) && (g == color.g) && (b == color.b) && (a = color.a);
}

void machineModeMemorize(SDL_Renderer *renderer, int n, int matrix[n][n])
{
    int cellSize = GRID_SIZE / n;
    printf("cellSize: %d\n", cellSize);
    initializeMatrix(n, matrix);

    for (int i = 1; i < n - 1; i++)
    {
        for (int j = 1; j < n - 1; j++)
        {
            int x = OFFSET + j * cellSize + cellSize / 2 + THICKNESS / 2;
            int y = OFFSET + i * cellSize + cellSize / 2 + THICKNESS / 2;

            if (j <= 0 || j >= n - 1)
            {
                continue;
            }

            SDL_Color color = getPixelColor(renderer, x, y);
            bool isDiagonal = compareColor(color, DIAGONAL_COLOR);
            if (!isDiagonal)
            {
                matrix[i][j] = 0;
                continue;
            }
            // check if the diagonal is up or down
            int of = cellSize / 4;
            color = getPixelColor(renderer, x - of, y + of);
            bool isUp = compareColor(color, DIAGONAL_COLOR);
            if (isUp)
            {
                matrix[i][j] = 1;
            }
            else
            {
                matrix[i][j] = -1;
            }
        }
    }
}

void machineModeSelecting(SDL_Renderer *renderer, int n, int matrix[n][n])
{
    int cellSize = GRID_SIZE / n;
    int startI = 0, startJ = 0;
    for (int i = 1; i < n - 1; i++)
    {
        for (int j = 0; j < n - 1; j++)
        {
            int x = OFFSET + j * cellSize + cellSize / 2 + THICKNESS / 2;
            int y = OFFSET + i * cellSize + cellSize / 2 + THICKNESS / 2;
            if (j <= 0 || j >= n - 1)
            {

                SDL_Color color1 = getPixelColor(renderer, x, y);
                SDL_Color color2 = getPixelColor(renderer, y, x);
                bool isStart1 = compareColor(color1, START_CIRCLE_COLOR);
                bool isStart2 = compareColor(color2, START_CIRCLE_COLOR);
                if (isStart1)
                {
                    startI = i;
                    startJ = j;
                }

                if (isStart2)
                {
                    startI = j;
                    startJ = i;
                }

                continue;
            }
        }
    }

    solution *s = solveMatrix(matrix[startI][startJ], 0, n, matrix); // obs = 0

    int endX = s->endJ * cellSize + OFFSET + cellSize / 2 + THICKNESS / 2;
    int endY = s->endI * cellSize + OFFSET + cellSize / 2 + THICKNESS / 2;
    // SDL_WarpMouseInWindow(NULL, endX, endY);
    graycefulDelay(1000);
    SDL_SetRenderDrawColor(renderer, HOVER_CIRCLE_COLOR);
    drawFilledCircle(renderer, endX, endY, cellSize / 4);
    SDL_RenderPresent(renderer);
    graycefulDelay(1000);

    // custom event
    SDL_Event e;
    e.type = SDL_USEREVENT;
    e.user.code = 139;
    // set x and y coordinates
    e.user.data1 = NULL;
    e.user.data2 = NULL;

    e.button.x = endX;
    e.button.y = endY;

    SDL_PushEvent(&e);
}
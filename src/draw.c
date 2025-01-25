#include <draw.h>

void drawArrow(SDL_Renderer *renderer, int x, int y, int l, direction d)
{

    int x2 = 0, y2 = 0;
    int x3 = 0, y3 = 0;

    if (d == Up)
    {

        y = y - l / 2;
        x2 = x - l;
        y2 = y + l;
        x3 = x + l;
        y3 = y + l;
    }
    else if (d == Down)
    {
        y = y + l / 2;
        x2 = x - l;
        y2 = y - l;
        x3 = x + l;
        y3 = y - l;
    }
    else if (d == Left)
    {
        x = x - l / 2;
        x2 = x + l;
        y2 = y - l;
        x3 = x + l;
        y3 = y + l;
    }
    else if (d == Right)
    {
        x = x + l / 2;
        x2 = x - l;
        y2 = y - l;
        x3 = x - l;
        y3 = y + l;
    }

    int bold = 8;

    for (int i = 0; i < bold / 2; i++)
    {
        if (d == Up || d == Down)
        {
            SDL_RenderDrawLine(renderer, x, y + i, x2, y2 + i);
            SDL_RenderDrawLine(renderer, x, y + i, x3, y3 + i);
            SDL_RenderDrawLine(renderer, x, y - i, x2, y2 - i);
            SDL_RenderDrawLine(renderer, x, y - i, x3, y3 - i);
        }
        else if (d == Left || d == Right)
        {
            SDL_RenderDrawLine(renderer, x + i, y, x2 + i, y2);
            SDL_RenderDrawLine(renderer, x + i, y, x3 + i, y3);
            SDL_RenderDrawLine(renderer, x - i, y, x2 - i, y2);
            SDL_RenderDrawLine(renderer, x - i, y, x3 - i, y3);
        }
    }
}

void drawHexagon(SDL_Renderer *renderer, int x, int y, int r)
{
    int n = 6;
    int angle = 360 / n;
    int x1 = x + r * SDL_cos(0);
    int y1 = y + r * SDL_sin(0);

    for (int i = 1; i <= n; i++)
    {
        int x2 = x + r * SDL_cos(i * angle * M_PI / 180);
        int y2 = y + r * SDL_sin(i * angle * M_PI / 180);
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        x1 = x2;
        y1 = y2;
    }
}
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

void drawImage(SDL_Renderer *renderer, char *path, int x, int y, int w, int h)
{

    SDL_Surface *surface = IMG_Load(path);
    if (!surface)
    {
        printf("IMG_Load Error: %s\n", IMG_GetError());
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
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

    SDL_Color color = {r, g, b, a};

    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    TTF_CloseFont(font);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void drawButton(SDL_Renderer *renderer, button *Button)
{
    int x = Button->centerX;
    int y = Button->centerY;
    int w = Button->width;
    int h = Button->height;

    SDL_Rect rect = {x - w / 2, y - h / 2, w, h};
    SDL_SetRenderDrawColor(renderer, Button->outlineColor.r, Button->outlineColor.g, Button->outlineColor.b, Button->outlineColor.a);
    for (int i = 0; i < Button->outlineThickness; i++)
    {
        SDL_RenderDrawRect(renderer, &rect);
        rect.x++;
        rect.y++;
        rect.w -= 2;
        rect.h -= 2;
    }
    SDL_Color color = Button->color;
    if (Button->hoverColor.r + Button->hoverColor.g + Button->hoverColor.b + Button->hoverColor.a != 0)
    {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        if (mouseX >= x - w / 2 && mouseX <= x + w / 2 && mouseY >= y - h / 2 && mouseY <= y + h / 2)
        {
            color = Button->hoverColor;
        }
    }

    bool hasIcon = Button->iconPath[0] != '\0';

    int iconSize = hasIcon ? h / 1.8 : 0;

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);

    int len = strlen(Button->label);
    int fontSize = h / 2;
    int textWidth = mesureTextWidth(GAMEPAUSED_FONT, Button->label, fontSize);
    int fontStartX = x - textWidth / 2 + iconSize / 2;
    int buttonStartX = x - w / 2 + Button->outlineThickness;
    int blankMiddle = buttonStartX + (fontStartX - buttonStartX) / 2;

    if (hasIcon)
    {
        drawImage(renderer, Button->iconPath, blankMiddle - iconSize / 2, y - iconSize / 2, iconSize, iconSize);
    }

    if (len == 0)
        return;
    color = Button->textColor;

    // calculate font size based on the button size and if the button has an icon or not

    writeText(renderer, GAMEPAUSED_FONT, Button->label, fontStartX, y - fontSize / 2, fontSize, color.r, color.g, color.b, color.a);
}

void drawBlurredBackground(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, BLUR_BACKGROUND_COLOR);

    int x = 0, y = 0;
    int radius = 50;

    while (x <= WINDOW_WIDTH / 2)
    {
        while (y <= WINDOW_HEIGHT / 2)
        {
            drawHexagon(renderer, x, y, radius);
            drawHexagon(renderer, WINDOW_WIDTH - x, WINDOW_HEIGHT - y, radius);

            if (y != WINDOW_HEIGHT - y && x != WINDOW_WIDTH - x)
            {

                drawHexagon(renderer, x, WINDOW_HEIGHT - y, radius);
                drawHexagon(renderer, WINDOW_WIDTH - x, y, radius);
            }

            y += radius;
        }
        x += radius;
        y = 0;
    }
}
void drawMainMenu(SDL_Renderer *renderer, game *Game)
{
    // background
    // title
    char text[100];
    int fontSize = 90;
    // draw the title and the logo of the game the logo is next to the title
    sprintf(text, GAME_TITLE);
    int w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);
    int logoSize = fontSize * 1.5;

    writeText(renderer, GAMEPAUSED_FONT, text, WINDOW_WIDTH / 2 - w / 2 - logoSize / 2, WINDOW_HEIGHT / 8, fontSize, FONT_COLOR);
    drawImage(renderer, NOBG_ICON_PATH, WINDOW_WIDTH / 2 + w / 2 - logoSize / 2, WINDOW_HEIGHT / 8 - logoSize / 3, logoSize, logoSize);

    // play button
    button playerGameModeButton = {
        WINDOW_WIDTH / 2,
        WINDOW_HEIGHT / 2,
        200,
        50,
        {BUTTON_PRIMARY_COLOR},
        {BUTTON_HOVER_COLOR},
        {BUTTON_LABEL_COLOR},
        {BUTTON_OUTLINE_COLOR},
        BUTTON_OUTLINE_THICKNESS,
        "Player Mode",
        HUMAN_ICON_PATH

    };
    Game->buttons.PlayerGameMode = playerGameModeButton;
    drawButton(renderer, &playerGameModeButton);

    // machine button
    button machineGameModeButton = {
        WINDOW_WIDTH / 2,
        WINDOW_HEIGHT / 2 + 100,
        200,
        50,
        {BUTTON_PRIMARY_COLOR},
        {BUTTON_HOVER_COLOR},
        {BUTTON_LABEL_COLOR},
        {BUTTON_OUTLINE_COLOR},
        BUTTON_OUTLINE_THICKNESS,
        "Machine Mode",
        ROBOT_ICON_PATH

    };
    Game->buttons.MachineGameMode = machineGameModeButton;

    drawButton(renderer, &machineGameModeButton);

    // top players button
    button topPlayersButton = {
        WINDOW_WIDTH / 2,
        WINDOW_HEIGHT / 2 + 200,
        200,
        50,
        {BUTTON_PRIMARY_COLOR},
        {BUTTON_HOVER_COLOR},
        {BUTTON_LABEL_COLOR},
        {BUTTON_OUTLINE_COLOR},
        BUTTON_OUTLINE_THICKNESS,
        "Top Players",
        CROWN_ICON_PATH

    };
    Game->buttons.TopPlayers = topPlayersButton;

    drawButton(renderer, &topPlayersButton);

    int wb = 75, hb = 20;

    button creditsButton = {
        wb / 2 + 5,
        WINDOW_HEIGHT - hb / 2 - 5,
        wb,
        hb,
        {255, 255, 255, 255},
        {255, 0, 0, 255},
        {0, 0, 0, 255},
        {BUTTON_OUTLINE_COLOR},
        2,
        "Credits",
        CREDITS_ICON_PATH

    };

    Game->buttons.Credits = creditsButton;

    drawButton(renderer, &creditsButton);
}

void drawCredits(SDL_Renderer *renderer, game *Game)
{

    // title
    char text[100];


    if (Game->helpers.creditsAnimationStartTime == 0)
    {
        printf("credits animation start\n");
        Game->helpers.creditsAnimationStartTime = SDL_GetTicks();
    }

    double elapsed = (SDL_GetTicks() - Game->helpers.creditsAnimationStartTime) / 1000.0;
    int fontSize = 30;
    int y = WINDOW_HEIGHT;
    int speed = 80; // speed of the text movement (pixels per second)
    y -= (int)(speed * elapsed);

    /*Developed By*/
    sprintf(text, "Developed by:");
    int w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);
    writeText(renderer, GAMEPAUSED_FONT, text, WINDOW_WIDTH / 2 - w / 2, y, fontSize, 255, 0, 0, 255);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (int k = 0; k < 3; k++)
        SDL_RenderDrawLine(renderer, WINDOW_WIDTH / 2 - w / 2, y + fontSize + k, WINDOW_WIDTH / 2 + w / 2, y + fontSize + k);

    y += 50;
    sprintf(text, DEV_1_NAME);
    w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);
    writeText(renderer, GAMEPAUSED_FONT, text, WINDOW_WIDTH / 2 - w / 2, y, fontSize, FONT_COLOR);
    y += 50;
    sprintf(text, DEV_2_NAME);
    w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);
    writeText(renderer, GAMEPAUSED_FONT, text, WINDOW_WIDTH / 2 - w / 2, y, fontSize, FONT_COLOR);

    /*Responsable Teacher*/
    y += 100;

    sprintf(text, "Responsable Teacher:");
    w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);
    writeText(renderer, GAMEPAUSED_FONT, text, WINDOW_WIDTH / 2 - w / 2, y, fontSize, 255, 0, 0, 255);

    for (int k = 0; k < 3; k++)
        SDL_RenderDrawLine(renderer, WINDOW_WIDTH / 2 - w / 2, y + fontSize + k, WINDOW_WIDTH / 2 + w / 2, y + fontSize + k);
    y += 50;
    sprintf(text, RESPONSABLE_TEACHER_NAME);

    w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);

    writeText(renderer, GAMEPAUSED_FONT, text, WINDOW_WIDTH / 2 - w / 2, y, fontSize, FONT_COLOR);

    /*Special Thanks*/

    y += 100;

    sprintf(text, "Special Thanks to:");

    w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);

    writeText(renderer, GAMEPAUSED_FONT, text, WINDOW_WIDTH / 2 - w / 2, y, fontSize, 255, 0, 0, 255);

    for (int k = 0; k < 3; k++)
        SDL_RenderDrawLine(renderer, WINDOW_WIDTH / 2 - w / 2, y + fontSize + k, WINDOW_WIDTH / 2 + w / 2, y + fontSize + k);

    y += 50;

    sprintf(text, SPECIAL_THANKS_1_Name);

    w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);

    writeText(renderer, GAMEPAUSED_FONT, text, WINDOW_WIDTH / 2 - w / 2, y, fontSize, FONT_COLOR);

    y += 50;

    sprintf(text, SPECIAL_THANKS_2_Name);

    w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);

    writeText(renderer, GAMEPAUSED_FONT, text, WINDOW_WIDTH / 2 - w / 2, y, fontSize, FONT_COLOR);

    y += 250;
    fontSize = 60;

    sprintf(text, GAME_TITLE);

    w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);

    writeText(renderer, GAMEPAUSED_FONT, text, WINDOW_WIDTH / 2 - w / 2, y, fontSize, FONT_COLOR);

    y += 50;
    drawImage(renderer, NOBG_ICON_PATH, WINDOW_WIDTH / 2 - 100, y, 200, 200);
    y += 210;
    fontSize = 20;
    sprintf(text, GAME_VERSION);
    w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);

    writeText(renderer, GAMEPAUSED_FONT, text, WINDOW_WIDTH / 2 - w / 2, y, fontSize, 255, 255, 255, 255);

    if (y + 100 < 0)
    {
        Game->helpers.creditsAnimationStartTime = 0;
        y = WINDOW_HEIGHT;
    }

    // back button in the top left corner
    button backButton = {
        60,
        60,
        100,
        50,
        {BUTTON_PRIMARY_COLOR},
        {BUTTON_HOVER_COLOR},
        {BUTTON_LABEL_COLOR},
        {BUTTON_OUTLINE_COLOR},
        BUTTON_OUTLINE_THICKNESS,
        "back",
        BACK_ICON_PATH};
    Game->buttons.MainMenu = backButton;
    drawButton(renderer, &backButton);
}

void drawTopPlayers(SDL_Renderer *renderer, game *Game)
{

    // title
    char text[100];
    int fontSize = 90;
    sprintf(text, "Top Players");
    int w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);
    writeText(renderer, GAMEPAUSED_FONT, text, WINDOW_WIDTH / 2 - w / 2, WINDOW_HEIGHT / 12, fontSize, FONT_COLOR);
    SDL_SetRenderDrawColor(renderer, FONT_COLOR);

    fontSize = 30;
    // top players
    static bool loaded = false;
    static int maxNameW = 0;
    static int maxScoreW = 0;
    static int maxTextW = 0;
    int iconSize = fontSize * 1.5;
    int gap = 50;
    player *players = Game->helpers.topPlayers;
    if (!loaded || Game->helpers.updatedTopPlayers)
    {
        getTopPlayers(Game->helpers.topPlayers);

        loaded = true;
        Game->helpers.updatedTopPlayers = false;
        for (int i = 0; i < 5; i++)
        {
            sprintf(text, "%s %d %d-%d-%d", players[i].name, players[i].score, players[i].date.day, players[i].date.month + 1, players[i].date.year);
            int nameW = mesureTextWidth(GAMEPAUSED_FONT, players[i].name, fontSize);
            sprintf(text, "%d", players[i].score);
            int scoreW = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);
            sprintf(text, "%d-%d-%d", players[i].date.day, players[i].date.month + 1, players[i].date.year);
            int dateW = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);

            int fullW = nameW + scoreW + dateW + 2 * gap + iconSize;

            if (fullW > w)
                maxTextW = fullW;
            if (nameW > maxNameW)
                maxNameW = nameW;
            if (scoreW > maxScoreW)
                maxScoreW = scoreW;
        }
    }
    // icons ,first.png , second.png , third.png

    for (int i = 0; i < 5; i++)
    {
        if (players[i].score == 0)
            break;

        sprintf(text, players[i].name);
        writeText(renderer, GAMEPAUSED_FONT, text, WINDOW_WIDTH / 2 - maxTextW / 2 + iconSize, WINDOW_HEIGHT / 5 + 100 * (i + 1), fontSize, FONT_COLOR);
        sprintf(text, "%d", players[i].score);
        writeText(renderer, GAMEPAUSED_FONT, text, WINDOW_WIDTH / 2 - maxTextW / 2 + iconSize + maxNameW + gap, WINDOW_HEIGHT / 5 + 100 * (i + 1), fontSize, FONT_COLOR);

        sprintf(text, "%d-%d-%d", players[i].date.day, players[i].date.month + 1, players[i].date.year);
        writeText(renderer, GAMEPAUSED_FONT, text, WINDOW_WIDTH / 2 - maxTextW / 2 + iconSize + maxNameW + maxScoreW + 2 * gap, WINDOW_HEIGHT / 5 + 100 * (i + 1), fontSize, FONT_COLOR);

        if (i < 3)
        {
            char *iconPath = i == 0 ? FIRST_ICON_PATH : i == 1 ? SECOND_ICON_PATH
                                                               : THIRD_ICON_PATH;
            drawImage(renderer, iconPath, WINDOW_WIDTH / 2 - maxTextW / 2 - iconSize / 2, WINDOW_HEIGHT / 5 + 100 * (i + 1) - iconSize / 2, iconSize, iconSize * 1.4);
        }
        else
        {
            sprintf(text, "%d", i + 1);
            writeText(renderer, GAMEPAUSED_FONT, text, WINDOW_WIDTH / 2 - maxTextW / 2 - iconSize / 2, WINDOW_HEIGHT / 5 + 100 * (i + 1) - iconSize / 2, iconSize * 1.5, FONT_COLOR);
        }
    }
    // back button in the top left corner
    button backButton = {
        60,
        60,
        100,
        50,
        {BUTTON_PRIMARY_COLOR},
        {BUTTON_HOVER_COLOR},
        {BUTTON_LABEL_COLOR},
        {BUTTON_OUTLINE_COLOR},
        BUTTON_OUTLINE_THICKNESS,
        "back",
        BACK_ICON_PATH};
    Game->buttons.MainMenu = backButton;
    drawButton(renderer, &backButton);
}

void drawChooseMachineGameMode(SDL_Renderer *renderer, game *Game)
{

    // title
    char text[100];
    int fontSize = 64;
    sprintf(text, "Choose Machine Game Mode");
    int w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);
    writeText(renderer, GAMEPAUSED_FONT, text, WINDOW_WIDTH / 2 - w / 2, WINDOW_HEIGHT / 8, fontSize, FONT_COLOR);

    // 2 play buttons one for auto and one for manual both are in the middle of the screen

    // auto button
    button machineGameAutoModeButton = {
        WINDOW_WIDTH / 2 - 100 - 5,
        WINDOW_HEIGHT / 2,
        200,
        50,
        {BUTTON_PRIMARY_COLOR},
        {BUTTON_HOVER_COLOR},
        {BUTTON_LABEL_COLOR},
        {BUTTON_OUTLINE_COLOR},
        BUTTON_OUTLINE_THICKNESS,
        "Auto",
        AUTO_ICON_PATH

    };
    Game->buttons.MachineGameAutoMode = machineGameAutoModeButton;
    drawButton(renderer, &machineGameAutoModeButton);

    // manual button

    button machineGameManualModeButton = {
        WINDOW_WIDTH / 2 + 100 + 5,
        WINDOW_HEIGHT / 2,
        200,
        50,
        {BUTTON_PRIMARY_COLOR},
        {BUTTON_HOVER_COLOR},
        {BUTTON_LABEL_COLOR},
        {BUTTON_OUTLINE_COLOR},
        BUTTON_OUTLINE_THICKNESS,
        "Manual",
        MANUAL_ICON_PATH

    };
    Game->buttons.MachineGameManualMode = machineGameManualModeButton;
    drawButton(renderer, &machineGameManualModeButton);

    button backButton = {
        60,
        60,
        100,
        50,
        {BUTTON_PRIMARY_COLOR},
        {BUTTON_HOVER_COLOR},
        {BUTTON_LABEL_COLOR},
        {BUTTON_OUTLINE_COLOR},
        BUTTON_OUTLINE_THICKNESS,
        "back",
        BACK_ICON_PATH};
    Game->buttons.MainMenu = backButton;
    drawButton(renderer, &backButton);
}

void drawTextInput(SDL_Renderer *renderer, game *Game)
{

    char text[100];
    int fontSize = 90;
    sprintf(text, "Enter Your Name:");
    int w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);

    writeText(renderer, GAMEPAUSED_FONT, text, WINDOW_WIDTH / 2 - w / 2, WINDOW_HEIGHT / 8, fontSize, FONT_COLOR);
    fontSize = 45;
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    sprintf(text, "%s", Game->player.name);

    w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);
    writeText(renderer, GAMEPAUSED_FONT, text, WINDOW_WIDTH / 2 - w / 2, WINDOW_HEIGHT / 2 - 50, fontSize, 255, 0, 0, 255);

    // draw wrting line
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    if (strlen(text))
        SDL_RenderDrawLine(renderer, WINDOW_WIDTH / 2 - w / 2, WINDOW_HEIGHT / 2 - 50 + fontSize, WINDOW_WIDTH / 2 + w / 2, WINDOW_HEIGHT / 2 - 50 + fontSize);

    fontSize = 24;
    // draw cursor to blink

    sprintf(text, "Press Enter to Start");
    w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);
    writeText(renderer, GAMEPAUSED_FONT, text, WINDOW_WIDTH / 2 - w / 2, WINDOW_HEIGHT / 2 + 150, fontSize, FONT_COLOR);
    int iconSize = 100;
    drawImage(renderer, ENTER_ICON_PATH, WINDOW_WIDTH / 2 - iconSize / 2, WINDOW_HEIGHT / 2 + 200, iconSize, iconSize);

    // back button in the top left corner
    button backButton = {
        60,
        60,
        100,
        50,
        {BUTTON_PRIMARY_COLOR},
        {BUTTON_HOVER_COLOR},
        {BUTTON_LABEL_COLOR},
        {BUTTON_OUTLINE_COLOR},
        BUTTON_OUTLINE_THICKNESS,
        "back",
        BACK_ICON_PATH};
    Game->buttons.MainMenu = backButton;
    drawButton(renderer, &backButton);
}

void drawGrid(SDL_Renderer *renderer, game *Game)
{
    int n = Game->level + 5;

    int cellSize = GRID_SIZE / n;

    SDL_SetRenderDrawColor(renderer, BORDER_COLOR);

    SDL_Rect matrixRect = {OFFSET, OFFSET, GRID_SIZE, GRID_SIZE};
    SDL_RenderDrawRect(renderer, &matrixRect);
    // Draw vertical lines
    int x = n % 2;
    for (int i = 0; i < n - 1; i++)
    {

        int startX = (i + 1) * cellSize + OFFSET;
        int startY = cellSize + OFFSET;
        int endX = (i + 1) * cellSize + OFFSET;
        int endY = GRID_SIZE + OFFSET - cellSize;

        for (int s = 0; s < THICKNESS + x; s++)
        {

            SDL_RenderDrawLine(renderer, startX + s, startY, endX + s, endY);
            SDL_RenderDrawLine(renderer, startY, startX + s, endY, endX + s);
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
                int current = Game->matrix[i][j];

                if (current != 0 && Game->state != Selecting)
                {
                    drawDiagonal(renderer, n, current, x, y);
                }

                continue;
            }

            SDL_SetRenderDrawColor(renderer, BORDER_COLOR); // Color of the big circle

            drawFilledCircle(renderer, x, y, cellSize / 3); // Draw the big circle vertically

            drawFilledCircle(renderer, y, x, cellSize / 3); // Draw the big circle horizontally
            int startI = -1, startJ = -1;
            if (Game->solution)
            {
                startI = Game->solution->startI, startJ = Game->solution->startJ;
            }
            int hoverX, hoverY;
            SDL_GetMouseState(&hoverX, &hoverY);

            int hoverI, hoverJ;
            bool isOutside = false;
            getMatrixClick(renderer, hoverX, hoverY, n, &hoverI, &hoverJ, &isOutside);
            if (!isOutside || (Game->machineMode && Game->state != Filling))
            {
                hoverI = -1;
                hoverJ = -1;
            }

            SDL_SetRenderDrawColor(renderer, SMALL_CIRCLE_COLOR); // Color of the small circle
            if (Game->state == Selecting || Game->state == Filling)
            {
                if (startI == i && startJ == j && Game->state != Filling)
                {
                    SDL_SetRenderDrawColor(renderer, START_CIRCLE_COLOR);
                }
                else if (hoverI == i && hoverJ == j)
                {
                    Game->manualFill && Game->state == Filling ? SDL_SetRenderDrawColor(renderer, START_CIRCLE_COLOR)
                                                               : SDL_SetRenderDrawColor(renderer, HOVER_CIRCLE_COLOR);
                }
                else if (Game->helpers.selectedI == i && Game->helpers.selectedJ == j)
                {
                    Game->manualFill && Game->state == Filling ? SDL_SetRenderDrawColor(renderer, START_CIRCLE_COLOR)
                                                               : SDL_SetRenderDrawColor(renderer, SELECTED_CIRCLE_COLOR);
                }
                else
                {
                    SDL_SetRenderDrawColor(renderer, SMALL_CIRCLE_COLOR);
                }
            }
            else if (Game->state == Result && Game->helpers.win == -1)
            {
                if (Game->helpers.selectedI == i && Game->helpers.selectedJ == j)
                    SDL_SetRenderDrawColor(renderer, WRONG_CIRCLE_COLOR);
            }

            if (Game->state == Selecting && Game->machineMode)
            {
                if (Game->helpers.selectedI == i && Game->helpers.selectedJ == j)
                {
                    SDL_SetRenderDrawColor(renderer, SELECTED_CIRCLE_COLOR);
                }
            }
            drawFilledCircle(renderer, x, y, cellSize / 4);       // Draw the small circle vertically
            SDL_SetRenderDrawColor(renderer, SMALL_CIRCLE_COLOR); // Color of the small circle

            if (Game->state == Selecting || Game->state == Filling)
            {
                if (startI == j && startJ == i && Game->state != Filling)
                {
                    SDL_SetRenderDrawColor(renderer, START_CIRCLE_COLOR);
                }
                else if (hoverI == j && hoverJ == i)
                {
                    Game->manualFill && Game->state == Filling ? SDL_SetRenderDrawColor(renderer, START_CIRCLE_COLOR)
                                                               : SDL_SetRenderDrawColor(renderer, HOVER_CIRCLE_COLOR);
                }
                else if (Game->helpers.selectedI == j && Game->helpers.selectedJ == i)
                {
                    Game->manualFill ? SDL_SetRenderDrawColor(renderer, START_CIRCLE_COLOR)
                                     : SDL_SetRenderDrawColor(renderer, SELECTED_CIRCLE_COLOR);
                }
                else
                {
                    SDL_SetRenderDrawColor(renderer, SMALL_CIRCLE_COLOR);
                }
            }
            else if (Game->state == Result && Game->helpers.win == -1)
            {
                if (Game->helpers.selectedI == j && Game->helpers.selectedJ == i)
                    SDL_SetRenderDrawColor(renderer, WRONG_CIRCLE_COLOR);
            }
            if (Game->state == Selecting && Game->machineMode)
            {
                if (Game->helpers.selectedI == j && Game->helpers.selectedJ == i)
                {
                    SDL_SetRenderDrawColor(renderer, SELECTED_CIRCLE_COLOR);
                }
            }

            drawFilledCircle(renderer, y, x, cellSize / 4); // Draw the small circle horizontally
        }
    }

    char text[100];
    int fontSize = 24;
    if (Game->state == Filling)
    {
        sprintf(text, "Fill the grid with obstacles");
    }
    else if (Game->state == Memorizing)
    {
        sprintf(text, "%s the obstacles positions", Game->machineMode ? "The machine memorizes" : "Memorize");
    }
    else if (Game->state == Selecting)
    {
        sprintf(text, "%s the end circle", Game->machineMode ? "The machine selects" : "Select");
    }
    else if (Game->state == Result)
    {
        sprintf(text, "Result");
    }

    int w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);
    writeText(renderer, GAMEPAUSED_FONT, text, OFFSET + GRID_SIZE / 2 - w / 2, OFFSET / 2 - fontSize / 2, fontSize, 255, 255, 255, 255);
    int lifes = 3 - Game->loseStreak;
    int iconSize = 30;
    int gap = 20;

    for (int i = 0; i < lifes; i++)
    {
        drawImage(renderer, HEART_ICON_PATH, OFFSET + GRID_SIZE / 2 + (i - 1) * (iconSize + gap) - iconSize / 2, OFFSET + GRID_SIZE + iconSize / 2, iconSize, iconSize);
    }

    if (strcmp(Game->player.name, "root") || Game->state != Selecting)
        return;

    SDL_SetRenderDrawColor(renderer, SELECTED_CIRCLE_COLOR);
    int endx = Game->solution->endJ * cellSize + OFFSET + cellSize / 2 + THICKNESS / 2;
    int endy = Game->solution->endI * cellSize + OFFSET + cellSize / 2 + THICKNESS / 2;
    drawFilledCircle(renderer, endx, endy, cellSize / 16);
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
bool drawPath(SDL_Renderer *renderer, game *Game)
{

    if (Game->helpers.pathDrawStartTime == 0)
        Game->helpers.pathDrawStartTime = SDL_GetTicks();

    if (Game->helpers.currentPath == NULL && Game->helpers.pathEndCircleTime == 0)
    {
        Game->helpers.currentPath = Game->solution->path->next;
        playSoundEffect(Game->sounds.step);
    }

    if (SDL_GetTicks() - Game->helpers.pathDrawStartTime > CIRCLE_DRAW_TIME && Game->helpers.currentPath != NULL)
    {
        playSoundEffect(Game->sounds.step);
        Game->helpers.pathDrawStartTime = 0;
        Game->helpers.currentPath = Game->helpers.currentPath->next;
    }

    path *start = Game->solution->path;
    path *end = Game->helpers.currentPath;
    int n = Game->level + 5;
    int cellSize = GRID_SIZE / n;
    while (start != end)
    {
        int x = OFFSET + start->y * cellSize + cellSize / 2 + THICKNESS / 2;
        int y = OFFSET + start->x * cellSize + cellSize / 2 + THICKNESS / 2;
        int w = cellSize / 6;

        if (start == Game->solution->path)
        {
            SDL_SetRenderDrawColor(renderer, START_CIRCLE_COLOR);
            w = cellSize / 4;
        }
        else if (start->next == NULL)
        {
            SDL_SetRenderDrawColor(renderer, END_CIRCLE_COLOR);
            w = cellSize / 4;
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, PATH_COLOR);
        }

        drawFilledCircle(renderer, x, y, w);
        SDL_SetRenderDrawColor(renderer, WRONG_CIRCLE_COLOR);

        if (start->next != NULL)
            drawArrow(renderer, x, y, w / 3, start->next->dir);

        start = start->next;
    }

    bool isTheEnd = end == NULL;
    if (!isTheEnd)
        return false;

    if (Game->helpers.pathEndCircleTime == 0)
        Game->helpers.pathEndCircleTime = SDL_GetTicks();

    if (SDL_GetTicks() - Game->helpers.pathEndCircleTime < FULL_PATH_SHOW_TIME)
    {

        return false;
    }

    return true;
}

void drawSideBar(SDL_Renderer *renderer, game *Game)
{
    // side bar
    int outline = 10;
    int sideBarWidth = WINDOW_WIDTH - GRID_SIZE - OFFSET * 2;

    SDL_Rect outerSideBar = {GRID_SIZE + OFFSET * 2, 0, sideBarWidth, WINDOW_HEIGHT};
    SDL_Rect innerSideBar = {GRID_SIZE + OFFSET * 2 + outline, outline, sideBarWidth - 2 * outline, WINDOW_HEIGHT - 2 * outline};

    SDL_SetRenderDrawColor(renderer, SIDE_BAR_OUTLINE_COLOR);
    SDL_RenderFillRect(renderer, &outerSideBar);

    SDL_SetRenderDrawColor(renderer, SIDE_BAR_COLOR);
    SDL_RenderFillRect(renderer, &innerSideBar);

    // Player name
    int nameStart = outline;
    int nameEnd = nameStart + 0.1 * (WINDOW_HEIGHT - 2 * outline);

    // Level display
    int levelStart = nameEnd + 1;
    int levelEnd = levelStart + 0.2 * (WINDOW_HEIGHT - 2 * outline);

    // Score display
    int scoreStart = levelEnd + 1;
    int scoreEnd = scoreStart + 0.2 * (WINDOW_HEIGHT - 2 * outline);

    // Time display
    int timeStart = scoreEnd + 1;
    int timeEnd = timeStart + 0.2 * (WINDOW_HEIGHT - 2 * outline);

    SDL_SetRenderDrawColor(renderer, FONT_COLOR);
    SDL_Rect nameRect = {GRID_SIZE + OFFSET * 2 + outline, nameStart, sideBarWidth - 2 * outline, nameEnd - nameStart};
    SDL_Rect levelRect = {GRID_SIZE + OFFSET * 2 + outline, levelStart, sideBarWidth - 2 * outline, levelEnd - levelStart};
    SDL_Rect scoreRect = {GRID_SIZE + OFFSET * 2 + outline, scoreStart, sideBarWidth - 2 * outline, scoreEnd - scoreStart};
    SDL_Rect timeRect = {GRID_SIZE + OFFSET * 2 + outline, timeStart, sideBarWidth - 2 * outline, timeEnd - timeStart};

    char text[100];

    sprintf(text, "%s", Game->player.name);
    int fontSize = 34;

    int w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);

    writeText(renderer, GAMEPAUSED_FONT, text, nameRect.x + nameRect.w / 2 - w / 2, nameRect.y + nameRect.h / 2 - fontSize / 2, fontSize, FONT_COLOR);
    // line under name
    SDL_RenderDrawLine(renderer, nameRect.x + nameRect.w / 2 - w / 2, nameRect.y + nameRect.h / 2 + fontSize / 2, nameRect.x + nameRect.w / 2 + w / 2, nameRect.y + nameRect.h / 2 + fontSize / 2);

    int displayBoxWidth = 200;

    // Level display

    sprintf(text, "Level :");
    w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);

    writeText(renderer, GAMEPAUSED_FONT, text, levelRect.x + levelRect.w / 2 - w / 2, levelRect.y + fontSize / 2, fontSize, FONT_COLOR);

    SDL_Rect levelDisplay = {levelRect.x + levelRect.w / 2 - displayBoxWidth / 2, levelRect.y + 2 * fontSize, displayBoxWidth, fontSize};

    sprintf(text, "%d", Game->level);
    w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);
    SDL_SetRenderDrawColor(renderer, DISPLAY_BOX_COLOR);
    SDL_RenderFillRect(renderer, &levelDisplay);

    // bold
    SDL_SetRenderDrawColor(renderer, SIDE_BAR_OUTLINE_COLOR);
    for (int i = 0; i < 2; i++)
    {
        SDL_Rect rect = {levelDisplay.x + i, levelDisplay.y + i, levelDisplay.w - 2 * i, levelDisplay.h - 2 * i};
        SDL_RenderDrawRect(renderer, &rect);
    }
    writeText(renderer, GAMEPAUSED_FONT, text, levelDisplay.x + levelDisplay.w / 2 - w / 2, levelDisplay.y, fontSize, FONT_COLOR);

    // Score display

    sprintf(text, "Score :");
    w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);
    writeText(renderer, GAMEPAUSED_FONT, text, scoreRect.x + scoreRect.w / 2 - w / 2, scoreRect.y + fontSize / 2, fontSize, FONT_COLOR);

    SDL_Rect scoreDisplay = {scoreRect.x + scoreRect.w / 2 - displayBoxWidth / 2, scoreRect.y + 2 * fontSize, displayBoxWidth, fontSize};

    sprintf(text, "%d", Game->player.score);

    w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);
    SDL_SetRenderDrawColor(renderer, DISPLAY_BOX_COLOR);
    SDL_RenderFillRect(renderer, &scoreDisplay);
    SDL_SetRenderDrawColor(renderer, SIDE_BAR_OUTLINE_COLOR);
    for (int i = 0; i < 2; i++)
    {
        SDL_Rect rect = {scoreDisplay.x + i, scoreDisplay.y + i, scoreDisplay.w - 2 * i, scoreDisplay.h - 2 * i};
        SDL_RenderDrawRect(renderer, &rect);
    }
    writeText(renderer, GAMEPAUSED_FONT, text, scoreDisplay.x + scoreDisplay.w / 2 - w / 2, scoreDisplay.y, fontSize, FONT_COLOR);

    // Time display
    sprintf(text, "Time :");
    w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);
    writeText(renderer, GAMEPAUSED_FONT, text, timeRect.x + timeRect.w / 2 - w / 2, timeRect.y + fontSize / 2, fontSize, FONT_COLOR);

    SDL_Rect timeDisplay = {timeRect.x + timeRect.w / 2 - displayBoxWidth / 2, timeRect.y + 2 * fontSize, displayBoxWidth, fontSize};

    sprintf(text, "%s", formatTime(SDL_GetTicks() - Game->helpers.gameStartTime));

    w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);
    SDL_SetRenderDrawColor(renderer, DISPLAY_BOX_COLOR);

    SDL_RenderFillRect(renderer, &timeDisplay);

    SDL_SetRenderDrawColor(renderer, SIDE_BAR_OUTLINE_COLOR);
    for (int i = 0; i < 2; i++)
    {
        SDL_Rect rect = {timeDisplay.x + i, timeDisplay.y + i, timeDisplay.w - 2 * i, timeDisplay.h - 2 * i};
        SDL_RenderDrawRect(renderer, &rect);
    }
    writeText(renderer, GAMEPAUSED_FONT, text, timeDisplay.x + timeDisplay.w / 2 - w / 2, timeDisplay.y, fontSize, FONT_COLOR);

    // draw the buttons

    int centerX = outline + GRID_SIZE + OFFSET * 2 + innerSideBar.w / 2;

    int centerY = outline + WINDOW_HEIGHT / 2;
    // draw the buttons
    button pauseButton = {
        centerX, // center of the side bar
        centerY + 170,
        200,
        50,
        {BUTTON_PRIMARY_COLOR},
        {BUTTON_HOVER_COLOR},
        {BUTTON_LABEL_COLOR},
        {BUTTON_OUTLINE_COLOR},
        BUTTON_OUTLINE_THICKNESS,
        "Pause Game",
        PAUSE_ICON_PATH

    };
    Game->buttons.pause = pauseButton;
    drawButton(renderer, &pauseButton);

    // save & quit button

    button saveAndQuitButton = {
        centerX,
        pauseButton.centerY + 100,
        200,
        50,
        {BUTTON_SECONDARY_COLOR},
        {BUTTON_HOVER_COLOR},
        {BUTTON_LABEL_COLOR},
        {BUTTON_OUTLINE_COLOR},
        BUTTON_OUTLINE_THICKNESS,
        "Save & Quit",
        SAVE_ICON_PATH

    };

    if (Game->machineMode)
    {
        strcpy(saveAndQuitButton.label, "Quit");
        strcpy(saveAndQuitButton.iconPath, EXIT_ICON_PATH);
    }

    Game->buttons.MainMenu = saveAndQuitButton;
    drawButton(renderer, &saveAndQuitButton);
}

void drawPause(SDL_Renderer *renderer, game *Game)
{
    char text[100];
    int fontSize = 90;
    sprintf(text, "Game Paused");
    int w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);

    writeText(renderer, GAMEPAUSED_FONT, text, WINDOW_WIDTH / 2 - w / 2, WINDOW_HEIGHT / 8, fontSize, FONT_COLOR);

    // resume button
    button resumeButton = {
        WINDOW_WIDTH / 2,
        WINDOW_HEIGHT / 2,
        200,
        50,
        {BUTTON_PRIMARY_COLOR},
        {BUTTON_HOVER_COLOR},
        {BUTTON_LABEL_COLOR},
        {BUTTON_OUTLINE_COLOR},
        BUTTON_OUTLINE_THICKNESS,
        "Resume",
        RESUME_ICON_PATH

    };
    Game->buttons.pause = resumeButton;
    drawButton(renderer, &resumeButton);

    // save and exit button

    button saveAndExitButton = {
        WINDOW_WIDTH / 2,
        WINDOW_HEIGHT / 2 + 100,
        200,
        50,
        {BUTTON_SECONDARY_COLOR},
        {BUTTON_HOVER_COLOR},
        {BUTTON_LABEL_COLOR},
        {BUTTON_OUTLINE_COLOR},
        BUTTON_OUTLINE_THICKNESS,
        "Save & Exit",
        SAVE_ICON_PATH

    };
    Game->buttons.MainMenu = saveAndExitButton;

    drawButton(renderer, &saveAndExitButton);
}

void drawGameOver(SDL_Renderer *renderer, game *Game)
{
    char text[100];
    int fontSize = 90;
    sprintf(text, "Game Over");
    int w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);

    writeText(renderer, GAMEPAUSED_FONT, text, WINDOW_WIDTH / 2 - w / 2, WINDOW_HEIGHT / 10, fontSize, FONT_COLOR);

    // you won if game->level = maxLevel or you lost if game->level = 0
    fontSize = 45;

    sprintf(text, "You %s The Game", Game->level == 0 ? "Lost" : "Won");

    w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);

    SDL_Color color = Game->level == 0 ? (SDL_Color){255, 0, 0, 255} : (SDL_Color){0, 255, 0, 255};

    writeText(renderer, GAMEPAUSED_FONT, text, WINDOW_WIDTH / 2 - w / 2, WINDOW_HEIGHT / 3, fontSize, color.r, color.g, color.b, color.a);

    sprintf(text, "Your Score:%d", Game->player.score);
    w = mesureTextWidth(GAMEPAUSED_FONT, text, fontSize);
    writeText(renderer, GAMEPAUSED_FONT, text, WINDOW_WIDTH / 2 - w / 2, WINDOW_HEIGHT / 2.1, fontSize, FONT_COLOR);

    // play again button

    button playAgainButton = {
        WINDOW_WIDTH / 2,
        WINDOW_HEIGHT / 2 + 100,
        200,
        50,
        {BUTTON_PRIMARY_COLOR},
        {BUTTON_HOVER_COLOR},
        {BUTTON_LABEL_COLOR},
        {BUTTON_OUTLINE_COLOR},
        BUTTON_OUTLINE_THICKNESS,
        "Play Again",
        AGAIN_ICON_PATH};

    Game->buttons.playAgain = playAgainButton;

    drawButton(renderer, &playAgainButton);

    // save and exit button

    button saveAndExitButton = {
        WINDOW_WIDTH / 2,
        WINDOW_HEIGHT / 2 + 200,
        200,
        50,
        {BUTTON_SECONDARY_COLOR},
        {BUTTON_HOVER_COLOR},
        {BUTTON_LABEL_COLOR},
        {BUTTON_OUTLINE_COLOR},
        BUTTON_OUTLINE_THICKNESS,
        "Exit",
        EXIT_ICON_PATH

    };

    Game->buttons.MainMenu = saveAndExitButton;
    drawButton(renderer, &saveAndExitButton);
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

SDL_Color getPixelColor(SDL_Renderer *renderer, int pixel_X, int pixel_Y)
{
    SDL_Surface *getPixel_Surface = SDL_CreateRGBSurface(0, WINDOW_WIDTH, WINDOW_HEIGHT, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
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

    SDL_FreeSurface(getPixel_Surface);

    return pixelColor;
}

void machineModeMemorize(SDL_Renderer *renderer, int n, int matrix[MAX_N][MAX_N])
{
    int cellSize = GRID_SIZE / n;
    initializeMatrix(n, matrix);

    for (int i = 1; i < n - 1; i++)
    {
        for (int j = 1; j < n - 1; j++)
        {
            int x = OFFSET + j * cellSize + cellSize / 2 + THICKNESS / 2;
            int y = OFFSET + i * cellSize + cellSize / 2 + THICKNESS / 2;

            if (j <= 0 || j >= n - 1 || i <= 0 || i >= n - 1)
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

            int of = cellSize / 4;
            color = getPixelColor(renderer, x - of, y + of);
            bool isUp = compareColor(color, DIAGONAL_COLOR); // check if the diagonal is up or down
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

void machineModeSelecting(SDL_Renderer *renderer, int n, int matrix[MAX_N][MAX_N], int *selectI, int *selectJ)
{
    int cellSize = GRID_SIZE / n;
    int startI = 0, startJ = 0;

    for (int i = 1; i < n - 1; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (j > 0 && j < n - 1)
                continue;
            int x = OFFSET + j * cellSize + cellSize / 2 + THICKNESS / 2;
            int y = OFFSET + i * cellSize + cellSize / 2 + THICKNESS / 2;

            SDL_Color color1 = getPixelColor(renderer, x, y);
            SDL_Color color2 = getPixelColor(renderer, y, x);
            bool isStart1 = compareColor(color1, START_CIRCLE_COLOR);
            bool isStart2 = compareColor(color2, START_CIRCLE_COLOR);
            if (isStart1 || isStart2)
            {
                startI = isStart1 ? i : j;
                startJ = isStart1 ? j : i;
                i = n;
                break;
            }
        }
    }

    solution *s = solveMatrix(matrix[startI][startJ], 0, n, matrix); // obs = 0
    // free Game->
    freePath(s->path);

    *selectI = s->endI;
    *selectJ = s->endJ;
    free(s);
    return;
}

void machineModeChoosing(SDL_Renderer *renderer, game *Game)
{
    if (Game->helpers.machineModeSelectingTime == 0)
        Game->helpers.machineModeSelectingTime = SDL_GetTicks();

    if ((SDL_GetTicks() - Game->helpers.machineModeSelectingTime) < MACHINE_SELECTING_TIME)
        return;

    int n = Game->level + 5;
    int cellSize = GRID_SIZE / n;
    // custom event
    SDL_Event e;
    e.type = SDL_USEREVENT;
    e.user.code = 139;
    int endX = Game->helpers.selectedJ * cellSize + OFFSET + cellSize / 2 + THICKNESS / 2;
    int endY = Game->helpers.selectedI * cellSize + OFFSET + cellSize / 2 + THICKNESS / 2;
    // set x and y coordinates
    e.user.data1 = NULL;
    e.user.data2 = NULL;
    e.button.x = endX;
    e.button.y = endY;
    SDL_PushEvent(&e);
}

void getMatrixClick(SDL_Renderer *renderer, int clickX, int clickY, int n, int *i, int *j, bool *isOutside)
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

    bool isCorner = (*i == 0 && *j == 0) || (*i == 0 && *j == n - 1) || (*i == n - 1 && *j == 0) || (*i == n - 1 && *j == n - 1);
    bool out = *i == 0 || *i == n - 1 || *j == 0 || *j == n - 1;
    if (isCorner)
    {
        *i = -1;
        *j = -1;
        return;
    }
    *isOutside = out;
    if (!out)
        return;

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

bool compareColor(SDL_Color color, int r, int g, int b, int a)
{
    return (r == color.r) && (g == color.g) && (b == color.b) && (a = color.a);
}

bool isClickInButton(SDL_Event event, game *Game, button Button)
{
    if (event.type != SDL_MOUSEBUTTONUP || event.button.button != SDL_BUTTON_LEFT)
        return false;

    int x = Button.centerX;
    int y = Button.centerY;
    int w = Button.width;
    int h = Button.height;

    int mouseX = event.button.x;
    int mouseY = event.button.y;

    if (mouseX >= x - w / 2 && mouseX <= x + w / 2 && mouseY >= y - h / 2 && mouseY <= y + h / 2)
    {
        playSoundEffect(Game->sounds.click);

        return true;
    }
    return false;
}
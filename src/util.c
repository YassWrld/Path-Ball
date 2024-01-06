#include "util.h"

int randomInt(int min, int max)
{
    static bool first = true;

    if (first)
    {
        srand(time(NULL));
        first = false;
    }
    return rand() % (max - min + 1) + min;
}

void printMatrix(int n, int matrix[n][n])
{
    printf("+---");
    for (int j = 0; j < n; j++)
    {
        printf("----");
    }
    printf("--+\n");

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (j == 0)
            {
                printf("| ");
            }
            printf("%2d | ", matrix[i][j]);
        }

        printf("\n+---");
        for (int j = 0; j < n; j++)
        {
            printf("----");
        }
        printf("--+\n");
    }
}

date_t getCurrentDate()
{
    date_t currentDate;
    time_t t;
    struct tm *tm_info;

    time(&t);
    tm_info = localtime(&t);

    currentDate.day = tm_info->tm_mday;
    currentDate.month = tm_info->tm_mon + 1;    // Month is 0-based, so we add 1
    currentDate.year = tm_info->tm_year + 1900; // Years since 1900

    return currentDate;
}

int graycefulDelay(Uint32 ms)
{

    if (ms < 100)
    {
        SDL_Delay(ms);
        return 0;
    }

    int start = SDL_GetTicks();
    SDL_Event e;

    while (SDL_GetTicks() - start < ms)
    {
        int exist = SDL_PollEvent(&e);
        if (!exist)
            continue;

        if (e.type == SDL_QUIT)
        {
            exit(0);
            return 1;
        }

        SDL_PushEvent(&e);
    }

    return 0;
}

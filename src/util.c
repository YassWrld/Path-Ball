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

void gracefulSDLDelay(int fps)
{
    clock_t start = clock();
    while (clock() < start + (CLOCKS_PER_SEC / fps));
}
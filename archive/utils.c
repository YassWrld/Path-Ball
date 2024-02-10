
#include <stdbool.h>
#include <stdlib.h>
#include "utils.h"
#include "functions.h"
#include <time.h>
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

color randomColor()
{
    return (color){randomInt(0, 255), randomInt(0, 255), randomInt(0, 255), 255};
}
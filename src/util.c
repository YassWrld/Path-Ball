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
bool checkAllowedString(char *str)
{

    int len = strlen(str);
    bool isAllowed = false;
    for (int i = 0; i < len; i++)
    {
        if (str[i] >= 'a' && str[i] <= 'z')
            isAllowed = true;
        else if (str[i] >= 'A' && str[i] <= 'Z')
            isAllowed = true;
        else if (str[i] >= '0' && str[i] <= '9')
            isAllowed = true;
        else if (str[i] == ' ' || str[i] == '_' || str[i] == '-' || str[i] == '.')
            isAllowed = true;
        else
        {
            isAllowed = false;
            break;
        }
    }
    return isAllowed;
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

char *formatTime(int ms)
{
    int seconds = ms / 1000;
    int minutes = seconds / 60;
    seconds %= 60;
    int hours = minutes / 60;
    minutes %= 60;

    char *time = malloc(9 * sizeof(char));
    if (hours > 0)
        sprintf(time, "%02dh:%02dm:%02ds", hours, minutes, seconds);
    else if (minutes > 0)
        sprintf(time, "%02dm:%02ds", minutes, seconds);
    else
        sprintf(time, "%02ds", seconds);
    return time;
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

Mix_Music *loadMusic(char *path)
{
    Mix_Music *music = Mix_LoadMUS(path);
    if (!music)
    {
        printf("Mix_LoadMUS Error: %s\n", Mix_GetError());
        return NULL;
    }

    return music;
}

Mix_Chunk *loadSoundEffect(char *path)
{
    Mix_Chunk *soundEffect = Mix_LoadWAV(path);
    if (!soundEffect)
    {
        printf("Mix_LoadWAV Error: %s\n", Mix_GetError());
        return NULL;
    }
    return soundEffect;
}

void playSoundEffect(Mix_Chunk *soundEffect)
{
    int status = Mix_PlayChannel(-1, soundEffect, 0);
    if (status == -1)
    {
        printf("Mix_PlayChannel Error: %s\n", Mix_GetError());
    }
}

void playMusic(Mix_Music *music)
{
    int status = Mix_PlayMusic(music, -1);
    if (status == -1)
    {
        printf("Mix_PlayMusic Error: %s\n", Mix_GetError());
    }
}
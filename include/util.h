#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>


typedef struct date_t // date structure
{
    int day; // 1-31
    int month; // 1-12
    int year; 
} date_t;





int randomInt(int min, int max); // returns a random number between min and max
date_t getCurrentDate(); // returns the current date
char *formatTime(int ms); // returns a string with the time in the format HH:MM:SS
int graycefulDelay(Uint32 ms); // delays the program for ms milliseconds without crashing the window
bool checkAllowedString(char *str); // checks if a string is allowed (For the player name characters)


Mix_Music *loadMusic(char *path); // loads a music file
Mix_Chunk *loadSoundEffect(char *path); // loads a sound effect file
void playMusic(Mix_Music *music);       // plays a music file
void playSoundEffect(Mix_Chunk *sound);  // plays a sound effect file

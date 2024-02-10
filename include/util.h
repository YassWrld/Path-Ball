#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

typedef struct date_t // date structure
{
    int day;   // 1-31
    int month; // 1-12
    int year;
} date_t;

/**
 * @brief Returns a random number between min and max
 *
 * @param min: minimum value of the random number range
 * @param max: maximum value of the random number range
 * @return int: random number within the specified range
 */
int randomInt(int min, int max);

/**
 * @brief Returns the current date
 *
 * @return date_t: structure representing the current date
 */
date_t getCurrentDate();

/**
 * @brief Returns a string with the time in the format HH:MM:SS
 *
 * @param ms: time in milliseconds
 * @return char*: string representing the time in HH:MM:SS format
 */
char *formatTime(int ms);

/**
 * @brief Delays the program for ms milliseconds without crashing the window
 *
 * @param ms: duration of delay in milliseconds
 * @return int: 0 on success, -1 on error
 */
int graycefulDelay(Uint32 ms);

/**
 * @brief Checks if a string is allowed (For the player name characters)
 *
 * @param str: string to be checked
 * @return bool: true if the string contains only allowed characters, false otherwise
 */
bool checkAllowedString(char *str);

/**
 * @brief Loads a music file
 *
 * @param path: path to the music file
 * @return Mix_Music*: pointer to the loaded music
 */
Mix_Music *loadMusic(char *path);

/**
 * @brief Loads a sound effect file
 *
 * @param path: path to the sound effect file
 * @return Mix_Chunk*: pointer to the loaded sound effect
 */
Mix_Chunk *loadSoundEffect(char *path);

/**
 * @brief Plays a music file
 *
 * @param music: pointer to the loaded music
 * @return void
 */
void playMusic(Mix_Music *music);

/**
 * @brief Plays a sound effect file
 *
 * @param sound: pointer to the loaded sound effect
 * @return void
 */
void playSoundEffect(Mix_Chunk *sound);
/**
 * @brief Initializes SDL Library (with TTF, Image and Mixer) and creates a window and a renderer
 *
 * @param window Pointer to a pointer to SDL_Window
 * @param renderer Pointer to a pointer to SDL_Renderer
 * @param TITLE Title of the window
 * @param WIDTH Width of the window
 * @param HEIGHT Height of the window
 * @param ICON_PATH Path to the window icon
 */
void InitializeSDL(SDL_Window **window, SDL_Renderer **renderer, char *TITLE, int WIDTH, int HEIGHT, char *ICON_PATH);

/**
 * @brief Quits SDL Library (with TTF, Image and Mixer) and destroys the window and the renderer
 *
 * @param window Pointer to a pointer to SDL_Window
 * @param renderer Pointer to a pointer to SDL_Renderer
 * @return int Returns 0 on success, -1 on failure
 */
int QuitSDL(SDL_Window **window, SDL_Renderer **renderer);

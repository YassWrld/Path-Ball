
#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "util.h"
#include "logic.h"

#define GAME_TITLE "Path Ball"

#define HEIGHT 700    // window height
#define WIDTH 1100    // window width
#define GRID_SIZE 580 // grid size (square)
#define OFFSET 60     // offset from the top left corner of the window
#define THICKNESS 4   // thickness of the lines

// Color Palette
#define TAN 227, 180, 122, 255       // Color Tan
#define DARK_ORANGE 141, 64, 47, 255 // Color Dark Orange

#define BACKGROUND_COLOR 94, 26, 32, 255 // Background Color (Dark Red)

#define BLUR_BACKGROUND_COLOR 180, 134, 88, 30 // Blurred Background Color (Light Brown)

#define DIAGONAL_COLOR TAN // Diagonal Color (Tan)

#define BORDER_COLOR 190, 121, 79, 255 // Border Color (Brown)

#define SMALL_CIRCLE_COLOR DARK_ORANGE // Small Circle Color (Dark Orange)

#define HOVER_CIRCLE_COLOR TAN // Hover Circle Color (Tan)

#define SELECTED_CIRCLE_COLOR 40, 80, 128, 255 // Selected Circle Color (Dark Blue)

#define PATH_COLOR 0, 169, 157, 255 // Path Color (Turquoise)

#define CIRCLE_OUTLINE_COLOR 241, 237, 232, 255 // Circle Outline Color (Light Grey)

#define WRONG_CIRCLE_COLOR 255, 0, 0, 255 // Wrong Circle Color (Red)
#define START_CIRCLE_COLOR PATH_COLOR     // Start Circle Color (Turquoise)

#define END_CIRCLE_COLOR 0, 255, 0, 255 // End Circle Color (Green)

#define SIDE_BAR_COLOR TAN // Inner Side Bar Color (Tan)

#define SIDE_BAR_OUTLINE_COLOR DARK_ORANGE // Outer Side Bar Color (Dark Orange)

#define DISPLAY_BOX_COLOR 190, 121, 79, 255 // Display Box Color (Brown)

#define FONT_COLOR 0, 0, 0, 255 // Default Font Color (Black)

#define BUTTON_PRIMARY_COLOR PATH_COLOR       // Button Primary Color (Turquoise)
#define BUTTON_SECONDARY_COLOR 255, 0, 0, 255 // Button Secondary Color (Red)
#define BUTTON_HOVER_COLOR 255, 255, 0, 255   // Button Hover Color (Yellow)
#define BUTTON_LABEL_COLOR 255, 255, 255, 255 // Button Label Color (White)
#define BUTTON_OUTLINE_COLOR 0, 0, 0, 255     // Button Outline Color (Black)
#define BUTTON_OUTLINE_THICKNESS 4            // Button Outline Thickness

#define GAMEPAUSED_FONT "assets/fonts/gamepaused.otf" // game paused font path

#define ICON_PATH "assets/images/icon.png"           // window icon path
#define NOBG_ICON_PATH "assets/images/nobg-icon.png" // window icon without background  path
#define ENTER_ICON_PATH "assets/images/enter.png"    // enter button icon path
#define BACK_ICON_PATH "assets/images/back.png"      // back button icon path
#define PAUSE_ICON_PATH "assets/images/pause.png"    // pause button icon path
#define RESUME_ICON_PATH "assets/images/resume.png"  // resume button icon path
#define ROBOT_ICON_PATH "assets/images/robot.png"    // robot icon path
#define HUMAN_ICON_PATH "assets/images/human.png"    // human icon path
#define CROWN_ICON_PATH "assets/images/crown.png"    // crown icon path
#define SAVE_ICON_PATH "assets/images/save.png"      // save icon path
#define AUTO_ICON_PATH "assets/images/auto.png"      // auto icon path
#define MANUAL_ICON_PATH "assets/images/manual.png"  // manual icon path
#define HEART_ICON_PATH "assets/images/heart.png"    // heart icon path
#define EXIT_ICON_PATH "assets/images/exit.png"      // exit button icon path
#define AGAIN_ICON_PATH "assets/images/again.png"    // again button icon path

#define FIRST_ICON_PATH "assets/images/first.png"   // first icon path
#define SECOND_ICON_PATH "assets/images/second.png" // second icon path
#define THIRD_ICON_PATH "assets/images/third.png"   // third icon path

// components drawing functions

/**
 * @brief Draws an arrow
 *
 * (Draws an arrow at the specified position with the specified length and direction)
 *
 * @param renderer: SDL_Renderer pointer
 * @param x: x coordinate of the arrow's starting point
 * @param y: y coordinate of the arrow's starting point
 * @param l: length of the arrow
 * @param d: direction of the arrow (up, down, left, right)
 * @return void
 */
void drawArrow(SDL_Renderer *renderer, int x, int y, int l, direction d);

/**
 * @brief Draws a hexagon
 *
 * (Draws a hexagon at the specified position with the specified radius)
 *
 * @param renderer: SDL_Renderer pointer
 * @param x: x coordinate of the hexagon's center
 * @param y: y coordinate of the hexagon's center
 * @param r: radius of the hexagon
 * @return void
 */
void drawHexagon(SDL_Renderer *renderer, int x, int y, int r);


/**
 * @brief Filled circle drawing
 *
 * (Draws a filled circle on the screen)
 *
 * @param renderer: SDL_Renderer pointer
 * @param centerX: x coordinate of the circle center
 * @param centerY: y coordinate of the circle center
 * @param radius: radius of the circle
 * @return void
 */
void drawFilledCircle(SDL_Renderer *renderer, int centerX, int centerY, int radius);

/**
 * @brief Image drawing
 *
 * (Draws an image on the screen)
 *
 * @param renderer: SDL_Renderer pointer
 * @param path: path to the image file
 * @param x: x coordinate of the top-left corner of the image
 * @param y: y coordinate of the top-left corner of the image
 * @param w: width of the image
 * @param h: height of the image
 * @return void
 */
void drawImage(SDL_Renderer *renderer, char *path, int x, int y, int w, int h);

/**
 * @brief Text writing
 *
 * (Writes text on the screen)
 *
 * @param renderer: SDL_Renderer pointer
 * @param fontPath: path to the font file
 * @param text: text to be written
 * @param x: x coordinate of the top-left corner of the text
 * @param y: y coordinate of the top-left corner of the text
 * @param size: size of the font
 * @param r: red component of the font color
 * @param g: green component of the font color
 * @param b: blue component of the font color
 * @param a: alpha (transparency) component of the font color
 * @return void
 */
void writeText(SDL_Renderer *renderer, char *fontPath, char *text, int x, int y, int size, int r, int g, int b, int a);

/**
 * @brief Button Drawing
 *
 * (Draws a button on the screen)
 *
 * @param renderer: SDL_Renderer pointer
 * @param Button: pointer to a button structure
 * @return void
 */
void drawButton(SDL_Renderer *renderer, button *Button);

/**
 * @brief Blurred background drawing (Hexagons pattern)
 *
 * (Draws a blurred background with a hexagons pattern)
 *
 * @param renderer: SDL_Renderer pointer
 * @return void
 */
void drawBlurredBackground(SDL_Renderer *renderer);


// Screens drawing functions

/**
 * @brief Main menu drawing
 *
 * (Draws the main menu screen)
 *
 * @param renderer: SDL_Renderer pointer
 * @param Game: pointer to the game structure
 * @return void
 */
void drawMainMenu(SDL_Renderer *renderer, game *Game);

/**
 * @brief Top players drawing
 *
 * (Draws the top players screen)
 *
 * @param renderer: SDL_Renderer pointer
 * @param Game: pointer to the game structure
 * @return void
 */
void drawTopPlayers(SDL_Renderer *renderer, game *Game);

/**
 * @brief Choose machine game mode drawing
 *
 * (Draws the choose machine game mode screen)
 *
 * @param renderer: SDL_Renderer pointer
 * @param Game: pointer to the game structure
 * @return void
 */
void drawChooseMachineGameMode(SDL_Renderer *renderer, game *Game);

// Game drawing functions

/**
 * @brief Text input
 *
 * (Draws text input on the screen)
 *
 * @param renderer: SDL_Renderer pointer
 * @param Game: pointer to the game structure
 * @return void
 */
void drawTextInput(SDL_Renderer *renderer, game *Game);

/**
 * @brief Grid (matrix)
 *
 * (Draws the grid (matrix) on the screen)
 *
 * @param renderer: SDL_Renderer pointer
 * @param Game: pointer to the game structure
 * @return void
 */
void drawGrid(SDL_Renderer *renderer, game *Game);

/**
 * @brief Diagonal
 *
 * (Draws a diagonal line on the screen)
 *
 * @param renderer: SDL_Renderer pointer
 * @param n: size of the diagonal
 * @param direction: direction of the diagonal (0 for left, 1 for right)
 * @param centerX: x coordinate of the center of the diagonal
 * @param centerY: y coordinate of the center of the diagonal
 * @return void
 */
void drawDiagonal(SDL_Renderer *renderer, int n, int direction, int centerX, int centerY);


/**
 * @brief Side bar
 *
 * (Draws the side bar on the screen)
 *
 * @param renderer: SDL_Renderer pointer
 * @param Game: pointer to the game structure
 * @return void
 */
void drawSideBar(SDL_Renderer *renderer, game *Game);


/**
 * @brief Path
 *
 * (Draws the path on the screen)
 *
 * @param renderer: SDL_Renderer pointer
 * @param Game: pointer to the game structure
 * @return bool: true if path is successfully drawn, false otherwise
 */
bool drawPath(SDL_Renderer *renderer, game *Game);


/**
 * @brief Game over
 *
 * (Draws the game over screen)
 *
 * @param renderer: SDL_Renderer pointer
 * @param Game: pointer to the game structure
 * @return void
 */
void drawGameOver(SDL_Renderer *renderer, game *Game);

/**
 * @brief Pause
 *
 * (Draws the pause screen)
 *
 * @param renderer: SDL_Renderer pointer
 * @param Game: pointer to the game structure
 * @return void
 */
void drawPause(SDL_Renderer *renderer, game *Game);

/**
 * @brief Machine Fill the matrix
 *
 * (Fills the matrix for the machine mode)
 *
 * @param renderer: SDL_Renderer pointer
 * @param n: size of the matrix
 * @param matrix: the matrix to be filled
 * @return void
 */
void machineModeMemorize(SDL_Renderer *renderer, int n, int matrix[MAX_N][MAX_N]);

/**
 * @brief Machine Find the start and end
 *
 * (Finds the start and end points for the machine mode)
 *
 * @param renderer: SDL_Renderer pointer
 * @param n: size of the matrix
 * @param matrix: the matrix
 * @param selectI: pointer to store the selected row
 * @param selectJ: pointer to store the selected column
 * @return void
 */
void machineModeSelecting(SDL_Renderer *renderer, int n, int matrix[MAX_N][MAX_N], int *selectI, int *selectJ);

/**
 * @brief Machine Send the virtual click
 *
 * (Sends the virtual click for the machine mode)
 *
 * @param renderer: SDL_Renderer pointer
 * @param Game: pointer to the game structure
 * @return void
 */
void machineModeChoosing(SDL_Renderer *renderer, game *Game);

/**
 * @brief Get matrix logic click (i, j) from screen click (x, y)
 *
 * (Converts screen click coordinates to matrix logic coordinates)
 *
 * @param renderer: SDL_Renderer pointer
 * @param clickX: x coordinate of the screen click
 * @param clickY: y coordinate of the screen click
 * @param n: size of the matrix
 * @param i: pointer to store the row index
 * @param j: pointer to store the column index
 * @param isOutside: pointer to store if the click is outside the matrix
 * @return void
 */
void getMatrixClick(SDL_Renderer *renderer, int clickX, int clickY, int n, int *i, int *j, bool *isOutside);

/**
 * @brief Get pixel color from the screen
 *
 * (Retrieves the color of a pixel on the screen)
 *
 * @param renderer: SDL_Renderer pointer
 * @param pixel_X: x coordinate of the pixel
 * @param pixel_Y: y coordinate of the pixel
 * @return SDL_Color: color of the pixel
 */
SDL_Color getPixelColor(SDL_Renderer *renderer, int pixel_X, int pixel_Y);

/**
 * @brief Compare color struct with a given color
 *
 * (Compares a color struct with a given color)
 *
 * @param color: SDL_Color struct representing the color
 * @param r: red component of the color
 * @param g: green component of the color
 * @param b: blue component of the color
 * @param a: alpha (transparency) component of the color
 * @return bool: true if colors are equal, false otherwise
 */
bool compareColor(SDL_Color color, int r, int g, int b, int a);

/**
 * @brief Measure text width (for centering)
 *
 * (Measures the width of a text for centering purposes)
 *
 * @param fontPath: path to the font file
 * @param text: text whose width needs to be measured
 * @param size: size of the font
 * @return int: width of the text
 */
int mesureTextWidth(char *fontPath, char *text, int size);

/**
 * @brief Check if a click event occurs inside a button
 *
 * (Checks if a click event occurs inside a button)
 *
 * @param event: SDL_Event containing the click event
 * @param Game: pointer to the game structure
 * @param Button: button structure to check against
 * @return bool: true if click is inside the button, false otherwise
 */
bool isClickInButton(SDL_Event event, game *Game, button Button);

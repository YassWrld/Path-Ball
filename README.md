# Path Ball Game

## Description

This is a game project for University of Tecknology and Sciences Houari Boumediene (USTHB) in Algiers, Algeria. It's a 2D p puzzle game made with SDL2 and C.

## Game Description

In Path Ball, your objective is to remember the positions of various bumpers before they disappear. This task extends beyond simple memorization, requiring players to mentally simulate how the ball will rebound off these bumpers and predict its subsequent trajectory. This challenge not only tests your ability to retain information but also demands the analytical skill to interpret and anticipate the ball's movements, making it a cognitive exercise that involves both memory and strategic reasoning.

## How to build

run `make` in the root directory of the project.
if it didn't work, try to run `mingw32-make` instead.

## Folder Structure

- `src/` : Contains all the source files.
- `include/` : Contains all the header files.
- `assets/` : Contains all the assets used in the game.
- `Makefile` : Makefile for the project (compile, clean, etc...).
- `archive/` : Contains old files (not used anymore) that represent the old version of the game.

## Files Description

- `main.c` : The main file of the game that contains the main loop.
- `logic.c` : Contains the game logic (matrix manipulation, ball movement, score, etc...).
- `render.c` : Contains the rendering functions (rendering the main menu, the game mode ...).
- `handlers.c` : Contains the event handling functions (keyboard, mouse, etc...).
- `draw.c` : Contains the drawing functions (draw Grid, draw main menu, etc...).
- `util.c` : Contains the utility functions (formatting, etc...).

#### each file has a corresponding header file in the `include/` folder except for `main.c`.

## Authors

-`Yahiaoui Abderrahmane` : [GitHub](https://github.com/Abdo30004)

-`Oubyi Mohamed Yacine` : [GitHub](https://github.com/YassWrld)

## License

[MIT](https://choosealicense.com/licenses/mit/)

# cTetris

## Overview

cTetris is a version of Tetris written in C using minimal graphics. The
third-party library SDL is used to replace the board's in-built graphics
routines.

This program was originally a class project for a low-level programming class.
The objective of the project was to write a game for the STM32 Discovery Board,
displaying output on the small 127x160 LCD screen, and controlling the game
using tilt controls as well as keyboard input over serial. The earliest commit
with code represents the project source unchanged from when it was submitted.
Note that the board drivers and makefile have been removed, as these were 
provided by the instructors and are not my code to release. This original
code is provided only for posterity and documentation, as it will not compile.

## Planned Changes
- Extend the window and board for a 10x20 board, as in the original game.
- Embed the font file within the executable, so the program won't require a
font file in the program directory

## Build and Run
The use of SDL2 should in theory make cTetris platform-independent, but it was
developed and tested on Linux.

cTetris requires SDL2 and SDL_ttf in order to build.

To build from source with gcc, enter the source directory and use the command 
`gcc -o cTetris control.c main.c graphics.c movement.c -lSDL2 -lSDL2_ttf`.

In its current state, cTetris requires that a font file called "font.ttf" be
placed in the program directory. A font file representing Liberation Mono is
included with the source files.

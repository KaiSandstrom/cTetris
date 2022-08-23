#include <stdio.h>
#include "include.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

SDL_Surface* surface;
SDL_Texture* texture;

void draw_string(uint16_t xpos, uint16_t ypos, char* string, SDL_Color color) {
	TTF_Font* sans = TTF_OpenFont("font.ttf", 36);
	xpos *= 4;
	ypos *= 4;
	surface = TTF_RenderText_Solid(sans, string, color);
	SDL_Rect dest_rect = (SDL_Rect){xpos, ypos, surface->w, surface->h};
	texture = SDL_CreateTextureFromSurface(renderer, surface);
  TTF_CloseFont(sans);
	SDL_RenderCopy(renderer, texture, NULL, &dest_rect);
  SDL_DestroyTexture(texture);
}

void draw_pixel(uint8_t x, uint8_t y, SDL_Color color) {
  int x_start = x*4;
  int y_start = y*4;
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  for (uint16_t i=x_start; i<x_start+4; i++) {
    for(uint16_t j=y_start; j<y_start+4; j++) {
      SDL_RenderDrawPoint(renderer, i, j);
    }
  }	
}

void draw_static_elements() {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);
  uint8_t i, j;
  for (i = 125; i < 127; i++) {
    for (j = 1; j < 159; j++) {
      draw_pixel(i, j, BLACK); // Draw right side border of board
    }
  }
  for (i = 33; i < 35; i++) {
    for (j = 1; j < 159; j++) {
      draw_pixel(i, j, BLACK); // Draw left side border of board
    }
  }
  for (i = 35; i < 125; i++) {
    for (j = 1; j < 4; j++) {
      draw_pixel(i, j, BLACK); // Draw top border of board
    }
  }
  for (i = 35; i < 125; i++) {
    for (j = 157; j < 159; j++) {
      draw_pixel(i, j, BLACK); // Draw bottom border of board
    }
  }
  draw_string(1, 10, "Score:", BLACK); // Actual score will be updated by another function
  draw_string(1, 50, "Next", BLACK);
  draw_string(1, 60, "piece:", BLACK); // Image of next piece will be placed by another function
}  

void update_score(void) { // Places score number on screen on the left side
  char score_string[5];   // Score is a global variable defined in common.h
  sprintf(score_string, "%d", score);
  for (uint8_t i = 0; i < 32; i++) {
    for (uint8_t j = 20; j < 30; j++) {
      draw_pixel(i, j, WHITE);
    }
  }
  draw_string(1, 20, score_string, BLACK); 
}

void instructions_screen(void) { // Places text of the instructions screen on the screen
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	draw_string(48, 10, "TETRIS", BLACK);
	draw_string(25, 30, "Programmed in C", BLACK);
	draw_string(40, 40, "using SDL", BLACK);
	draw_string(23, 50, "by Kai Sandstrom", BLACK);
	draw_string(2, 65, "Controls:", BLACK);
	draw_string(2, 75, "Use WASD: A and D", BLACK);
	draw_string(2, 85, "move piece left/right,", BLACK);
	draw_string(2, 95, "S drops piece,", BLACK);
	draw_string(2, 105, "W rotates falling", BLACK);
	draw_string(2, 115, "piece clockwise.", BLACK);
	draw_string(2, 125, "ESC pauses the game.", BLACK);
	draw_string(2, 135, "Use any control to", BLACK);
	draw_string(2, 145, "start the game.", BLACK);
  SDL_RenderPresent(renderer);
} 

void draw_cell(uint8_t x, uint8_t y, uint8_t color) { // Draws an individual cell from its
  uint8_t xStart = 35 + (9 * x);                   // x and y coordinates within the board.
  uint8_t xEnd = xStart + 9;
  uint8_t yStart = 4 + (9 * y);
  uint8_t yEnd = yStart + 9;
  SDL_Color inner, outer;
  uint8_t i, j;
  switch (color) {
    case 0: inner = RED; outer = DARK_RED; break;
    case 1: inner = GREEN; outer = DARK_GREEN; break;
    case 2: inner = BLUE; outer = DARK_BLUE; break;
    case 3: inner = CYAN; outer = DARK_CYAN; break;
    case 4: inner = MAGENTA; outer = DARK_MAGENTA; break;
    case 5: inner = YELLOW; outer = DARK_YELLOW; break;
    case 6: inner = LIGHT_ORANGE; outer = ORANGE; break;
    case 7: inner = WHITE; outer = WHITE; break;
    case 8: inner = BLACK; outer = BLACK; break;
  }
  for (i = xStart; i < xEnd; i++) {
    for (j = yStart; j < yEnd; j++) {
      if ((i > xStart+1) && (i < xEnd-2) && (j > yStart+1) && (j < yEnd-2)) {
        draw_pixel(i, j, inner);
      } else {
        draw_pixel(i, j, outer);
      }
    }
  }
}

void draw_cell_nextpiece(uint8_t x, uint8_t y, uint8_t color) { //Special function for drawing next piece
	                     // on the left side of the screen. Uses a special 3x4 cell grid for this purpose.
  uint8_t xStart = 2 + (9 * x);   // This is because draw_cell is only designed to draw within the board
  uint8_t xEnd = xStart + 9;
  uint8_t yStart = 70 + (9 * y);
  uint8_t yEnd = yStart + 9;
  SDL_Color inner, outer;
  signed short i, j;
  switch (color) {
    case 0: inner = RED; outer = DARK_RED; break;
    case 1: inner = GREEN; outer = DARK_GREEN; break;
    case 2: inner = BLUE; outer = DARK_BLUE; break;
    case 3: inner = CYAN; outer = DARK_CYAN; break;
    case 4: inner = MAGENTA; outer = DARK_MAGENTA; break;
    case 5: inner = YELLOW; outer = DARK_YELLOW; break;
    case 6: inner = LIGHT_ORANGE; outer = ORANGE; break;
  }
  for (i = xStart; i < xEnd; i++) {
    for (j = yStart; j < yEnd; j++) {
      if ((i > xStart+1) && (i < xEnd-2) && (j > yStart+1) && (j < yEnd-2)) {
        draw_pixel(i, j, inner);
      } else {
        draw_pixel(i, j, outer);
      }
    }
  }
}

void animate_full_rows(void) {     // Flickering animation when a row is filled and           
  for (uint8_t j = 0; j < 17; j++) {       // removed from the board
    if (full[j]) {
      for (uint8_t i = 0; i < 10; i++) {
        draw_cell(i, j, 8);
      }
    }
  }
  SDL_RenderPresent(renderer);
  SDL_Delay(100);
  for (uint8_t j = 0; j < 17; j++) {
    if (full[j]) {
      for (uint8_t i = 0; i < 10; i++) {
        draw_cell(i, j, 7);
      }
    }
  }
  SDL_RenderPresent(renderer);
  SDL_Delay(100);
}

void show_next(uint8_t next) { // Draws the upcoming piece to the left of the board
  signed short i, j;
  for (i = 2; i < 29; i++) {
    for (j = 70; j < 107; j++) {
      draw_pixel(i, j, WHITE);
    }
  }
  if (next == 0) {
    draw_cell_nextpiece(2, 0, 0);
    draw_cell_nextpiece(0, 1, 0);
    draw_cell_nextpiece(1, 1, 0);
    draw_cell_nextpiece(2, 1, 0);
  } else if (next == 1) {
    draw_cell_nextpiece(0, 0, 1);
    draw_cell_nextpiece(0, 1, 1);
    draw_cell_nextpiece(1, 1, 1);
    draw_cell_nextpiece(2, 1, 1);
  } else if (next == 2) {
    draw_cell_nextpiece(0, 0, 2);
    draw_cell_nextpiece(0, 1, 2);
    draw_cell_nextpiece(1, 0, 2);
    draw_cell_nextpiece(1, 1, 2);
  } else if (next == 3) {
    draw_cell_nextpiece(0, 0, 3);
    draw_cell_nextpiece(1, 0, 3);
    draw_cell_nextpiece(1, 1, 3);
    draw_cell_nextpiece(2, 1, 3);
  } else if (next == 4) {
    draw_cell_nextpiece(1, 0, 4);
    draw_cell_nextpiece(0, 1, 4);
    draw_cell_nextpiece(1, 1, 4);
    draw_cell_nextpiece(2, 1, 4);
  } else if (next == 5) {
    draw_cell_nextpiece(0, 1, 5);
    draw_cell_nextpiece(1, 1, 5);
    draw_cell_nextpiece(1, 0, 5);
    draw_cell_nextpiece(2, 0, 5);
  } else if (next == 6) {
    draw_cell_nextpiece(0, 0, 6); 
    draw_cell_nextpiece(0, 1, 6);
    draw_cell_nextpiece(0, 2, 6);
    draw_cell_nextpiece(0, 3, 6);
    draw_pixel(20, 82, BLACK);
    draw_pixel(21, 82, BLACK);
    draw_pixel(22, 82, BLACK);
    draw_pixel(23, 82, BLACK);
    draw_pixel(24, 82, BLACK);
    draw_pixel(25, 82, BLACK);
    draw_pixel(26, 82, BLACK);
    draw_pixel(20, 83, BLACK);
    draw_pixel(21, 83, BLACK);
    draw_pixel(22, 83, BLACK);
    draw_pixel(20, 84, BLACK);
    draw_pixel(23, 84, BLACK);
    draw_pixel(24, 84, BLACK);
    draw_pixel(20, 85, BLACK);
    draw_pixel(25, 85, BLACK);  // Circular arrow indicating that this
    draw_pixel(20, 86, BLACK);  // piece is shown rotated here. It can't
    draw_pixel(26, 86, BLACK);  // fit horizontally to the left of the
    draw_pixel(20, 87, BLACK);  // board.
    draw_pixel(27, 87, BLACK);
    draw_pixel(27, 88, BLACK);
    draw_pixel(28, 89, BLACK);
    draw_pixel(28, 90, BLACK);
    draw_pixel(28, 91, BLACK);
    draw_pixel(28, 92, BLACK);
    draw_pixel(28, 93, BLACK);
    draw_pixel(27, 94, BLACK);
    draw_pixel(27, 95, BLACK);
    draw_pixel(26, 96, BLACK); 
    draw_pixel(25, 97, BLACK);
    draw_pixel(24, 98, BLACK);
    draw_pixel(23, 98, BLACK);
    draw_pixel(22, 99, BLACK);
    draw_pixel(21, 99, BLACK);
    draw_pixel(20, 99, BLACK);
    draw_pixel(19, 99, BLACK);
    draw_pixel(18, 99, BLACK);
    draw_pixel(17, 98, BLACK);
    draw_pixel(16, 98, BLACK);
    draw_pixel(15, 97, BLACK);
    draw_pixel(14, 96, BLACK);
    draw_pixel(13, 95, BLACK);
    draw_pixel(13, 94, BLACK);
  }
  SDL_RenderPresent(renderer);
}

void game_over_screen(void) { // Draws a game over message on screen
  signed short i, j;
  for (j = 40; j < 120; j++) {
    for (i = 0; i < 128; i++) {
      draw_pixel(i, j, BLACK);
    }
  }
  draw_string(40, 60, "GAME OVER", WHITE);
  draw_string(10, 80, "Use any control to", WHITE);
  draw_string(35, 90, "play again.", WHITE);
  SDL_RenderPresent(renderer);
}

void draw_board(void) { // Draws every cell on the board for which update == 1
  signed short i, j;
  for (i = 0; i < 10; i++) {
    for (j = 0; j < 17; j++) {
      if (board[i][j].update == 1) {
        if (board[i][j].state != 0) {
          draw_cell(i, j, board[i][j].color);
        } else {
          draw_cell(i, j, 7);
        }
        board[i][j].update = 0;	
      }
    }
  }
  SDL_RenderPresent(renderer);
}

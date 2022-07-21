#include <stdio.h>
#include <graphics.h>
#include <f3d_lcd_sd.h>
#include <f3d_delay.h>
#include <common.h>

// This function was used for the testing of new colors. It fills half the screen
// with one color and half with the other.
/*
void fillHalf(uint16_t first, uint16_t second){
  uint8_t i, j;
  for (i = 0; i < 128; i++){
    for (j = 0; j < 80; j++){
      f3d_lcd_drawPixel(i, j, first);
      f3d_lcd_drawPixel(i, j+80, second);
    }
  }
}
*/

void draw_static_elements(void) {
  f3d_lcd_fillScreen(WHITE);
  signed short i, j;
  for (i = 125; i < 127; i++) {
    for (j = 1; j < 159; j++) {
      f3d_lcd_drawPixel(i, j, BLACK); // Draw right side border of board
    }
  }
  for (i = 33; i < 35; i++) {
    for (j = 1; j < 159; j++) {
      f3d_lcd_drawPixel(i, j, BLACK); // Draw left side border of board
    }
  }
  for (i = 35; i < 125; i++) {
    for (j = 1; j < 4; j++) {
      f3d_lcd_drawPixel(i, j, BLACK); // Draw top border of board
    }
  }
  for (i = 35; i < 125; i++) {
    for (j = 157; j < 159; j++) {
      f3d_lcd_drawPixel(i, j, BLACK); // Draw bottom border of board
    }
  }
  f3d_lcd_drawString(2, 10, "Score", BLACK, WHITE); // Actual score will be updated by another function
  f3d_lcd_drawString(2, 50, "Next", BLACK, WHITE);
  f3d_lcd_drawString(2, 60, "piece", BLACK, WHITE); // Image of next piece will be placed by another function
}

void update_score(void) { // Places score number on screen on the left side
  char score_string[5];   // Score is a global variable defined in common.h
  signed short i, j;
  sprintf(score_string, "%d", score);
  for (i = 2; i < 32; i++) {
    for (j = 20; j < 25; j++) {
      f3d_lcd_drawPixel(i, j, WHITE);
    }
  }
  f3d_lcd_drawString(2, 20, score_string, BLACK, WHITE); 
}

void instructions_screen(void) { // Places text of the instructions screen on the screen
  f3d_lcd_fillScreen(WHITE);
  f3d_lcd_drawString(49, 10, "TETRIS", BLACK, WHITE);
  f3d_lcd_drawString(10, 30, "Programmed for the", BLACK, WHITE);
  f3d_lcd_drawString(1, 40, "STM32 Discovery Board", BLACK, WHITE);
  f3d_lcd_drawString(17, 50, "by Kai Sandstrom", BLACK, WHITE);
  f3d_lcd_drawString(2, 65, "Controls:", BLACK, WHITE);
  f3d_lcd_drawString(2, 75, "Use WASD or tilt", BLACK, WHITE);
  f3d_lcd_drawString(2, 85, "controls to move", BLACK, WHITE);
  f3d_lcd_drawString(2, 95, "left, right, or down.", BLACK, WHITE);
  f3d_lcd_drawString(2, 105, "UP rotates falling", BLACK, WHITE);
  f3d_lcd_drawString(2, 115, "piece clockwise.", BLACK, WHITE);
  f3d_lcd_drawString(2, 125, "User button pauses.", BLACK, WHITE);
  f3d_lcd_drawString(2, 135, "Use any control to", BLACK, WHITE);
  f3d_lcd_drawString(2, 145, "start the game.", BLACK, WHITE);
} 

void draw_cell(uint8_t x, uint8_t y, uint8_t color) { // Draws an individual cell from its
  uint8_t xStart = 35 + (9 * x);                   // x and y coordinates within the board.
  uint8_t xEnd = xStart + 9;
  uint8_t yStart = 4 + (9 * y);
  uint8_t yEnd = yStart + 9;
  uint16_t inner, outer;
  signed short i, j;
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
        f3d_lcd_drawPixel(i, j, inner);
      } else {
        f3d_lcd_drawPixel(i, j, outer);
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
  uint16_t inner, outer;
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
        f3d_lcd_drawPixel(i, j, inner);
      } else {
        f3d_lcd_drawPixel(i, j, outer);
      }
    }
  }
}


void animate_full_rows(void) {     // Flickering animation when a row is filled and
  signed short i, j;               // removed from the board
  for (j = 0; j < 17; j++) {
    if (full[j]) {
      for (i = 0; i < 10; i++) {
        draw_cell(i, j, 8);
      }
    }
  }
  delay(100);
  for (j = 0; j < 17; j++) {
    if (full[j]) {
      for (i = 0; i < 10; i++) {
        draw_cell(i, j, 7);
      }
    }
  }
  delay(100);
}

void show_next(uint8_t next) { // Draws the upcoming piece to the left of the board
  signed short i, j;
  for (i = 2; i < 29; i++) {
    for (j = 70; j < 107; j++) {
      f3d_lcd_drawPixel(i, j, WHITE);
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
    f3d_lcd_drawPixel(20, 82, BLACK);
    f3d_lcd_drawPixel(21, 82, BLACK);
    f3d_lcd_drawPixel(22, 82, BLACK);
    f3d_lcd_drawPixel(23, 82, BLACK);
    f3d_lcd_drawPixel(24, 82, BLACK);
    f3d_lcd_drawPixel(25, 82, BLACK);
    f3d_lcd_drawPixel(26, 82, BLACK);
    f3d_lcd_drawPixel(20, 83, BLACK);
    f3d_lcd_drawPixel(21, 83, BLACK);
    f3d_lcd_drawPixel(22, 83, BLACK);
    f3d_lcd_drawPixel(20, 84, BLACK);
    f3d_lcd_drawPixel(23, 84, BLACK);
    f3d_lcd_drawPixel(24, 84, BLACK);
    f3d_lcd_drawPixel(20, 85, BLACK);
    f3d_lcd_drawPixel(25, 85, BLACK);  // Circular arrow indicating that this
    f3d_lcd_drawPixel(20, 86, BLACK);  // piece is shown rotated here. It can't
    f3d_lcd_drawPixel(26, 86, BLACK);  // fit horizontally to the left of the
    f3d_lcd_drawPixel(20, 87, BLACK);  // board.
    f3d_lcd_drawPixel(27, 87, BLACK);
    f3d_lcd_drawPixel(27, 88, BLACK);
    f3d_lcd_drawPixel(28, 89, BLACK);
    f3d_lcd_drawPixel(28, 90, BLACK);
    f3d_lcd_drawPixel(28, 91, BLACK);
    f3d_lcd_drawPixel(28, 92, BLACK);
    f3d_lcd_drawPixel(28, 93, BLACK);
    f3d_lcd_drawPixel(27, 94, BLACK);
    f3d_lcd_drawPixel(27, 95, BLACK);
    f3d_lcd_drawPixel(26, 96, BLACK); 
    f3d_lcd_drawPixel(25, 97, BLACK);
    f3d_lcd_drawPixel(24, 98, BLACK);
    f3d_lcd_drawPixel(23, 98, BLACK);
    f3d_lcd_drawPixel(22, 99, BLACK);
    f3d_lcd_drawPixel(21, 99, BLACK);
    f3d_lcd_drawPixel(20, 99, BLACK);
    f3d_lcd_drawPixel(19, 99, BLACK);
    f3d_lcd_drawPixel(18, 99, BLACK);
    f3d_lcd_drawPixel(17, 98, BLACK);
    f3d_lcd_drawPixel(16, 98, BLACK);
    f3d_lcd_drawPixel(15, 97, BLACK);
    f3d_lcd_drawPixel(14, 96, BLACK);
    f3d_lcd_drawPixel(13, 95, BLACK);
    f3d_lcd_drawPixel(13, 94, BLACK);
  }
}

void game_over_screen(void) { // Draws a game over message on screen
  signed short i, j;
  for (j = 40; j < 120; j++) {
    for (i = 0; i < 128; i++) {
      f3d_lcd_drawPixel(i, j, BLACK);
    }
  }
  f3d_lcd_drawString(40, 60, "GAME OVER", WHITE, BLACK);
  f3d_lcd_drawString(10, 80, "Use any control to", WHITE, BLACK);
  f3d_lcd_drawString(35, 90, "play again.", WHITE, BLACK);
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
}

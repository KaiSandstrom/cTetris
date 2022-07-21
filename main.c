// Kai Sandstrom
// TETRIS

#include <stm32f30x.h>
#include <f3d_uart.h>
#include <stdio.h>
#include <stdlib.h>
#include <f3d_gyro.h>
#include <f3d_user_btn.h>
#include <f3d_lcd_sd.h>
#include <math.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <graphics.h>
#include <common.h>
#include <movement.h>
#include <f3d_delay.h>
#include <control.h>
#include <f3d_systick.h>

uint8_t next_piece[9][7] = {{2, 0, 1, 4, 6, 3, 5},
                            {0, 3, 5, 6, 2, 1, 4},  // Randomly-generated using random.org
                            {1, 3, 2, 4, 5, 6, 0},  // Solution to STM32 board's lack of
                            {6, 3, 1, 5, 0, 4, 2},  // RNG capability. Will be offset by
                            {5, 4, 6, 0, 3, 2, 1},  // rows each time a new game is
                            {5, 0, 1, 2, 6, 4, 3},  // started.
                            {1, 6, 5, 3, 0, 2, 4},
                            {4, 2, 0, 5, 1, 6, 3},
                            {4, 3, 2, 5, 0, 1, 5}};

int main(void) {
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  f3d_uart_init(); // initialize uart
  delay(15);
  f3d_gyro_init(); //intialize gyro
  delay(15);
  f3d_i2c1_init(); //intialize i2c1 and accel and mag with delays in between
  delay(15);
  f3d_accel_init();
  delay(15);
  f3d_lcd_init(); //initialize lcd
  delay(15);
  f3d_user_btn_init(); //initialize button

  // The following is test code. Several pieces are hardcoded into existence for
  /*
  board[8][3].state = 2;  board[8][3].color = 0;
  board[6][4].state = 2;  board[6][4].color = 0;
  board[7][4].state = 2;  board[7][4].color = 0;
  board[8][4].state = 2;  board[8][4].color = 0;

  board[1][12].state = 1; board[1][12].color = 1;
  board[1][13].state = 1; board[1][13].color = 1;
  board[1][14].state = 1; board[1][14].color = 1;
  board[0][14].state = 1; board[0][14].color = 1;

  board[0][15].state = 1; board[0][15].color = 2;
  board[0][16].state = 1; board[0][16].color = 2;
  board[1][15].state = 1; board[1][15].color = 2;
  board[1][16].state = 1; board[1][16].color = 2;

  board[2][15].state = 1; board[2][15].color = 3;
  board[2][14].state = 1; board[2][14].color = 3;
  board[3][14].state = 1; board[3][14].color = 3;
  board[3][13].state = 1; board[3][13].color = 3;

  board[2][16].state = 1; board[2][16].color = 4;
  board[3][16].state = 1; board[3][16].color = 4;
  board[4][16].state = 1; board[4][16].color = 4;
  board[3][15].state = 1; board[3][15].color = 4;

  board[4][14].state = 1; board[4][14].color = 5;
  board[4][15].state = 1; board[4][15].color = 5;
  board[5][15].state = 1; board[5][15].color = 5;
  board[5][16].state = 1; board[5][16].color = 5;

  board[9][13].state = 1; board[9][13].color = 6;
  board[9][14].state = 1; board[9][14].color = 6;
  board[9][15].state = 1; board[9][15].color = 6;
  board[9][16].state = 1; board[9][16].color = 6;
  */
  uint8_t nprow = 0; // These govern which value is selected from the array
  uint8_t npcol = 0; // of random numbers 0-7 as the next piece
  uint8_t npoff = 0; // row offset for subsequent games after game over, to be less predictable
  uint8_t button_toggle = 0;
  uint8_t current_rotation = 0;
  signed short i, j;
  while (1) { // Runtime loop
    for (i = 0; i < 10; i++) {
      for (j = 0; j < 17; j++) {
        board[i][j].state = 0;
        board[i][j].color = 0;
        board[i][j].update = 1;
      }
    }
    score = 0; // new game, so score is 0
    npcol = 0; // always start at the first piece of the row in the table
    nprow = npoff; // this will be 0 in the first game, 4 in the next, etc
    uint8_t tick_count = 0;
    instructions_screen();
    while(!get_control());
    draw_static_elements();
    update_score();
    show_next(next_piece[nprow][npcol+1]);
    place_next(next_piece[nprow][npcol]);
    while (1) { // Current game loop
      if (user_btn_read()) {
        while (user_btn_read());
        button_toggle = (button_toggle + 1) % 2;
      }
      if (button_toggle) {
        continue;
      }	
      uint8_t control = get_control();
      uint8_t needs_new = 0;
      uint8_t game_over = 0; // is set to 1 if it's game over
      if (control) {  // Control handler
        if (control == 1) {
          move_left();
        } else if (control == 2) {
          move_right();
        } else if (control == 3) {
          push_down();
          needs_new = 1; // new piece must be placed on top
        } else if (control == 4) {
          if(rotate(current_rotation)) { // rotation is carried out in the condition
            current_rotation = (current_rotation + 1) % 4; // if it succeeded, update current rotation
          }
        }
      }
      if ((tick_count == 4) && !needs_new) {
        needs_new = step_down(); // step down every four ticks, unless a new piece is being placed on top
      }
      if (needs_new) {
        if (++npcol == 7) { // get next piece type indexes
          npcol = 0;
          nprow++;
        }
        if (nprow == 9) {
          nprow = 0;
        }
        uint16_t add_to_score = check_rows(); // check if a row was finished
        if (add_to_score) {
          score += add_to_score; // update score
          update_score();
          draw_board();
          animate_full_rows();
          clear_rows();  // remove full rows
        }
        game_over = place_next(next_piece[nprow][npcol]); // game over flag will be set to 1 if a collision occurred
        // while placing the next piece
        uint8_t temprow, tempcol; // Temporary values used to display next piece
        tempcol = npcol + 1;
        temprow = nprow;
        if (tempcol == 7) {
          tempcol = 0;
          temprow++;
        }
        if (temprow == 9) {
          temprow = 0;
        }
        current_rotation = 0;
        show_next(next_piece[temprow][tempcol]);
      }
      if (game_over) {
        break; // Break this while loop, go to the outer
      } 
      draw_board();
      tick_count = ++tick_count%5;
      delay(70);
    }
    game_over_screen();
    while(!get_control());
    npoff = (npoff + 2) % 9;
  } 
}  

void assert_failed(uint8_t* file, uint32_t line) {
  while (1) {puts("oops");}
}
// Kai Sandstrom

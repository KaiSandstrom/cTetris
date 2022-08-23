// Kai Sandstrom
// TETRIS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "include.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>

void get_random_sequence(uint8_t* sequence) {
  uint8_t count = 0;
  uint8_t not_yet_placed[7] = {1, 1, 1, 1, 1, 1, 1};
  while (count < 7) {
    uint8_t rand_piece = rand() % 7;
    if (not_yet_placed[rand_piece]) {
      not_yet_placed[rand_piece] = 0;
      sequence[count] = rand_piece;
      count++;
    }
  }
}

int main(void) {

  TTF_Init();
  SDL_Init(SDL_INIT_VIDEO); // Setup window
  SDL_CreateWindowAndRenderer(512, 640, 0, &win, &renderer);
  SDL_SetWindowTitle(win, "cTetris");

  uint8_t current_rotation = 0;
  signed short i, j;
  uint8_t control;
  while (1) { // Runtime loop
    for (i = 0; i < 10; i++) { // Initialize empty board
      for (j = 0; j < 17; j++) {
        board[i][j].state = 0;
        board[i][j].color = 0;
        board[i][j].update = 1;
      }
    }
    score = 0; // new game, so score is 0

    uint8_t tick_count = 0;
    instructions_screen();
    while(!get_control());
    draw_static_elements();
    update_score();
    srand(time(NULL));
    uint8_t next_pieces[7];
    get_random_sequence(next_pieces);
    show_next(next_pieces[1]);
    place_next(next_pieces[0]);
    uint8_t npoff = 1;
    int is_paused = 0;
    while (1) { // Current game loop
      control = get_control();
      uint8_t needs_new = 0;
      uint8_t game_over = 0; // is set to 1 if it's game over
      if (control) {  // Control handler
        if (control == 5) {
          is_paused = (is_paused+1)%2;
        }
        if (!is_paused) {
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
      }
      if ((tick_count == 5) && !needs_new) {
        needs_new = step_down(); // step down every six ticks, unless a new piece is being placed on top
      }
      if (needs_new) {
        uint16_t add_to_score = check_rows(); // check if a row was finished
        if (add_to_score) {
          score += add_to_score; // update score
          update_score();
          draw_board();
          animate_full_rows();
          clear_rows();  // remove full rows
        }
        game_over = place_next(next_pieces[npoff]); // game over flag will be set to 1 if a collision occurred
        // while placing the next piece
        if (npoff < 6) {
          npoff++;
        } else {
          get_random_sequence(next_pieces);
          npoff = 0;
        }
        show_next(next_pieces[npoff]);
        current_rotation = 0;
      }
      if (game_over || (control == 6)) {
        break; // Break this while loop, go to the outer
      } 
      draw_board();
      if (!is_paused) {
        tick_count = (tick_count+1)%6;
      }
      SDL_Delay(50);
    }
    if (control == 6) {
      break;
    }
    game_over_screen();
    while(!get_control());
  } 
  return 0;
}  

// Kai Sandstrom

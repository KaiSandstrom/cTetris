#include <stdint.h>
#include <common.h>
#include <movement.h>
#include <f3d_user_btn.h>
#include <f3d_uart.h>
#include <f3d_gyro.h>

void move_left(void) {
  signed short i, j;
  for (j = 16; j >= 0; j--) {
    for (i = 0; i < 10; i++) {
      if ((board[i][j].state == 2) && ((i == 0) || (board[i-1][j].state == 1))) {
        return; // Cannot move left because either the piece is at the left wall or
	        // is blocked by frozen pieces.
      }
    }
  }
  for (j = 16; j >= 0; j--) { // Loop is only reached if every cell has an open cell to the left
    for (i = 0; i < 10; i++) {
      if (board[i][j].state == 2) {
        board[i-1][j].state = 2; // Set new cell state to moving
        board[i][j].state = 0;   // Set old cell state to empty
        board[i-1][j].color = board[i][j].color; // Set new cell color to old cell color
	board[i-1][j].update = 1;
	board[i][j].update = 1; // set update to 1 to draw changes on next tick
      }
    }
  }   
}

void move_right(void) {
  signed short i, j;
  for (j = 16; j >= 0; j--) {
    for (i = 9; i >= 0; i--) {
      if ((board[i][j].state == 2) && ((i == 9) || (board[i+1][j].state == 1))) {
        return; // Cannot move right because either the piece is at the right wall or
                // is blocked by frozen pieces.
      }
    }
  }
  for (j = 16; j >= 0; j--) { // Loop is only reached if every cell has an open cell to the left
    for (i = 9; i >= 0; i--) {
      if (board[i][j].state == 2) {
        board[i+1][j].state = 2; // Set new cell state to moving
        board[i][j].state = 0;   // Set old cell state to empty
        board[i+1][j].color = board[i][j].color; // Set new cell color to old cell color
        board[i+1][j].update = 1;
        board[i][j].update = 1;	
      }
    }
  }
}

uint8_t step_down(void) { // Returns 0 if no collision occurred, 1 if collision occured
  uint8_t collision = 0;
  signed short i, j;
  for (j = 16; ((j >= 0) && !collision); j--) { // Break both loops if there's a collision
    for (i = 0; ((i < 10) && !collision); i++) {
      if ((board[i][j].state == 2) && ((j == 16) || (board[i][j+1].state == 1))) {
        collision = 1; // Collision detected
      }
    }
  }
  if (collision) {
    for (j = 16; j >= 0; j--) {
      for (i = 0; i < 10; i++) {
        if (board[i][j].state == 2) {
          board[i][j].state = 1; // Change state of all moving cells to stationary
	}
      }
    }
  } else { // Else case: No collision detected, piece moves down normally
    for (j = 16; j >= 0; j--) {
      for (i = 0; i < 10; i++) {  
        if (board[i][j].state == 2) {
	  board[i][j+1].state = 2; // Set new cell state to moving
	  board[i][j].state = 0; // Set new cell state to moving
	  board[i][j+1].color = board[i][j].color;
	  board[i][j+1].update = 1;
	  board[i][j].update = 1;
	}
      }
    }
  }
  return collision;
}

void push_down(void) {
  while(!step_down()) {}
}

// Places the next piece at the top of the board, returns 1 if there's a collision with another piece. This is game over.
uint8_t place_next(uint8_t type) { 
  if (type == 0) { // Right-facing L type piece
    if ((board[5][0].state == 1) || (board[5][1].state == 1) || (board[4][1].state == 1) || (board[3][1].state == 1)) {
      return 1;
    } else {
      board[5][0].state = 2; board[5][0].color = type; board[5][0].update = 1;
      board[5][1].state = 2; board[5][1].color = type; board[5][1].update = 1;
      board[4][1].state = 2; board[4][1].color = type; board[4][1].update = 1;
      board[3][1].state = 2; board[3][1].color = type; board[3][1].update = 1;
    }
  } else if (type == 1) { // Left-facing L shape 
    if ((board[3][0].state == 1) || (board[5][1].state == 1) || (board[4][1].state == 1) || (board[3][1].state == 1)) {
      return 1;
    } else {
      board[3][0].state = 2; board[3][0].color = type; board[3][0].update = 1;
      board[5][1].state = 2; board[5][1].color = type; board[5][1].update = 1;
      board[4][1].state = 2; board[4][1].color = type; board[4][1].update = 1;
      board[3][1].state = 2; board[3][1].color = type; board[3][1].update = 1;
    }
  } else if (type == 2) { // Square
    if ((board[4][0].state == 1) || (board[5][0].state == 1) || (board[4][1].state == 1) || (board[5][1].state == 1)) {
      return 1;
    } else {
      board[4][0].state = 2; board[4][0].color = type; board[4][0].update = 1;
      board[5][0].state = 2; board[5][0].color = type; board[5][0].update = 1;
      board[4][1].state = 2; board[4][1].color = type; board[4][1].update = 1;
      board[5][1].state = 2; board[5][1].color = type; board[5][1].update = 1;
    }
  } else if (type == 3) { // Z shape
    if ((board[3][0].state == 1) || (board[4][0].state == 1) || (board[4][1].state == 1) || (board[5][1].state == 1)) {
      return 1;
    } else {
      board[3][0].state = 2; board[3][0].color = type; board[3][0].update = 1;
      board[4][0].state = 2; board[4][0].color = type; board[4][0].update = 1;
      board[4][1].state = 2; board[4][1].color = type; board[4][1].update = 1;
      board[5][1].state = 2; board[5][1].color = type; board[5][1].update = 1;
    }
  } else if (type == 4) { // T shape
    if ((board[4][0].state == 1) || (board[5][1].state == 1) || (board[4][1].state == 1) || (board[3][1].state == 1)) {
      return 1;
    } else {
      board[4][0].state = 2; board[4][0].color = type; board[4][0].update = 1;
      board[5][1].state = 2; board[5][1].color = type; board[5][1].update = 1;
      board[4][1].state = 2; board[4][1].color = type; board[4][1].update = 1;
      board[3][1].state = 2; board[3][1].color = type; board[3][1].update = 1;
    }
  } else if (type == 5) { // S shape
    if ((board[4][0].state == 1) || (board[5][0].state == 1) || (board[4][1].state == 1) || (board[3][1].state == 1)) {
      return 1;
    } else {
      board[5][0].state = 2; board[5][0].color = type; board[5][0].update = 1;
      board[4][0].state = 2; board[4][0].color = type; board[4][0].update = 1;
      board[4][1].state = 2; board[4][1].color = type; board[4][1].update = 1;
      board[3][1].state = 2; board[3][1].color = type; board[3][1].update = 1;
    }
  } else if (type == 6) { // Stick
    if ((board[3][0].state == 1) || (board[4][0].state == 1) || (board[5][0].state == 1) || (board[6][0].state == 1)) {
      return 1;
    } else {
      board[3][0].state = 2; board[3][0].color = type; board[3][0].update = 1;
      board[4][0].state = 2; board[4][0].color = type; board[4][0].update = 1;
      board[5][0].state = 2; board[5][0].color = type; board[5][0].update = 1;
      board[6][0].state = 2; board[6][0].color = type; board[6][0].update = 1;
    }
  }
  return 0;
}
  
uint8_t rotate(uint8_t rot_state) { // Returns 1 for success, 0 for failure
  signed short i, j; 
  uint8_t breakflag = 0;
  uint8_t type;
  for (j = 0; ((j < 17) && !breakflag); j++) {   // This loop finds the "first" cell in motion, which is the left-most
    for (i = 0; ((i < 10) && !breakflag); i++) { // cell of the top row of the shape. After breaking, variables i and j
      if (board[i][j].state == 2) {              // remain set to this index in board.
	type = board[i][j].color; // color is unique to each piece type so it works to code for both
        breakflag = 1;
      }
    }
  }
  i--; 
  j--; // i and j have been incremented, so set them back to where they were when breakflag was set.
  //putchar(48+type);
  //putchar(48+rot_state);
  if ((type == 0) && (rot_state == 0)) { // If piece type is 1 and it is in first rotation position
    if ((j >= 15) || (board[i-1][j].state == 1) || (board[i-1][j+2].state == 1) || (board[i][j+2].state == 1)) {
      return 0; // Check to make sure the rotation will not cause a collision. If it does, return 0 for failure. 
                // The rotation state variable will not increment in this case, and the piece does not rotate.
    } else {
      board[i-1][j].state = 2;    // All of these rotation cases follow the same basic format:
      board[i-1][j].color = 0;    // First, update the state, color, and update flag of each cell
      board[i-1][j].update = 1;   // that needs to be occupied by the rotated piece that isn't
      board[i-1][j+2].state = 2;  // currently occupied.
      board[i-1][j+2].color = 0;
      board[i-1][j+2].update = 1;
      board[i][j+2].state = 2;
      board[i][j+2].color = 0;
      board[i][j+2].update = 1;
      board[i][j].state = 0;      // Then, set state to 0 for any cells that were part of the
      board[i][j].update = 1;     // piece before rotation but aren't after, and set update flag
      board[i][j+1].state = 0;
      board[i][j+1].update = 1;
      board[i-2][j+1].state = 0;
      board[i-2][j+1].update = 1;
      return 1;                   // Return 1 for successful rotation so that the rotation state variable increments
    }
  } else if ((type == 0) && (rot_state == 1)) {
    if ((i == 0) || (board[i-1][j+1].state == 1) || (board[i+1][j+1].state == 1) || (board[i-1][j+2].state == 1)) {
      return 0;
    } else {
      board[i-1][j+1].state = 2;
      board[i-1][j+1].color = 0;
      board[i-1][j+1].update = 1;
      board[i+1][j+1].state = 2;
      board[i+1][j+1].color = 0;
      board[i+1][j+1].update = 1;
      board[i-1][j+2].state = 2;
      board[i-1][j+2].color = 0;
      board[i-1][j+2].update = 1;
      board[i][j].state = 0;
      board[i][j].update = 1;
      board[i][j+2].state = 0;
      board[i][j+2].update = 1;
      board[i+1][j+2].state = 0;
      board[i+1][j+2].update = 1;
      return 1;
    }
  } else if ((type == 0) && (rot_state == 2)) {
    if ((j == 0) || (board[i][j-1].state == 1) || (board[i+1][j-1].state == 1) || (board[i+1][j+1].state == 1)) { 
      return 0;
    } else {
      board[i][j-1].state = 2;
      board[i][j-1].color = 0;
      board[i][j-1].update = 1;
      board[i+1][j-1].state = 2;
      board[i+1][j-1].color = 0;
      board[i+1][j-1].update = 1;
      board[i+1][j+1].state = 2;
      board[i+1][j+1].color = 0;
      board[i+1][j+1].update = 1;
      board[i][j].state = 0;
      board[i][j].update = 1;
      board[i][j+1].state = 0;
      board[i][j+1].update = 1;
      board[i+2][j].state = 0;
      board[i+2][j].update = 1;
      return 1;
    }
  } else if ((type == 0) && (rot_state == 3)) {
    if ((i >= 8) || (board[i+2][j].state == 1) || (board[i+2][j+1].state == 1) || (board[i][j+1].state == 1)) {
      return 0;
    } else {
      board[i+2][j].state = 2;
      board[i+2][j].color = 0;
      board[i+2][j].update = 1;
      board[i+2][j+1].state = 2;
      board[i+2][j+1].color = 0;
      board[i+2][j+1].update = 1;
      board[i][j+1].state = 2;
      board[i][j+1].color = 0;
      board[i][j+1].update = 1;
      board[i][j].state = 0;
      board[i][j].update = 1;
      board[i+1][j].state = 0;
      board[i+1][j].update = 1;
      board[i+1][j+2].state = 0;
      board[i+1][j+2].update = 1;
      return 1;
    }
  } else if ((type == 1) && (rot_state == 0)) {
    if ((j >= 15) || (board[i+1][j].state == 1) || (board[i+2][j].state == 1) || (board[i+1][j+2].state == 1)) {
      return 0;
    } else {
      board[i+2][j].state = 2;
      board[i+2][j].color = 1;
      board[i+2][j].update = 1;
      board[i+1][j].state = 2;
      board[i+1][j].color = 1;
      board[i+1][j].update = 1;
      board[i+1][j+2].state = 2;
      board[i+1][j+2].color = 1;
      board[i+1][j+2].update = 1;
      board[i][j].state = 0;
      board[i][j].update = 1;
      board[i][j+1].state = 0;
      board[i][j+1].update = 1;
      board[i+2][j+1].state = 0;
      board[i+2][j+1].update = 1;
      return 1;
    }
  } else if ((type == 1) && (rot_state == 1)) {
    if ((i == 0) || (board[i-1][j+1].state == 1) || (board[i+1][j+1].state == 1) || (board[i+1][j+2].state == 1)) {
      return 0;
    } else {
      board[i-1][j+1].state = 2;
      board[i-1][j+1].color = 1;
      board[i-1][j+1].update = 1;
      board[i+1][j+1].state = 2;
      board[i+1][j+1].color = 1;
      board[i+1][j+1].update = 1;
      board[i+1][j+2].state = 2;
      board[i+1][j+2].color = 1;
      board[i+1][j+2].update = 1;
      board[i][j].state = 0;
      board[i][j].update = 1;
      board[i+1][j].state = 0;
      board[i+1][j].update = 1;
      board[i][j+2].state = 0;
      board[i][j+2].update = 1;
      return 1;
    }
  } else if ((type == 1) && (rot_state == 2)) {
    if ((j == 0) || (board[i-1][j+1].state == 1) || (board[i][j+1].state == 1) || (board[i+1][j+1].state == 1)) {
      return 0;
    } else {
      board[i+1][j-1].state = 2;
      board[i+1][j-1].color = 1;
      board[i+1][j-1].update = 1;
      board[i][j+1].state = 2;
      board[i][j+1].color = 1;
      board[i][j+1].update = 1;
      board[i+1][j+1].state = 2;
      board[i+1][j+1].color = 1;
      board[i+1][j+1].update = 1;
      board[i][j].state = 0;
      board[i][j].update = 1;
      board[i+2][j].state = 0;
      board[i+2][j].update = 1;
      board[i+2][j+1].state = 0;
      board[i+2][j+1].update = 1;
      return 1;
    }
  } else if ((type == 1) && (rot_state == 3)) {
    if ((i >= 8) || (board[i-1][j].state == 1) || (board[i-1][j+1].state == 1) || (board[i+1][j+1].state == 1)) {
      return 0;
    } else {
      board[i-1][j].state = 2;
      board[i-1][j].color = 1;
      board[i-1][j].update = 1;
      board[i-1][j+1].state = 2;
      board[i-1][j+1].color = 1;
      board[i-1][j+1].update = 1;
      board[i+1][j+1].state = 2;
      board[i+1][j+1].color = 1;
      board[i+1][j+1].update = 1;
      board[i][j].state = 0;
      board[i][j].update = 1;
      board[i][j+2].state = 0;
      board[i][j+2].update = 1;
      board[i-1][j+2].state = 0;
      board[i-1][j+2].update = 1;
      return 1;
    }
  } else if ((type == 3) && ((rot_state == 0 || rot_state == 2))) {// Skipping type 2 because that's a square. Squares don't have rotation
    if ((j >= 15) || (board[i+2][j].state == 1) || (board[i+1][j+2].state == 1)) { 
      return 0;
    } else {
      board[i+2][j].state = 2;
      board[i+2][j].color = 3;
      board[i+2][j].update = 1;
      board[i+1][j+2].state = 2;
      board[i+1][j+2].color = 3;
      board[i+1][j+2].update = 1;
      board[i][j].state = 0;
      board[i][j].update = 1;
      board[i+1][j].state = 0;
      board[i+1][j].update = 1;
      return 1;
    }
  } else if ((type == 3) && ((rot_state == 1) || (rot_state == 3))) { // S and Z type pieces (type 3 and 5) only have two states due to symmetry
    if ((i==0) || (board[i-2][j].state == 1) || (board[i-1][j].state == 1)) {
      return 0;
    } else {
      board[i-2][j].state = 2;
      board[i-2][j].color = 3;
      board[i-2][j].update = 1;
      board[i-1][j].state = 2;
      board[i-1][j].color = 3;
      board[i-1][j].update = 1;
      board[i][j].state = 0;
      board[i][j].update = 1;
      board[i-1][j+2].state = 0; 
      board[i-1][j+2].update = 1;
      return 1;
    }
  } else if ((type == 4) && (rot_state == 0)) {
    if ((j >= 15) || (board[i][j+2].state == 1)) {
      return 0;
    } else {
      board[i][j+2].state = 2;
      board[i][j+2].color = 4;
      board[i][j+2].update = 1;
      board[i-1][j+1].state = 0;
      board[i-1][j+1].update = 1;
      return 1;
    }
  } else if ((type == 4) && (rot_state == 1)) { 
    if ((i == 0) || (board[i-1][j+1].state == 1)) {
      return 0;
    } else {
      board[i-1][j+1].state = 2;
      board[i-1][j+1].color = 4;
      board[i-1][j+1].update = 1;
      board[i][j].state = 0;
      board[i][j].update = 1;
      return 1;
    }
  } else if ((type == 4) && (rot_state == 2)) { 
    if ((j == 0) || (board[i+1][j-1].state == 1)) {
      return 0;
    } else {
      board[i+1][j-1].state = 2;
      board[i+1][j-1].color = 4;
      board[i+1][j-1].update = 1;
      board[i+2][j].state = 0;
      board[i+2][j].update = 1;
      return 1;
    }
  } else if ((type == 4) && (rot_state == 3)) {
    if ((i >= 8) || (board[i+1][j+1].state == 1)) {
      return 0;
    } else {
      board[i+1][j+1].state = 2;
      board[i+1][j+1].color = 4;
      board[i+1][j+1].update = 1;
      board[i][j+2].state = 0;
      board[i][j+2].update = 1;
      return 1;
    }
  } else if ((type == 5) && ((rot_state == 0) || (rot_state == 2))) {
    if ((i >= 15) || (board[i-1][j].state == 1) || (board[i][j+2].state == 1)) {
      return 0;
    } else {
      board[i-1][j].state = 2;
      board[i-1][j].color = 5;
      board[i-1][j].update = 1;
      board[i][j+2].state = 2;
      board[i][j+2].color = 5;
      board[i][j+2].update = 1;
      board[i][j].state = 0;
      board[i][j].update = 1;
      board[i+1][j].state = 0;
      board[i+1][j].update = 1;
      return 1;      
    }
  } else if ((type == 5) && ((rot_state == 1) || (rot_state == 3))) {
    if ((i >=8) || (board[i+1][j].state == 1) || (board[i+2][j].state == 1)) {
      return 0;
    } else {
      board[i+1][j].state = 2;
      board[i+1][j].color = 5;
      board[i+1][j].update = 1;
      board[i+2][j].state = 2;
      board[i+2][j].color = 5;
      board[i+2][j].update = 1;
      board[i][j].state = 0;
      board[i][j].update = 1;
      board[i+1][j+2].state = 0;
      board[i+1][j+2].update = 1;
      return 1;
    }
  } else if ((type == 6) && ((rot_state == 0) || (rot_state == 2))) { // Like 3 and 5, the stick piece has only 2 states
    if ((j <= 1) || (j == 16) || (board[i+1][j-2].state == 1) || (board[i+1][j-1].state == 1) || (board[i+1][j+1].state == 1)) {
      return 0;
    } else {
      board[i+1][j-2].state = 2;
      board[i+1][j-2].color = 6;
      board[i+1][j-2].update = 1;
      board[i+1][j-1].state = 2;
      board[i+1][j-1].color = 6;
      board[i+1][j-1].update = 1;
      board[i+1][j+1].state = 2;
      board[i+1][j+1].color = 6;
      board[i+1][j+1].update = 1;
      board[i][j].state = 0;
      board[i][j].update = 1;
      board[i+2][j].state = 0;
      board[i+2][j].update = 1;
      board[i+3][j].state = 0;
      board[i+3][j].update = 1;
      return 1;
    }
  } else if ((type == 6) && ((rot_state == 1) || (rot_state == 3))) {
    if ((i == 0) || (i >= 8) || (board[i-1][j+2].state == 1) || (board[i+1][j+2].state == 1) || (board[i+2][j+2].state == 1)) {
      return 0;
    } else {
      board[i-1][j+2].state = 2;
      board[i-1][j+2].color = 6;
      board[i-1][j+2].update = 1;
      board[i+1][j+2].state = 2;
      board[i+1][j+2].color = 6;
      board[i+1][j+2].update = 1;
      board[i+2][j+2].state = 2;
      board[i+2][j+2].color = 6;
      board[i+2][j+2].update = 1;
      board[i][j].state = 0;
      board[i][j].update = 1;
      board[i][j+1].state = 0;
      board[i][j+1].update = 1;
      board[i][j+3].state = 0;
      board[i][j+3].update = 1;
      return 1;
    }
  }
} 

uint16_t check_rows(void) {   // Checks if any rows of the board are full. If a row is full, the 
  signed short i, j;          // flag in its respective index of the global array full[] is set to 1.
  uint8_t num_full = 0;       // The function keeps track of how many rows are full, and returns the 
  for (j = 16; j >= 0; j--) { // amount of score to be added to the total.
    for (i = 0; i < 10; i++) {
      if (board[i][j].state != 1) {
	full[j] = 0;
        break;
      }
      if (i == 9) {
        full[j] = 1;
	num_full++;
      }
    }
  }
  uint16_t extra_score;
  switch (num_full) {
    case 0: extra_score = 0; break;     // From the internet: more score earned
    case 1: extra_score = 40; break;    //   if more rows cleared at once
    case 2: extra_score = 100; break;
    case 3: extra_score = 300; break;
    default: extra_score = 1200; break;
  }
  return extra_score;
}

void clear_rows() {  // This function will remove full rows from the board and shift everything
  signed short i, j; // above them down.
  uint8_t current_row;
  for (current_row = 0; current_row < 17; current_row++) {
    if (full[current_row]) {
      for (i = 0; i < 10; i++) {
        board[i][current_row].state = 0;
	board[i][current_row].update = 1;
      }
      for (j = current_row-1; j >= 0; j--) {
        for (i = 0; i < 10; i++) {
	  if (board[i][j].state == 1) {
            board[i][j+1].state = 1;
	    board[i][j].state = 0;
	    board[i][j+1].color = board[i][j].color;
	    board[i][j+1].update = 1;
	    board[i][j].update = 1;
	  }
	}
      }
      full[current_row] = 0;
    }
  }
}	
  





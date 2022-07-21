#include <stdint.h>

typedef struct {
  uint8_t state;  // 0 = empty, 1 = filled, 2 = in motion
  uint8_t color;  // 0 = red, 1 = green, 2 = blue, 3 = cyan, 4 = magenta, 5 = yellow, 6 = orange
  uint8_t update; // 0 = no update, 1 = update
} cell;

cell board[10][17];
uint8_t full[17]; // Keeps track of which rows were marked full
uint16_t score;


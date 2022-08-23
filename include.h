#include <stdint.h>
#include <SDL2/SDL.h>

#define BLACK        (SDL_Color) {0, 0, 0, 255}
#define BLUE         (SDL_Color) {0, 0, 255, 255}
#define CYAN         (SDL_Color) {0, 255, 255, 255}
#define GREEN        (SDL_Color) {0, 255, 0, 255}
#define MAGENTA      (SDL_Color) {255, 0, 255, 255}
#define RED          (SDL_Color) {255, 0, 0, 255}
#define WHITE        (SDL_Color) {255, 255, 255, 255}
#define YELLOW       (SDL_Color) {255, 255, 0, 255}
#define DARK_YELLOW  (SDL_Color) {185, 185, 0, 255}
#define ORANGE       (SDL_Color) {255, 85, 0, 255}
#define LIGHT_ORANGE (SDL_Color) {255, 153, 51, 255}
#define DARK_GREEN   (SDL_Color) {0, 127, 0, 255}
#define DARK_MAGENTA (SDL_Color) {127, 0, 255, 255}
#define DARK_RED     (SDL_Color) {127, 0, 0, 255}
#define DARK_BLUE    (SDL_Color) {0, 0, 127, 255}
#define DARK_CYAN    (SDL_Color) {0, 127, 127, 255}

typedef struct {
  uint8_t state;  // 0 = empty, 1 = filled, 2 = in motion
  uint8_t color;  // 0 = red, 1 = green, 2 = blue, 3 = cyan, 4 = magenta, 5 = yellow, 6 = orange
  uint8_t update; // 0 = no update, 1 = update
} cell;

//  In this case, a "row" of this 2D array represents a column of the game
//    board. The first index is an X position and the second is a Y position.
cell board[10][17];
uint8_t full[17]; // Keeps track of which rows were marked full
uint16_t score;
SDL_Renderer *renderer;
SDL_Window *win;
SDL_Event event;

void move_left(void);
void move_right(void);
uint8_t step_down(void);
void push_down(void);
uint8_t place_next(uint8_t);
uint8_t rotate(uint8_t);
uint16_t check_rows(void);
void clear_rows(void);

uint8_t get_control(void);

void instructions_screen(void);
void draw_static_elements(void);
void show_next(uint8_t);
void draw_board(void);
void update_score(void);
void animate_full_rows(void);
void game_over_screen(void);




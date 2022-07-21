#define DARK_BLUE 0xD000
#define DARK_CYAN 0xCE60
#define YELLOW 0x07FF
#define DARK_YELLOW 0x067A
#define ORANGE 0x041F
#define LIGHT_ORANGE 0x6DBF
#define DARK_GREEN 0x0660
#define DARK_MAGENTA 0xD01A
#define DARK_RED 0x001A

void instructions_screen(void);
void draw_static_elements(void);
void draw_cell(uint8_t x, uint8_t y, uint8_t color);
void draw_cell_nextpiece(uint8_t x, uint8_t y, uint8_t color);
void show_next(uint8_t next);
void draw_board(void);
void update_score(void);
void animate_full_rows(void);
void game_over_screen(void);

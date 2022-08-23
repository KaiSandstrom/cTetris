#include <stdint.h>
#include "include.h"
#include <math.h>
#include <stdio.h>
#include <SDL2/SDL.h>

uint8_t tick_delay = 0;

uint8_t get_control() {
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      SDL_DestroyRenderer(renderer);
      SDL_DestroyWindow(win);
      SDL_Quit();
      return 6;
    }
	}
  if (tick_delay != 0) {
    tick_delay--;
    return 0;
  }
  const uint8_t* key_states = SDL_GetKeyboardState(NULL);
  uint8_t num_pressed = 0;
  uint8_t control = 0;
  tick_delay = 0;
  if (key_states[SDL_SCANCODE_A]) { 
    control = 1; // corresponds to moving left
    num_pressed++;
  } if (key_states[SDL_SCANCODE_D]) {
    control = 2; // corresponds to moving right
    num_pressed++;
  } if (key_states[SDL_SCANCODE_S]) {
    tick_delay = 3;
    control = 3; // corresponds to pushing down
    num_pressed++;
  } if (key_states[SDL_SCANCODE_W]) {
    tick_delay = 3;
    control = 4; // correstponds to rotating
    num_pressed++;
  } if (key_states[SDL_SCANCODE_ESCAPE]) {
    tick_delay = 6;
    control = 5; // pauses the game
    num_pressed++;
  } if (num_pressed == 0 || num_pressed > 1) {
    control = 0;
  }
  return control;
}

#include <stdint.h>
#include <control.h>
#include <f3d_uart.h>
#include <math.h>
#include <f3d_accel.h>
#include <f3d_lcd_sd.h>
#include <stdio.h>

float acc[3];

uint8_t get_control() {
  int key = getchar();
  float roll = get_roll();
  float pitch = get_pitch();
  //printf("roll: %f\npitch: %f\n", roll, pitch);
  uint8_t control = 0;
  if (((char) key == 'a') || (roll > 0.8)) { 
    control = 1; // corresponds to moving left
  } else if (((char) key == 'd') || (roll < -0.8)) {
    control = 2; // corresponds to moving right
  } else if (((char) key == 's') || (pitch < -0.8)) {
    control = 3; // corresponds to pushing down
  } else if (((char) key == 'w') || (pitch > 0.8)) {
    control = 4; // correstponds to rotating
  } else {
    control = 0; // no control active
  }
  return control;
}

float get_roll(void) { // From a previous lab, partner Grant Dowling
  f3d_accel_read(acc);
  return atan2(acc[1],(sqrt(pow(acc[0],2)+pow(acc[2],2))));
}

float get_pitch(void) { // From a previous lab, partner Grant Dowling
  f3d_accel_read(acc);
  return atan2(acc[0],(sqrt(pow(acc[1],2)+pow(acc[2],2)))); 
}

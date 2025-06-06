#include <stdbool.h>

#define PS2_BASE 0xFF200100
#define HEX5_HEX4_BASE 0xFF200030
#define PIXEL_BUF_CTRL_BASE 0xFF203020

#define BOX_SIZE 24
#define CIRCLE_RADIUS 12
#define DRAW_COLOR 0xFFFF  // white
#define BG_COLOR 0x0000    // black

#define WIDTH 320
#define HEIGHT 240

void keyboard();            // takes input from PS/2 port
void handle_key(char key);  // handles key presses
void init_vga();            // initializes the VGA display
void capture();             // capture the image into an array

/* VGA display subroutines */
void plot_pixel(int x, int y, short int line_color);
void clear_screen();
void draw_box(int x, int y, short int colour);
void draw_circle(int x_center, int y_center, short int colour);

/* Global Variables */
int pixel_buffer_start;
int x_cur = 160, y_cur = 30;   // cursor position
int x_draw = -1, y_draw = -1;  // drawing position (erasing position)
bool erase = false;            // erase flag
int image[HEIGHT][WIDTH];      // captured image array

int main() {
  init_vga();
  keyboard();
}

void keyboard() {
  volatile int *PS2_ptr = (int *)PS2_BASE;
  int PS2_data, RVALID;
  char byte3 = 0;     // last byte read
  *(PS2_ptr) = 0xFF;  // reset

  while (1) {
    PS2_data = *(PS2_ptr);       // read the Data register in the PS/2 port
    RVALID = PS2_data & 0x8000;  // extract the RVALID field
    if (RVALID) {
      byte3 = PS2_data & 0xFF;
      handle_key(byte3);
    }
  }
}

void handle_key(char key) {
  volatile unsigned int *hex = (volatile unsigned int *)HEX5_HEX4_BASE;

  volatile int *pixel_ctrl_ptr = (int *)PIXEL_BUF_CTRL_BASE;
  pixel_buffer_start = *pixel_ctrl_ptr;

  switch (key) {
    case 0x75:                               // up arrow
      *hex = 0b0111110;                      // U
      if (y_cur - CIRCLE_RADIUS * 2 >= 0) {  // check in bounds
        short int pixel =
            *(short int *)(pixel_buffer_start + (y_cur << 10) + (x_cur << 1));
        if (pixel == BG_COLOR) {  // only erase cursor if on background
          draw_circle(x_cur, y_cur, BG_COLOR);  // erase cursor
        }
        y_cur -= CIRCLE_RADIUS * 2 / 4;
        draw_circle(x_cur, y_cur, DRAW_COLOR);  // draw cursor
      }

      if (x_draw != -1 && y_draw != -1 && erase) {  // redundant condition?
        draw_circle(x_draw, y_draw, BG_COLOR);      // erase prev
        erase = false;
      }
      break;
    case 0x72:                                   // down arrow
      *hex = 0b1011110;                          // d
      if (y_cur + CIRCLE_RADIUS * 2 < HEIGHT) {  // check in bounds
        short int pixel =
            *(short int *)(pixel_buffer_start + (y_cur << 10) + (x_cur << 1));
        if (pixel == BG_COLOR) {  // only erase cursor if on background
          draw_circle(x_cur, y_cur, BG_COLOR);  // erase cursor
        }
        y_cur += CIRCLE_RADIUS * 2 / 4;
        draw_circle(x_cur, y_cur, DRAW_COLOR);  // draw cursor
      }

      if (x_draw != -1 && y_draw != -1 && erase) {
        draw_circle(x_draw, y_draw, BG_COLOR);  // erase prev
        erase = false;
      }
      break;
    case 0x6B:                               // left arrow
      *hex = 0b0111000;                      // L
      if (x_cur - CIRCLE_RADIUS * 2 >= 0) {  // check in bounds
        short int pixel =
            *(short int *)(pixel_buffer_start + (y_cur << 10) + (x_cur << 1));
        if (pixel == BG_COLOR) {  // only erase cursor if on background
          draw_circle(x_cur, y_cur, BG_COLOR);  // erase cursor
        }
        x_cur -= CIRCLE_RADIUS * 2 / 4;
        draw_circle(x_cur, y_cur, DRAW_COLOR);  // draw cursor
      }

      if (x_draw != -1 && y_draw != -1 && erase) {
        draw_circle(x_draw, y_draw, BG_COLOR);  // erase prev
        erase = false;
      }
      break;
    case 0x74:                                  // right arrow
      *hex = 0b0110001;                         // r
      if (x_cur + CIRCLE_RADIUS * 2 < WIDTH) {  // check in bounds
        short int pixel =
            *(short int *)(pixel_buffer_start + (y_cur << 10) + (x_cur << 1));
        if (pixel == BG_COLOR) {  // only erase cursor if on background
          draw_circle(x_cur, y_cur, BG_COLOR);  // erase cursor
        }
        x_cur += CIRCLE_RADIUS * 2 / 4;
        draw_circle(x_cur, y_cur, DRAW_COLOR);  // draw cursor
      }

      if (x_draw != -1 && y_draw != -1 && erase) {
        draw_circle(x_draw, y_draw, BG_COLOR);  // erase prev
        erase = false;
      }
      break;
    case 0x1C:           // A key - erase
      *hex = 0b1110111;  // A
      x_draw = x_cur;  // save cursor position to draw upon next location change
      y_draw = y_cur;
      erase = true;
      break;
    case 0x5A:           // enter key - submit
      *hex = 0b1111001;  // E

      // call lenet.c main which calls capture() for the input_image
      capture();

      break;
    case 0x66:           // backspace key - clear/reset
      *hex = 0b0111001;  // C
      init_vga();
      break;
    default:
      *hex = 0;
      break;
  }
}

void init_vga() {
  volatile int *pixel_ctrl_ptr = (int *)PIXEL_BUF_CTRL_BASE;
  pixel_buffer_start = *pixel_ctrl_ptr;

  clear_screen();
  draw_circle(x_cur, y_cur, DRAW_COLOR);  // draw cursor
}

void capture() {
  volatile int *pixel_ctrl_ptr = (int *)PIXEL_BUF_CTRL_BASE;
  pixel_buffer_start = *pixel_ctrl_ptr;

  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      short int pixel =
          *(short int *)(pixel_buffer_start + (y << 10) + (x << 1));
      if (pixel != BG_COLOR) {
        image[y][x] = 255;
      } else {
        image[y][x] = 0;
      }
    }
  }
  return image;
}

void plot_pixel(int x, int y, short int line_color) {
  volatile short int *one_pixel_address;

  one_pixel_address = pixel_buffer_start + (y << 10) + (x << 1);

  *one_pixel_address = line_color;
}

void clear_screen() {
  for (int x = 0; x < 320; x++) {
    for (int y = 0; y < 240; y++) {
      plot_pixel(x, y, BG_COLOR);  // draw black pixels
    }
  }
}

void draw_box(int x, int y, short int colour) {
  for (int i = x; i < x + BOX_SIZE; i++) {
    for (int j = y; j < y + BOX_SIZE; j++) {
      plot_pixel(i, j, colour);
    }
  }
}

void draw_circle(int x_center, int y_center, short int colour) {
  int x = CIRCLE_RADIUS;
  int y = 0;
  int decision_over_2 =
      1 - x;  // decision criterion divided by 2 evaluated at x=r, y=0

  while (y <= x) {
    // draw horizontal lines between the points on the circle's circumference
    for (int i = x_center - x; i <= x_center + x; i++) {
      plot_pixel(i, y_center + y, colour);  // Octant 1 and 4
      plot_pixel(i, y_center - y, colour);  // Octant 5 and 8
    }
    for (int i = x_center - y; i <= x_center + y; i++) {
      plot_pixel(i, y_center + x, colour);  // Octant 2 and 3
      plot_pixel(i, y_center - x, colour);  // Octant 6 and 7
    }

    y++;
    if (decision_over_2 <= 0) {
      decision_over_2 +=
          2 * y + 1;  // change in decision criterion for y -> y+1
    } else {
      x--;
      decision_over_2 += 2 * (y - x) + 1;  // change for y -> y+1, x -> x-1
    }
  }
}
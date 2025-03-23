#define PS2_BASE 0xFF200100
#define HEX5_HEX4_BASE 0xFF200030
#define PIXEL_BUF_CTRL_BASE 0xFF203020

#define BOX_SIZE 10
#define CURSOR_COLOR 0xF800  // red

void keyboard();            // takes input from PS/2 port
void handle_key(char key);  // handles key presses
void init_vga();            // initializes the VGA display

/* VGA display subroutines */
void plot_pixel(int x, int y, short int line_color);
void clear_screen();
void draw_box(int x, int y, short int colour);

/* Global Variables */
int pixel_buffer_start;
int x_cur = 160, y_cur = 30;  // cursor position

int main() {
  init_vga();
  keyboard();
}

void keyboard() {
  volatile int *PS2_ptr = (int *)PS2_BASE;
  int PS2_data, RVALID;
  char byte1 = 0, byte2 = 0, byte3 = 0;  // last three bytes read
  *(PS2_ptr) = 0xFF;                     // reset

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
    case 0x75:           // up arrow
      *hex = 0b0111110;  // U
      if (y_cur - BOX_SIZE >= 0) {
        draw_box(x_cur, y_cur, 0x0000);  // erase cursor
        y_cur -= BOX_SIZE;
        draw_box(x_cur, y_cur, CURSOR_COLOR);  // draw cursor
      }
      break;
    case 0x72:           // down arrow
      *hex = 0b1011110;  // d
      if (y_cur + BOX_SIZE < 240) {
        draw_box(x_cur, y_cur, 0x0000);  // erase cursor
        y_cur += BOX_SIZE;
        draw_box(x_cur, y_cur, CURSOR_COLOR);  // draw cursor
      }
      break;
    case 0x6B:           // left arrow
      *hex = 0b0111000;  // L
      if (x_cur - BOX_SIZE >= 0) {
        draw_box(x_cur, y_cur, 0x0000);  // erase cursor
        x_cur -= BOX_SIZE;
        draw_box(x_cur, y_cur, CURSOR_COLOR);  // draw cursor
      }
      break;
    case 0x74:           // right arrow
      *hex = 0b0110001;  // r
      if (x_cur + BOX_SIZE < 320) {
        draw_box(x_cur, y_cur, 0x0000);  // erase cursor
        x_cur += BOX_SIZE;
        draw_box(x_cur, y_cur, CURSOR_COLOR);  // draw cursor
      }
      break;
    case 0x1C:           // A key - draw
      *hex = 0b1110111;  // A
      break;
    case 0x5A:           // enter key - submit
      *hex = 0b1111001;  // E
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
  draw_box(x_cur, y_cur, CURSOR_COLOR);  // draw cursor
}

void plot_pixel(int x, int y, short int line_color) {
  volatile short int *one_pixel_address;

  one_pixel_address = pixel_buffer_start + (y << 10) + (x << 1);

  *one_pixel_address = line_color;
}

void clear_screen() {
  for (int x = 0; x < 320; x++) {
    for (int y = 0; y < 240; y++) {
      plot_pixel(x, y, 0x0000);  // draw black pixels
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
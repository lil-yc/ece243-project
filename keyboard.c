#define PS2_BASE 0xFF200100
#define HEX5_HEX4_BASE 0xFF200030

void keyboard();
void handle_key(char key);

int main() { keyboard(); }

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

  switch (key) {
    case 0x75:           // up arrow
      *hex = 0b0111110;  // U
      break;
    case 0x72:           // down arrow
      *hex = 0b1011110;  // d
      break;
    case 0x6B:           // left arrow
      *hex = 0b0111000;  // L
      break;
    case 0x74:           // right arrow
      *hex = 0b0110001;  // r
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
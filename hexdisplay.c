#define HEXBASE 0xff200020

void display_hex(int digit);

int main() {
  while (1) {
    display_hex(8);
  }
}

void display_hex(int digit) {
  int output = 0;
  switch (digit) {
    case 0:
      output = 0b0111111;
      break;
    case 1:
      output = 0b0000110;
      break;
    case 2:
      output = 0b1011011;
      break;
    case 3:
      output = 0b1001111;
      break;
    case 4:
      output = 0b1100110;
      break;
    case 5:
      output = 0b1101101;
      break;
    case 6:
      output = 0b1111101;
      break;
    case 7:
      output = 0b0000111;
      break;
    case 8:
      output = 0b1111111;
      break;
    case 9:
      output = 0b1101111;
      break;
    default:
      output = 0b000000;
      break;
  }
  volatile unsigned int *hex = (volatile unsigned int *)HEXBASE;
  *hex = output;
}
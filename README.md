# Project Overview

This project implements a handwritten digit recognition system on the DE1-SoC FPGA using the NIOS V soft processor. Users can draw digits (0-9) on a VGA display using a PS/2 keyboard. The drawn image is processed and downsampled to a 28x28 grayscale image, which is then passed through a modified LeNet-5 CNN model for classification. The predicted digit is displayed on a seven-segment HEX display. Different keyboard keys are for controlling the program's execution.

# Block Diagram

<img width="1008" alt="Screenshot 2025-03-30 at 7 30 51 PM" src="https://github.com/user-attachments/assets/ac5e835a-816f-4bdf-ab18-92c1b3b60ca3" />

## User Input
PS/2 Keyboard: Used for drawing digits on the VGA display.

Keyboard Controls:
- space bar:  start program
- arrow keys: draw and move cursor
- WASD keys:  move cursor only
- backspace:  erase
- esc:        clear canvas
- enter:      submit digit
  

## Display Interface
VGA Controller: Displays the drawing canvas and feedback to the user.

## Processing System
LeNet CNN Inference Engine: Classifies the digit using pre-trained weights.

Memory (On-chip RAM): Stores the captured image and CNN model parameters. 

Resize Module: Downsamples and normalizes the image to 28x28 for CNN input.

## Output Display
HEX Display: Shows the predicted digit (0-9).

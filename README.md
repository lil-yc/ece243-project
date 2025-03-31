# Project Overview

This project implements handwritten digit classification on the DE1-SoC FPGA using the NIOS V soft processor. Users can draw digits (0-9) on a VGA display using controls from the PS/2 keyboard. The drawn image of 240x240 pixels down sampled using bilinear resizing to a 28x28 pixel grayscale image, which is then passed through a modified LeNet-5 CNN model for classification. Side panel of the VGA display (80x240 pixels) is used to show the probability of each digit. The predicted digit (highest probability) is displayed on the 7-segment HEX display. Different keys are used for controlling the program's execution.

# Block Diagram


<img width="828" alt="Screenshot 2025-03-31 at 2 39 22 PM" src="https://github.com/user-attachments/assets/7fd5bdd9-2278-497f-badc-eee3d9a43f5a" />

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

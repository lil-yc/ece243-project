# Project Overview

This project implements a handwritten digit recognition system on the DE1-SoC FPGA using the NIOS V soft processor. Users can draw digits (0-9) on a VGA display using a PS/2 mouse. The drawn image is processed and downsampled to a 28x28 grayscale image, which is then passed through a LeNet-5 CNN model for classification. The predicted digit is displayed on a seven-segment HEX display. Push buttons are used to reset the display and capture new input images.

# Block Diagram

<img width="941" alt="ECE243 project block diagram" src="https://github.com/user-attachments/assets/775d0452-1631-42f2-9a05-fa80468b65cd" />

## User Input
PS/2 Mouse: Used for drawing digits on the VGA display.

Push Buttons: Used to reset or capture the image.

## Display Interface
VGA Controller: Displays the drawing canvas and feedback to the user.

## Processing System
LeNet CNN Inference Engine: Classifies the digit using pre-trained weights.

Memory (On-chip RAM): Stores the captured image and CNN model parameters. 

Resize Module: Downsamples and normalizes the image to 28x28 for CNN input.

## Output Display
HEX Display: Shows the predicted digit (0-9).

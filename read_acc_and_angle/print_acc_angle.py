import serial
import tkinter as tk
import math
import matplotlib.pyplot as plt
import numpy as np


def draw_rotating_line(angle):
    # Clear the plot
    plt.clf()

    # Center coordinates of the line
    center_x = 0
    center_y = 0

    # Length of the line
    length = 10

    # Convert angle from degrees to radians
    angle_rad = np.radians(angle)

    # Calculate endpoint coordinates of the line after rotation
    end_x = center_x + length * np.cos(angle_rad)
    end_y = center_y + length * np.sin(angle_rad)

    # Draw the line
    plt.plot([center_x, end_x], [center_y, end_y], color='black')

    # Set plot limits
    plt.xlim(-15, 15)
    plt.ylim(-15, 15)

    # Show the plot
    #plt.pause(0.01)

    # Schedule the next update
    #draw_rotating_line(angle + 1)  # Adjust the rotation speed as needed




def get_number(input_string):
    # Extract the last 4 characters
    decimal = input_string[-2:]
    integer = input_string[-5:-3]

    dec_number = int(integer)+int(decimal)/100

    return dec_number






# Open serial port (adjust the port and baud rate as needed)
ser = serial.Serial('/dev/cu.usbmodem11101', 9600)  # Change 'COM3' to the appropriate port for your Arduino

try:
    while True:
        # Read line from serial port
        line = ser.readline().decode('utf-8').strip()  # Decode bytes to string

        # Process the received data
        print(float(line))

        #draw_rotating_line(get_number(line))
        #break

except KeyboardInterrupt:
    # Close the serial port when the script is interrupted
    ser.close()
    print("Serial port closed.")

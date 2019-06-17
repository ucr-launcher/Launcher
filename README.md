# Launcher
Code repository for Group 3's UCR CS179J project

- added functionality to drive stepper motor

The working softare modules for this project are:

- Stm Communication
    - This script controls the communication between the Jetson and stm32
    - It contains the finite state machine that controls all of the hardware and launch procedure.
    
- Roi extractor
    - This script performs object detection to extract the users position.
    - It publishes the location for the depth extractor and orientation controller to use.
    
- Depth Extractor
    - This script extracts the region of interest's depth for the launching controller to use.
    
- Digital Pot Driver, Motor & Servo Driver
    - Contains driver software for the abstracted hardware.
    - COntains test modules to verify the functionality of the drivers.

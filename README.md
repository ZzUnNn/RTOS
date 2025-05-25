# RTOS
# Project Repository

This repository contains the majority of files for this project. It includes two main parts:

## FreeRTOS on STM32F103ZET6
The `F103ZET6` directory contains example code for deploying FreeRTOS on the STM32F103ZET6 development board.

**Requirements:**
- Keil5 MDK (Microcontroller Development Kit) IDE
- No additional software required

**Usage:**
1. Open the project in Keil5
2. Build and flash to your STM32F103ZET6 board

## RT-Thread on Raspberry Pi 4B
The `rt-thread` directory contains example code for deploying RT-Thread on Raspberry Pi 4B.

**Requirements:**
- env-windows (Official RT-Thread development environment)
- SCons build system

**Usage:**
1. Install env-windows following the [official documentation](https://www.rt-thread.io/document/site/)
2. Navigate to `/bsp/raspberry-pi` directory
3. Run `scons` to compile the project

For more detailed instructions, please refer to the official RT-Thread documentation for Raspberry Pi 4B porting.

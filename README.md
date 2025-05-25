# Project Repository  

This repository contains the majority of files for this project. It includes three main parts:  

## 1. FreeRTOS on STM32F103ZET6  
The `F103ZET6` directory contains example code for deploying FreeRTOS on the STM32F103ZET6 development board.  

**Requirements:**  
- Keil5 MDK (Microcontroller Development Kit) IDE  
- No additional software required  

**Usage:**  
1. Open the project in Keil5  
2. Build and flash to your STM32F103ZET6 board  

## 2. RT-Thread on Raspberry Pi 4B  
The `rt-thread` directory contains example code for deploying RT-Thread on Raspberry Pi 4B.  

**Requirements:**  
- env-windows (Official RT-Thread development environment)  
- SCons build system  
- u-boot bootloader compilation  

**Usage:**  
1. Install env-windows following the [official documentation](https://www.rt-thread.io/document/site/)  
2. Compile the u-boot bootloader  
3. Navigate to `/bsp/raspberry-pi` directory  
4. Run `scons` to compile the project  

## 3. Realtime-Test with RT-Patched Linux Kernel  
The `realtime-test` directory contains example code for running real-time applications in a container using an RT-patched Linux kernel.  

**Requirements:**  
- Linux kernel with real-time (RT) patches  
- Container runtime (Docker, Podman, etc.)  

**Usage:**  
1. Ensure your Linux kernel has RT patches applied  
2. Run the container with real-time scheduling policies  

For detailed instructions, refer to the respective documentation for each component.  
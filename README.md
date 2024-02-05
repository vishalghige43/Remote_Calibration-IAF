# Remote_Calibration-IAF

## Overview

This project implements a remote pressure control system using an embedded system with ESP32 Cam for video capture and a Raspberry Pi for computer vision processing. The system includes a calibration algorithm for precise gauge readings.

## Output

<img src="Test Result/Screenshot 1.png" alt="Image Description" width="550" height="400" />     <img src="Test Result/Screenshot 4.png" alt="Image Description" width="250" height="400" />


## Setup Instructions

1. **Hardware Requirements:**
    - ESP32 Cam module
    - Raspberry Pi
  
2. **Software Dependencies:**
    - Install OpenCV on Raspberry Pi
    - Flash appropriate firmware to ESP32 Cam
  

## Usage Example

```bash
git clone https://github.com/vishalghige43/Remote_Calibration-IAF
cd Remote_Calibration-IAF
mkdir build
cmake -S . -B build
cmake --build build
```

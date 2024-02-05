# Remote_Calibration-IAF

## Overview

This project implements a remote pressure control system using an embedded system with ESP32 Cam for video capture and a Raspberry Pi for computer vision processing. The system includes a calibration algorithm for precise gauge readings.\
The main functions used from the OpenCV* library are HoughCircles (to detect the outline of the gauge and center point) and HoughLines (to detect the dial).
### Overview of working :
- Basic filtering is done.
- Prgram return circles from `HoughCircles` that are within reasonable range of the image height (this assumes the gauge takes up most of the view)
- average the resulting circles and use the average for the center point and radius For lines.
- apply a threshold using `cv2.threshold.` and `cv2.THRESH_BINARY_INV` with threshold of 175 and maxValue of 255 work fine
- remove all lines outside a given radius
-check if a line is within an acceptable range of the radius
- use the first acceptable line as the dial

## Test Output

<img src="Test Result/Screenshot 1.png" alt="Image Description" width="550" height="350" />     <img src="Test Result/Screenshot 4.png" alt="Image Description" width="250" height="350" />


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

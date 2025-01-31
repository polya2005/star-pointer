# The Star Pointing and Identification Device

I started this project in my sophomore year of high school as a part of my science project sponsored by the Thai government's Development and Promotion of Science and Technology Talents Project (DPST) program. By the time I finished high school, I had built the first prototype, which served as a proof of concept but would benefit from further refining. This repository contains the code for the first prototype. For an improved version of this project, which I am working on at the time of writing (2025), please see the [new repository](https://github.com/polya2005/new-star-pointer).

## Introduction
As the name suggests, the device requires these core functionalities:
- Given a choice of star, point the (physical) pointer toward the position of the star in the sky
- Given the pointer's direction, identify the star it is poining toward

For the device to be useful, it also needs the way to
- Store the star data (SD Card)
- Interact with the user (TFT LCD touchscreen)
- Obtain necessary information for the relevant spherical astronomical calculations (GPS, Magnetometer, and Accelerometer)
- Rotate the pointer to the desired azimuth and altitude angle (28BYJ-48 stepper motors; will be replaced with a more robust model of stepper motors in the new version)

## The design
With those in mind, I designed the device in three main parts: electronics, code, and enclosure.

### Electronics
The main controller board is the Arduino Mega 2560 which provides plenty of pins to connect to the required peripherals:
- 3.2" TFT LCD touchscreen with ILI9341 display controller and XPT2046 touch controller
- Micro SD card reader
- uBlox NEO6MV2 GPS receiver
- QMC5883L magnetometer
- ADXL345 accelerometer
- ULN2003 drivers for controlling the unipolar stepper motors

The system has a 2.1 mm DC socket that accepts 7-30 V input voltage. I recommend using a lithium polymer battery for power as it can provide the current required for the stepper motors without frequent recharging. The screen brightness is controlled with a potentiometer as a voltage divider, but the circuit can be easily modified to use PWM for (slightly) higher efficiency.

There is also a separate infrared keyboard circuit, which is used as an input device during the star search. It simplifies the programming as I did not have to make an on-screen keyboard. The circuit is controlled by an Arduino Nano with 54 pushbuttons in 5x11 matrix configuration and a 940 nm infrared LED. It is powered by a small lithium polymer battery with a suitable charge/protect/boost circuit.

The circuit itself is assembled mainly on a homemade arduino shield made from a piece of perfboard with connectors for the peripheral that needs to be mounted in a specific place. It is not the most reliable method of implementing the connections. Therefore, the next version will use a professionally fabricated PCB instead.

### Code
The code for the main circuit consists of several files:
- `Astrotime.h` and `Astrotime.cpp`: Classes representing the sexagesimal angles and times
- `FieldLib.h`: Classes for the graphical user interface
- `FieldReader.h`: A template class for reading the binary database stored in the micro SD card
- `Pointer.h` and `Pointer.cpp`: A class for controlling the movement
- `Starpointer.h` and `Starpointer.cpp`: A class for the high-level operation of the star pointer
- `main.ino`: The main code that connects the `StarPointer`, the database, and the GUI together

In retrospect, this is not the most organized or the optimal way to structure the project. Again, this will be addresed in the next version.

The code for the keyboard circuit is much simpler. It is contained in just one `.ino` file.

### Enclosure
I used SketchUp to design all the 3D models. The 3D printed results somewhat worked, but it was not the easiest to assemble. In the new version, I am using Fusion 360, which is more suited for CAD modeling. Furthermore, I did not have an instant access to 3D printers during most of my high school years, making the iteration process far too slow to develop a well working model. Now, I live on MIT campus and will be able to iterate more thanks to MIT's makerspace located within 10-minute walk from my location.

## Demo
[This video](https://youtu.be/CIAAN1MuVBc) demonstrates the working of this prototype. Please consider liking and sharing this video!

# purdue-sps-couch
Motor controller code for Purdue SPS motor couch.

## Description

Code for the SPS couch's Arduino motor controller interface. This code reads serial input from the game controller connected to the USB Host Shield, translating to some target left- and right-wheel speed, and handles sending those target values to the motor controllers.

## Getting Started

### IDE installation and setup

First, download Arduino IDE v1.8+

* Plug Arduino into computer via the USB 2.0 A/B cable.
* Select the board port via Tools->Port. Make sure the Tools->Board field is "Arduino Uno"
* For more detailed instructions, follow the official Getting Started: https://www.arduino.cc/en/Guide/ArduinoUno

### Dependencies

* USB Host Shield 2.0

From the Arduino IDE, go to Sketch->Include Library->Manage Libraries. Search for USB Host Shield 2.0, and install the latest version.

As of USB Host Shield 2.0 v1.6.0, there is a known issue preventing it from working with our Logitech F310 game controller. To patch this, follow the instructions here (https://github.com/AlanFord/Logitech_F310_and_Arduino). 


### Running the code

Open the main Arduino file `sps_couch_main/sps_couch_main.ino`, and verify that it builds. Once you connect your Arduino to the computer, you should be able to hit "Upload" and the motor controller should respond to joystick inputs. For testing, the received and sent values are all printed to Serial on baud 115200.


## Button Mappings
![gamepad_logitech_buttons](https://user-images.githubusercontent.com/3460254/132637436-eb833f64-042f-4bdc-9431-5c734b44bf5c.png)

Button | Function
-----|----
Axis 1 (left joy up/down ) | Left wheels forward/back
Axis 3 (right joy up/down) | Right wheels forward/back
Button 5 (RB) | Press for faster; release for slow mode

## Help

---

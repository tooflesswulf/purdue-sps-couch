#include <usbhid.h>
#include <hiduniversal.h>
#include <usbhub.h>

#include "hidjoystickrptparser.h"
#include "CouchController.h"

#define MOTOR_LEFT 3
#define MOTOR_RIGHT 5


USB Usb;
//USBHub Hub(&Usb);
HIDUniversal Hid(&Usb);

CouchController controller(MOTOR_LEFT, MOTOR_RIGHT);
JoystickEvents JoyEvents(&controller);
JoystickReportParser Joy(&JoyEvents);

void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  Serial.println("Start");

  if (Usb.Init() == -1)
    Serial.println("OSC did not start.");

  Serial.println("unga bunga");

  delay(200);

  if (!Hid.SetReportParser(0, &Joy))
    ErrorMessage<uint8_t > (PSTR("SetReportParser"), 1);
}

void loop() {
  Usb.Task();
  controller.send_cmd();
}

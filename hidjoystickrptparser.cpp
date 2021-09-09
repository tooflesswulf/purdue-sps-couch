#include "hidjoystickrptparser.h"
#include "jsconditioning.h"

#define J1_X         (float)(x1 - 128) * 0.0078125
#define J1_Y         (float)(y1 - 127) * -0.0078125
#define J2_X         (float)(x2 - 128) * 0.0078125
#define J2_Y         (float)(y2 - 127) * -0.0078125

#define A_BTN        (rz & 0x20) != 0
#define B_BTN        (rz & 0x40) != 0
#define X_BTN        (rz & 0x10) != 0
#define Y_BTN        (rz & 0x80) != 0

#define START_BTN    (dn & 0x02) != 0
#define BACK_BTN     (dn & 0x01) != 0

#define DPAD_U       (rz | 0x0F) == 0
#define DPAD_UR      (rz | 0x0F) == 1
#define DPAD_R       (rz | 0x0F) == 2
#define DPAD_DR      (rz | 0x0F) == 3
#define DPAD_D       (rz | 0x0F) == 4
#define DPAD_DL      (rz | 0x0F) == 5
#define DPAD_L       (rz | 0x0F) == 6
#define DPAD_UL      (rz | 0x0F) == 7
#define DPAD_NONE    (rz | 0x0F) == 8

#define LT           (hat & 0x04) != 0
#define LB           (hat & 0x01) != 0
#define RT           (hat & 0x08) != 0
#define RB           (hat & 0x02) != 0

#define J1_DOWN      (dn & 0x04) != 0
#define J2_DOWN      (dn & 0x08) != 0

uint8_t x1, y1, x2, y2, rz, hat, dn;

double pos_max_1, neg_max_1, pos_max_2, neg_max_2;
bool slow_mode = 1;

void update_stuff() {

  /*Serial.print("x1: ");
  Serial.print(J1_X);
  Serial.print("  y1: ");
  Serial.print(J1_Y);
  Serial.print("  x2: ");
  Serial.print(J2_X);
  Serial.print("  y2: ");
  Serial.print(J2_Y);

  Serial.print("  a: ");
  Serial.print(A_BTN);
  Serial.print("  b: ");
  Serial.print(B_BTN);
  Serial.print("  x: ");
  Serial.print(X_BTN);
  Serial.print("  y: ");
  Serial.print(Y_BTN);

  Serial.print("  lb: ");
  Serial.print(LB);
  Serial.print("  lt: ");
  Serial.print(LT);
  Serial.print("  rb: ");
  Serial.print(RB);
  Serial.print("  rt: ");
  Serial.println(RT);*/
   // condition( value, pos_max, neg_max, pos_offset, neg_offset, pos_deadband, neg_deadband )
  // *_max should be greater in magnitude than *_offset

  slow_mode = !RB;
  Serial.println(slow_mode);

  if (slow_mode) {
    pos_max_1 = 0.55;
    neg_max_1 = 0.55;
    pos_max_2 = 0.5;
    neg_max_2 = 0.5;
  } else {
    pos_max_1 = 0.8;
    neg_max_1 = 0.8;
    pos_max_2 = 0.8;
    neg_max_2 = 0.8;
  }
    
  int out;
  if (J1_Y >= 0) {
    float w = condition(J1_Y, pos_max_1, neg_max_1, 0.45, -0.45, 0.1, -0.1);
    Serial.print("joy in: ");
    Serial.println(w);
    out = (int)(w * 255.0);
    Serial.print("out cycle: ");
    Serial.println(out);
    analogWrite(3, out);
  }

  if (J2_Y >= 0) {
    float v = condition(J2_Y, pos_max_2, neg_max_2, 0.4, -0.4, 0.1, -0.1);
    Serial.print("joy in: ");
    Serial.println(v);
    out = (int)(v * 255.0);
    Serial.print("out cycle: ");
    Serial.println(out);
    analogWrite(5, out);
  }
  
}



JoystickReportParser::JoystickReportParser(JoystickEvents *evt) :
joyEvents(evt),
oldHat(0xDE),
oldButtons(0) {
  for (uint8_t i = 0; i < RPT_GEMEPAD_LEN; i++)
    oldPad[i] = 0xD;
}

void JoystickReportParser::Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf) {
  bool match = true;

  // Checking if there are changes in report since the method was last called
  for (uint8_t i = 0; i < RPT_GEMEPAD_LEN; i++)
    if (buf[i] != oldPad[i]) {
      match = false;
      break;
    }

  // Calling Game Pad event handler
  if (!match && joyEvents) {
    joyEvents->OnGamePadChanged((const GamePadEventData*)buf);

    for (uint8_t i = 0; i < RPT_GEMEPAD_LEN; i++) oldPad[i] = buf[i];
  }

  uint8_t hat = (buf[5] & 0xF);

  // Calling Hat Switch event handler
  if (hat != oldHat && joyEvents) {
    joyEvents->OnHatSwitch(hat);
    oldHat = hat;
  }

  uint16_t buttons = (0x0000 | buf[6]);
  buttons <<= 4;
  buttons |= (buf[5] >> 4);
  uint16_t changes = (buttons ^ oldButtons);

  // Calling Button Event Handler for every button changed
  if (changes) {
    for (uint8_t i = 0; i < 0x0C; i++) {
      uint16_t mask = (0x0001 << i);

      if (((mask & changes) > 0) && joyEvents)
        if ((buttons & mask) > 0)
          joyEvents->OnButtonDn(i + 1);
        else
          joyEvents->OnButtonUp(i + 1);
    }
    oldButtons = buttons;
  }
}

void JoystickEvents::OnGamePadChanged(const GamePadEventData *evt) {
  x1 = evt->X;
  y1 = evt->Y;
  x2 = evt->Z1;
  y2 = evt->Z2;
  rz = evt->Rz;
  //update_stuff();

  controller->inputJ1(J1_X, J1_Y);
  controller->inputJ2(J2_X, J2_Y);
}

void JoystickEvents::OnHatSwitch(uint8_t htsw) {
  hat = htsw;

  controller->inputRB(!RT);
  //update_stuff();
}

void JoystickEvents::OnButtonUp(uint8_t but_id) {
  dn &= ~(1<<but_id);
  //update_stuff();
}

void JoystickEvents::OnButtonDn(uint8_t but_id) {
  dn |= (1<<but_id);
  //update_stuff();
}

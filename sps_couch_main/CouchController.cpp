#include "CouchController.h"
#include <Arduino.h>

// Cubic scaling between joystick input and commanded duty cycle.
double condition(double val, double pos_max, double neg_max, double pos_offset, double neg_offset, double pos_deadband, double neg_deadband) {

  if (val > neg_deadband && val < pos_deadband) return 0.0;
  
  if (val > 0.99) return pos_max;
  
  if (val < -0.99) return -neg_max;
  
  if (val < neg_deadband) {
    double temp = (val - neg_deadband) / (1 - neg_deadband);
    return (temp*temp*temp * (neg_max - neg_offset) + neg_offset);
  }
  
  if (val > pos_deadband) {
    double temp = (val - pos_deadband) / (1 - pos_deadband);
    return (temp*temp*temp * (pos_max - pos_offset) + pos_offset);
  }
  
}

CouchController::CouchController(int left, int right) {
  motor_left = left;
  motor_right = right;
}

// Currently hacked so that negative commands (reversing) is not allowed.
void CouchController::send_cmd() {
  float v1_send;
  if (slowmode) {
    v1_send = condition(v1_targ, s_pos_max_1, s_neg_max_1, 0.4, -0.4, 0.1, -0.1);
  } else {
    v1_send = condition(v1_targ, pos_max_1, neg_max_1, 0.4, -0.4, 0.1, -0.1);    
  }

  float v2_send;
  if (slowmode) {
    v2_send = condition(v2_targ, s_pos_max_2, s_neg_max_2, 0.4, -0.4, 0.1, -0.1);
  } else {
    v2_send = condition(v2_targ, pos_max_2, neg_max_2, 0.4, -0.4, 0.1, -0.1);
  }
  
  int w1 = (int)(v1_send * 255.0);
  
  if (w1 > 0) {
    analogWrite(motor_left, w1);
    Serial.print("Write1: ");
    Serial.println(w1);
  } else {
    analogWrite(motor_left, 0);
  }
  
  int w2 = (int)(v2_send * 255.0);
  if (w2 > 0) {
    analogWrite(motor_right, w2);
    Serial.print("Write2: ");
    Serial.println(w2);
  } else {
    analogWrite(motor_right, 0);
  }
}

void CouchController::inputRB(bool v) {
  slowmode = v;
}

void CouchController::inputJ1(float x, float y) {
  v1_targ = y;
}

void CouchController::inputJ2(float x, float y) {
  v2_targ = y;
}

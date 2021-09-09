#include "jsconditioning.h"

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


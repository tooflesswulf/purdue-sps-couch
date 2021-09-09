#ifndef __COUCHCONTROLLER_H__
#define __COUCHCONTROLLER_H__

class CouchController {
  protected:
    double pos_max_1 = 0.8, neg_max_1 = 0.8, pos_max_2 = 0.8, neg_max_2 = 0.8;
    double s_pos_max_1 = 0.5, s_neg_max_1 = 0.5, s_pos_max_2 = 0.5, s_neg_max_2 = 0.5;

    int motor_left, motor_right;

    // Control params
    float v1_targ = 0;
    float v2_targ = 0;
    int slowmode = 1;

  public:
    CouchController(int, int);
    void send_cmd();

    void inputJ1(float x1, float y1);
    void inputJ2(float x2, float y2);

    void inputRB(bool v);
};

#endif // __COUCHCONTROLLER_H__

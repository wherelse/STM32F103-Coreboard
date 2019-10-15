#ifndef __CONTROL_H
#define __CONTROL_H

struct PID
{
    float kp;
    float kd;
    float ki;
    float I;
    float err;
    float err_last;
    float pidout;
};
#endif


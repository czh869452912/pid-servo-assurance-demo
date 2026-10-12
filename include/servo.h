#ifndef SERVO_H
#define SERVO_H
#include "pid.h"
#include <stdint.h>
typedef enum { SERVO_DISABLED, SERVO_ACTIVE, SERVO_FAULT } ServoMode;
typedef struct {
    Pid pid;
    ServoMode mode;
    uint64_t previous_us, period_us;
    bool sampled;
} Servo;
PidStatus servo_init(Servo *servo, PidConfig config);
void servo_reset(Servo *servo);
PidStatus servo_step(Servo *servo, uint64_t time_us, double reference,
                     double measurement, bool valid_sensor, bool enabled, double *output);
#endif

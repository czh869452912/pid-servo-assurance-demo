#ifndef PID_H
#define PID_H
#include <stdbool.h>
typedef struct { double kp, ki, kd, dt, min, max; } PidConfig;
typedef struct { PidConfig config; double integral, previous; bool ready, initialized; } Pid;
typedef enum { PID_OK, PID_INVALID } PidStatus;
PidStatus pid_init(Pid *pid, PidConfig config);
PidStatus pid_step(Pid *pid, double reference, double measurement, bool enabled, double *output);
void pid_reset(Pid *pid);
#endif

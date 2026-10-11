#include "pid.h"
#include <math.h>
#include <stddef.h>
static double clamp(double value, double low, double high) {
    return value < low ? low : value > high ? high : value;
}
void pid_reset(Pid *pid) {
    if (pid != NULL) { pid->integral = 0.0; pid->previous = 0.0; pid->ready = false; }
}
PidStatus pid_init(Pid *pid, PidConfig c) {
    if (pid == NULL) return PID_INVALID;
    pid->initialized = false;
    pid_reset(pid);
    if (!isfinite(c.kp) || !isfinite(c.ki) || !isfinite(c.kd) || !isfinite(c.dt) ||
        !isfinite(c.min) || !isfinite(c.max) || c.kp < 0.0 || c.kp > 100.0 ||
        c.ki < 0.0 || c.ki > 100.0 || c.kd < 0.0 || c.kd > 100.0 ||
        c.dt < 0.001 || c.dt > 1.0 || c.min < -100.0 || c.max > 100.0 ||
        c.min > 0.0 || c.max < 0.0 || c.min >= c.max) return PID_INVALID;
    pid->config = c;
    pid->initialized = true;
    return PID_OK;
}
PidStatus pid_step(Pid *pid, double reference, double measurement, bool enabled, double *output) {
    if (output == NULL) return PID_INVALID;
    *output = 0.0;
    if (pid == NULL || !pid->initialized) return PID_INVALID;
    if (!enabled) { pid_reset(pid); return PID_OK; }
    if (!isfinite(reference) || !isfinite(measurement) || reference < -10000.0 ||
        reference > 10000.0 || measurement < -10000.0 || measurement > 10000.0) return PID_INVALID;
    const PidConfig c = pid->config;
    const double error = reference - measurement;
    const double derivative = pid->ready ? c.kd * (measurement - pid->previous) / c.dt : 0.0;
    double integral = clamp(pid->integral + c.ki * error * c.dt, c.min, c.max);
    double raw = c.kp * error + integral + derivative;
    if ((raw > c.max && error > 0.0) || (raw < c.min && error < 0.0)) {
        integral = pid->integral;
        raw = c.kp * error + integral + derivative;
    }
    *output = clamp(raw, c.min, c.max);
    pid->integral = integral;
    pid->previous = measurement;
    pid->ready = true;
    return PID_OK;
}

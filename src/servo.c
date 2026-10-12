#include "servo.h"
#include <stddef.h>
/* REQ-PID-009 valid supervised sampling; REQ-PID-010 invalid sensor;
 * REQ-PID-011 sample-time validity; REQ-PID-012 latched fault/recovery.
 * Safe output zero is a bench assumption, not an aircraft-level conclusion. */
PidStatus servo_init(Servo *servo, PidConfig config) {
    if (servo == NULL) return PID_INVALID;
    *servo = (Servo){0};
    if (pid_init(&servo->pid, config) != PID_OK) return PID_INVALID;
    servo->period_us = (uint64_t)(config.dt * 1000000.0);
    servo->mode = SERVO_DISABLED;
    return PID_OK;
}
void servo_reset(Servo *servo) {
    if (servo != NULL) {
        pid_reset(&servo->pid);
        servo->sampled = false;
        servo->previous_us = 0;
        servo->mode = SERVO_DISABLED;
    }
}
static PidStatus fault(Servo *servo) {
    pid_reset(&servo->pid);
    servo->mode = SERVO_FAULT;
    return PID_INVALID;
}
PidStatus servo_step(Servo *servo, uint64_t time_us, double reference,
                     double measurement, bool valid_sensor, bool enabled, double *output) {
    if (output == NULL) return PID_INVALID;
    *output = 0.0;
    if (servo == NULL || !servo->pid.initialized) return PID_INVALID;
    if (servo->mode == SERVO_FAULT) return PID_INVALID;
    if (!enabled) { servo_reset(servo); return PID_OK; }
    if (!valid_sensor) return fault(servo);
    if (servo->sampled && (time_us <= servo->previous_us ||
        time_us - servo->previous_us > 2 * servo->period_us)) return fault(servo);
    if (pid_step(&servo->pid, reference, measurement, true, output) != PID_OK)
        return fault(servo);
    servo->previous_us = time_us;
    servo->sampled = true;
    servo->mode = SERVO_ACTIVE;
    return PID_OK;
}

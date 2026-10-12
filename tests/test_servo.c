#include "servo.h"
#include <math.h>
#include <float.h>
#include <stdio.h>
#include <string.h>
#define CHECK(expr) do { if (!(expr)) { fprintf(stderr,"FAIL line %d: %s\n",__LINE__,#expr); return 1; } } while(0)
int main(int argc, char **argv) {
    if (argc != 2) return 2;
    Servo s = {0}; double out = 99.0;
    PidConfig c = {2.0, 0.0, 0.0, 0.1, -10.0, 10.0};
    CHECK(servo_init(&s,c) == PID_OK);
    if (!strcmp(argv[1],"SENSOR")) { /* REQ-PID-010; REQ-PID-009 */
        CHECK(servo_step(&s,0,1,0,true,true,&out) == PID_OK);
        CHECK(s.mode == SERVO_ACTIVE && fabs(out-2.0)<1e-9);
        CHECK(servo_step(&s,100000,1,0,false,true,&out) == PID_INVALID);
        CHECK(s.mode == SERVO_FAULT && out == 0.0 && !s.pid.ready);
    } else if (!strcmp(argv[1],"TIME")) { /* REQ-PID-011 */
        PidConfig invalid = c; invalid.dt = 0.0000005;
        CHECK(servo_init(&s,invalid) == PID_INVALID && !s.pid.initialized);
        invalid.dt = DBL_MAX;
        CHECK(servo_init(&s,invalid) == PID_INVALID && !s.pid.initialized);
        invalid.dt = 0.001;
        CHECK(servo_init(&s,invalid) == PID_OK && s.period_us == 1000);
        CHECK(servo_init(&s,c) == PID_OK);
        CHECK(servo_step(&s,0,1,0,true,true,&out) == PID_OK);
        CHECK(servo_step(&s,200000,1,0,true,true,&out) == PID_OK);
        CHECK(servo_step(&s,400001,1,0,true,true,&out) == PID_INVALID);
        CHECK(s.mode == SERVO_FAULT && out == 0.0);
        servo_reset(&s);
        CHECK(servo_step(&s,100000,1,0,true,true,&out) == PID_OK);
        CHECK(servo_step(&s,100000,1,0,true,true,&out) == PID_INVALID);
        servo_reset(&s);
        CHECK(servo_step(&s,100000,1,0,true,true,&out) == PID_OK);
        CHECK(servo_step(&s,99999,1,0,true,true,&out) == PID_INVALID);
    } else if (!strcmp(argv[1],"RECOVERY")) { /* REQ-PID-012 */
        CHECK(servo_step(&s,0,1,0,false,true,&out) == PID_INVALID);
        CHECK(servo_step(&s,100000,1,0,true,true,&out) == PID_INVALID);
        CHECK(servo_step(&s,200000,1,0,true,false,&out) == PID_INVALID);
        CHECK(s.mode == SERVO_FAULT && out == 0.0);
        servo_reset(&s);
        CHECK(s.mode == SERVO_DISABLED);
        CHECK(servo_step(&s,300000,1,0,true,true,&out) == PID_OK);
        CHECK(s.mode == SERVO_ACTIVE && fabs(out-2.0)<1e-9);
    } else if (!strcmp(argv[1],"SIM")) { /* REQ-PID-009; REQ-PID-010; REQ-PID-012 */
        double y = 0.0;
        for (uint64_t k=0;k<200;k++) {
            CHECK(servo_step(&s,k*100000,1,y,true,true,&out) == PID_OK);
            CHECK(out>=-10.0 && out<=10.0);
            y += 0.05*out; /* Explicit bench plant, not an aircraft dynamics model. */
        }
        CHECK(fabs(y-1.0)<1e-6);
        CHECK(servo_step(&s,20000000,1,y,false,true,&out) == PID_INVALID);
        CHECK(out == 0.0 && s.mode == SERVO_FAULT);
        servo_reset(&s);
        CHECK(servo_step(&s,20100000,1,y,true,true,&out) == PID_OK);
        CHECK(servo_step(&s,20200000,1,y,true,false,&out) == PID_OK);
        CHECK(s.mode == SERVO_DISABLED && out == 0.0);
    } else return 2;
    printf("PASS %s\n",argv[1]); return 0;
}

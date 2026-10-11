#include "pid.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#define CHECK(expr) do { if (!(expr)) { fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #expr); return 1; } } while (0)
#define NEAR(a,b) (fabs((a)-(b)) < 1e-9)
int main(int argc, char **argv) {
    if (argc != 2) return 2;
    Pid p = {0}; double out = 0.0; PidConfig c = {2.0, 0.0, 0.0, 0.1, -10.0, 10.0};
    if (!strcmp(argv[1], "RANGE")) { /* REQ-PID-001 */
        CHECK(pid_init(&p, c) == PID_OK);
        c.kp = 100.0; c.ki = 100.0; c.kd = 100.0; c.dt = 0.001; c.min = -100.0; c.max = 100.0;
        CHECK(pid_init(&p, c) == PID_OK);
        c.dt = 1.0; CHECK(pid_init(&p, c) == PID_OK);
        c.dt = 0.0; CHECK(pid_init(&p, c) == PID_INVALID);
        c.dt = 0.1; c.kp = 100.01; CHECK(pid_init(&p, c) == PID_INVALID);
        c.kp = -0.01; CHECK(pid_init(&p, c) == PID_INVALID);
        c.kp = NAN; CHECK(pid_init(&p, c) == PID_INVALID);
        c.kp = 1.0; c.min = 1.0; CHECK(pid_init(&p, c) == PID_INVALID);
    } else if (!strcmp(argv[1], "P")) { /* REQ-PID-002 */
        CHECK(pid_init(&p, c) == PID_OK); CHECK(pid_step(&p, 3.0, 1.0, true, &out) == PID_OK); CHECK(NEAR(out,4.0));
    } else if (!strcmp(argv[1], "I")) { /* REQ-PID-003 */
        c.kp = 0.0; c.ki = 2.0; CHECK(pid_init(&p,c) == PID_OK);
        CHECK(pid_step(&p,1.0,0.0,true,&out) == PID_OK); CHECK(NEAR(out,0.2));
        CHECK(pid_step(&p,1.0,0.0,true,&out) == PID_OK); CHECK(NEAR(out,0.4));
    } else if (!strcmp(argv[1], "D")) { /* REQ-PID-004 */
        c.kp = 0.0; c.kd = 1.0; CHECK(pid_init(&p,c) == PID_OK);
        CHECK(pid_step(&p,0.0,0.0,true,&out) == PID_OK); CHECK(NEAR(out,0.0));
        CHECK(pid_step(&p,0.0,0.5,true,&out) == PID_OK); CHECK(NEAR(out,-5.0));
        CHECK(pid_step(&p,99.0,0.5,true,&out) == PID_OK); CHECK(NEAR(out,0.0));
    } else if (!strcmp(argv[1], "SAT")) { /* REQ-PID-005 */
        CHECK(pid_init(&p,c) == PID_OK); CHECK(pid_step(&p,10000.0,-10000.0,true,&out) == PID_OK); CHECK(NEAR(out,10.0));
        CHECK(pid_step(&p,-10000.0,10000.0,true,&out) == PID_OK); CHECK(NEAR(out,-10.0));
    } else if (!strcmp(argv[1], "AW")) { /* REQ-PID-006 */
        c.ki = 1.0; CHECK(pid_init(&p,c) == PID_OK);
        for (int i=0;i<100;i++) CHECK(pid_step(&p,100.0,0.0,true,&out) == PID_OK);
        CHECK(NEAR(p.integral,0.0)); CHECK(pid_step(&p,0.0,0.0,true,&out) == PID_OK); CHECK(NEAR(out,0.0));
        for (int i=0;i<100;i++) CHECK(pid_step(&p,-100.0,0.0,true,&out) == PID_OK);
        CHECK(NEAR(p.integral,0.0));
    } else if (!strcmp(argv[1], "RESET")) { /* REQ-PID-007 */
        c.kp = 0.0; c.ki = 1.0; c.kd = 1.0; CHECK(pid_init(&p,c) == PID_OK);
        CHECK(pid_step(&p,1.0,0.0,true,&out) == PID_OK);
        CHECK(pid_step(&p,1.0,4.0,false,&out) == PID_OK); CHECK(NEAR(out,0.0)); CHECK(!p.ready); CHECK(NEAR(p.integral,0.0));
        CHECK(pid_step(&p,4.0,4.0,true,&out) == PID_OK); CHECK(NEAR(out,0.0));
        pid_reset(&p); CHECK(!p.ready); CHECK(NEAR(p.integral,0.0));
    } else if (!strcmp(argv[1], "INVALID")) { /* REQ-PID-008 */
        CHECK(pid_init(&p,c) == PID_OK); CHECK(pid_step(&p,1.0,0.0,true,&out) == PID_OK);
        Pid before = p; CHECK(pid_step(&p,NAN,0.0,true,&out) == PID_INVALID); CHECK(NEAR(out,0.0)); CHECK(!memcmp(&p,&before,sizeof p));
        CHECK(pid_step(&p,10000.01,0.0,true,&out) == PID_INVALID); CHECK(NEAR(out,0.0));
        CHECK(pid_step(&p,0.0,INFINITY,true,&out) == PID_INVALID);
        CHECK(pid_step(NULL,0.0,0.0,true,&out) == PID_INVALID); CHECK(NEAR(out,0.0));
        CHECK(pid_step(&p,0.0,0.0,true,NULL) == PID_INVALID);
    } else return 2;
    printf("PASS %s\n",argv[1]); return 0;
}

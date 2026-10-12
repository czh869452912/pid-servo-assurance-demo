# 软件设计说明（演示摘要）

## 配置项与设计决策

CSCI-PID-SERVO 的单元为 SU-PID，接口定义在 `include/pid.h`，实现位于 `src/pid.c`，由主机测试单元调用。固定周期避免在核心算法中读取系统时间；测量微分降低参考阶跃引起的微分冲击；条件积分用于饱和时停止进一步积累。各调用实例独占状态，无动态内存和共享全局控制状态。

| 设计 ID | 需求分配 | 实现/数据/验证关注 |
| --- | --- | --- |
| DD-PID-01 参数校验 | REQ-PID-001 | `pid_init` 校验有限性、增益、周期、输出限值；失败清除 initialized |
| DD-PID-02 比例计算 | REQ-PID-002 | `error=r−y`、`P=Kp×error` |
| DD-PID-03 积分与限幅 | REQ-PID-003 | integral 状态；候选 `clamp(I+Ki×error×dt,min,max)` |
| DD-PID-04 测量微分 | REQ-PID-004 | previous 与 ready 状态；`D=−Kd×Δy/dt`，首采样 D=0 |
| DD-PID-05 输出限幅 | REQ-PID-005 | 合成输出再次 clamp；数值范围受输入与参数约束 |
| DD-PID-06 条件积分 | REQ-PID-006 | 检查候选合成值与误差方向；拒绝积分后重新合成 |
| DD-PID-07 复位 | REQ-PID-007 | `pid_reset` 清 integral、previous、ready；禁用返回0 |
| DD-PID-08 错误出口 | REQ-PID-008 | 先置有效输出为0；合法性失败在历史更新前返回 INVALID |

执行顺序：输出指针检查 → 输出置0 → 初始化状态检查 → 禁用复位出口 → 输入合法性检查 → 误差/微分 → 积分候选 → 抗饱和 → 输出限幅 → 更新历史。错误输入不改变 integral、previous、ready。

接口 `pid_init(Pid*,PidConfig)`、`pid_step(Pid*,r,y,enabled,double*)`、`pid_reset(Pid*)` 由主机进程内 C 函数调用，不涉及网络协议。参数与 IO 范围、状态和错误行为以 SRS 为准。设计审查仍需独立人员确认数值精度、边界充分性与实际对象动力学适配。

## 监督层扩展（待人工评审的台架衍生需求）

- REQ-PID-009 / DD-PID-09：有效配置下，监督层应在有效采样后进入ACTIVE并调用PID，输出由PID限幅。台架模型y(k+1)=y(k)+0.05u(k)，r=1、Kp=2、Ki=Kd=0、dt=0.1、y(0)=0，200步后|y-1|<1e-6；该模型不代表飞机动力学。 验证：REQ-SERVO-SIM。
- REQ-PID-010 / DD-PID-10：启用时sensor_valid=false或PID报告非法数值应进入FAULT，输出0并清PID历史。0输出是台架假设，须由上层确认其真实安全性。 验证：REQ-SERVO-SENSOR, REQ-SERVO-SIM。
- REQ-PID-011 / DD-PID-11：启用后连续采样时间戳必须严格递增；相邻间隔不得超过2×floor(dt×1000000)微秒，含该上界；违例进入FAULT并输出0。首个采样不限制绝对时刻。 验证：REQ-SERVO-TIME。
- REQ-PID-012 / DD-PID-12：FAULT应锁存；恢复有效输入或禁用不自动清故障。显式servo_reset清历史与采样时刻并进入DISABLED，之后有效启用可进入ACTIVE。 验证：REQ-SERVO-RECOVERY, REQ-SERVO-SIM。

监督层模块为 SU-SERVO，接口 include/servo.h，实现 src/servo.c。单元、模块集成与配置项台架仿真分别执行；仿真输入为确定性台架模型，目标机与真实航空系统测试未执行。

# 软件验证计划与程序（演示裁剪版）

## 1 验证准备与环境

验证对象由提交 SHA 唯一标识。主机 C11 编译器构建，告警作为错误，CTest 按需求逐项执行并输出 JUnit。测试在独立 CI 工作区执行，不把未保存的 IDE 文本当成提交版本。

## 2 用例和通过准则

六项用例要素和执行映射见 `.factory/test-cases.json`，程序说明见 STD；实测报告由工程证据工具生成，不预填测试结论。

| 需求 | CTest 名称 | 激励与通过准则 |
| --- | --- | --- |
| REQ-PID-001 | REQ-PID-RANGE | 有效边界参数接受，周期0/超限增益/NaN/不含0的限值拒绝；其他参数组合待补充 |
| REQ-PID-002 | REQ-PID-P | r=3,y=1,Kp=2，输出4 |
| REQ-PID-003 | REQ-PID-I | Ki=2,e=1,dt=0.1，两步输出0.2、0.4；积分限幅另需审查 |
| REQ-PID-004 | REQ-PID-D | 首步0，测量0→0.5输出−5，参考阶跃保持测量时输出0 |
| REQ-PID-005 | REQ-PID-SAT | 正负极限输入分别输出+10和−10 |
| REQ-PID-006 | REQ-PID-AW | 正负饱和各100步无积分累积，移除正误差后输出0 |
| REQ-PID-007 | REQ-PID-RESET | 禁用清历史、输出0，重新启用首次微分0；显式复位清历史 |
| REQ-PID-008 | REQ-PID-INVALID | NaN、Infinity、越界、空指针拒绝；输出指针有效时置0；非法数值输入不改变历史 |

执行：先运行 `tools/check_traceability.py`，再 CMake 构建和 CTest。证据为 Actions 的 run ID/attempt/head SHA、实际日志、`build/report.xml` 与文档/矩阵归档。矩阵检查通过只表示引用存在，不表示测试覆盖充分。

故障注入：演示分支把微分负号去掉，应使 REQ-PID-D 失败；其余测试继续执行。将其改回、检查 Diff、提交后再次 CI 应通过，但 PR 必须继续由独立人员正式评审。

尚未执行：目标机测试、调度/时序测量、全部参数组合、强健性完整分析、结构覆盖与验证独立性论证。八个通过的主机测试不代表全部 DO-178C 验证目标满足。

## 监督层扩展（待人工评审的台架衍生需求）

- REQ-PID-009 / DD-PID-09：有效配置下，监督层应在有效采样后进入ACTIVE并调用PID，输出由PID限幅。台架模型y(k+1)=y(k)+0.05u(k)，r=1、Kp=2、Ki=Kd=0、dt=0.1、y(0)=0，200步后|y-1|<1e-6；该模型不代表飞机动力学。 验证：REQ-SERVO-SIM。
- REQ-PID-010 / DD-PID-10：启用时sensor_valid=false或PID报告非法数值应进入FAULT，输出0并清PID历史。0输出是台架假设，须由上层确认其真实安全性。 验证：REQ-SERVO-SENSOR, REQ-SERVO-SIM。
- REQ-PID-011 / DD-PID-11：启用后连续采样时间戳必须严格递增；相邻间隔不得超过2×floor(dt×1000000)微秒，含该上界；违例进入FAULT并输出0。首个采样不限制绝对时刻。 验证：REQ-SERVO-TIME。
- REQ-PID-012 / DD-PID-12：FAULT应锁存；恢复有效输入或禁用不自动清故障。显式servo_reset清历史与采样时刻并进入DISABLED，之后有效启用可进入ACTIVE。 验证：REQ-SERVO-RECOVERY, REQ-SERVO-SIM。

监督层模块为 SU-SERVO，接口 include/servo.h，实现 src/servo.c。单元、模块集成与配置项台架仿真分别执行；仿真输入为确定性台架模型，目标机与真实航空系统测试未执行。

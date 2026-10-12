# 软件测试说明 · 主机单元验证

## 1 配置与准备

执行对象为已保存输入的受控副本。CMake、C11 编译器与 CTest 是实际执行工具；编译告警作为错误。输入文件清单、SHA-256、工具输出与 JUnit 均归档；环境变化须重跑。不自动保存、提交或推送开发者文件。

## 2 用例规格

机器可执行映射与六项要素见 `.factory/test-cases.json`：初始状态、输入序列、执行条件、预期、容差、终止条件。每个 CTest 用例在新进程内初始化，实际断言见 `tests/test_pid.c`。这些是现有用例的规格，不以“生成建议”代替已运行结果。

| 用例 | 需求 | 执行名称 | 类型 |
|---|---|---|---|
| TC-PID-001 | REQ-PID-001 | REQ-PID-RANGE | 参数边界/异常 |
| TC-PID-002 | REQ-PID-002 | REQ-PID-P | 正常功能 |
| TC-PID-003 | REQ-PID-003 | REQ-PID-I | 状态序列 |
| TC-PID-004 | REQ-PID-004 | REQ-PID-D | 阶跃/回归 |
| TC-PID-005 | REQ-PID-005 | REQ-PID-SAT | 正负极值 |
| TC-PID-006 | REQ-PID-006 | REQ-PID-AW | 组合/持续饱和 |
| TC-PID-007 | REQ-PID-007 | REQ-PID-RESET | 模式切换 |
| TC-PID-008 | REQ-PID-008 | REQ-PID-INVALID | 非法调用/历史保持 |
| TC-SERVO-SENSOR | REQ-PID-009、010 | REQ-SERVO-SENSOR | 模块集成：无效传感器 |
| TC-SERVO-TIME | REQ-PID-011 | REQ-SERVO-TIME | 模块集成：上界/超限/重复/倒退 |
| TC-SERVO-RECOVERY | REQ-PID-012 | REQ-SERVO-RECOVERY | 模块集成：锁存/复位 |
| TC-SERVO-SIM | REQ-PID-009、010、012 | REQ-SERVO-SIM | 配置项台架：闭环/故障/恢复 |

## 3 执行、记录和复核

工程根命令 `npm run assure:sample` 实际执行模板验证；`npm run assure:pid` 使用 DEMO_WORKSPACE。每次新建证据目录，不复用旧 JUnit。记录 `report.json`、`report.md`、`commands.json`、`source/build/report.xml` 与输入副本。构建失败、测试失败、未执行和跳过分别记录，只有实测 passed 才能贡献映射用例通过数。

## 4 准出与限制

十二项用例全部通过仅表示这些主机单元、模块集成与配置项台架断言通过。人工复核应评价需求正确性、用例充分性、未覆盖参数组合及异常。接口覆盖、语句/分支/MC/DC、时序与目标机验证留空，不能从需求映射数推算。故障版本必须真实触发 D 用例失败；修复后的新输入须重新验证。

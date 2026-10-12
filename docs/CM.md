# 配置项清单与证据管理

## 1 受控范围

CSCI-PID-SERVO。受控输入含 PLAN/SRS/SDD/IDD/SVP/STD/SCM-SQA/SAFETY/CM、`.factory` 配置与追踪、CMake、头文件、源码、测试、工程工具和 CI 工作流。模型、提示词、Skill 另有版本责任；确定性检查记录 model=not-used。

## 2 版本标识

Git HEAD 仅标识已提交基准；未提交输入必须用逐文件 SHA-256 与总快照哈希标识。报告不以 dirty=true 当作精确版本。每次采集独立目录，源文件在复制期间变化则拒绝，执行中工作区变化则报告 currentAtCompletion=false，实际测试结果仍属于已归档副本。

## 3 证据包

report.json 为检查规则版本、输入清单、执行结果和度量；commands.json 为真实工具命令及输出；source/build/report.xml 为实际 JUnit；report.md 供人工阅读。证据包保留需求映射与未采集覆盖项目。源码副本支持复核，但重新构建仍需相容工具链，不能宣称仅凭哈希实现位级二进制复现。

## 4 基线与变更

当前为工作版本，未正式批准基线。正式基线须包含指定版本人工评审、问题处置、配置审计和放行回执。改变需求时更新矩阵与用例；旧证据不自动适用于新输入。GitHub 需求和 Git 文档独立版本，不能声称跨系统原子冻结。

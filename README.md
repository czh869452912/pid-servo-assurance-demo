# PID 伺服控制配置项示例

这是用于演示 VS Code 原生 Harness 与软件工厂关联的原创教学项目。GitHub Issues 管理需求，Git 管理文档与代码，Actions 运行需求测试，PR 与 reviews 展示变更和人工评审。

配置项：CSCI-PID-SERVO。固定采样周期、比例/积分/测量微分、输出饱和、条件积分抗饱和、禁用复位与非法输入处理。

完整样件还包括监督层 SU-SERVO：无效传感器、重复/倒退/超时采样、故障锁存与显式恢复；合计12条需求、12项真实测试（8单元、3模块集成、1配置项台架仿真）。REQ-PID-009～012 是待评审台架衍生候选。GitHub生命周期联演在`codex/pid-lifecycle`分支维护，既有八需求主分支与故障PR独立保留。

[engineering.manifest.json](.factory/engineering.manifest.json)声明模块、文档、能力与验证级别；[过程定义](.factory/project.json)提供显式前置证据关系。[生命周期管理](docs/LIFECYCLE.md)说明真实Issue、变更、测试和评审的管理方式。工作面消费受控声明、[追溯矩阵](.factory/traceability.json)和真实GitHub观察，验证结果来自实际采集输入。

```sh
python3 tools/check_traceability.py
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure --output-junit report.xml
```

文档入口：[计划](docs/PLAN.md)、[需求](docs/SRS.md)、[设计](docs/SDD.md)、[验证](docs/SVP.md)、[配置与质量](docs/SCM-SQA.md)。追溯数据在 [.factory/traceability.json](.factory/traceability.json)，过程声明在 [.factory/project.json](.factory/project.json)。

补充：[接口](docs/IDD.md)、[测试说明](docs/STD.md)、[安全假设](docs/SAFETY.md)、[配置索引](docs/CM.md)。六要素测试规格在 `.factory/test-cases.json`，标准适用性与等级状态在 `.factory/assurance-profile.json`。主平台的工程证据工具对输入副本实际构建，报告不制造未采集的结构覆盖或批准。

`demo/derivative-sign-defect` 分支故意把测量微分的符号改错，用于展示失败 CI、待评审 PR 和团队诊断；它不是生产推荐代码。主分支提供正确实现。

GJB 438C 用于文档组织参考，DO-178C 用于生命周期、验证与证据组织参考，二者承担不同职责。依据 [FAA AC 20-115D](https://www.faa.gov/documentLibrary/media/Advisory_Circular/AC_20-115D.pdf) 的公开说明组织演示；不复制标准原文。本例不包含系统安全评估、安全等级分配、目标机验证、结构覆盖、工具鉴定或完整目标符合性分析，不构成满足 GJB 438C、DO-178C 或 GJB 5000B 的声明。

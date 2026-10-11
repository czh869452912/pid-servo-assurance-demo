# PID 伺服控制配置项示例

这是用于演示 VS Code 原生 Harness 与软件工厂关联的原创教学项目。GitHub Issues 管理需求，Git 管理文档与代码，Actions 运行需求测试，PR 与 reviews 展示变更和人工评审。

配置项：CSCI-PID-SERVO。固定采样周期、比例/积分/测量微分、输出饱和、条件积分抗饱和、禁用复位与非法输入处理。

```sh
python3 tools/check_traceability.py
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure --output-junit report.xml
```

文档入口：[计划](docs/PLAN.md)、[需求](docs/SRS.md)、[设计](docs/SDD.md)、[验证](docs/SVP.md)、[配置与质量](docs/SCM-SQA.md)。追溯数据在 [.factory/traceability.json](.factory/traceability.json)，过程声明在 [.factory/project.json](.factory/project.json)。

`demo/derivative-sign-defect` 分支故意把测量微分的符号改错，用于展示失败 CI、待评审 PR 和团队诊断；它不是生产推荐代码。主分支提供正确实现。

GJB 438C 用于文档组织参考，DO-178C 用于生命周期、验证与证据组织参考，二者承担不同职责。依据 [FAA AC 20-115D](https://www.faa.gov/documentLibrary/media/Advisory_Circular/AC_20-115D.pdf) 的公开说明组织演示；不复制标准原文。本例不包含系统安全评估、安全等级分配、目标机验证、结构覆盖、工具鉴定或完整目标符合性分析，不构成满足 GJB 438C、DO-178C 或 GJB 5000B 的声明。

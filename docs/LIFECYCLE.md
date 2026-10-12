# PID 工程生命周期与 GitHub 信息管理

配置项为 CSCI-PID-SERVO。受控主数据保存在 Git：需求SRS、设计SDD/IDD、测试SVP/STD、追溯矩阵、测试规格、过程定义及配置索引。Issue承担对象讨论、问题处置、任务、评审安排与状态；PR承担指定提交的变更和人工评审；Actions承担实际执行及证据归档，三类状态分别管理。

## 对象、标识与来源

需求为REQ-PID-001～012，设计为DD-PID-*或DD-SERVO-*，用例为TC-PID-*。每项需求、设计及用例分别建立Issue，标签为requirement/design/test。问题、变更、评审和过程事项分别用problem/change/review/process。所有记录使用机器标记，id是稳定领域标识，Issue编号只作GitHub定位：

```json
{"id":"PR-PID-001","kind":"problem","requirementIds":["REQ-PID-004"]}
```

JSON置于`<!-- aviation-object ... -->`注释中。需求Issue承载当前需求描述及讨论版本；冻结后的SRS、矩阵和设计以Git提交定位。Issue修改后产生新的观测版本，不能自动改变已冻结提交。重复ID、未知需求引用、缺少对应Issue或不可访问文件分别报错，不隐式回退本地模板。

## 工作流

1. 计划：维护PLAN、SAFETY、配置与质量记录，定义适用性、人员及生命周期资料。计划评审Issue保持未关闭，直至实际人工审查及记录完成。
2. 需求：逐条记录需求、上级来源、验证方法和接受准则。台架衍生需求009～012标注待评审，不能自行分配软件等级；独立审核反馈保留在来源。
3. 设计：将需求分配至明确设计ID、接口、模块及实现。设计Issue列明对应需求和受控文件，设计评审不从测试状态推导。
4. 验证：用例Issue包含初始状态、输入、执行、预期、容差和终止条件；实际用例名与CTest一致。Actions检查追溯引用、构建、执行12项测试，归档逐用例JUnit及verification.json。
5. 变更/问题：问题Issue记录观察版本、影响、复现和处置；变更PR引用需求/问题，分析受影响生命周期数据，按新head执行回归。故障演示PR独立保留，不合并。
6. 基线：完成指定版本的人工评审、问题处置、验证核对、配置索引及质量记录后，才可授权形成基线。阶段由受控过程定义声明，Actions不擅自推进阶段，关闭Issue不等于批准。

## 证据与交付包

Actions使用PR实际head或推送提交检出。证据包名为`lifecycle-evidence-<run-id>-<attempt>`，包含逐用例结果、输入文件SHA-256、实际sourceSha、workflowSha、run ID/attempt、矩阵、用例定义、过程和生命周期文档。适用性按实际sourceSha、运行次数及资产摘要确认；失败、跳过、未运行、缺失及过期证据分别呈现。成功的其他工作流不能替代需求测试结果，旧运行不证明新提交或本地未提交编辑。

拟交付配置包应包含受控源代码及接口、构建配置、需求/设计/测试资料、追溯、实际验证报告、问题处置、配置索引和质量记录，并定位同一提交；当前Actions包是验证资料包，不是已批准发布包。实际独立评审、等级相关结构覆盖、目标机验证及工具鉴定仍未完成。

## 标准适用性

依据FAA公开的AC 20-115D组织生命周期目标、活动和证据；该文件要求满足所分配软件等级的适用目标并形成相应生命周期资料。本样件未获得系统安全评估和软件等级分配，按DO-178C方向组织工程信息，不宣称完整符合性。GJB 5000B原文未提供，不建立未经核查的条款映射或标准等价结论。

变更影响包含需求、设计、接口、代码、测试、环境、工具及配置数据；这是结合FAA AC 00-69公开说明形成的样例工程约定。

来源：[FAA AC 20-115D](https://www.faa.gov/documentLibrary/media/Advisory_Circular/AC_20-115D.pdf)、[FAA AC 00-69](https://www.faa.gov/documentLibrary/media/Advisory_Circular/AC_00-69.pdf)、[GitHub Issues API](https://docs.github.com/en/rest/issues/issues)、[GitHub Actions证据接口](https://docs.github.com/en/rest/actions/artifacts)。

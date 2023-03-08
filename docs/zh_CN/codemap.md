---
title: 代码地图
---

- [代码地图](#代码地图)
  - [目录结构](#目录结构)
  - [模块介绍](#模块介绍)

# 代码地图

本文档旨在帮助您了解 zetton-common 代码的结构和功能。

## 目录结构

zetton-inference-tensorrt 代码的目录结构如下：

```bash
$ tree -L 3

.
├── cmake/
├── docs/
├── LICENSE
├── README.md
├── README_zh-CN.md
├── src/
│   ├── zetton_common_ros/
│   └── zetton_inference_ros/
└── tools/
```

其中：

- `.github/`：GitHub 相关的配置文件
- `cmake/` 与 `CMakeLists.txt`：CMake 构建相关的文件
- `docs/`：文档目录
- `src`：源代码目录
- `tools/`：工具脚本目录
- `LICENSE`：软件包许可证
- `README.md`：软件包说明文档
- `README_zh-CN.md`：软件包说明文档（中文）
- `package.xml`：软件包描述文件，用于 colcon 构建

## 模块介绍

zetton-ros-vendor 代码包含如下模块：

- `zetton_common_ros`: 基于 zetton-common 的 ROS 通用库
- `zetton_inference_ros`: 基于 zetton-inference 的 ROS 推理库

# 【tensorflow】TensorFlow技术内幕（一）：源码结构 - zkq_1986的博客 - CSDN博客





2018年06月09日 22:37:52[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：1392








转载自：https://blog.csdn.net/gaofeipaopaotang/article/details/80540630

本章讲解TensorFlow的源码结构。

下面的内容我们一TF1.0的代码为例，介绍TF的源码结构，如果是其他版本，则结构可能会略有差异。

## 根目录结构

首先整体看一下整体的目录结构：

![这里写图片描述](https://img-blog.csdn.net/20180601171924458?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dhb2ZlaXBhb3Bhb3Rhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

图1：TF根目录


然后了解一下每个目录的功能：
|目录|功能|
|----|----|
|tensorflow/c|C API代码|
|tensorflow/cc|C++ API代码|
|tensorflow/compiler|XLA,JIT等编译优化相关|
|tensorflow/contrib|contributor贡献的代码，这个目录并不是官方支持的， 很有可能在高级 API 完善后被官方迁移到核心的 TensorFlow 目录中或去掉|
|tensorflow/core|tf核心代码|
|tensorflow/docs_src|文档相关文件|
|tensorflow/examples|例子相关代码|
|tensorflow/g3doc|TF文档|
|tensorflow/go|go API相关代码|
|tensorflow/java|java API相关代码|
|tensorflow/python|Python API相关代码|
|tensorflow/stream_executor|并行计算框架代码|
|tensorflow/tools|各种辅助工具工程代码，例如第二章中生成Python安装包的代码就在这里|
|tensorflow/user_ops|tf插件代码|
|third_party/|依赖的第三方代码|
|tools|工程编译配置相关|
|util|工程编译相关|
表1：TF根目录


其中tensorflow/core是tf的核心模块

## 核心目录结构

![这里写图片描述](https://img-blog.csdn.net/20180601171937948?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dhb2ZlaXBhb3Bhb3Rhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

图2：TF Core目录


目录功能如下：
|目录|功能|
|----|----|
|tensorflow/core/common_runtime|公共运行库|
|tensorflow/core/debug|调试相关|
|tensorflow/core/distributed_runtime|分布式执行模块|
|tensorflow/core/example|例子代码|
|tensorflow/core/framework|基础功能模块|
|tensorflow/core/graph|计算图相关|
|tensorflow/core/grappler|模型优化模块|
|tensorflow/core/kernels|操作核心的实现代码，包括CPU和GPU上的实现|
|tensorflow/core/lib|公共基础库|
|tensorflow/core/ops|操作代码|
|tensorflow/core/platform|平台实现相关代码|
|tensorflow/core/protobuf|.proto定义文件|
|tensorflow/core/public|API头文件|
|tensorflow/core/user_ops||
|tensorflow/core/util||
表2：TF Core目录


## 小结

本章比较简短，粗略的梳理了一下源码目录结构。




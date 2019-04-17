# 终于！大家心心念念的PyTorch Windows官方支持来了 - 知乎
# 



> PyTorch 官方 GitHub 发布 0.4.0 版本，大家心心念念的 Windows 支持终于来了。
- GitHub 发布地址：[https://github.com/pytorch/pytorch/releases](https://link.zhihu.com/?target=https%3A//github.com/pytorch/pytorch/releases)
- PyTorch 官网：[http://pytorch.org/](https://link.zhihu.com/?target=http%3A//pytorch.org/)
![](https://pic2.zhimg.com/v2-3ca2b0f4b91a86a69957cfb970f3dbb9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='995' height='424'></svg>)
这一次更新从 0.3.1 到 0.4.0 有着非常多的改进，最重要的就是官方支持 Windows 系统。在没有官方支持前，Windows 上安装 PyTorch 需要借助其它开发者发布的第三方 conda 包，而现在我们可以直接在 PyTorch 首页上获取使用 conda 或 pip 安装的命令行，或跟随教程使用源文件安装。

当然除了 Windows 支持外，0.4.0 版的 PyTorch 还重点权衡了计算中的内存、支持多种概率分布、优化数据类型和修正张量等。机器之心也尝试在 Windows 安装简单的 CPU 版，如下所示我们使用 pip 可以非常轻松而流畅地安装 PyTorch。
![](https://pic2.zhimg.com/v2-4343fb5510a140bae8ea190e3e776ed1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='457'></svg>)
但当前使用 Conda 安装 PyTorch 会遇到一些问题，例如小编的 Conda 会报错说找不到对应的包。

此次版本更新目录如下：




**1.重大核心变化包括：**
- Tensor/Variable 合并
- 零维张量
- 数据类型
- 迁移指南




**2.现添加的新特征包括：**
- Tensors：
- 全面支持高级索引
- 快速傅立叶变换
- 神经网络：
- 计算时的存储权衡
- bottleneck-识别代码中热点（hotspots）的工具
- torch.distributions 
- 24 个基础的概率分布 
- 增加cdf、方差、信息熵、困惑度等
- 分布式训练 
- 易于使用的 Launcher utility
- NCCL2 后端
- C++拓展
- Windows 支持
- ONNX 改进
- RNN 支持

3.性能改进

4.Bug 修复



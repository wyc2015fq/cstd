# 【Pytorch 0.4.0】Windows操作系统下搭建Pytorch平台 - CD's Coding - CSDN博客





2018年04月25日 14:34:26[糖果天王](https://me.csdn.net/okcd00)阅读数：1269








## 0x00 前言

就在今天，PyTorch 官方 GitHub 发布 0.4.0 版本，[Pytorch主页](http://pytorch.org/)上的OS也令人愉快的增加了Windows的选项。 

这意味着PyTorch对于没有Linux/Unix系统，且没有远程开发机的学习者而言，更加易于上手便于使用了。 

而对于非初学者而言，这次的改进合并了Tensor和Variable，着实是一项非常伟大的改进， 

不仅如此，听说Caffe加入了PyTorch大家庭，在底层优化和新功能的支持也提供了帮助。
二话不说先给出解决方案：命令行下输入下述两行即可 

（如果python版本不为python3.6需要安装于自己python版本对应的文件） 
`pip install http://download.pytorch.org/whl/cu80/torch-0.4.0-cp36-cp36m-win_amd64.whl`
`pip install torchvision`
那么，今天就来试着给我的Surface装一个看看，顺便作为环境配置的备忘记录。 
![Windows](https://img-blog.csdn.net/20180425105343339?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29rY2QwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 0x01 更新内容

### 更新列表及简要吐槽

本次更新内容的完整详细版，见[Github Release](https://github.com/pytorch/pytorch/releases)中的 `0.4.0` 部分 

简要列举本次 `0.4.0` 版本的更新内容如下，括号内为自己的吐槽（中文翻译部分参考自[机器之心](http://mp.weixin.qq.com/s?__biz=MzA3MzI4MjgzMw==&mid=2650741377&idx=1&sn=3115999abcfa6c744cec334e636072f3&chksm=871adeffb06d57e9155c373255ad38c30fd06f82ba7d31105b77e810b0cc0632de9c58419628&scene=0#rd)）
- 重大核心变化 
- Tensor/Variable 合并（这一点包含了太多东西，之后看有没有时间为这一点单独写一篇） 
- （我认为需要单独拎出来说一下，`volatile`这个flag在该版本起就弃用了，变为无效flag）

- 零维张量（shape为[0, 2, 3]的张量现在变为允许，不报错了，动态生长树结构的时候曾受限于这一点）
- 数据类型（现在Tensor具有其对应的dtype属性定义了，详情可以见[这里](http://pytorch.org/docs/0.4.0/tensor_attributes.html#torch.torch.dtype)的官方文档）
- 迁移指南（友情提醒，新手请务必从`0.4.0`版本开始（简单），如果已有完整项目最好不要急着升级到`0.4.0`（改动多）)
- 新特征： 
- Tensors： 
- 全面支持高级索引（现在Tensors索引不知能否到达Numpy的易用程度了，Tensorflow先前在这一点上优于PyTorch）
- 快速傅立叶变换（这个以前写某个BP解析的时候提到过PyTorch的高阶求导效果受限于其FFT未实现，现在完成了）

- 神经网络： 
- 计算时的存储权衡（之前有些呆板的内存共享，比较影响编码流畅性，没有reshape只有view便是其导致结果之一）
- bottleneck-识别代码中热点（hotspots）的工具（这个还不太懂，后期去看看，但都说是瓶颈了应该蛮厉害）

- torch.distributions 
- 24 个基础的概率分布
- 增加cdf、方差、信息熵、困惑度等

- 分布式训练 
- 易于使用的 Launcher utility
- NCCL2 后端

- C++拓展
- **Windows 支持！**
- ONNX 改进
- RNN 支持
- 性能改进
- Bug 修复

### tensor的高级索引

> 
单独把索引部分挑出来做个例子，也顺便作为给新入门的同学简要看一下PyTorch在数据处理上大概的写法


```python
a = torch.rand(10, 10, 10, 10)

# the indexing elements can have other shapes than 1
b = a[[[3, 2]], :, [[1, 3]]]  # NICE! EXCELLENT!

# broadcasting also supported in the indices, as well as lists,
# negative indices, slices, elipses, numbers
c = a[[1, -2], 2:4, :, [1]]

# can also support tensors as indices
index = torch.tensor([2, 4])
d = a[index]  # NICE!

# and the indices can be on the GPU
# or CPU
e = a[index.cuda()]
f = a.cuda()[index]

mask = torch.rand(10) > 0.5
# we can now index with a mask that has fewer
# dimensions than the indexing tensor
c = a[mask, :5]
```

## 0x02 环境的安装与配置

首先打开你的命令行，直接敲python，第一行就能看到你的python版本

```
C:\Users\okcd0>python
Python 3.6.0 |Anaconda 4.3.1 (64-bit)| (default, Dec 23 2016, 11:57:41) [MSC v.1900 64 bit (AMD64)] on win32
Type "help", "copyright", "credits" or "license" for more information.

>>> exit()
```

得知自己是python3.6，那么去[官方主页](http://pytorch.org/)上选中python3.6和Windows， 

cuda…… 鉴于cuda装起来还是相当费劲的，而且刚入门的时候大家主要还是使用CPU进行训练的， 

所以我们主要讲PyTorch在CPU上的训练与使用，这里就先不讲CUDA相关的部分了，随便选一个好啦~
这里的python3.6就是文件名中的`cp36`了，所以我们使用这两个命令来安装pytorch 
`pip install http://download.pytorch.org/whl/cu80/torch-0.4.0-cp36-cp36m-win_amd64.whl`
`pip install torchvision`
```
C:\Users\okcd0>pip install http://download.pytorch.org/whl/cu80/torch-0.4.0-cp36-cp36m-win_amd64.whl
Collecting torch==0.4.0 from http://download.pytorch.org/whl/cu80/torch-0.4.0-cp36-cp36m-win_amd64.whl
  Downloading http://download.pytorch.org/whl/cu80/torch-0.4.0-cp36-cp36m-win_amd64.whl (579.1MB)
    13% |████▌                           | 80.8MB 232kB/s eta 0:35:42
```

这文件还不小，所以现需要经过一段时间的等待， 

安装完成……就可以直接用了……

```
Downloading http://download.pytorch.org/whl/cu80/torch-0.4.0-cp36-cp36m-win_amd64.whl (579.1MB)
    100% |████████████████████████████████| 579.1MB 71kB/s
Installing collected packages: torch
Successfully installed torch-0.4.0

C:\Users\okcd0>python
Python 3.6.0 |Anaconda 4.3.1 (64-bit)| (default, Dec 23 2016, 11:57:41) [MSC v.1900 64 bit (AMD64)] on win32
Type "help", "copyright", "credits" or "license" for more information.
>>> import torch
>>> torch.zeros([2,3])
tensor([[ 0.,  0.,  0.],
        [ 0.,  0.,  0.]])
>>>
```











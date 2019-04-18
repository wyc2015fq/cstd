# 分布式机器学习框架：MxNet - wishchinYang的专栏 - CSDN博客
2016年07月12日 09:57:48[wishchin](https://me.csdn.net/wishchin)阅读数：3025
     MxNet官网： [http://mxnet.readthedocs.io/en/latest/](http://mxnet.readthedocs.io/en/latest/)
**前言：**
caffe是很优秀的dl平台。影响了后面很多相关框架。
cxxnet借鉴了很多caffe的思想。相比之下，cxxnet在实现上更加干净，例如依赖很少，通过mshadow的模板化使得gpu和cpu代码只用写一份，分布式接口也很干净。
mxnet是cxxnet的下一代，目前实现了cxxnet所有功能，但借鉴了minerva/torch7/theano，加入更多新的功能。
- ndarray编程接口，类似matlab/numpy.ndarray/torch.tensor。独有优势在于通过背后的engine可以在性能上和内存使用上更优
- symbolic接口。这个可以使得快速构建一个神经网络，和自动求导。
- 更多binding 目前支持比较好的是python，马上会有julia和R
- 更加方便的多卡和多机运行
- 性能上更优。目前mxnet比cxxnet快40%，而且gpu内存使用少了一半。
目前mxnet还在快速发展中。这个月的主要方向有三，更多的binding，更好的文档，和更多的应用（language model、语音，机器翻译，视频）。地址在[dmlc/mxnet · GitHub](https://github.com/dmlc/mxnet/releases)
**官方简介：            **
       MXNet is a deep learning framework designed for both *efficiency* and*flexibility*.It allows you to***mix*** the[flavours](http://mxnet.readthedocs.io/en/latest/system/program_model.html)
 of symbolicprogramming and imperative programming to ***maximize*** efficiency and productivity.In its core, a dynamic dependency scheduler that automatically parallelizes both symbolic and imperative operations on the fly.A graph optimization
 layer on top of that makes symbolic execution fast and memory efficient.The library is portable and lightweight, and it scales to multiple GPUs and multiple machines.
       MXNet is also more than a deep learning project. It is also a collection of[blue prints and guidelines](http://mxnet.readthedocs.io/en/latest/system/index.html#system-design-note) for buildingdeep learning
 system, and interesting insights of DL systems for hackers.
      MxNet混合了符号式设计和命令式设计，来最大化效率和提高产出。其核心是一个动态调度器，不停的并行执行符号和命令操作。顶层的图优化层使符号执行快速且有效。这个包轻量级可移植性好，并且可以扩展到多GPU和多个机器。
      MxNet不仅是一个深度学习工程，并且是一个为构建DL系统提供蓝图和指导的集合，并且为hackers 提供了一个有趣的视野。
**最新发展**
## What's New
- [MXNet Memory Monger, Training Deeper Nets with Sublinear Memory Cost](https://github.com/dmlc/mxnet-memonger)
- [Tutorial for NVidia GTC 2016](https://github.com/dmlc/mxnet-gtc-tutorial)
- [Embedding Torch layers and functions in MXNet](http://mxnet.readthedocs.org/en/latest/how_to/torch.html)
- [MXNet.js: Javascript Package for Deep Learning in Browser (without server)](https://github.com/dmlc/mxnet.js/)
- [Design Note: Design Efficient Deep Learning Data Loading Module](http://mxnet.readthedocs.org/en/latest/system/note_data_loading.htmll)
- [MXNet on Mobile Device](http://mxnet.readthedocs.org/en/latest/how_to/smart_device.html)
- [Distributed Training](http://mxnet.readthedocs.org/en/latest/how_to/multi_devices.html)
- [Guide to Creating New Operators (Layers)](http://mxnet.readthedocs.org/en/latest/how_to/new_op.html)
- [Amalgamation and Go Binding for Predictors](https://github.com/jdeng/gomxnet/)
- [Training Deep Net on 14 Million Images on A Single Machine](http://mxnet.readthedocs.org/en/latest/tutorials/imagenet_full.html)
- MxNet的内存管理：子线性的内存代价
- NVIDIA GTC2016上的 教程
- 嵌入 Torch网络层和函数 到MxNet
- MxNet.js : 可运行到浏览器中的javascript包
- 设计节点：设计有效的深度学习数据载入模型
- 移动设备的上的 Mxnet
- 分布式训练方法
- 网络层 的运算符重载
- 使用一个深度网络 训练1400万张图片
## Contents
- [Documentation and Tutorials](http://mxnet.readthedocs.org/en/latest/)
- [Design Notes](http://mxnet.readthedocs.org/en/latest/system/index.html)
- [Code Examples](https://github.com/dmlc/mxnet/blob/master/example)
- [Installation](http://mxnet.readthedocs.org/en/latest/how_to/build.html)
- [Pretrained Models](https://github.com/dmlc/mxnet-model-gallery)
- [Contribute to MXNet](http://mxnet.readthedocs.org/en/latest/how_to/contribute.html)
- [Frequent Asked Questions](http://mxnet.readthedocs.org/en/latest/how_to/faq.html)
## [](https://github.com/dmlc/mxnet/#features)Features
- Design notes providing useful insights that can re-used by other DL projects
- Flexible configuration for arbitrary computation graph
- Mix and match good flavours of programming to maximize flexibility and efficiency
- Lightweight, memory efficient and portable to smart devices
- Scales up to multi GPUs and distributed setting with auto parallelism
- Support for python, R, C++ and Julia
- Cloud-friendly and directly compatible with S3, HDFS, and Azure
## [](https://github.com/dmlc/mxnet/#ask-questions)Ask Questions
- Please use [mxnet/issues](https://github.com/dmlc/mxnet/issues) for how to use mxnet and reporting bugs
## [](https://github.com/dmlc/mxnet/#license)License
© Contributors, 2015. Licensed under an [Apache-2.0](https://github.com/dmlc/mxnet/blob/master/LICENSE) license.
## [](https://github.com/dmlc/mxnet/#reference-paper)Reference Paper
Tianqi Chen, Mu Li, Yutian Li, Min Lin, Naiyan Wang, Minjie Wang, Tianjun Xiao,Bing Xu, Chiyuan Zhang, and Zheng Zhang.[MXNet: A Flexible
 and Efficient Machine Learning Library for Heterogeneous Distributed Systems](https://github.com/dmlc/web-data/raw/master/mxnet/paper/mxnet-learningsys.pdf).In Neural Information Processing Systems, Workshop on Machine Learning Systems, 2015
## [](https://github.com/dmlc/mxnet/#history)History
MXNet is initiated and designed in collaboration by the authors of [cxxnet](https://github.com/dmlc/cxxnet), [minerva](https://github.com/dmlc/minerva) and[purine2](https://github.com/purine/purine2). The project reflects what we have learnt from the past projects. It combines important flavours of the existing projects for efficiency, flexibility and
 memory efficiency.

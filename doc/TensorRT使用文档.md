# TensorRT使用文档



# 

发表于2018-05-07 15:14| 来源NVIDIA| 作者NVIDIA

[人工智能](http://www.csdn.net/tag/%E4%BA%BA%E5%B7%A5%E6%99%BA%E8%83%BD/news)[深度学习](http://www.csdn.net/tag/%E6%B7%B1%E5%BA%A6%E5%AD%A6%E4%B9%A0/news)

**摘要：**NVIDIA TensorRT™是一个C ++库，可以帮助NVIDIA图形处理器（GPU）进行高性能推理。 TensorRT通过合并张量和图层，转换权重，选择高效的中间数据格式，并根据图层参数和测量的性能从大型内核目录中进行选择，从而对网络进行定义并对其进行优化。

TensorRT下载地址：<https://developer.nvidia.com/nvidia-tensorrt-download>

TesnsoRT介绍文档：<https://devblogs.nvidia.com/tensorrt-3-faster-tensorflow-inference/>

TensorRT开发者指南：<http://docs.nvidia.com/deeplearning/sdk/tensorrt-developer-guide/index.html>

TensorRT样例代码：<http://docs.nvidia.com/deeplearning/sdk/tensorrt-developer-guide/index.html#samples>

 

Overview

NVIDIA TensorRT™是一个C ++库，可以帮助NVIDIA图形处理器(GPU)进行高性能推理。 TensorRT通过合并张量和图层，转换权重，选择高效的中间数据格式，并根据图层参数和测量的性能从大型内核目录中进行选择，从而对网络进行定义并对其进行优化。

TensorRT包含导入方法，可帮助您为TensorRT表达训练有素的深度学习模型以优化和运行。 它是一种优化工具，可以应用图形优化和图层融合，并利用各种高度优化的内核集合找到该模型的最快实现，以及可用于在推理上下文中执行此网络的运行时方法。

TensorRT包含一个基础架构，可让您利用Pascal和Volta GPU的高速降低精度功能作为可选优化。

TensorRT 是基于GCC4.8的。

\1. TensorRT Layers

TensorRT可以直接支持下面类型的网络层：

**Activation(激活层)：**

激活层是每个元素的激活方法，它目前支持一下几种类型的激活层：

ReLU，tanh，sigmod

**Concatenation(连接层)**：

连接层链接通过通道维度链接多个相同高度和宽度的张量

**Convolution (卷基层)：**

卷基层是一个三维的卷积操作，有时会加入bias(偏置量)

**Deconvolution**

反卷基层是一个反卷积操作，有时会加入bias(偏置量)

**ElementWise**

ElementWise层是一些常用的元素操作。目前支持的有： sum, product, maximum, subtraction, division **和** power.

**Flatten**

Flatten层的作用是使输入的张量在大小不变的情况下，变得扁平化。举个例子：一个输入张量形状为：n*c*w*h(4维);输出为一个大小为n*(c*w*h)(一维)的向量

**FullyConnected**

FullyConnected实现的是一个矩阵向量的乘积，有时会加入bias(偏置量)

**LRN**

LRN层实现了一个跨通道的局部响应归一化

**Padding**

Padding层实现了在空间上的张量的零填充。填充可以是在每个轴上不对称的，正的(使张量膨胀)，负的(使张量剪裁)

**Plugin**

Plugin层允许您自定义网络层的实现，而这些网络层可能是TensorRT不支持的。

**Pooling**

Pooling层在一个通道上实现了池化操作。支持的池化类型有最大和平均。

**RNN**

RNN层实现了递归网络层。它包括：RNN，GRU(Gated Recurrent Units)，LSTM(Long Short-term memory)

**Scale**

Scale层实现了每一个张量，每一个通道或者每一个权重的仿射变换和常数求幂

**Shuffle**

Shuffle层实现了张量的调整。它可以用于改变数据的形状或者顺序

**SoftMax**

SoftMax层实现了跨通道的SoftMax操作

**Squeeze**

Squeeze层从张量形状中移除尺寸为1的维度。 Squeeze层仅实现二元挤压(去除特定尺寸1维)。 批量维度无法删除。

**Unary**

Unary层支持点态一元操作。支持的操作有**exp、log、sqrt、replp、abs、neg**。

\2. Key Concepts

请确定您熟悉以下内容：

**Network definition**

神经网络的定义，它包括了一个按顺序排列好的网络层，以及一个张量的集合

**Layer**

每一个网络层输入一系列的张量，计算并输出一系列的张量。每一个网络层都有一些参数，举个例子：在卷基层中有：**convolution size, stride，convolution filter weights。**

**Tensor**

一个张量是一个网络层的输入，或者是一个网络层的输出。张量会有数据类型来详述他们的精度，举个例子：16位或者32位的浮点数(16-bit float or 32-bit float)，3维(3-dimensions)，通道(channel)，宽和高。输入张量的尺寸由应用程序定义，对于输出张量，则由生成器推断。

所支持的维数为n(p1p2...)chw，其中p1，p2等是索引维数。张量最多有**Dims::MAX_DIMENSIONS维度**，其中该常数设置为8。

每个网络层和张量都有一个名称，在分析或读取tensorrt构建日志时非常有用。

当使用NvCaffeParser时, 张量和网络层的名字取自NVCaffe prototxt文件中。

\3. TensorRT API's

TensorRT的API允许开发人员导入、校准、生成和部署优化的网络。网络可以直接从NVCaffe导入，也可以通过UFF格式从其他框架导入。它们也可以通过实例化各个层和直接设置参数和权重来创建。

除了早期的C++接口，TensorRT还绑定了python的接口。TensorRT的Python接口支持除了RNN以外的所有功能。它通过使用Numpy作为网络层权重和使用PyCUDA，使它的输出和输出具有通用性。它会提供一系列的功能，来解决开发者经常面临的问题。这些功能包括：NVCaffe 模型分析，在一个流中的UFF模型分析，还有在一个UFF文件中读取和写入PLAN文件。这些功能更都在 tensort.utils中。

tensorrt/examples/custom_layers提供了一个工作流程给那些希望用Python实现传统网络层的开发者。

3.1 Python Samples

Python接口支持之前提供的所有的C++的接口:

‣ NvCaffeParser

‣ Nvinfer的图定义API

‣ 优化推理引擎的构建器

‣ 用于计算推理引擎执行时间的接口

‣ 用于注册自定义层实现的调用。

你可以在**{PYTHON_PACKAGE_DIR}/tensorrt/ examples**这个地址里面找到Python的例子

TensorRT包附带了几个示例应用程序实现。根据您是在系统中安装TensorRT，还是只为用户安装，可以找到这些选项。

一些样例程序的位置：

![img](http://articles.csdn.net/uploads/allimg/180508/260_180508201340_1.jpg)

For more information about importing a trained model to TensorRT using Python, see NVCaffe Python Workflow, TensorFlow Python Workflow, and Converting A Model From An Unsupported Framework To TensorRT With The TensorRT Python API.

对于如何使用Python导入训练好的模型到TensorRT，您可以查看NVCaffe Python Workflow, TensorFlow Python Workflow, 和 Converting A Model From An Unsupported Framework To TensorRT With The TensorRT Python API.
# MXNet开放支持Keras，高效实现CNN与RNN的分布式训练 - 知乎
# 



选自AWS Machine Learning Blog，作者：Lai Wei、Kalyanee Chendke、Aaron Markham、Sandeep Krishnamurthy，机器之心编译。

> 今日 AWS 发布博客宣布 Apache MXNet 已经支持 Keras 2，开发者可以使用 Keras-MXNet 深度学习后端进行 CNN 和 RNN 的训练，安装简便，速度提升，同时支持保存 MXNet 模型。

感谢 Keras 和 Apache MXNet（孵化）开源项目的贡献者，Keras-MXNet 深度学习后端现已可用。

地址：[https://github.com/awslabs/keras-apache-mxnet](https://link.zhihu.com/?target=https%3A//github.com/awslabs/keras-apache-mxnet)

Keras 是用 Python 编写的高级神经网络 API，因其快速、简单的 CNN 和 RNN 原型而广受欢迎。

Keras 开发人员现在可以使用高性能的 MXNet 深度学习引擎展开卷积神经网络（CNN）和循环神经网络（RNN）的分布式训练。通过更新几行代码，Keras 开发人员可以利用 MXNet 的多 GPU 分布式训练功能来提高训练速度。保存 MXNet 模型是该版本的另一个宝贵功能。你可以在 Keras 中进行设计，利用 Keras-MXNet 进行训练，并使用 MXNet 在生产中运行大规模推断。




**用 Keras 2 和 MXNet 进行分布式训练**

本文介绍了如何安装 Keras-MXNet，以及如何训练 CNN 和 RNN。如果你以前使用过其它深度学习引擎进行分布式训练，那你可能了解其中的难度和无趣。本文将展示如何使用 Keras-MXNet 进行训练。




**安装仅需几步**

1. 部署 AWS 深度学习 AMI

2. 安装 Keras-MXNet

3. 配置 Keras-MXNet




**1. 部署 AWS 深度学习 AMI**

按照 AWS 深度学习 AMI（DLAMI）部署教程来操作，教程地址：[https://aws.amazon.com/getting-started/tutorials/get-started-dlami/](https://link.zhihu.com/?target=https%3A//aws.amazon.com/getting-started/tutorials/get-started-dlami/)。要利用多 GPU 训练示例，请启用 p3.8xlarge 或类似的多 GPU 实例类型。

想安装用于运行 CUDA、Keras、MXNet 和其他框架（如 TensorFlow）的依赖项？请根据 Keras-MXNet 安装指南进行操作：[https://github.com/awslabs/keras-apache-mxnet/blob/master/docs/mxnet_backend/installation.md](https://link.zhihu.com/?target=https%3A//github.com/awslabs/keras-apache-mxnet/blob/master/docs/mxnet_backend/installation.md)。




**2. 安装 Keras-MXNet**

在 DLAMI 上的 MXnet Conda 环境中安装 Keras-MXnet 及其依赖项。它已经包含 Keras 1.0 版，因此你需要先卸载此版本。登录到 DLAMI 并运行以下命令：


```
# Activate the MXNet Python 3 environment on the DLAMI
$ source activate mxnet_p36

# Install a dependency needed for Keras datasets
$ pip install h5py

# Uninstall older versions Keras-MXNet
$ pip uninstall keras-mxnet

# Install Keras-MXNet v2.1.6 
$ pip install keras-mxnet
```


Keras-MXnet 及其依赖项现已安装在 DLAMI 上的 MXnet Conda 环境中。




**3. 验证 Keras-MXNet 安装是否正确**

使用以下代码验证 Keras 是否正在运行 MXNet 后端：


```
$ python
>>>import keras as k
   Using MXNet backend
```





**支持 CNN**

现在我们在 CIFAR-10 数据集上训练 ResNet 模型，来识别 10 个类别：飞机、汽车、鸟、猫、鹿、狗、青蛙、马、船和卡车。我们可以使用 Keras-MXNet repo 示例部分中的 Keras 2 示例脚本（[https://github.com/awslabs/keras-apache-mxnet/blob/master/examples/cifar10_resnet_multi_gpu.py](https://link.zhihu.com/?target=https%3A//github.com/awslabs/keras-apache-mxnet/blob/master/examples/cifar10_resnet_multi_gpu.py)）。使用 MXNet 作为 Keras 的后端几乎不需要对脚本进行什么更新。

首先，从 Keras-MXNet repo 文件夹中下载示例脚本：

`$ wget https://raw.githubusercontent.com/awslabs/keras-apache-mxnet/master/examples/cifar10_resnet_multi_gpu.py`

脚本启用 multi_gpu_model API，并输入要使用的 GPU 数量。

然后，在终端窗口运行 nvidia-smi，以确定 DLAMI 上可用 GPU 的数量。接下来，如果你有四个 GPU，那么你需要原样运行该脚本；否则，你需要运行以下命令来打开脚本进行编辑：

`$ vi cifar10_resnet_multi_gpu.py`

该脚本具备以下行，该行定义 GPU 的数量。如有必要，更新该行。

`model = multi_gpu_model(model, gpus=4)`

现在，开始训练。

`$ python cifar10_resnet_multi_gpu.py`

（可选）训练过程中，使用 nvidia-smi 命令检查 GPU 利用和内存使用。打开另一个终端会话。




**支持 RNN**

Keras-MXNet 目前提供对 RNN 的实验性支持。在使用 RNN 和 MXNet 后端时有一些局限性。更多信息，请查看 Keras-MXNet 文档：[https://github.com/awslabs/keras-apache-mxnet/blob/master/docs/mxnet_backend/using_rnn_with_mxnet_backend.md](https://link.zhihu.com/?target=https%3A//github.com/awslabs/keras-apache-mxnet/blob/master/docs/mxnet_backend/using_rnn_with_mxnet_backend.md)。此处的示例包括使用 LSTM 层训练 IMDB 数据集时需要的一些变通方案。尽管有这些方案，但在多 GPU AMI 上训练 RNN 会比你之前的训练经验容易一些，速度也更快。

使用 imdb_lstm 示例脚本：[https://github.com/awslabs/keras-apache-mxnet/blob/master/examples/imdb_lstm.py](https://link.zhihu.com/?target=https%3A//github.com/awslabs/keras-apache-mxnet/blob/master/examples/imdb_lstm.py)。将输入长度传输到嵌入层，按以下说明设置 unroll=True。

首先，在 DLAMI 的终端会话中，从 Keras-MXNet repo 文件夹中下载示例脚本：

`$ wget https://raw.githubusercontent.com/awslabs/keras-apache-mxnet/master/examples/imdb_lstm.py`

然后，打开脚本，跳至下列行进行 review：


```
model.add(Embedding(max_features, 128, input_length=maxlen))
model.add(LSTM(128, unroll=True))
```


现在，示例脚本已经过修改，可与 MXNet 后端兼容，你可以运行以下行：

`$ python imdb_lstm.py`

（可选）训练过程中使用 nvidia-smi 命令检查 GPU 利用和内存使用。打开另一个终端会话进行该操作。




**基准**

为了帮助大家评估不同 Keras 后端的性能，AWS 向 Keras-MXNet 添加了一个基准模块。按表中描述在 CPU、单个 GPU 和多 GPU 上使用不同的模型和数据集，你会发现 Keras-MXNet 训练 CNN 的速度更快，且在多个 GPU 上实现高效的性能提升。详见训练速度柱状图。关于如何运行基准脚本以及生成详细的基准结果，详见 Keras Benchmarks readme 文档：[https://github.com/awslabs/keras-apache-mxnet/tree/master/benchmark](https://link.zhihu.com/?target=https%3A//github.com/awslabs/keras-apache-mxnet/tree/master/benchmark)。




**基准配置**
- Keras Version 2.1.6
- MXNet Version 1.2.0
- Image Data Format: Channel first

训练 CIFAR10 数据集导致子线性缩放，因为该数据集图像更小。CIFAR10 数据集包含 5 万张图像，每张图像的大小是 32×32 像素。使用四个 GPU 传输这些小图像所需的通信开销比使用八个 GPU 的开销高。ImageNet 和合成数据数据集更好地展示了 Keras-MXNet 可能带来的性能改进。详见下表。
![](https://pic4.zhimg.com/v2-8f5124878b7a6c7c1a5986a305b1f5d7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='477' height='270'></svg>)
**使用 Keras-MXNet 的图像处理速度对比**
![](https://pic4.zhimg.com/v2-6affa5495016032249da9c000dcb0497_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='817' height='281'></svg>)
**接下来**

尝试其他的 Keras-MXNet 教程或阅读版本注释。
- Keras-MXNet 教程地址：[https://github.com/awslabs/keras-apache-mxnet/tree/master/docs/mxnet_backend](https://link.zhihu.com/?target=https%3A//github.com/awslabs/keras-apache-mxnet/tree/master/docs/mxnet_backend)
- 版本注释：[https://github.com/awslabs/keras-apache-mxnet/releases/tag/v2.1.6](https://link.zhihu.com/?target=https%3A//github.com/awslabs/keras-apache-mxnet/releases/tag/v2.1.6)
![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)
原文链接：[https://aws.amazon.com/cn/blogs/machine-learning/apache-mxnet-incubating-adds-support-for-keras-2/](https://link.zhihu.com/?target=https%3A//aws.amazon.com/cn/blogs/machine-learning/apache-mxnet-incubating-adds-support-for-keras-2/)



# TensorFlow教程 1 Tensorflow简介 - 博客堂 - CSDN博客





2017年07月12日 21:44:12[最小森林](https://me.csdn.net/u012052268)阅读数：800








# 1 Tensorflow简介


- [Tensorflow简介](#1-tensorflow简介)- [1 为什么选 Tensorflow](#11-为什么选-tensorflow)
- [2 安装](#12-安装)- [21 安装 Tensorflow](#121-安装-tensorflow)- [Linux 和 MacOS](#linux-和-macos)
- [Windows](#windows)


- [3 神经网络在干嘛](#13-神经网络在干嘛)




本系列博客是学习《莫烦tensorflow视频教程》的读书笔记，非常推荐他关于机器学习和深度学习的视频。这个是他的个人网站：[https://morvanzhou.github.io/tutorials/machine-learning/tensorflow/1-3-what-does-NN-do/](https://morvanzhou.github.io/tutorials/machine-learning/tensorflow/1-3-what-does-NN-do/)

## 1.1 为什么选 Tensorflow
- 
TensorFlow是Google开发的一款神经网络的Python外部的结构包, 也是一个采用数据流图来进行数值计算的开源软件库.

- 
TensorFlow 让我们可以先绘制计算结构图, 也可以称是一系列可人机交互的计算操作, 然后把编辑好的Python文件 转换成 更高效的C++, 并在后端进行计算.

- 
TensorFlow 无可厚非地能被认定为 神经网络中最好用的库之一. 它擅长的任务就是训练深度神经网络.通过使用TensorFlow我们就可以快速的入门神经网络, 大大降低了深度学习（也就是深度神经网络）的开发成本和开发难度.

- 
TensorFlow 的开源性, 让所有人都能使用并且维护, 巩固它. 使它能迅速更新, 提升.


## 1.2 安装

安装 Tensorflow 时需要注意的几点:
- MacOS, Linux, Windows 系统均已支持 Tensorflow
- 确定你的 python 版本
- 你的 GPU 是 NVIDIA, 就可以安装 GPU 版本的 Tensorflow; 你的 GPU 不是 NVIDIA 也没有关系, 安装 CPU 版本的就好了.

### 1.2.1 安装 Tensorflow

#### Linux 和 MacOS

本文将提到第一种最简单的安装方式, pip 安装. 使用 pip 安装的时候要确保你的 pip 已经存在于你的电脑中. 如果还没有安装 pip. 你可以在 Terminal 窗口中运行这个:

```bash
# Ubuntu/Linux 64-位 系统的执行代码:
$ sudo apt-get install python-pip python-dev

# Mac OS X 系统的执行代码:
$ sudo easy_install pip
$ sudo easy_install --upgrade six
```

CPU 版

Tensorflow (0.12) 刚刚做了更新, 绕过了复杂的安装步骤, 如果你只需要安装 CPU 版本的 Tensorflow, 运行下面这个就好了:

```
# python 2+ 的用户:
$ pip install tensorflow

# python 3+ 的用户:
$ pip3 install tensorflow
```

GPU 版

如果你想安装 GPU 版的 Tensorflow, 你要在下面找一个适合你版本的安装文件, 并在 terminal 中执行:

```
# Ubuntu/Linux 64-bit, CPU only, Python 2.7
$ export TF_BINARY_URL=https://storage.googleapis.com/tensorflow/linux/cpu/tensorflow-0.12.0rc0-cp27-none-linux_x86_64.whl

# Ubuntu/Linux 64-bit, GPU enabled, Python 2.7
# Requires CUDA toolkit 8.0 and CuDNN v5. For other versions, see "Installing from sources" below.
$ export TF_BINARY_URL=https://storage.googleapis.com/tensorflow/linux/gpu/tensorflow_gpu-0.12.0rc0-cp27-none-linux_x86_64.whl

# Mac OS X, CPU only, Python 2.7:
$ export TF_BINARY_URL=https://storage.googleapis.com/tensorflow/mac/cpu/tensorflow-0.12.0rc0-py2-none-any.whl

# Mac OS X, GPU enabled, Python 2.7:
$ export TF_BINARY_URL=https://storage.googleapis.com/tensorflow/mac/gpu/tensorflow_gpu-0.12.0rc0-py2-none-any.whl

# Ubuntu/Linux 64-bit, CPU only, Python 3.4
$ export TF_BINARY_URL=https://storage.googleapis.com/tensorflow/linux/cpu/tensorflow-0.12.0rc0-cp34-cp34m-linux_x86_64.whl

# Ubuntu/Linux 64-bit, GPU enabled, Python 3.4
# Requires CUDA toolkit 8.0 and CuDNN v5. For other versions, see "Installing from sources" below.
$ export TF_BINARY_URL=https://storage.googleapis.com/tensorflow/linux/gpu/tensorflow_gpu-0.12.0rc0-cp34-cp34m-linux_x86_64.whl

# Ubuntu/Linux 64-bit, CPU only, Python 3.5
$ export TF_BINARY_URL=https://storage.googleapis.com/tensorflow/linux/cpu/tensorflow-0.12.0rc0-cp35-cp35m-linux_x86_64.whl

# Ubuntu/Linux 64-bit, GPU enabled, Python 3.5
# Requires CUDA toolkit 8.0 and CuDNN v5. For other versions, see "Installing from sources" below.
$ export TF_BINARY_URL=https://storage.googleapis.com/tensorflow/linux/gpu/tensorflow_gpu-0.12.0rc0-cp35-cp35m-linux_x86_64.whl

# Mac OS X, CPU only, Python 3.4 or 3.5:
$ export TF_BINARY_URL=https://storage.googleapis.com/tensorflow/mac/cpu/tensorflow-0.12.0rc0-py3-none-any.whl

# Mac OS X, GPU enabled, Python 3.4 or 3.5:
$ export TF_BINARY_URL=https://storage.googleapis.com/tensorflow/mac/gpu/tensorflow_gpu-0.12.0rc0-py3-none-any.whl
```

最后, 根据自己的 python 版本, 在 terminal 中执行以下语句:

```bash
# 如果你是 Python 2, 请复制下面
$ sudo pip install --upgrade $TF_BINARY_URL

# 如果你是 Python 3, 请复制下面
$ sudo pip3 install --upgrade $TF_BINARY_URL
```

#### Windows

## 1.3 神经网络在干嘛

模型通过学习数据, 得到能表达数据的参数,然后对我们另外给的数据所作出预测.




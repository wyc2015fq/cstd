# macOS上安装TensorFlow并在Jupyter Notebook中使用 - 硬曲奇小屋 - CSDN博客





2018年03月08日 10:09:28[HardCookies](https://me.csdn.net/james_616)阅读数：1551








# 1 TensorFlow概要

[TensorFlow](www.tensorflow.org)是用于表达机器学习算法的接口，以及用于执行这些算法的实现。使用TensorFlow表达的计算可以在各种各样的异构系统上执行，从移动设备（如手机和平板电脑）到数百台大型分布式系统以及数千个GPU卡等计算设备。该系统灵活多变，可用于表达各种算法，包括深度神经网络模型的训练和推理算法，并已用于开展研究并将机器学习系统部署到十几个领域计算机科学和其他领域，包括语音识别，计算机视觉，机器人技术，信息检索，自然语言处理，地理信息提取和计算药物发现。

# 2 在macOS上安装TensorFlow

参见TensorFlow的[官方教程](https://www.tensorflow.org/install/install_mac?hl=zh-cn)，官方建议使用Virtualenv（虚拟环境）安装。假设我们之前已经在你的macOS上安装了Anaconda，它已经预装了很多科学计算库。下面我们讲解如何使用Virtualenv安装TensorFlow。

## 2.1 使用Virtualenv安装

分别执行以下步骤来安装：
- 打开终端，执行以下全部操作。
- 安装Vituralenv通过命令 
`$ pip install --upgrade virtualenv`
- 创建一个Virtual虚拟环境 
`$ virtualenv --system-site-packages -p python3 targetDirectory # for Python 3.n`

targetDirectory是你的目标路径，这里我们采用的目标路径是*~/tensorflow*，你也可以算则其他路径。- 激活虚拟环境 
`$ cd targetDirectory`
`$ source ./bin/activate`

添加完后你的命令行将会显示为： 
`(targetDirectory)$`- 在虚拟环境中安装TensorFlow和所需要的库 
`(targetDirectory)$ pip install --upgrade tensorflow`

## 2.2 Virtualenv的使用

安装完成后我们需要来验证是否成功安装，以来保证能够正常使用。 

在新的shell上使用TensorFlow前必须要先激活虚拟环境，如果你的TensorFlow所在的虚拟环境没有激活，请输入之前的激活命令。激活后将会在命令提示符前的括号中显示当前的虚拟环境。 

在每次使用完TensorFlow之后，需要取消激活当前虚拟环境。`(targetDirectory)$ deactivate`
## 2.3 运行一个TensorFlow程序

在交互式命令中输入以下代码：

```python
# Python
import tensorflow as tf
hello = tf.constant('Hello, TensorFlow!')
sess = tf.Session()
print(sess.run(hello))
```

系统输出显示`Hello, TensorFlow!`则成功安装并运行TensorFlow。

# 3. 在Jupyter Notebook中使用TensorFlow

## 3.1 Jupyter Notebook介绍

Jupyter Notebook是基于网页的用于交互计算的应用程序。其可被应用于全过程计算：开发、文档编写、运行代码和展示结果。

## 3.2 遇到问题

我们想在Jupyter Notebook中使用TensorFlow，那么应该怎么做呢？ 

因为我们之前已经安装过Anaconda所以已经有了Jupyter Notebook，试试直接在激活的虚拟环境中调用Jupyter Notebook。
`(targetDirectory)$ jupyter notebook`
将会在浏览器中打开Jupyter Notebook，试着新建一个python3的文档，输入之前的测试程序。运行，则会显示无法导入tensorflow。

## 3.3 解决方法

这是因为此时调用的Jupyter Notebook是你Anaconda中的，如果要使用TensorFlow需要在TensorFlow所在的虚拟环境当中也安装Jupyter Notebook。 

参考Stackoverflow网友`asif`的[回答](https://stackoverflow.com/questions/37061089/trouble-with-tensorflow-in-jupyter-notebook)，在虚拟环境中
`pip3 install --ignore-installed jupyter`
此时查看虚拟环境bin目录下，就能看到jupyter已经安装。这时候我们就能在Jupyter Notebook中使用TensorFlow了，那么我们来试试吧！

## 3.4 成功运行

激活虚拟环境，调用Juptyter Notebook，新建文档，输入代码，运行。 
![这里写图片描述](https://img-blog.csdn.net/20180308100714275?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

Bingo~! 

希望这篇博客能帮助到有需要的同学！







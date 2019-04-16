# 五分钟带你入门TensorFlow - PeterBishop - CSDN博客





2018年11月29日 14:29:38[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：103








> 
本文是《人人都能学人工智能-TensorFlow系列》文章的第一篇，这个系列会对TensorFlow的基础使用，SoftMax，交叉熵，Dropout，CNN，LSTM和NLP等内容进行系列介绍，尽量使用通俗的语言，让更多的人都能了解人工智能，了解TensorFlow。


TensorFlow是Google开源的一款人工智能学习系统。为什么叫这个名字呢？Tensor的意思是张量，代表N维数组；Flow的意思是流，代表基于数据流图的计算。把N维数字从流图的一端流动到另一端的过程，就是人工智能神经网络进行分析和处理的过程。

话说在Android占领了移动端后，Google开源了TensorFlow，希望占领AI端。TF的特点是可以支持多种设备，大到GPU、CPU，小到平板和手机都可以跑起来TF。而且TF的使用很方便，几行代码就能开始跑模型，这让神经网络的入门变得非常简单。

**本文是第一篇，都是最基础的内容，老手可以略过**

# 一、TensorFlow的安装

在开始写代码之前，咱们先得把TensorFlow安装到电脑上。这里有两种方法，一是一个一个手动命令行安装，二是批量的图形化界面安装。

先看手动安装，我们安装好TensorFlow和upyter Notebook就可以了。

### 1）安装TensorFlow

Windows上：

安装CPU版本：管理员模式打开命令行，输入命令：pip install tensorflow

安装GPU版本：管理员模式打开命令行，输入命令：pip install tensorflow-gpu

Linux上：

命令和上面一样，如果你使用的是Python3点几的版本，那么安装命令为：

安装CPU版本：输入命令：pip3 install tensorflow

安装GPU版本：输入命令：pip3 install tensorflow-gpu

![](https://upload-images.jianshu.io/upload_images/280127-d846dd87f84293ab.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/871/format/webp)

如果提示没有安装pip，mac的同学可以先通过命令安装pip：

1 命令行输入：curl[https://bootstrap.pypa.io/ez_setup.py](https://link.jianshu.com?t=https://bootstrap.pypa.io/ez_setup.py)-o - | sudo python

2 命令行输入：sudo easy_install pip

2）安装Jupyter Notebook

这是一个交互式的笔记本，你可以理解为一个比较漂亮和简洁的编辑器。可以很方便地创建和共享文学化程序文档，支持实时代码，数学方程，可视化和 markdown。一般用与做数据清理和转换，数值模拟，统计建模，机器学习等等。

输入命令：pip install jupyter

前面的方法是自己手动一个一个安装，其实已经有人帮我们做了一个大集合，不需要再一个一个手动安装了，下面介绍图形界面的批量安装方式：

**1）安装Anaconda**。Anaconda是什么？如果你把TensorFlow看做火箭筒，那么Anaconda就是军火库，里面有各种的科学计算，机器学习的Python工具库。在官网下载后，直接安装，然后打开Anaconda，就可以看到下面的页面：

![](https://upload-images.jianshu.io/upload_images/280127-8671f6aed92bef2d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)

2）点到第二行的Environments，可以看到很多的工具包，搜索TensorFlow，勾选上，然后点击Apply，进行安装就可以了。如果以后还需要其他的工具包，也可以在里面找到，勾选上进行安装。

![](https://upload-images.jianshu.io/upload_images/280127-8450026e7e297119.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)

OK，安装教程到此告一段落。有了jupyter和tensorflow，我们就可以开始基础的运算了。

清华提供的anaconda镜像，使用以后真的很快！尤其在学校龟速的网络环境里提速非常明显。

https://mirrors.tuna.tsinghua.edu.cn/help/anaconda/

TUNA 还提供了 Anaconda 仓库的镜像，运行以下命令:

conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/

conda config --set show_channel_urls yes

实在不行，看这里[https://blog.csdn.net/qq_40061421/article/details/84632905](https://blog.csdn.net/qq_40061421/article/details/84632905)

## 二、TensorFlow的基础运算

在搞神经网络之前，先让我们把TensorFlow的基本运算，也就是加减乘除搞清楚。

首先，TensorFlow有几个概念需要进行明确：

**1 图（Graph）：**用来表示计算任务，也就我们要做的一些操作。

**2 会话（Session）：**建立会话，此时会生成一张空图；在会话中添加节点和边，形成一张图，一个会话可以有多个图，通过执行这些图得到结果。如果把每个图看做一个车床，那会话就是一个车间，里面有若干个车床，用来把数据生产成结果。

**3 Tensor：**用来表示数据，是我们的原料。

**4 变量（Variable）：**用来记录一些数据和状态，是我们的容器。

**5 feed和fetch：**可以为任意的操作(arbitrary operation) 赋值或者从其中获取数据。相当于一些铲子，可以操作数据。

形象的比喻是：把会话看做车间，图看做车床，里面用Tensor做原料，变量做容器，feed和fetch做铲子，把数据加工成我们的结果。

**2.1 创建图和运行图：**

下面我们创建一个图，并在Session中执行它，不用担心看不懂，每句代码都会注释，只有有编程基础，都能OK：

![](https://upload-images.jianshu.io/upload_images/280127-2f789d07a707e088.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/723/format/webp)

上面就是用TensorFlow进行了一个最简单的矩阵乘法。

### **2.2 创建一个变量，并用for循环对变量进行赋值操作**

![](https://upload-images.jianshu.io/upload_images/280127-bcee732fd6de56fe.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/449/format/webp)

可以看到，除了变量创建稍微麻烦一些和必须建立session来运行，其他的操作基本和普通Python一样。

### **2.3 通过feed设置placeholder的值**

有的时候，我们会在声明变量的时候不赋值，计算的时候才进行赋值，这个时候feed就派上用场了

![](https://upload-images.jianshu.io/upload_images/280127-8f1594476f3b03b6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/590/format/webp)

到这里，恭喜你，已经成功入门TensorFlow~ 是不是觉得太简单?好像和深度学习毛线关系都没有嘛。不要急，下一篇文章和大家一起用TensorFlow做一个简单的用神经网络来做手写图片识别的实战。




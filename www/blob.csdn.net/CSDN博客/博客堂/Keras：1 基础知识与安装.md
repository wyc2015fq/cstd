# Keras：1 基础知识与安装 - 博客堂 - CSDN博客





2017年08月24日 19:06:51[最小森林](https://me.csdn.net/u012052268)阅读数：1278








![image](https://morvanzhou.github.io/static/img/course_cover/keras.jpg)


- [Keras1 基础知识与安装](#keras1-基础知识与安装)- [1 为什么学习keras](#11-为什么学习keras)- [11 keras的优点](#111-keras的优点)
- [12 我的进程](#112-我的进程)

- [2 安装keras](#12-安装keras)
- [3后台兼容 backend](#13后台兼容-backend)- [31 如何看当前使用的是什么后台Backend](#131-如何看当前使用的是什么后台backend)
- [32 如何修改Backend](#132-如何修改backend)





# Keras：1 基础知识与安装

## 1.1 为什么学习keras

### 1.1.1 keras的优点

如果说 Tensorflow 或者 Theano 神经网络方面的巨人. 那 Keras 就是站在巨人肩膀上的人. Keras 是一个兼容 Theano 和 Tensorflow 的神经网络高级包, 用他来组件一个神经网络更加快速, 几条语句就搞定了. 而且广泛的兼容性能使 Keras 在 Windows 和 MacOS 或者 Linux 上运行无阻碍。

如果对机器学习还不是很了解的朋友们, 可以大概粗略的看一遍莫烦的 [机器学习-简介系列](https://morvanzhou.github.io/tutorials/machine-learning/ML-intro/). 这个系列非常的简单通俗, 大多视频不过5分钟. 有了这些机器学习的基础. 使用 Keras 起来就更容易. 因为磨刀不误砍柴工. 如果你的时间很充裕, 也可以从莫烦的 Theano [教程](https://morvanzhou.github.io/tutorials/machine-learning/theano/) 或者 Tensorflow [教程](https://morvanzhou.github.io/tutorials/machine-learning/tensorflow/) 看起, 这里有更加全面的知识框架.

### 1.1.2 我的进程

学习了tensorflow一段时间以来，对tensorflow有了更全面与直观的了解，这样学习keras来也会更加得心应手。keras确实更适合快速的进行模型的建立。

实验室师兄的论文的实验就是用keras实现的，更加激发了我尽快学习keras的兴趣。与上次一样，这个系列的笔记是我学习[莫烦大大](https://morvanzhou.github.io/tutorials/machine-learning/keras/)的视频教程的笔记。方便我自己回过头来进行复盘，以及加上自己的感悟。

推荐大家多多关注莫烦：[https://morvanzhou.github.io/about/](https://morvanzhou.github.io/about/)

多多给他赞助~

## 1.2 安装keras
- 在安装 Keras 之前, 需要确认自己已经安装好了 Numpy 和 Scipy. 可参考我的 Numpy 安装教程
- 因为 Keras 是基于 Tensorflow 或者 Theano 的. 所以可以先自己安装 Tensorflow 或者 Theano. 可参考我的Tensorflow 安装教程 或者 Theano 安装教程
- 安装 Keras. 在你的 Terminal 窗口中输入.

```
pip install keras
```

验证是否安装成功？
- Terminal 窗口中输入python，进入python环境
- 输入import keras，回车
- 界面显示Using tensorflow Backend

## 1.3后台兼容 backend

我们来介绍 Keras 的两个 Backend，也就是Keras基于什么东西来做运算。

Keras 可以基于两个Backend，一个是 Theano（发音：涩啊NO），一个是 Tensorflow。如果我们选择Theano作为Keras的Backend， 那么Keras就用 Theano 在底层搭建你需要的神经网络；同样，如果选择 Tensorflow 的话呢，Keras 就使用 Tensorflow 在底层搭建神经网络。

目前 Tensorflow 支持 Mac 和 Linux 系统（现在也支持windows了），而 Theano 不但支持包括 Mac 和 Linux，还支持 Windows 系统， 所以我们就可以选择自己可以用的 Backend 就可以。我推荐tensorflow哦，大势所趋，坐等一年后打脸。

### 1.3.1 如何看当前使用的是什么后台Backend

每次当我们import keras的时候，就会看到屏幕显示当前使用的 Backend
`Using Theano Backend`
这就说明现在使用的是Theano在作Backend。

### 1.3.2 如何修改Backend

打开文件（按ctrl + h 显示隐藏文件）
`~/.keras/keras.json`
文件内容：

```
{
    "image_dim_ordering": "tf",
    "epsilon": 1e-07,
    "floatx": "float32",
    "backend": "theano"
}
```

每次import的时候，keras 就会检查这个 keras.json 文件。一般我们以为，如果需要把 Backend 改成 Tensorflow 的话，只需要改动最后一行”backend”对应的值，修改后的文件内容：

```
{
    "image_dim_ordering": "tf",
    "epsilon": 1e-07,
    "floatx": "float32",
    "backend": "tensorflow"
}
```

但这样修改后，import 的时候会出现错误信息。

解决的方法有几种: 

1. 可以在其他文本编辑器内编辑好这段文本，然后整体拷贝到这个文件里。 

2. 在python代码中import keras前加入一个环境变量修改的语句：
```
import os
os.environ['KERAS_BACKEND']='theano'
```

这时import keras就会显示Using Theano backend。

第2种修改影响的范围是仅这个脚本内，所以其他文件的执行Keras还是会去找keras.json配置文件来确定用什么backend。





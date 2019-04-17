# 【TensorFlow】Windows10下配置TensorFlow环境 - CD's Coding - CSDN博客





2017年05月04日 14:01:35[糖果天王](https://me.csdn.net/okcd00)阅读数：13419








## 0x00 前言

前言什么的也懒得说了……

总之：

用`TeamViewer`中转+`VNC`连接来图形化，
`SecureCRT`两重`ssh`跳转来改代码，

以及`paste.ubuntu.com`来复制粘贴，

这样的日子我已经受够了…… 我要在我的windows上配一个！
## 0x01 官方说明

**Installing with Anaconda**

The Anaconda installation is community supported, not officially supported.

Take the following steps to install TensorFlow in an Anaconda environment:

Follow the instructions on the Anaconda download site to download and install Anaconda.
Create a conda environment named tensorflow by invoking the following command:

使用如下指令创建一个名为tensorflow的conda环境：

```
C:> conda create -n tensorflow
```

Activate the conda environment by issuing the following command:

可以用如下指令来激活conda环境：

```
C:> activate tensorflow
 (tensorflow)C:>  # Your prompt should change
```

Issue the appropriate command to install TensorFlow inside your conda environment. To install the CPU-only version of TensorFlow, enter the following command:

```
(tensorflow)C:> pip install --ignore-installed --upgrade https://storage.googleapis.com/tensorflow/windows/cpu/tensorflow-1.1.0-cp35-cp35m-win_amd64.whl
```

To install the GPU version of TensorFlow, enter the following command (on a single line):

```
(tensorflow)C:> pip install --ignore-installed --upgrade https://storage.googleapis.com/tensorflow/windows/gpu/tensorflow_gpu-1.1.0-cp35-cp35m-win_amd64.whl
```

## 0x02 尝试

于是我跑去装了个[Anaconda](https://www.continuum.io/downloads#windows)，
![anaconda](https://img-blog.csdn.net/20170504134645156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
装好之后愉快的开始乱敲：

```
C:> conda create -n tensorflow
C:> activate tensorflow
 (tensorflow)C:>
```

conda环境是需要有的，虽然我开一个环境是因为dalao们都这么开，但是实际上这是很有意义的，比如……你瞎配环境配炸了怎么办呢对不对，另一个方面也是为了不影响你原生系统里的环境，在这个你自己设定的env里就是一个纯净环境，转为某种工程环境配置的，在各种env之间切换，也比较高效和稳定。

```
(tensorflow)C:> pip install --ignore-installed --upgrade https://storage.googleapis.com/tensorflow/windows/gpu/tensorflow_gpu-1.1.0-cp35-cp35m-win_amd64.whl
```

哎？为什么用不了呢，于是我注意到了一件事情：

> 
tensorflow_gpu-1.1.0-cp **35** -cp **35** m-win_amd64.whl


原来如此，python版本变成python3.5.x就可以了对吧~

(P.S. 顺带一提，这是当时2017/05/04时的情况，现在支持各类版本了，所以现在可以方便地选用各种版本的tensorflow)

但是，这个conda是python3.6的呀，我总不能去archive里看着连python版本都不标明的仓库里一个一个试哪个是python3.5吧……
这个时候conda环境的好处就来啦~ 我们可以在这个名为tensorflow的环境里使用python3.5，还不影响外面的python3.6~

并且装好tensorflow之后和这个python3.5完美适配的在这个环境里用来敲tf代码：

![conda](https://img-blog.csdn.net/20170504140033272?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```
C:\Users\Work> activate tensorflow
(tensorflow) C:\Users\Work> conda install python==3.5
Fetching package metadata ...........
Solving package specifications: .

Package plan for installation in environment C:\ProgramData\Anaconda3\envs\tensorflow:

The following NEW packages will be INSTALLED:

    msvc_runtime: 1.0.1-vc14_0  [vc14]
    pip:          9.0.1-py35_1
    python:       3.5.0-4
    setuptools:   27.2.0-py35_1
    wheel:        0.29.0-py35_0

Proceed ([y]/n)?
```

哈哈，果不其然，可以装啦可以装啦——

```
(tensorflow) C:\Users\Work>pip install --ignore-installed --upgrade https://storage.googleapis.com/tensorflow/windows/cpu/tensorflow-1.1.0-cp35-cp35m-win_amd64.whl
Collecting tensorflow==1.1.0 from https://storage.googleapis.com/tensorflow/windows/cpu/tensorflow-1.1.0-cp35-cp35m-win_amd64.whl
  Downloading https://storage.googleapis.com/tensorflow/windows/cpu/tensorflow-1.1.0-cp35-cp35m-win_amd64.whl (19.4MB)
    69% |██████████████████████▏         | 13.4MB 877kB/s eta 0:00:07

......

Installing collected packages: wheel, numpy, six, pyparsing, packaging, appdirs, setuptools, protobuf, werkzeug, tensorflow
Successfully installed appdirs-1.4.3 numpy-1.12.1 packaging-16.8 protobuf-3.3.0 pyparsing-2.2.0 setuptools-35.0.2 six-1.10.0 tensorflow-1.1.0 werkzeug-0.12.1 wheel-0.29.0
```

顺便再把GPU版本的也装了吧——

```
(tensorflow) C:\Users\Work>pip install --ignore-installed --upgrade https://storage.googleapis.com/tensorflow/windows/gpu/tensorflow_gpu-1.1.0-cp35-cp35m-win_amd64.whl
Collecting tensorflow-gpu==1.1.0 from https://storage.googleapis.com/tensorflow/windows/gpu/tensorflow_gpu-1.1.0-cp35-cp35m-win_amd64.whl
  Downloading https://storage.googleapis.com/tensorflow/windows/gpu/tensorflow_gpu-1.1.0-cp35-cp35m-win_amd64.whl (48.5MB)
    4% |█▋                              | 2.4MB 1.1MB/s eta 0:00:41

......

Installing collected packages: wheel, numpy, pyparsing, six, packaging, appdirs, setuptools, protobuf, werkzeug, tensorflow-gpu
Successfully installed appdirs-1.4.3 numpy-1.12.1 packaging-16.8 protobuf-3.3.0 pyparsing-2.2.0 setuptools-35.0.2 six-1.10.0 tensorflow-gpu-1.1.0 werkzeug-0.12.1 wheel-0.29.0
```

## 0x03 测试运行

安装完了哎……

真的假的？跑跑试试——

```
(tensorflow) C:\Users\Work>python
Python 3.5.0 |Continuum Analytics, Inc.| (default, Dec  1 2015, 11:46:22) [MSC v.1900 64 bit (AMD64)] on win32
Type "help", "copyright", "credits" or "license" for more information.
>>> import tensorflow as tf
>>> hello = tf.constant('Hello, TensorFlow!')
>>> sess = tf.Session()
2017-05-04 13:48:07.019445: W c:\tf_jenkins\home\workspace\release-win\device\cpu\os\windows\tensorflow\core\platform\cpu_feature_guard.cc:45] The TensorFlow library wasn't compiled to use SSE instructions, but these are available on your machine and could speed up CPU computations.
2017-05-04 13:48:07.020593: W c:\tf_jenkins\home\workspace\release-win\device\cpu\os\windows\tensorflow\core\platform\cpu_feature_guard.cc:45] The TensorFlow library wasn't compiled to use SSE2 instructions, but these are available on your machine and could speed up CPU computations.
2017-05-04 13:48:07.021313: W c:\tf_jenkins\home\workspace\release-win\device\cpu\os\windows\tensorflow\core\platform\cpu_feature_guard.cc:45] The TensorFlow library wasn't compiled to use SSE3 instructions, but these are available on your machine and could speed up CPU computations.
2017-05-04 13:48:07.022356: W c:\tf_jenkins\home\workspace\release-win\device\cpu\os\windows\tensorflow\core\platform\cpu_feature_guard.cc:45] The TensorFlow library wasn't compiled to use SSE4.1 instructions, but these are available on your machine and could speed up CPU computations.
2017-05-04 13:48:07.024544: W c:\tf_jenkins\home\workspace\release-win\device\cpu\os\windows\tensorflow\core\platform\cpu_feature_guard.cc:45] The TensorFlow library wasn't compiled to use SSE4.2 instructions, but these are available on your machine and could speed up CPU computations.
2017-05-04 13:48:07.026923: W c:\tf_jenkins\home\workspace\release-win\device\cpu\os\windows\tensorflow\core\platform\cpu_feature_guard.cc:45] The TensorFlow library wasn't compiled to use AVX instructions, but these are available on your machine and could speed up CPU computations.
2017-05-04 13:48:07.028016: W c:\tf_jenkins\home\workspace\release-win\device\cpu\os\windows\tensorflow\core\platform\cpu_feature_guard.cc:45] The TensorFlow library wasn't compiled to use AVX2 instructions, but these are available on your machine and could speed up CPU computations.
2017-05-04 13:48:07.028824: W c:\tf_jenkins\home\workspace\release-win\device\cpu\os\windows\tensorflow\core\platform\cpu_feature_guard.cc:45] The TensorFlow library wasn't compiled to use FMA instructions, but these are available on your machine and could speed up CPU computations.
>>> sess.run(hello)
b'Hello, TensorFlow!'
```

虽然不知道蹦出来些什么，不过看上去是装好了的样子，可喜可贺可喜可贺~

## 0x04 后记

> 
**Fold runs under Linux**; we have not tested it on other platforms. Python 2.7 and 3.3+ are both supported. We recommend installing using Virtualenv and pip. See here for instructions on installing from sources, if that’s how you roll. If you run into trouble, the TensorFlow main site has a list of common problems with some solutions that might be helpful.


> 
Please note that Fold requires TensorFlow 1.0; it is not compatible with earlier versions due to breaking API changes.


Tensorflow Fold 的话就别想了QvQ 官方表示现在早期版本不兼容，容我哭一会儿

（后记：Tensorflow Fold 之后都没有更新了……）








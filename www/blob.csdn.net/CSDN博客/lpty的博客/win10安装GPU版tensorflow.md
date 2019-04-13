
# win10安装GPU版tensorflow - lpty的博客 - CSDN博客

2017年07月14日 22:56:47[lpty](https://me.csdn.net/sinat_33741547)阅读数：2899标签：[python																](https://so.csdn.net/so/search/s.do?q=python&t=blog)[tenforflow																](https://so.csdn.net/so/search/s.do?q=tenforflow&t=blog)[windows																](https://so.csdn.net/so/search/s.do?q=windows&t=blog)[gpu																](https://so.csdn.net/so/search/s.do?q=gpu&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=windows&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=tenforflow&t=blog)个人分类：[深度学习																](https://blog.csdn.net/sinat_33741547/article/category/7028998)
[
																								](https://so.csdn.net/so/search/s.do?q=tenforflow&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=python&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=python&t=blog)


## 一 环境
系统：win10专业版
显卡：GTX1050Ti
## 二 安装Anaconda
Anaconda是一个用于科学计算的Python发行版，支持 Linux, Mac, Windows系统，提供了包管理与环境管理的功能，可以很方便地解决多版本python并存、切换以及各种第三方包安装问题。Anaconda利用工具/命令conda来进行package和environment的管理，并且已经包含了Python和相关的配套工具。
目前官网提供python3.6版本的[anaconda](https://www.continuum.io/downloads)，但尚不清楚tensorflow支不支持python3.6，谨慎起见，决定使用3.5版本的，后文介绍方法。
按步骤安装exe格式的安装包即可，后续配置环境变量：
`D:\Anaconda3
D:\Anaconda3\Scripts
D:\Anaconda3\Library\bin`
## 三 安装CUDA 8.0
深度学习库如果使用CUDA进行GPU加速，可以大大缩短计算时间。（CUDA 目前只有NVIDIA 的显卡才支持）
### 1.下载cuda
[官网](https://developer.nvidia.com/cuda-downloads)
[百度云](http://pan.baidu.com/s/1qXEMdm0)
### 2.安装
按步骤安装即可，默认地址 C:\ProgramFiles\NVIDIA GPU Computing Toolkit
### 3.验证
cmd下输入
`nvcc -V`显示：
nvcc: NVIDIA (R) Cuda compiler driver
Copyright (c) 2005-2016 NVIDIA Corporation
Built on Mon_Jan__9_17:32:33_CST_2017
Cuda compilation tools, release 8.0, V8.0.60
### 三 环境变量
添加以下环境变量
`C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v8.0\bin
C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v8.0\lib\x64`
## 四 安装cuDNN 5.1
cuDNN可以在前面 GPU 加速基础上大概再提升1.5倍的速度
### 1.下载
[官网](https://developer.nvidia.com/rdp/cudnn-download)
[百度云](http://pan.baidu.com/s/1i4Ztg6D)
### 2.配置
解压，将目录下的bin  include  lib三个文件覆盖 C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v8.0\bin
## 五 Tensorflow
上文提到python版本问题，这里进行配置：
### 1.配置清华镜像仓库
`conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/
conda config --set show_channel_urls yes`
### 2.新建tensorflow虚拟环境
`conda create -n tensorflow python=3.5`
### 3.启动虚拟环境
`activate tensorflow`
### 4.安装tensorflow_gpu
`pip install --ignore-installed --upgrade tensorflow_gpu`
### 5.测试
进入python3.5环境
`import tensorflow as tf
hello = tf.constant('Hello, TensorFlow!')
sess = tf.Session()
print(sess.run(hello))`运行正常则证明安装成功


# win7安装anaconda包管理器装tensorflow、pytorch、theano深度学习框架 - 站在巨人的肩膀上coding - CSDN博客





2018年04月26日 13:33:58[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：343








        ubuntu下依赖问题较多，难以同时安装多个深度学习框架，而windows下通过anaconda包管理器可方便的安装和管理多个深度学习网络框架。

## 一、cuda及cudnn安装

        百度搜索cuda 8.0，选择网络安装方式和本地安装方式【注意要下载补丁包】,还是直接贴上地址吧：https://developer.nvidia.com/cuda-80-ga2-download-archive

        或者注册开发者账号，登录：




从[https://developer.nvidia.com/cuda-downloads](https://developer.nvidia.com/cuda-downloads) 下载cuda_8.0.44_windows.exe

从[https://developer.nvidia.com/rdp/cudnn-download](https://developer.nvidia.com/rdp/cudnn-download)下载cudnn-8.0-windows7-x64-v5.0-ga.zip；

cudnn的安装方法：https://blog.csdn.net/xuyanan3/article/details/73866883


安装好重启或者注销后，终端上输入nvcc  -V检查是否安装好cuda

![](https://img-blog.csdn.net/20180426160622585)


二、anaconda包管理器安装

        安装4.2.0python3.5版本，在[清华大学 TUNA 镜像源](https://mirrors.tuna.tsinghua.edu.cn/anaconda/archive/)选择对应的操作系统与所需的Python版本下载Anaconda安装包。


## 三、TensorFlow安装

    cpu版本的tensorflow: 安装方式为https://blog.csdn.net/r1254/article/details/76735740


## 四、theano安装测试

    C:\Users\Administrator>python


    >>>theano.test()


测试缺少对应模块就用pip install的方法安装。

## 五、pytorch特点及安装   

### 5.1 特点



        5.1.1Python优先支持策略
        Pytorch主推的特性之一，就是支持python（官方的提法：puts Python first）。因为直接构建自 Python C API，Pytorch从细粒度上直接支持python的访问。相比于原生Python实现，引入的新概念很少，这不仅降低了 python 用户理解的门槛，也能保证代码基本跟原生的 python 实现一致。事实上，开发者可以直接用原生 python 代码扩展 Pytorch 的 operation。

        5.1.2.动态图的良好支持
        Tensorflow运行必须提前建好静态计算图，然后通过feed和run重复执行建好的图。但是Pytorch却不需要这么麻烦：PyTorch的程序可以在执行时动态构建/调整计算图。相对来说，pytorch具有更好的灵活性。这得益于Pytorch直接基于 python C API 构建的 python 接口。

        5.1.3.易于Debug
        Pytorch在运行时可以生成动态图，开发者就可以在堆栈跟踪中看到哪一行代码导致了错误。你甚至可以在调试器中停掉解释器并看看某个层会产生什么。

好了，吹完了Pytorch这么多的好处，想安装体验一把，结果上官网一看，Pytorch不支持windows！！！ 
这就没得玩了？不！

###  5.2、离线安装方法

（1）下载安装包：https://pan.baidu.com/s/1k-adw9E2wujoOAWAkc4YDQ，注意python版本【今天18.4.26GitHub上放出了0.4版本的pytorch，有需要的自行去GitHub下载】

（2） 进入cmd


注意：安装GPU版本之前注意已经安装好了cuda和cudnn

conda install numpy mkl cffi

  conda install --offline pytorch-0.1.12-py35_0.1.12cu80.tar.bz2 #离线安装，对应你的python版本

  C:\Users\Administrator>python

  >>>import torch


![](https://img-blog.csdn.net/20180426130243343)


![](https://img-blog.csdn.net/20180426130712808)


## 参考教程【可能对你有用】

[1]https://blog.csdn.net/mysql403/article/details/50902959

[2]https://blog.csdn.net/xiangxianghehe/article/details/73500031

[3]https://www.cnblogs.com/AngelaSunny/p/7766153.html

[4]https://blog.csdn.net/mysql403/article/details/50902959

[5]http://pytorch.org/


[6]https://blog.csdn.net/zzlyw/article/details/78674543




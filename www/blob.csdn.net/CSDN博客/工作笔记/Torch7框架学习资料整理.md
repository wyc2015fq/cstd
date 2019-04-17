# Torch7框架学习资料整理 - 工作笔记 - CSDN博客





2017年02月15日 15:21:23[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5748








以下，主要是自己在学习Torch7时的一些记录和体会，不喜勿喷，指出错误更加感激。

1.Torch7 老版本，主要是基于Lua5.1接口提供，看框架之前最好先看看Lua。

2.这里介绍里th,iTorch等关系，先安装好iTroch,因为第一份学习资料，博主用的时itorch,

http://blog.csdn.net/ethan_apple/article/details/48627639 这里主要是关系，入门用。我之前还不知道有个itorch....


https://github.com/facebook/iTorch 这个是itorch的官方介绍，有例子，安装教程，非常简单。


2.第一份：转自：[http://blog.csdn.net/u010946556/article/details/51329208](http://blog.csdn.net/u010946556/article/details/51329208)，博主给的例子，cuda部分，在测试代码出，要对testset.data也传递给CUDA,否则会报错。

3.CIFAR-10,CIFAR-100数据集及说明：https://www.cs.toronto.edu/~kriz/cifar.html

4.下面博客中用到了mnist数据集，这里做下补充：http://yann.lecun.com/exdb/mnist/

5.第二份，转自：http://blog.csdn.net/revolver/article/details/49678889

6.torch 官方教程：https://github.com/torch/tutorials

https://github.com/clementfarabet/ipam-tutorials/tree/master/th_tutorials 建议先看这个。后来发现这个教程里的样本数据要下载，但下载的链接过时了，因此，建议用上面链接的源码，对着下文链接的文档学习。

7.SVHN数据集，与mnist类似，http://ufldl.stanford.edu/housenumbers/

8.混淆矩阵：http://blog.csdn.net/vesper305/article/details/44927047

9.torch + opencv https://github.com/VisionLabs/torch-opencv

10.多GPU训练https://blog.rescale.com/deep-learning-with-multiple-gpus-on-rescale-torch/?utm_source=tuicool&utm_medium=referral

11.ImageNet 官网http://image-net.org/





# 分布式机器学习框架：CXXNet - wishchinYang的专栏 - CSDN博客
2016年07月08日 11:04:44[wishchin](https://me.csdn.net/wishchin)阅读数：994
##        caffe是很优秀的dl平台。影响了后面很多相关框架。       cxxnet借鉴了很多caffe的思想。相比之下，cxxnet在实现上更加干净，例如依赖很少，通过mshadow的模板化使得gpu和cpu代码只用写一份，分布式接口也很干净。
## CXXNET：极致的C++深度学习库
        cxxnet是一个并行的深度神经网络计算库，它继承了xgboost的简洁和极速的基因，并开始被越来越多人使用。例如Happy Lantern Festival团队借助Cxxnet在近期的Kaggle数据科学竞赛中获得了第二名。在技术上，cxxnet有如下两个亮点。
### 灵活的公式支持和极致的C++模板编程
        追求速度极致的开发者通常使用C++来实现深度神经网络。但往往需要给每个神经网络的层和更新公式编写独立的CUDA kernel。很多以C++为核心的代码之所以没有向matlab/numpy那样支持非常灵活的张量计算，是因为因为运算符重载和临时空间的分配会带来效率的降低。
        然而，cxxnet利用深盟的mshadow提供了类似matlab/numpy的编程体验，但同时保留了C++性能的高效性。其背后的核心思想是expression template，它通过模板编程技术将开发者写的公式自动展开成优化过的代码，避免重载操作符等带来的额外数据拷贝和系统消耗。另外，mshadow通过模板使得非常方便的讲代码切换到CPU还是GPU运行。
### 通用的分布式解决方案
         在分布式深度神经网络中，我们既要处理一台机器多GPU卡，和多台机器多GPU卡的情况。然而后者的延迟和带宽远差于前者，因此需要对这种两个情形做不同的技术考虑。cxxnet采用mshadow-ps这样一个统一的参数共享接口，并利用接下来将要介绍Parameter Server实现了一个异步的通讯接口。其通过单机多卡和多机多卡采用不同的数据一致性模型来达到算法速度和系统性能的最佳平衡。
        我们在单机4块GTX 980显卡的环境下测试了流行的图片物体识别数据集ImageNet和神经网络配置AlexNet。在单卡上，cxxnet能够处理244张图片每秒，而在4卡上可以提供3.7倍的加速。性能超过另一个流行深度学习计算库Caffe（均使用CUDA 6.5，未使用cuDNN加速）。
        在多机情况下，我们使用Amazon EC2的GPU实例来测试性能。由于优秀的异步通信，cxxnet打满了机器的物理带宽，并提供了几乎是线性的加速比，如图2所示。
![](http://img.ptcms.csdn.net/article/201505/21/555d6d83d8db7.jpg)
图2 cxxnet在Amazon EC2上的加速比
**cxxnet的另外一些特性：**
- 轻量而齐全的框架：推荐环境下仅需要CUDA、OpenCV、MKL或BLAS即可编译。
- cuDNN支持：Nvidia原生卷积支持，可加速计算30%。
- 及时更新的最新技术：及时跟进学术界的动态，例如现在已经支持MSRA的ParametricRelu和Google的BatchNormalization。
- Caffe模型转换：**支持将训练好的Caffe模型直接转化为cxxnet模型**。

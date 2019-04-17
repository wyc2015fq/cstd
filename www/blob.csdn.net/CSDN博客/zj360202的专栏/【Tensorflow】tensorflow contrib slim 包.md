# 【Tensorflow】tensorflow.contrib.slim 包 - zj360202的专栏 - CSDN博客





2017年11月20日 18:39:36[zj360202](https://me.csdn.net/zj360202)阅读数：5803













TensorFlow是谷歌基于DistBelief进行研发的第二代[人工智能](https://baike.so.com/doc/2952526-3114987.html)[学习系统](https://baike.so.com/doc/10020706-10368631.html)，其命名来源于本身的运行原理。Tensor(张量)意味着N维数组，Flow(流)意味着基于数据流图的计算，TensorFlow为张量从流图的一端流动到另一端计算过程。TensorFlow是将复杂的数据结构传输至人工智能神经网中进行分析和处理过程的系统。


TensorFlow可被用于[语音识别](https://baike.so.com/doc/1662577-1757537.html)或[图像识别](https://baike.so.com/doc/6527187-6740919.html)等多项机器深度学习领域，对2011年开发的深度学习基础架构DistBelief进行了各方面的改进，它可在小到一部智能手机、大到数千台数据中心服务器的各种设备上运行。TensorFlow将完全开源，任何人都可以用。

原生接口文章


- [【Tensorflow】tf.placeholder函数](http://blog.csdn.net/zj360202/article/details/70243127)
- [【TensorFlow】tf.nn.conv2d是怎样实现卷积的](http://blog.csdn.net/zj360202/article/details/70243424)
- [【TensorFlow】tf.nn.max_pool实现池化操作](http://blog.csdn.net/zj360202/article/details/70243836)
- [【Tensorflow】tf.nn.relu函数](http://blog.csdn.net/zj360202/article/details/70256545)
- [【Tensorflow】tf.reshape
 函数](http://blog.csdn.net/zj360202/article/details/70256835)
- [【Tensorflow】tf.nn.dropout函数](http://blog.csdn.net/zj360202/article/details/70257048)
- [【Tensorflow】tf.argmax函数](http://blog.csdn.net/zj360202/article/details/70259999)
- [【Tensorflow】tf.cast
 类型转换 函数](http://blog.csdn.net/zj360202/article/details/70260265)
- [【Tensorflow】tf.train.AdamOptimizer函数](http://blog.csdn.net/zj360202/article/details/70263110)
- [【Tensorflow】tf.Graph()函数](http://blog.csdn.net/zj360202/article/details/78539464)
- [【TensorFlow】tf.nn.softmax_cross_entropy_with_logits的用法](http://blog.csdn.net/zj360202/article/details/78582895)

- [【Tensorflow】tf.dynamic_partition
 函数 分拆数组](http://blog.csdn.net/zj360202/article/details/78642340)

原生接口实例



- [【Tensorflow】实现简单的卷积神经网络CNN实际代码](http://blog.csdn.net/zj360202/article/details/70265414)
- [【Tensorflow
 实战】实现欧式距离](http://blog.csdn.net/zj360202/article/details/78623609)

slim接口文章



- [【Tensorflow】tensorflow.contrib.slim
 包](http://blog.csdn.net/zj360202/article/details/78585197)

- [【Tensorflow
 slim】 slim.arg_scope的用法](http://blog.csdn.net/zj360202/article/details/78590285)

- [【Tensorflow
 slim】slim.data包](http://blog.csdn.net/zj360202/article/details/78591068)

- [【Tensorflow
 slim】slim evaluation 函数](http://blog.csdn.net/zj360202/article/details/78593236)

- [【Tensorflow
 slim】slim layers包](http://blog.csdn.net/zj360202/article/details/78593626)

- [【Tensorflow
 slim】slim learning包](http://blog.csdn.net/zj360202/article/details/78594089)
- [【Tensorflow
 slim】slim losses包](http://blog.csdn.net/zj360202/article/details/78594359)

- [【Tensorflow
 slim】slim nets包](http://blog.csdn.net/zj360202/article/details/78595102)

- [【Tensorflow
 slim】slim variables包](http://blog.csdn.net/zj360202/article/details/78595220)

- [【Tensorflow
 slim】slim metrics包](http://blog.csdn.net/zj360202/article/details/78595315)
slim
 实例



- [【Tensorflow
 slim 实战】写MobileNet](http://blog.csdn.net/zj360202/article/details/78623567)

- [【Tensorflow
 slim 实战】写Inception-V4 Inception-ResNet-v2结构](http://blog.csdn.net/zj360202/article/details/78627217)
kera
 接口文章



- [【Tensorflow
 keras】Keras:基于Theano和TensorFlow的深度学习库](http://blog.csdn.net/zj360202/article/details/78645039)

- [【Tensorflow
 keras】轻量级深度学习框架 Keras简介](http://blog.csdn.net/zj360202/article/details/78646969)

tensorflow使用过程中的辅助接口或通过tensorflow实现的批量操作接口



- [将非RGB图片转换为RGB图片](http://blog.csdn.net/zj360202/article/details/78539040)

- [【opencv】python3
 将图片生成视频文件](http://blog.csdn.net/zj360202/article/details/71561925)

- [【opencv】selective_search函数](http://blog.csdn.net/zj360202/article/details/70847144)





=========================================================================





TF-Slim 是Tensorflow为了定义，训练和评估复杂模型的一个轻量级工具， tf-slim的组件可以与本地tensorflow以及其他框架（如tf.contrib.learn）自由混合。

[github:Tersorflow/contrib/slim](https://github.com/tensorflow/tensorflow/tree/master/tensorflow/contrib/slim)




## Usage



```python
import tensorflow.contrib.slim as slim
```







## Why TF-Slim?

TF-Slim 可以使得建立，训练和预估神经网络变的简单


- 允许用户通过消除样板代码来更加紧凑地定义模型。 这是通过使用[argument](https://www.tensorflow.org/code/tensorflow/contrib/framework/python/ops/arg_scope.py)scoping和许多高层次layers和variables来完成的。
 这些工具提高了可读性和可维护性，通过复制粘贴超参数值降低了出错的可能性，并简化了超参数调优。
- 通过提供常用的regularizers使开发模型变得简单。

- 几种广泛使用的计算机视觉模型（例如，VGG，AlexNet）已经被开发出来，并且可供用户使用。 这些可以用作黑盒，或者可以以各种方式扩展，例如通过向不同的内部层添加“multiple heads”。

- Slim使扩展复杂模型变得容易，并通过使用预先存在的模型检查点来热启动训练算法。


## What are the various components of TF-Slim?


TF-Slim由设计独立存在的几个部分组成。 这些包括以下主要部分（在下面详细解释）(**点击相关访问详细描述**)。
- [arg_scope](http://blog.csdn.net/zj360202/article/details/78590285)：提供了一个名为arg_scope的新作用域，允许用户为该作用域内的特定操作定义默认参数。
- [data](http://blog.csdn.net/zj360202/article/details/78591068)：包含TF-slim的数据集定义，数据提供者，并行编译器和解码工具。
- [evaluation](http://blog.csdn.net/zj360202/article/details/78593236)：包含评估模型的例程。
- [layers](http://blog.csdn.net/zj360202/article/details/78593626)：包含使用张量流建立模型的高层次图层。
- [learning](http://blog.csdn.net/zj360202/article/details/78594089)：包含训练模型的例程。
- [losses](http://blog.csdn.net/zj360202/article/details/78594359)：包含常用损失函数。
- [metrics](http://blog.csdn.net/zj360202/article/details/78595315)：包含流行的评估指标。
- [nets](http://blog.csdn.net/zj360202/article/details/78595102)：包含流行的网络定义，如VGG和AlexNet模型。
- [queues](http://blog.csdn.net/zj360202/article/details/78585197)：提供了一个上下文管理器，可以轻松安全地启动和关闭QueueRunners。
- [regularizers](http://blog.csdn.net/zj360202/article/details/78585197)：包含权重正规化者。
- [variables](http://blog.csdn.net/zj360202/article/details/78595220)：为变量创建和操作提供便利包装







# 【Tensorflow】tf.cast  类型转换 函数 - zj360202的专栏 - CSDN博客





2017年04月20日 15:29:17[zj360202](https://me.csdn.net/zj360202)阅读数：47131








### ``


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


### ``````

### `tf.cast(x, dtype, name=None)`

`此函数是类型转换函数`


```

```


`参数`

- x：输入
- dtype：转换目标类型
- name：名称
返回：Tensor




例：


```java
# tensor `a` is [1.8, 2.2], dtype=tf.float
tf.cast(a, tf.int32) ==> [1, 2]  # dtype=tf.int32
```







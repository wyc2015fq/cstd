# 【TensorFlow】tf.nn.softmax_cross_entropy_with_logits的用法 - zj360202的专栏 - CSDN博客





2017年11月20日 15:45:08[zj360202](https://me.csdn.net/zj360202)阅读数：2204













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






在计算loss的时候，最常见的一句话就是tf.nn.softmax_cross_entropy_with_logits，那么它到底是怎么做的呢？


首先明确一点，loss是代价值，也就是我们要最小化的值



### `tf.nn.softmax_cross_entropy_with_logits(logits, labels, name=None)`
除去name参数用以指定该操作的name，与方法有关的一共两个参数`：`


`第一个参数logits：就是神经网络最后一层的输出，如果有batch的话，它的大小就是[batchsize，num_classes]，单样本的话，大小就是`num_classes``

``第二个参数labels：实际的标签，大小同上``


```
```

```
```


``具体的执行流程大概分为两步：``

``第一步是先对网络最后一层的输出做一个softmax，这一步通常是求取输出属于某一类的概率，对于单样本而言，输出就是一个``num_classes``大小的向量（[Y1，Y2,Y3...]其中Y1，Y2，Y3...分别代表了是属于该类的概率）``


```
```
softmax的公式是：![](https://img-blog.csdn.net/20161128203449282)
```
```


``至于为什么是用的这个公式？这里不介绍了，涉及到比较多的理论证明``


```
```

```
```


``第二步是``softmax的输出向量[Y1，Y2,Y3...]和样本的实际标签做一个交叉熵，公式如下：````


```
```
```
```
![](https://img-blog.csdn.net/20161128203840317)
```
```
```
```


````其中![](https://img-blog.csdn.net/20161128204121097)指代实际的标签中第i个的值（用mnist数据举例，如果是3，那么标签是[0，0，0，1，0，0，0，0，0，0]，除了第4个值为1，其他全为0）````

````![](https://img-blog.csdn.net/20161128204500600)就是````softmax的输出向量[Y1，Y2,Y3...]````````中，第i个元素的值

````显而易见，预测![](https://img-blog.csdn.net/20161128204500600)越准确，结果的值越小（别忘了前面还有负号），最后求一个平均，得到我们想要的loss````


```
```
```
```
注意！！！这个函数的返回值并不是一个数，而是一个向量，如果要求交叉熵，我们要再做一步tf.reduce_sum操作,就是对向量里面所有元素求和，最后才得到![](https://img-blog.csdn.net/20161128213206933)，如果求loss，则要做一步````tf.reduce_mean操作，对向量求均值！````
```
```
```
```



```
```
```
```

```
```
```
```


````理论讲完了，上代码````

````````



[python][view
 plain](http://blog.csdn.net/mao_xiao_feng/article/details/53382790#)[copy](http://blog.csdn.net/mao_xiao_feng/article/details/53382790#)



- import tensorflow as tf  
- 
- #our NN's output
- logits=tf.constant([[1.0,2.0,3.0],[1.0,2.0,3.0],[1.0,2.0,3.0]])  
- #step1:do softmax
- y=tf.nn.softmax(logits)  
- #true label
- y_=tf.constant([[0.0,0.0,1.0],[0.0,0.0,1.0],[0.0,0.0,1.0]])  
- #step2:do cross_entropy
- cross_entropy = -tf.reduce_sum(y_*tf.log(y))  
- #do cross_entropy just one step
- cross_entropy2=tf.reduce_sum(tf.nn.softmax_cross_entropy_with_logits(logits, y_))#dont forget tf.reduce_sum()!!
- 
- with tf.Session() as sess:  
-     softmax=sess.run(y)  
-     c_e = sess.run(cross_entropy)  
-     c_e2 = sess.run(cross_entropy2)  
- print("step1:softmax result=")  
- print(softmax)  
- print("step2:cross_entropy result=")  
- print(c_e)  
- print("Function(softmax_cross_entropy_with_logits) result=")  
- print(c_e2)  


输出结果是：


````````



[python][view
 plain](http://blog.csdn.net/mao_xiao_feng/article/details/53382790#)[copy](http://blog.csdn.net/mao_xiao_feng/article/details/53382790#)



- step1:softmax result=  
- [[ 0.090030570.244728480.66524094]  
-  [ 0.090030570.244728480.66524094]  
-  [ 0.090030570.244728480.66524094]]  
- step2:cross_entropy result=  
- 1.22282
- Function(softmax_cross_entropy_with_logits) result=  
- 1.2228

最后大家可以试试e^1/(e^1+e^2+e^3)是不是0.09003057，发现确实一样！！这也证明了`我们的输出是符合公式逻辑的`



# 【TensorFlow】tf.nn.max_pool实现池化操作 - zj360202的专栏 - CSDN博客





2017年04月19日 18:34:13[zj360202](https://me.csdn.net/zj360202)阅读数：3420标签：[max_pool																[tensorflow																[深度学习																[神经网络																[python](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[tensorflow](https://blog.csdn.net/zj360202/article/category/6872046)








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


原文地址：http://blog.csdn.net/mao_xiao_feng/article/details/53453926


max pooling是CNN当中的最大值池化操作，其实用法和卷积很类似


有些地方可以从卷积去参考[【TensorFlow】tf.nn.conv2d是怎样实现卷积的？](http://blog.csdn.net/mao_xiao_feng/article/details/53444333)



### `tf.nn.max_pool(value, ksize, strides, padding, name=None)`
参数是四个，和卷积很类似：


第一个参数value：需要池化的输入，一般池化层接在卷积层后面，所以输入通常是feature map，依然是`[batch, height, width, channels]这样的shape`


第二个参数ksize：池化窗口的大小，取一个四维向量，一般是`[1, height, width, 1]，因为我们不想在`batch和`channels`上做池化，所以这两个维度设为了1``

第三个参数strides：和卷积类似，窗口在每一个维度上滑动的步长，一般也是`[1, stride,`stride`, 1]`

第四个参数padding：和卷积类似，可以取'VALID' 或者'SAME'


返回一个Tensor，类型不变，shape仍然是`[batch, height, width, channels]`这种形式



示例源码：


假设有这样一张图，双通道


第一个通道：

![](https://img-blog.csdn.net/20161204140649120)


第二个通道：

![](https://img-blog.csdn.net/20161204140850442)


用程序去做最大值池化：





**[python]**[view
 plain](http://blog.csdn.net/mao_xiao_feng/article/details/53453926#)[copy](http://blog.csdn.net/mao_xiao_feng/article/details/53453926#)

![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- import tensorflow as tf  
- 
- a=tf.constant([  
-         [[1.0,2.0,3.0,4.0],  
-         [5.0,6.0,7.0,8.0],  
-         [8.0,7.0,6.0,5.0],  
-         [4.0,3.0,2.0,1.0]],  
-         [[4.0,3.0,2.0,1.0],  
-          [8.0,7.0,6.0,5.0],  
-          [1.0,2.0,3.0,4.0],  
-          [5.0,6.0,7.0,8.0]]  
-     ])  
- 
- a=tf.reshape(a,[1,4,4,2])  
- 
- pooling=tf.nn.max_pool(a,[1,2,2,1],[1,1,1,1],padding='VALID')  
- with tf.Session() as sess:  
- print("image:")  
-     image=sess.run(a)  
- print (image)  
- print("reslut:")  
-     result=sess.run(pooling)  
- print (result)  

这里步长为1，窗口大小2×2，输出结果：






**[python]**[view
 plain](http://blog.csdn.net/mao_xiao_feng/article/details/53453926#)[copy](http://blog.csdn.net/mao_xiao_feng/article/details/53453926#)

![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- image:  
- [[[[ 1.2.]  
-    [ 3.4.]  
-    [ 5.6.]  
-    [ 7.8.]]  
- 
-   [[ 8.7.]  
-    [ 6.5.]  
-    [ 4.3.]  
-    [ 2.1.]]  
- 
-   [[ 4.3.]  
-    [ 2.1.]  
-    [ 8.7.]  
-    [ 6.5.]]  
- 
-   [[ 1.2.]  
-    [ 3.4.]  
-    [ 5.6.]  
-    [ 7.8.]]]]  
- reslut:  
- [[[[ 8.7.]  
-    [ 6.6.]  
-    [ 7.8.]]  
- 
-   [[ 8.7.]  
-    [ 8.7.]  
-    [ 8.7.]]  
- 
-   [[ 4.4.]  
-    [ 8.7.]  
-    [ 8.8.]]]]  

池化后的图就是：


![](https://img-blog.csdn.net/20161204141417771)

![](https://img-blog.csdn.net/20161204141505210)


证明了程序的结果是正确的。





我们还可以改变步长





**[python]**[view
 plain](http://blog.csdn.net/mao_xiao_feng/article/details/53453926#)[copy](http://blog.csdn.net/mao_xiao_feng/article/details/53453926#)

![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- pooling=tf.nn.max_pool(a,[1,2,2,1],[1,2,2,1],padding='VALID')  

![](http://static.blog.csdn.net/images/save_snippets_01.png)

最后的result就变成：






**[python]**[view
 plain](http://blog.csdn.net/mao_xiao_feng/article/details/53453926#)[copy](http://blog.csdn.net/mao_xiao_feng/article/details/53453926#)

![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- reslut:  
- [[[[ 8.7.]  
-    [ 7.8.]]  
- 
-   [[ 4.4.]  
-    [ 8.8.]]]]  
-](https://so.csdn.net/so/search/s.do?q=神经网络&t=blog)](https://so.csdn.net/so/search/s.do?q=深度学习&t=blog)](https://so.csdn.net/so/search/s.do?q=tensorflow&t=blog)](https://so.csdn.net/so/search/s.do?q=max_pool&t=blog)





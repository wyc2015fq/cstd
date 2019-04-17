# 【Tensorflow】tf.Graph()函数 - zj360202的专栏 - CSDN博客





2017年11月15日 14:37:39[zj360202](https://me.csdn.net/zj360202)阅读数：18713标签：[graph																[pb文件																[tensorflow																[tf.Graph																[get_default_graph](https://so.csdn.net/so/search/s.do?q=get_default_graph&t=blog)
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





tf.Graph() 函数非常重要，注意提现在两个方面

1. 它可以通过tensorboard用图形化界面展示出来流程结构

2. 它可以整合一段代码为一个整体存在于一个图中




**声明情况大体有三种**

**1. tensor:通过张量本身直接出graph**



```python
# -*- coding: utf-8 -*-  
import tensorflow as tf

c = tf.constant(4.0)

sess = tf.Session()
sess.run(tf.global_variables_initializer())
c_out = sess.run(c)
print(c_out)
print(c.graph == tf.get_default_graph())
print(c.graph)
print(tf.get_default_graph())
```
输出





```python
4.0
True
<tensorflow.python.framework.ops.Graph object at 0x7f382f9ef110>
<tensorflow.python.framework.ops.Graph object at 0x7f382f9ef110>
```




**2.通过声明一个默认的，然后定义张量内容，在后面可以调用或保存**



```python
# -*- coding: utf-8 -*-  
import tensorflow as tf

g = tf.Graph()
with g.as_default():
    c = tf.constant(4.0)

sess = tf.Session(graph=g)
c_out = sess.run(c)
print(c_out)
print(g)
print(tf.get_default_graph())
```


输出





```python
4.0
<tensorflow.python.framework.ops.Graph object at 0x7f65f1cb2fd0>
<tensorflow.python.framework.ops.Graph object at 0x7f65de447c90>
```




**3.通过多个声明，在后面通过变量名来分别调用**



```python
# -*- coding: utf-8 -*-  
import tensorflow as tf

g1 = tf.Graph()
with g1.as_default():
    c1 = tf.constant(4.0)

g2 = tf.Graph()
with g2.as_default():
    c2 = tf.constant(20.0)

with tf.Session(graph=g1) as sess1:
    print(sess1.run(c1))
with tf.Session(graph=g2) as sess2:
    print(sess2.run(c2))
```


输出





```python
4.0
20.0
```

**对graph的操作大体有三种**


**1.保存**



```python
# -*- coding: utf-8 -*-  
import tensorflow as tf

g1 = tf.Graph()
with g1.as_default():
    # 需要加上名称，在读取pb文件的时候，是通过name和下标来取得对应的tensor的
    c1 = tf.constant(4.0, name='c1')

g2 = tf.Graph()
with g2.as_default():
    c2 = tf.constant(20.0)

with tf.Session(graph=g1) as sess1:
    print(sess1.run(c1))
with tf.Session(graph=g2) as sess2:
    print(sess2.run(c2))

# g1的图定义，包含pb的path, pb文件名，是否是文本默认False
tf.train.write_graph(g1.as_graph_def(),'.','graph.pb',False)
```




输出



```python
4.0
20.0
```
并且在当前文件夹下面生成graph.pb文件




**2.从pb文件中调用**



```python
# -*- coding: utf-8 -*-  
import tensorflow as tf
from tensorflow.python.platform import gfile

#load graph
with gfile.FastGFile("./graph.pb",'rb') as f:
    graph_def = tf.GraphDef()
    graph_def.ParseFromString(f.read())
    tf.import_graph_def(graph_def, name='')

sess = tf.Session()
c1_tensor = sess.graph.get_tensor_by_name("c1:0")
c1 = sess.run(c1_tensor)
print(c1)
```


输出





```python
4.0
```




**3.穿插调用**



```python
# -*- coding: utf-8 -*-  
import tensorflow as tf

g1 = tf.Graph()
with g1.as_default():
    # 声明的变量有名称是一个好的习惯，方便以后使用
    c1 = tf.constant(4.0, name="c1")

g2 = tf.Graph()
with g2.as_default():
    c2 = tf.constant(20.0, name="c2")

with tf.Session(graph=g2) as sess1:
    # 通过名称和下标来得到相应的值
    c1_list = tf.import_graph_def(g1.as_graph_def(), return_elements = ["c1:0"], name = '')
    print(sess1.run(c1_list[0]+c2))
```


输出





```python
24.0
```


当然还有很多比较好的地方，比如graph中自带的名称函数，通过with写的内容丰富的代码块，这里就不讲了](https://so.csdn.net/so/search/s.do?q=tf.Graph&t=blog)](https://so.csdn.net/so/search/s.do?q=tensorflow&t=blog)](https://so.csdn.net/so/search/s.do?q=pb文件&t=blog)](https://so.csdn.net/so/search/s.do?q=graph&t=blog)





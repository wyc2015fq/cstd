# CS 20SI|Lecture 1 Introduction to TensorFlow - 浅梦的博客 - CSDN博客





2017年02月02日 22:24:20[浅梦s](https://me.csdn.net/u012151283)阅读数：1219








本文是基于课程CS 20SI: TensorFlow for Deep Learning Research进行总结。

# 课程目标
- 理解TF计算图方法
- 探索TF内建函数
- 学习如何构建适合DL项目的结构化模型

# 参考书籍
- TensorFlow for Machine Intelligence (TFFMI)
- Hands-On Machine Learning with Scikit-Learn and TensorFlow. Chapter 9: Up and running with TensorFlow 
- Fundamentals of Deep Learning. Chapter 3: Implementing Neural Networks in TensorFlow (FODL) 

TensorFlow正在快速迭代，上述书籍可能会过时，直接[参考官方网站](www.tensorflow.org)

# 开始

## High level API
- TF Learn
- TF Slim
- Keras(官方宣布支持作为上层API)
- Tensor Layer

# Graphs and Sessions

## Data Flow Graphs

TensorFlow将计算图的定义从执行过程中分离。TF的工作分为两个阶段： 

阶段1：定义计算图 

阶段2：使用一个`session`执行图中的`operations`

在一张计算图中，结点表示一种数学运算。边表示两个结点之间通信的多维张量。
## Tensor

Tensor是一个n维矩阵，相当于`numpy`中的`ndarray`

```python
import tensorflow as tf
a = tf.add(3, 5)
```

![数据流图](https://img-blog.csdn.net/20170202223926278?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

上述代码构建了上面的数据流图，在数据流图中结点Node可以是**operators, variables, and constants**,边Edge是**tensors**

Tensor Flow即为Tensor(Data)在图中流动。
```python
import tensorflow as tf
a = tf.add(3, 5)
print(a)
>> Tensor("Add:0", shape=(), dtype=int32)
```

注意到`print(a)`并没有输出结果5.下面介绍如何得到`a`的数值

## Session

Session对象封装了Operation对象执行和Tensor对象evaluated的环境

```python
import tensorflow as tf
a = tf.add(3, 5)
sess = tf.Session()
print(sess.run(a))
sess.close()
>> 8
```

Session会查看计算图中的结点，寻找如何能够得到a的结果，并计算沿途的nodes. 

使用上下文管理器可以不用每次显式关闭Session.

```python
import tensorflow as tf
a = tf.add(3, 5)
with tf.Session() as sess:
    print(sess.run(a))
```

## More (sub) graph

`tf.Session.run(fetches, feed_dict=None, options=None, run_metadata=None)`

将想要计算的变量作为一个`list`传递给`fetches`

```python
import tensorflow as tf
x = 2
y = 3
op1 = tf.add(x, y)
op2 = tf.mul(x, y)
useless = tf.mul(x,op1)
op3 = tf.pow(op2, op1)
with tf.Session() as sess:
    op3, not_useless = sess.run([op3,useless])
```

## Distributed Computation

![分隔](https://img-blog.csdn.net/20170202231023902?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

将计算图拆分适合于多机多核并行计算
## Another Graph
- Multiple graphs require multiple sessions, each will try to use all available resources by default 
- Can’t pass data between them without passing them through python/numpy, which doesn’t work in distributed 
- It’s better to have disconnected subgraphs within one grap 

```python
g = tf.Graph()
#to add operators to a graph, set it as default:
with g.as_default():
    x = tf.add(3, 5)
sess = tf.Session(graph=g)
with tf.Session() as sess:
    sess.run(x)
```

**小心不要混淆默认graph和用户定义graph,向任何计算图添加操作时，先将其定义为默认图**

```python
g1 = tf.get_default_graph()
g2 = tf.Graph()
#add ops to the default graph
with g1.as_default():
    a = tf.Constant(3)
#add ops to the user created graph
with g2.as_default():
    b = tf.Constant(5)
```

## Why graphs
- Save computation (only run subgraphs that lead to the values you want to fetch)
- Break computation into small, differential pieces to facilitates auto-differentiation 
- Facilitate distributed computation, spread the work across multiple CPUs, GPUs, or devices
- Many common machine learning models are commonly taught and visualized as directed graphs already







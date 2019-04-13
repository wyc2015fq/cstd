
# Tensorflow基础（1） - leofionn的博客 - CSDN博客


2018年08月08日 21:39:49[leofionn](https://me.csdn.net/qq_36142114)阅读数：82


# 前言：
tensorflow中有很多基本概念要理解，最好的方法是去官网跟着教程一步一步看，这里还有一些翻译的版本，对照着看有助于理解：[tensorflow中文社区](http://www.tensorfly.cn/)
1.TensorFlow是基于计算图的框架：
使用图 (graph)来表示计算任务.
使用tensor表示数据.
在被称之为会话 (Session)的上下文 (context) 中执行图.
通过变量 (Variable)维护状态.
使用feed和fetch可以为任意的操作(operation)赋值或者从其中获取数据.
Tensorflow是一个编程系统，使用图（graphs）来表示计算任务，图（graphs）中的节点称之为op （operation），一个op获得0个或多个Tensor，执行计算，产生0个或多个Tensor。Tensor 看作是 一个 n 维的数组或列表。图必须在会话（Session）里被启动。
一，构建并执行计算图的必要过程
1，graph（图计算）：详见tf.Graph类
使用tensorflow训练神经网络包括两部分：构建计算图和运行计算图。
首先来讲构建图，一个计算图包含了一组操作（operation objects，也叫节点）和一些tensor objects（节点之间传递的数据单元）。系统会默认构建一个计算图，这也是mnist入门案例上来就可以定义节点的原因。不过我们在写代码的时候，应该将构建计算图的代码写在一个with块中
`myGraph = tf.Graph()
with myGraph.as_default():
  # Define operations and tensors in `myGraph `.(在下面定义op和tensor)
  c = tf.constant(30.0)`2，operations（图节点）：详见tf.Operation类
op是计算图中的节点，它能接收tensor作为输入，并能产生tensor作为输出。op的创建方式有两种，一种是调用了计算操作，比如mnist例子中的tf.matmul(),tf.nn.conv2d(),tf.nn.max_pool()函数等，这些函数名就是op。另一种方法是调用Graph.create_op()方法往图里加op。常用第一种方法。op在构建图的阶段不会执行，在运行图阶段才执行。
3，tensor（向量）：详见tf.Tensor类
tensor就是op计算的输入/输出结果。同样的，tensor在构建图时并不持有值，而是在运行时持有数值。tensor作为op的输入/输出在graph中传递，从而使得tensorflow能执行代表着大规模计算的计算图，也正是Tensorflow得名的原因（向量流动）。常量和变量都是tensor。举例如下，a,b,c,d就是tensor，它们可以作为op的输出值，也可以作为op的输入值。
`# Build a dataflow graph.
a = tf.constant([[1.0, 2.0], [3.0, 4.0]])
b = tf.constant([[1.0, 1.0], [0.0, 1.0]])
c = tf.matmul(c, d)
d = tf.Variable([1.0, 2.0])`4，session（会话）：详见tf.Session类
前面3步我们已经构建完了一个计算图，现在需要执行这个计算图。session就是执行计算图的类。 tensor的执行要通过sess.run（tensor）来执行。session对象封装了op执行和tensor传递的环境。session对象开启后会持有资源，所以用完后要记得关闭，通常写到一个with块里面。综合1234，我们写一个例子。
`myGraph = tf.Graph()
with myGraph.as_default():
    # Build a graph.
    a = tf.constant(5.0)
    b = tf.constant(6.0)
    c = a * b
# Using the context manager.
with tf.Session() as sess:
    print(sess.run(c))`二， Tensor相关的属性和方法
1，tensor的属性，常用的有下面这些：
dtype：tensor中元素的类型（tf.int32, tf.float32等）
graph：tensor所属的计算图（返回Graph类对象的地址）
name：tensor的字符串名字，若定义tensor的时候取了名字，则输出你取的名字。若你自己没有取，系统会按规则帮你给tensor取名字，规则如下：若tensor是常量，则第一个tensor取名“Const”，第二个tensor取名“Const_1”,第三个tensor取名“Const_2”,以此类推。若tensor是变量，则第一个tensor取名“Variable”，第二个tensor取名“Variable_1”,第三个tensor取名“Variable_2”,以此类推。这个知识点在save/restore模型的时候会用到。
op：产生改tensor的op名。会列出该op的详细信息。
shape：tensor的形状。
举例说明（tensor的属性可直接打印，不需要放进sess.run()里面）：
`# encoding=utf-8
import tensorflow as tf
graph = tf.Graph()
with graph.as_default():
    x= tf.constant([[1.,2.,3.,4.],[5.,6.,7.,8.]],dtype=tf.float32)
    x1= tf.constant([[1.,2.,3.,4.],[5.,6.,7.,8.]],dtype=tf.float32)
    x2= tf.constant([[1.,2.,3.,4.],[5.,6.,7.,8.]],dtype=tf.float32)
    y= tf.Variable([[0.,0.,-2.,1.],[-1,-2,-2,-3]])
    y1= tf.Variable([[0.,0.,-2.,1.],[-1,-2,-2,-3]])
    y2= tf.Variable([[0.,0.,-2.,1.],[-1,-2,-2,-3]])
    y3= y1+y2
# shape:tensor的形状
# size:tensor元素个数
# rank:tensor的维度
with tf.Session(graph=graph) as sess:
    print(x.shape)
    print(x.dtype)
    print(x.graph)
    print(x.name)
    print(x1.name)
    print(x2.name)
    print(y.name)
    print(y1.name)
    print(y2.name)
    print(y3.shape)`2，tensor的方法
–get_shape()：得到tensor的shape，效果跟shape属性一样。
–eval（feed_dict=None, session=None）：执行tensor的方法，调用这个方法将执行该tensor所需输入的所有op。eval()方法要在session中调用。tensor.eval()效果与sess.run(tensor)相同，而官网原文是这样说的：t.eval() is a shortcut for calling tf.get_default_session().run(t)。
# 架构图：
![](https://img-blog.csdn.net/20180808214624964?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM2MTQyMTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


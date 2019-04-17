# TensorFlow学习笔记 —— 基础概念 - Hxyue2018's Blog - CSDN博客





2018年11月08日 17:48:54[Hxyue2018](https://me.csdn.net/Super_Json)阅读数：28标签：[Graph																[Session](https://so.csdn.net/so/search/s.do?q=Session&t=blog)](https://so.csdn.net/so/search/s.do?q=Graph&t=blog)
个人分类：[TensorFlow](https://blog.csdn.net/Super_Json/article/category/8529926)








### 计算图

计算图是TensorFlow中最基本的一个概念，TensorFlow中的所有计算都会被转化为计算图上的节点。

TensorFlow程序一般分为两个阶段。在第一个阶段需要定义计算图中所有的计算，第二个阶段为执行计算，得到计算结果。

在TensorFlow程序中，系统会自动维护一个默认的计算图，通过tf.get_default_graph函数可以获取当前默认的计算图。除了使用默认的计算图外，TensorFlow支持通过tf.Graph函数来生成新的计算图，不同计算图上的张量和运行都不会共享。

下面是一个生成计算图并定义变量的代码：

import tensorflow as tf

g = tf.Graph()

with g.as_default():

#在计算图中定义变量“v”，并设置初始值为0

v = tf.get_variable("v", initializer = tf.zeros_initializer(shape=[1]))

TensorFlow中的计算图不仅可以用来隔离张量和计算，它还提供了管理张量和计算的机制。计算图可以通过tf.Graph.device函数来指定运行计算的设备，这为TensorFlow使用GPU提供了机制。

g = tf.Graph()

#指定计算运行的设备

with g.device('/gpu:0'):

result = a + b

### 张量

张量（tensor）是TensorFlow管理数据的形式，在TensorFlow所有程序中，所有的数据都是通过张量的形式来表示。从功能的角度上来看，张量可以被简单理解为多维数组。其中零阶张量表示标量，也就是一个数；一阶张量表示一个向量，也就是一个一维数组；n阶张量可以理解为一个n维数组。但张量在TensorFlow中的实现并不是直接采用数组的形式，它只是对TensorFlow中运算结果的引用。在张量中并没有真正保存数字，它保存的是如何得到这些数字的计算过程。

张量主要有三个属性：名字（name）、维度（shape）和类型（type）。如：Tensor("add:0", shape=(2,), dtype=float32)

张量的名字不仅是一个张量的唯一标识符，同样也给出了这个张量是如何计算出来的。张量的命名通过“node:src_output”的形式给出，其中node为结点的名称，src_output表示当前张量来自结点的第几个输出。如上面的"add:0"表明这个张量是计算节点"add"输出的第一个结果。

张量的维度描述了张量的维度信息，如上面的“shape=(2,)”说明这个张量是一个一维数组，数组长度为2。TensorFlow程序中维度的对应是一个需要十分注意的地方。

张量的类型表明了数据的类型，TensorFlow会对参与运算的所有张量进行类型检查，当发现类型不匹配时会报错。主要有以下14种类型：tf.int8,tf.int16,tf.int32,tf.int64,tf.uint8,tf.float32,tf.float64,tf.bool,tf.complex64,tf.complex128.

张量的使用：

第一类用途是对中间运算结果的引用，如：

#使用张量记录中间结果

a = tf.constant([1.0, 2.0], name = "a")

b = tf.constant([2.0, 3.0], name = "b")

c = a + b

可以使用c.get_shape函数来获取结果张量的维度信息

张量的第二类使用情况是当计算图构造完成之后，张量可以用来获得计算结果，也就是得到真实的数字。可以使用tf.Session().run(c)语句得到计算结果。

### 会话

TensorFlow中使用会话（session）来执行定义好的运算。会话拥有并管理TensorFlow程序运行时的所有资源。

TensorFlow中使用会话的模式有两种，第一种模式需要明确调用会话生成函数和关闭会话函数，代码流程如下：

#创建一个会话

sess = tf.Session()

#使用会话进行运算

sess.run(...)

#关闭会话

sess.close()

上面这种模式在所有计算完成之后需要明确调用Session.close()函数来关闭会话并释放资源。如果程序因为异常而退出，则会导致会话未关闭而资源泄露。因此，为了解决异常退出时资源释放的问题，可以通过Python的上下文管理器来使用会话，这就是第二种模式，代码流程如下：

#创建一个会话

with tf.Session() as sess:

#使用这个会话进行运算，下面两个命令有相同的功能

sess.run(rusult)

result.eval()

通过上面的学习，在了解了TensorFlow的一些基本概念之后，接下来就用一个实例来亲身体验一下吧。

用TensorFlow实现神经网络，主要分为以下3个步骤：

1）定义神经网络的结构和前向传播的输出结果

2）定义损失函数以及选择方向传播优化算法

3）生成会话并在训练数据上反复运行反向传播优化算法







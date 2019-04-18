# 简要介绍下tensorflow的计算图 - sinat_32043495的博客 - CSDN博客





2017年12月21日 09:51:24[LZXandTM](https://me.csdn.net/sinat_32043495)阅读数：1140







1.请简要介绍下tensorflow的计算图，深度学习 DL框架 中

@寒小阳&AntZ：Tensorflow是一个通过计算图的形式来表述计算的编程系统，计算图也叫数据流图，可以把计算图看做是一种有向图，Tensorflow中的每一个节点都是计算图上的一个Tensor, 也就是张量，而节点之间的边描述了计算之间的依赖关系(定义时)和数学操作(运算时)。Tensorflow计算的过程就是利用的Tensor来建立一个计算图，然后使用Session会话来启动计算，最后得到结果的过程。

无论是编译类型的语言还是脚本语言，都是一步一步的，变量计算后，就会得到结果，比如c=a+b，当执行完语句后，就会得到c的值。但tensorflow不是，它首先要通过编程，构建一个计算图出来，然后启用一个会话来把数据作为输入，通过这个图规定的计算步骤计算，最后得到结果。

![](https://img-blog.csdn.net/20171221100429378?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


计算图的创建

import tensorflow as tf


if __name__ == "__main__":

    a = tf.constant([1.0,2.0],name = "a")

    b = tf.constant([2.0,3.0],name = "b")

    result = a + b

print(a.graph)

print(a.graph is tf.get_default_graph())


g1 = tf.Graph()

with g1.as_default():

    c = tf.get_variable("c",initializer = tf.zeros_initializer,shape=(1))


g2 = tf.Graph()

with g2.as_default():

    c = tf.get_variable("c",initializer = tf.ones_initializer,shape=(1))


with tf.Session(graph=g1) as sess:

    tf.initialize_all_variables().run()

    with tf.variable_scope("",reuse = True):

        print(sess.run(tf.get_variable("c")))


with tf.Session(graph=g2) as sess:

    tf.initialize_all_variables().run()

    with tf.variable_scope("",reuse = True):

        print(sess.run(tf.get_variable("c")))


结果：

<tensorflow.python.framework.ops.Graph object at 0x7f972806de50>

True

WARNING:tensorflow:From /home/lee/anaconda2/lib/python2.7/site-packages/tensorflow/python/util/tf_should_use.py:175: initialize_all_variables (from tensorflow.python.ops.variables) is deprecated and will be removed after 2017-03-02.

Instructions for updating:

Use `tf.global_variables_initializer` instead.

[ 0.]

WARNING:tensorflow:From /home/lee/anaconda2/lib/python2.7/site-packages/tensorflow/python/util/tf_should_use.py:175: initialize_all_variables (from tensorflow.python.ops.variables) is deprecated and will be removed after 2017-03-02.

Instructions for updating:

Use `tf.global_variables_initializer` instead.

[ 1.]


分别在计算图g1和g2中都定义张量c，在g1中初始化为0，在g2中初始化为1，从上面的代码可以看出，当我们运行不同的计算图的时候张量c的值是不一样的。所以，在TensorFlow中可以通过计算图来隔离张量的运算


张量：

import tensorflow as tf  

import numpy as np   

if __name__ == "__main__":  

    a = tf.constant([1.0,2.0],name="a")  

    b = tf.constant([2.0,3.0],name="b")  

    result = tf.add(a,b,name="add")  

    print(result)  

    # Tensor("add:0", shape=(2,), dtype=float32)  

    np_a = np.array([1.0,2.0])  

    np_b = np.array([2.0,3.0])  

    np_result = np_a + np_b  

    print(np_result)  

    # [ 3.  5.]  


在TensorFlow中，可以将张量理解为数组。如果是0阶张量，那么将代表这个张量是一个标量，也就是一个数字，如果是一阶张量可以理解为向量或者是一维数组，n阶张量可以理解为n维的数组。但，TensorFlow张量的实现并没有直接采用数组的形式，张量它只是对运算结果的引用，从下面的例子中可以发现，TensorFlow的张量和numpy的数组是不一样的，TensorFlow的计算结果不是一个数组而是一个张量的结构形式，在这个张量中，它包含了三个重要的属性，名字、维度、类型。


张量的名字，是张量的唯一标识符，通过名字可以发现张量是如何计算出来的。计算图中的每一个节点都代表了一个计算，而计算的结果就保存在张量之中，张量和计算图上的节点所代表的计算结果是对应的。“add:0”代表的是计算节点"add"的第一个输出结果（编号都是从0开始）。

张量的维度，说明了张量的维度信息，如shape=(2,)代表张量是一个二维数组。

张量的类型，说明了张量的数据类型，每一个张量都会有一个对应的数据类型。在TensorFlow的计算中，先会对参与运算的所有张量进行类型检测，如果发现张量的类型不匹配的时候就会报错。
**会话：**
会话：用来执行定义好的运算，而且会话拥有和管理程序运行时的所有资源。当计算完成之后，需要通过关闭会话来帮助系统回收资源，否则可能导致资源泄露的问题。在TensorFlow中使用会话有两种方式。第一种需要明确调用会话生成函数和关闭会话函数

a
 = tf.constant([1.,2.],name="a")  

b = tf.constant([2.0,3.0],name="b")  

result = tf.add(a,b,name="add")  

# 创建session  

sess = tf.Session()  

#获取运算结果  

sess.run(result)  

#关闭会话,释放资源  

sess.close() 


在使用这种模式的时候，需要明确的调用sess.close()来关闭会话释放资源。如果，当程序因为异常而退出的时候，sess.close()有可能不会执行从而导致资源泄露问题的发生。为了解决这种情况，TensorFlow提供了通过python的上下文管理器来使用会话，也就是第二种方式


#通过python的上下文管理器来创建Session，管理会话  

#通过上下文管理来管理会话，不需要调用sess.close()来关闭会话  

#当上下文退出的时候会话会自动关闭和释放资源  

with tf.Session() as sess:  

    sess.run(result) 

参考来自:

1. http://m.blog.csdn.net/v_JULY_v/article/details/78121924


2.http://blog.csdn.net/sinat_29957455/article/details/78307179




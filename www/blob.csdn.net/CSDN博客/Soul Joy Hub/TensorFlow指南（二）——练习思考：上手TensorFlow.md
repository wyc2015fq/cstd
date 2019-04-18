# TensorFlow指南（二）——练习思考：上手TensorFlow - Soul Joy Hub - CSDN博客

2018年01月16日 15:26:46[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：411


[http://blog.csdn.net/u011239443/article/details/79075392](http://blog.csdn.net/u011239443/article/details/79075392)

> 
创建一个计算图而不是直接执行计算的主要好处是什么?主要的缺点是什么?

答：主要好处:
- TensorFlow可以自动计算你的梯度(使用反向模式autodiff)。
- TensorFlow可以在不同的线程中并行地运行并行操作。
- 它使得在不同的设备上运行相同的模型变得更加容易。
- 它简化了检查——例如，在TensorBoard中查看模型。

主要缺点:
- 这使得学习门槛更加陡峭。
- 它使逐步调试更加困难。

> 
以下声明：

```python
a_val = a.eval(session=sess)
```

和：

```python
a_val = sess.run(a)
```

等价么？

答：等价。

> 
以下声明：

```python
a_val, b_val = a.eval(session=sess), b.eval(ses sion=sess)
```

和：

```python
a_val, b_val = sess.run([a, b])
```

等价么？

答：不等价。实际上，第一个语句两次运行该图形(一次用于计算a，一次用于计算b)，而第二个语句只运行一次计算图。如果这些操作(或者它们依赖的操作)有副作用(例如，一个变量被修改，一个条目被插入到一个队列中，或者一个读取同一个文件)，那么效果将会不同。如果它们没有副作用，那么两个语句将返回相同的结果，但是第二个语句将比第一个语句的速度更快。

> 
您能在同一个会话中运行两个计算图吗?

不行

> 
如果您创建一个包含变量w的计算图g，那么启动两个线程并在每个线程中打开一个会话，这两个线程都使用相同的图g，那么每个会话都有自己的变量w的副本，还是它会被共享?

在本地TensorFlow，会话管理变量值,如果您创建一个包含一个变量w图g,然后启动两个线程,每个线程中打开一个本地会话,都使用相同的图g，每个会话将有它自己的变量的副本w。然而,在分布式TensorFlow,变量值存储在容器管理的集群中,如果两个会话连接到相同的集群，并且使用相同的容器中，那么将共享相同的变量值w。

> 
一个变量什么时候初始化?什么时候销毁?

变量在调用它的初始化器时被初始化，当会话结束时它会被销毁。在分布式TensorFlow中，变量在集群中的容器中生存，因此关闭一个会话不会破坏变量。要销毁一个变量，您需要清除它的容器。

> 
placeholder 和 variable 的区别是什么？

- variable 是一个保存值的操作。如果运行该变量，它将返回该值。在运行它之前，需要初始化它。可以更改变量的值(例如，通过使用赋值操作)。它是有状态的:变量在连续运行的计算图上保持相同的值。它通常用于保存模型参数，但也用于其他目的(例如，计算全局训练步骤)。
- 从技术上来说，placeholder 其实并没有什么作用:它们只是持有它们所代表的张量的类型和形状的信息，但它们没有任何价值。事实上，如果试图评估一个依赖于placeholder的操作，那么必须为TensorFlow提供 placeholder 的值(使用提要参数)，否则将得到一个异常。placeholder 通常用于在执行阶段为TensorFlow提供训练或测试数据。它们也可以用于将值传递给赋值节点，以更改变量的值(例如，模型权重)。

> 
如何将一个变量设置为您想要的任何值(在执行阶段)?

在构造计算图时，可以指定一个变量的初始值，当在执行阶段运行变量的初始化器时，它将被初始化。如果您想在执行阶段将该变量的值更改为您想要的任何值，那么最简单的选择是使用 tf.assign()  函数创建一个赋值节点(在图构建阶段)，将variable 和 placeholder 作为参数传递。在执行阶段，可以运行赋值操作，并使用placeholder 填充变量的新值：

```python
import tensorflow as tf
x = tf.Variable(tf.random_uniform(shape=(), minval=0.0, maxval=1.0))
x_new_val = tf.placeholder(shape=(), dtype=tf.float32)
x_assign = tf.assign(x, x_new_val)
with tf.Session():
  x.initializer.run() 
  print(x.eval()) # 0.646157 
  x_assign.eval(feed_dict={x_new_val: 5.0}) 
  print(x.eval()) # 5.0
```

> 
为了计算10个变量的成本函数的梯度，反向传播 autodiff（自动微分发） 需要多少次遍历图?关于正向传播 autodiff 呢? 符号微分法呢?

符号微分法、自动微分法 参阅：[http://blog.csdn.net/u011239443/article/details/79074931](http://blog.csdn.net/u011239443/article/details/79074931)

反向 autodiff(由TensorFlow实现)只需遍历图两次，就可以计算成本函数的梯度，与任意数量的变量有关。另一方面，正向 autodiff 需要为每个变量运行一次(如果我们想要10个不同的变量，则需要10次)。至于符号微分，它会构建一个不同的图来计算梯度，所以它不会完全穿越原始的图(除了构建新的梯度图)。一个高度优化的符号微分系统可能运行新的梯度图，一次计算所有变量的梯度，但是与原始图相比，这个新图可能会非常复杂低效。 
![](http://upload-images.jianshu.io/upload_images/1621805-e7dd694099f756b7?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)


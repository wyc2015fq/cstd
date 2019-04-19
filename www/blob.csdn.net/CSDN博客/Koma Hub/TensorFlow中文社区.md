# TensorFlow中文社区 - Koma Hub - CSDN博客
2019年03月25日 21:45:26[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：46
[http://www.tensorfly.cn/tfdoc/get_started/introduction.html](http://www.tensorfly.cn/tfdoc/get_started/introduction.html)
**Table of Contents**
[简介](#%E7%AE%80%E4%BB%8B-)
# 简介
本章的目的是让你了解和运行 TensorFlow!
在开始之前, 让我们先看一段使用 Python API 撰写的 TensorFlow 示例代码, 让你对将要学习的内容有初步的印象.
这段很短的 Python 程序生成了一些三维数据, 然后用一个平面拟合它.
```python
import tensorflow as tf
import numpy as np
# 使用 NumPy 生成假数据(phony data), 总共 100 个点.
x_data = np.float32(np.random.rand(2, 100)) # 随机输入
y_data = np.dot([0.100, 0.200], x_data) + 0.300
# 构造一个线性模型
# 
b = tf.Variable(tf.zeros([1]))
W = tf.Variable(tf.random_uniform([1, 2], -1.0, 1.0))
y = tf.matmul(W, x_data) + b
# 最小化方差
loss = tf.reduce_mean(tf.square(y - y_data))
optimizer = tf.train.GradientDescentOptimizer(0.5)
train = optimizer.minimize(loss)
# 初始化变量
init = tf.initialize_all_variables()
# 启动图 (graph)
sess = tf.Session()
sess.run(init)
# 拟合平面
for step in xrange(0, 201):
    sess.run(train)
    if step % 20 == 0:
        print step, sess.run(W), sess.run(b)
# 得到最佳拟合结果 W: [[0.100  0.200]], b: [0.300]
```
为了进一步激发你的学习欲望, 我们想让你先看一下 TensorFlow 是如何解决一个经典的机器 学习问题的. 在神经网络领域, 最为经典的问题莫过于 MNIST 手写数字分类问题. 我们准备了 两篇不同的教程, 分别面向机器学习领域的初学者和专家. 如果你已经使用其它软件训练过许多 MNIST 模型, 请阅读高级教程 (红色药丸链接). 如果你以前从未听说过 MNIST, 请阅读初级教程 (蓝色药丸链接). 如果你的水平介于这两类人之间, 我们建议你先快速浏览初级教程, 然后再阅读高级教程.
**blue bill：**[http://www.tensorfly.cn/tfdoc/tutorials/mnist_beginners.html](http://www.tensorfly.cn/tfdoc/tutorials/mnist_beginners.html)
**red bill：**[http://www.tensorfly.cn/tfdoc/tutorials/mnist_pros.html](http://www.tensorfly.cn/tfdoc/tutorials/mnist_pros.html)
如果你已经下定决心, 准备学习和安装 TensorFlow, 你可以略过这些文字, 直接阅读 后面的章节. 不用担心, 你仍然会看到 MNIST -- 在阐述 TensorFlow 的特性时, 我们还会使用 MNIST 作为一个样例.

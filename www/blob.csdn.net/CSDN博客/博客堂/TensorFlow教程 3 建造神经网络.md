# TensorFlow教程 3 建造神经网络 - 博客堂 - CSDN博客





2017年07月16日 22:46:54[最小森林](https://me.csdn.net/u012052268)阅读数：517








# 3 建造神经网络


- [建造神经网络](#3-建造神经网络)- [1 构造添加一个神经层的函数](#31-构造添加一个神经层的函数)
- [2 构造神经网络并可视化训练](#32-构造神经网络并可视化训练)
- [3 优化器 optimizer](#33-优化器-optimizer)




## 3.1 构造添加一个神经层的函数

定义添加神经层的函数def add_layer(),它有四个参数：输入值、输入的大小、输出的大小和激励函数，我们设定默认的激励函数是None。

```python
def add_layer(inputs, in_size, out_size, activation_function=None):  #  我们设定默认的激励函数是None。
    Weights = tf.Variable(tf.random_normal([in_size, out_size]))  #  weights为一个in_size行, out_size列的随机变量矩阵
    biases = tf.Variable(tf.zeros([1, out_size]) + 0.1)
    Wx_plus_b = tf.matmul(inputs, Weights) + biases  #  我们定义Wx_plus_b, 即神经网络未激活的值。其中，tf.matmul()是矩阵的乘法
    if activation_function is None:
        outputs = Wx_plus_b
    else:
        outputs = activation_function(Wx_plus_b)
    return outputs
```

## 3.2 构造神经网络并可视化训练

```python
# 完整的构建神经网络并训练

import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt

## 定义一个方法，用于构建神经层（在上次课程中有详细介绍）
def add_layer(inputs, in_size, out_size, activation_function=None):
    Weights = tf.Variable(tf.random_normal([in_size, out_size]))
    biases = tf.Variable(tf.zeros([1, out_size]) + 0.1)
    Wx_plus_b = tf.matmul(inputs, Weights) + biases
    if activation_function is None:
        outputs = Wx_plus_b
    else:
        outputs = activation_function(Wx_plus_b)
    return outputs

## 主体方法
# 构建所需的数据。 这里的x_data和y_data并不是严格的一元二次函数的关系，因为我们多加了一个noise,这样看起来会更像真实情况。
x_data = np.linspace(-1,1,300, dtype=np.float32)[:, np.newaxis]
noise = np.random.normal(0, 0.05, x_data.shape).astype(np.float32)
y_data = np.square(x_data) - 0.5 + noise


#数据可视化,用散点图画出真实数据
# plot the real data
fig = plt.figure()
ax = fig.add_subplot(1,1,1)
ax.scatter(x_data, y_data)
plt.ion()#plt原本会暂停程序，加上这句就不会暂停了
plt.show()


# 定义输入占位符
# 利用占位符定义我们所需的神经网络的输入。 tf.placeholder()就是代表占位符，这里的None代表无论输入有多少都可以，因为输入只有一个特征，所以这里是1。
xs = tf.placeholder(tf.float32, [None, 1])
ys = tf.placeholder(tf.float32, [None, 1])

#默认一个输入（维度1），定义一个隐藏层 ，一个输出层
l1 = add_layer(xs, 1, 10, activation_function=tf.nn.relu)
prediction = add_layer(l1, 10, 1, activation_function=None)

#定义误差函数
loss = tf.reduce_mean(tf.reduce_sum(tf.square(ys - prediction),reduction_indices=[1]))

#选取梯度下降优化器进行训练 很关键的一步，如何让机器学习提升它的准确率。tf.train.GradientDescentOptimizer()中的值通常都小于1，这里取的是0.1，代表以0.1的效率来最小化误差loss。
train_step = tf.train.GradientDescentOptimizer(0.1).minimize(loss)

#初始化 tensorflow变量，并用会话激活
init = tf.global_variables_initializer()
sess = tf.Session()
sess.run(init)

#开始训练
#我们让机器学习1000次。机器学习的内容是train_step, 用 Session 来 run 每一次 training 的数据，逐步提升神经网络的预测准确性。 (注意：当运算要用到placeholder时，就需要feed_dict这个字典来指定输入。)
for i in range(1000):
    #  to visualize the result and improvement
    try:
        ax.lines.remove(lines[0])
    except Exception:
        pass
    sess.run(train_step, feed_dict={xs: x_data, ys: y_data})
    if i % 50 == 0:
        # to see the step improvement
        prediction_value = sess.run(prediction, feed_dict={xs: x_data})
         # plot the prediction
        lines = ax.plot(x_data, prediction_value, 'r-', lw=5)
        plt.pause(0.7)
```

最后，机器学习的结果为： 
![image](https://morvanzhou.github.io/static/results/tensorflow/3_3_2.png)
## 3.3 优化器 optimizer

Tensorflow 中的优化器会有很多不同的种类。最基本, 也是最常用的一种就是GradientDescentOptimizer。

在Google搜索中输入“tensorflow optimizer”可以看到Tensorflow提供了7种优化器：[链接](https://www.tensorflow.org/versions/r0.11/api_docs/python/train.html)

![image](https://morvanzhou.github.io/static/results/tensorflow/3_4_1.png)

更多关系 Optimizer 的解释, 请参考 机器学习-简介系列 [Optimizer](https://morvanzhou.github.io/tutorials/machine-learning/ML-intro/3-06-speed-up-learning/)





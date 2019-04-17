# TensorFlow教程 4 Tensorboard 可视化好帮手 - 博客堂 - CSDN博客





2017年07月19日 15:48:54[最小森林](https://me.csdn.net/u012052268)阅读数：3358








# 4 Tensorboard 可视化好帮手


- [Tensorboard 可视化好帮手](#4-tensorboard-可视化好帮手)- [1 Tensorboard简介](#41-tensorboard简介)
- [2 绘制图层与其中的参数](#42-绘制图层与其中的参数)- [21 对隐藏层](#421-对隐藏层)
- [22 loss函数](#422-loss函数)
- [23 train_step部分](#423-trainstep部分)

- [3 保存并执行绘图](#43-保存并执行绘图)- [31 保存绘画](#431-保存绘画)
- [32 运行绘画1](#432-运行绘画1)
- [33 运行绘画2](#433-运行绘画2)

- [4 可视化训练过程](#44-可视化训练过程)- [41 distributions](#441-distributions)
- [42 events](#442-events)
- [43 给所有训练图合并](#443-给所有训练图合并)
- [44 训练数据](#444-训练数据)





## 4.1 Tensorboard简介

学会用 Tensorflow 自带的 tensorboard 去可视化我们所建造出来的神经网络是一个很好的学习理解方式.

用最直观的流程图告诉你你的神经网络是长怎样,有助于你发现编程中间的问题和疑问.

通过tensorboard的工具大致可以看到，上节课要显示的神经网络差不多是这样子的： 
![image](https://morvanzhou.github.io/static/results/tensorflow/4_1_1.png)
同时我们也可以展开看每个layer中的一些具体的结构： 
![image](https://morvanzhou.github.io/static/results/tensorflow/4_1_2.png)
## 4.2 绘制图层与其中的参数

主要用到两个语法： 

1. 定义图层：with tf.name_scope() （ 里面写名字，下面用缩进） 

2. 定义参数：增加参数变量的属性name
### 4.2.1 对隐藏层

例如对添加的隐藏层进行画图，最终，layer形式如下：

```python
def add_layer(inputs, in_size, out_size, activation_function=None):
    # add one more layer and return the output of this layer
    with tf.name_scope('layer'):
        with tf.name_scope('weights'):
            Weights = tf.Variable(tf.random_normal([in_size, out_size]),name='W')
        with tf.name_scope('biases'):
            biases = tf.Variable(tf.zeros([1,out_size]) + 0.1,name='b')
        with tf.name_scope('Wx_plus_b'):
            Wx_plus_b = tf.add(tf.matmul(inputs, Weights),biases)
        if activation_function is None:
            outputs = Wx_plus_b
        else:
            outputs = activation_function(Wx_plus_b, )
        return outputs
```

效果如下：（有没有看见刚才定义layer里面的“内部构件”呢？） 
![image](https://morvanzhou.github.io/static/results/tensorflow/4_1_4.png)
### 4.2.2 loss函数

再例如对loss部分：

```
# the error between prediciton and real data
with tf.name_scope('loss'):
    loss = tf.reduce_mean(
    tf.reduce_sum(
    tf.square(ys - prediction),
    eduction_indices=[1]
    ))
```

这句话就是“绘制” loss了， 如下： 
![image](https://morvanzhou.github.io/static/results/tensorflow/4_1_4.png)
### 4.2.3 train_step部分

```java
with tf.name_scope('train'):
    train_step = tf.train.GradientDescentOptimizer(0.1).minimize(loss)
```

形式都相同。

## 4.3 保存并执行绘图
- 
保存绘画：tf.summary.FileWriter()运行程序，生成绘画文件

- 
运行绘画1：在CMD中tensorboard –logdir logs设定文件目录

- 
打开Google Chrome：[http://localhost:6006](http://localhost:6006)


### 4.3.1 保存绘画

我们需要使用 tf.summary.FileWriter()  将上面‘绘画’出的图保存到一个目录中，以方便后期在浏览器中可以浏览。

这个方法中的第二个参数需要使用sess.graph ， 因此我们需要把这句话放在获取session的后面。 这里的graph是将前面定义的框架信息收集起来，然后放在logs/目录下面。

```
sess = tf.Session() # get session

writer = tf.summary.FileWriter("logs/", sess.graph)
```

这句话就把当前的绘图保存到，当前py文件下的logs中去了。

### 4.3.2 运行绘画1

1.在你的terminal（终端）中 ，cd进入你的logs所在目录。（或者shift进入）

2.使用以下命令

```
tensorboard --logdir logs
# 设置告诉tensorboard绘画在/logs 目录中
```

### 4.3.3 运行绘画2

浏览器是 “Google Chrome”. 使用 [http://localhost:6006](http://localhost:6006)

tensorboard 还有很多其他的参数，希望大家可以多多了解, 可以使用 tensorboard –help 查看tensorboard的详细参数 

4.3最终的全部[代码在这里](https://github.com/MorvanZhou/tutorials/blob/master/tensorflowTUT/tf14_tensorboard/full_code.py)

## 4.4 可视化训练过程

tensorboard还可以可视化训练过程( biase等参数的变化过程) , 这节重点讲一下可视化训练过程的图标是如何做的 。请看下图, 这是如何做到的呢？ 
![image](https://morvanzhou.github.io/static/results/tensorflow/4_2_1.png)
### 4.4.1 distributions

制作对Weights和biases的变化图表distributions。

我们层中的Weights设置变化图, tensorflow中提供了tf.histogram_summary()方法,用来绘制图片, 第一个参数是图表的名称, 第二个参数是图表要记录的变量

```java
with tf.name_scope('layer'):
         with tf.name_scope('weights'):
              Weights= tf.Variable(tf.random_normal([in_size,out_size]),name='W')
              tf.summary.histogram(layer_name + '/weights', Weights)
```

同样的方法我们对biases进行绘制图标:

```java
with tf.name_scope('biases'):
    biases = tf.Variable(tf.zeros([1,out_size])+0.1, name='b')
    tf.summary.histogram(layer_name + '/biases', biases)
```

至于activation_function 可以不绘制. 我们对output 使用同样的方法:

```bash
tf.summary.histogram(layer_name + '/outputs', outputs)
```

最终经过我们的修改 , addlayer()方法成为如下的样子:

```python
def add_layer(inputs , 
              in_size, 
              out_size,n_layer, 
              activation_function=None):
    ## add one more layer and return the output of this layer
    layer_name='layer%s'%n_layer
    with tf.name_scope('layer'):
         with tf.name_scope('weights'):
              Weights= tf.Variable(tf.random_normal([in_size, out_size]),name='W')
              # tf.histogram_summary(layer_name+'/weights',Weights)
              tf.summary.histogram(layer_name + '/weights', Weights) # tensorflow >= 0.12

         with tf.name_scope('biases'):
              biases = tf.Variable(tf.zeros([1,out_size])+0.1, name='b')
              # tf.histogram_summary(layer_name+'/biase',biases)
              tf.summary.histogram(layer_name + '/biases', biases)  # Tensorflow >= 0.12

         with tf.name_scope('Wx_plus_b'):
              Wx_plus_b = tf.add(tf.matmul(inputs,Weights), biases)

         if activation_function is None:
            outputs=Wx_plus_b
         else:
            outputs= activation_function(Wx_plus_b)

         # tf.histogram_summary(layer_name+'/outputs',outputs)
         tf.summary.histogram(layer_name + '/outputs', outputs) # Tensorflow >= 0.12

    return outputs
```

修改之后的名称会显示在每个tensorboard中每个图表的上方显示, 如下图所示: 
![image](https://morvanzhou.github.io/static/results/tensorflow/4_2_5.png)
### 4.4.2 events

Loss 的变化图和之前设置的方法略有不同. loss是在tesnorBorad 的event下面的, 这是由于我们使用的是tf.scalar_summary() 方法.

![image](https://morvanzhou.github.io/static/results/tensorflow/4_2_6.png)

观看loss的变化比较重要. 当你的loss呈下降的趋势,说明你的神经网络训练是有效果的.

修改后的代码片段如下：

```python
with tf.name_scope('loss'):
     loss= tf.reduce_mean(tf.reduce_sum(tf.square(ys- prediction), reduction_indices=[1]))
     tf.summary.scalar('loss', loss) # tensorflow >= 0.12
```

### 4.4.3 给所有训练图‘合并‘

接下来， 开始合并打包。 tf.summary.merge_all() 方法会对我们所有的 summaries 合并到一起. 因此在原有代码片段中添加：

```
sess= tf.Session()

merged = tf.summary.merge_all() 

writer = tf.summary.FileWriter("logs/", sess.graph) 

sess.run(tf.global_variables_initializer())
```

### 4.4.4 训练数据

假定给出了x_data,y_data并且训练1000次.

以上这些仅仅可以记录很绘制出训练的图表， 但是不会记录训练的数据。 为了较为直观显示训练过程中每个参数的变化，我们每隔上50次就记录一次结果 , 同时我们也应注意, merged 也是需要run 才能发挥作用的,所以在for循环中写下：

```
if i%50 == 0:
    rs = sess.run(merged,feed_dict={xs:x_data,ys:y_data})
    writer.add_summary(rs, i)
```

最后修改后的片段如下：

```
for i in range(1000):
   sess.run(train_step, feed_dict={xs:x_data, ys:y_data})
   if i%50 == 0:
      rs = sess.run(merged,feed_dict={xs:x_data,ys:y_data})
      writer.add_summary(rs, i)
```

程序运行完毕之后, 会产生logs目录 , 使用命令 tensorboard –logdir logs

将输出中显示的URL地址粘贴到浏览器中便可以查看. 最终的效果如下: 
![image](https://morvanzhou.github.io/static/results/tensorflow/4_2_8.png)
4.3最终的全部[代码在这里](https://github.com/MorvanZhou/tutorials/blob/master/tensorflowTUT/tf15_tensorboard/full_code.py)

```python
"""
Please note, this code is only for python 3+. If you are using python 2+, please modify the code accordingly.
"""
from __future__ import print_function
import tensorflow as tf
import numpy as np


def add_layer(inputs, in_size, out_size, n_layer, activation_function=None):
    # add one more layer and return the output of this layer
    layer_name = 'layer%s' % n_layer
    with tf.name_scope(layer_name):
        with tf.name_scope('weights'):
            Weights = tf.Variable(tf.random_normal([in_size, out_size]), name='W')
            tf.summary.histogram(layer_name + '/weights', Weights)
        with tf.name_scope('biases'):
            biases = tf.Variable(tf.zeros([1, out_size]) + 0.1, name='b')
            tf.summary.histogram(layer_name + '/biases', biases)
        with tf.name_scope('Wx_plus_b'):
            Wx_plus_b = tf.add(tf.matmul(inputs, Weights), biases)
        if activation_function is None:
            outputs = Wx_plus_b
        else:
            outputs = activation_function(Wx_plus_b, )
        tf.summary.histogram(layer_name + '/outputs', outputs)
    return outputs


# Make up some real data
x_data = np.linspace(-1, 1, 300)[:, np.newaxis]
noise = np.random.normal(0, 0.05, x_data.shape)
y_data = np.square(x_data) - 0.5 + noise

# define placeholder for inputs to network
with tf.name_scope('inputs'):
    xs = tf.placeholder(tf.float32, [None, 1], name='x_input')
    ys = tf.placeholder(tf.float32, [None, 1], name='y_input')

# add hidden layer
l1 = add_layer(xs, 1, 10, n_layer=1, activation_function=tf.nn.relu)
# add output layer
prediction = add_layer(l1, 10, 1, n_layer=2, activation_function=None)

# the error between prediciton and real data
with tf.name_scope('loss'):
    loss = tf.reduce_mean(tf.reduce_sum(tf.square(ys - prediction),
                                        reduction_indices=[1]))
    tf.summary.scalar('loss', loss)

with tf.name_scope('train'):
    train_step = tf.train.GradientDescentOptimizer(0.1).minimize(loss)

sess = tf.Session()
merged = tf.summary.merge_all()

writer = tf.summary.FileWriter("logs/", sess.graph)

init = tf.global_variables_initializer()
sess.run(init)

for i in range(1000):
    sess.run(train_step, feed_dict={xs: x_data, ys: y_data})
    if i % 50 == 0:
        result = sess.run(merged,
                          feed_dict={xs: x_data, ys: y_data})
        writer.add_summary(result, i)

# direct to the local dir and run this in terminal:
# $ tensorboard --logdir logs
# Google Chrome”访问 http://localhost:6006
```












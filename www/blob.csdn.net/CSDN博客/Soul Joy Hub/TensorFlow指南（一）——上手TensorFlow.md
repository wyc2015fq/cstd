# TensorFlow指南（一）——上手TensorFlow - Soul Joy Hub - CSDN博客

2018年01月15日 16:35:15[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：1190


[http://blog.csdn.net/u011239443/article/details/79066094](http://blog.csdn.net/u011239443/article/details/79066094)

TensorFlow是谷歌开源的深度学习库。不多介绍，相信准备学习TensorFlow的同学也会自己去更多的了解。本系列博文讲尽量不涉及深度学习理论，但是会给出相关理论对应的博文等资料供大家参阅。

这里，首先要跟大家介绍一个计算图的概念： 
![](http://upload-images.jianshu.io/upload_images/1621805-fd9d31cd208ba061.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

TensorFlow会根据代码先创建好计算图，然后数据会再流入这样的计算图中： 
![](http://upload-images.jianshu.io/upload_images/1621805-158841ca120f6499.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

这个概念能帮助我们在编码的时候更好的去理解。我们再来理解一下TensorFlow字面上的意思:

> 
Tensor，张量，其实我们可以简单的理解为是多维数组，这也是TensorFlow中的基本数据结构。 

  Flow，流，很直观的表达了，Tensor之间的转化是一种类似于数据流的方式。

# 1. 安装

大家可以根据官网 [https://www.tensorflow.org/install/](https://www.tensorflow.org/install/)  ，安装还是非常方便的。

# 2. 初体验

先建立计算图：

```python
import tensorflow as tf
x = tf.Variable(3, name="x") 
y = tf.Variable(4, name="y") 
f=x*x*y+y+2
```

计算：

```python
init = tf.global_variables_initializer() # 准备初始化全局变量，及 x 和 y
with tf.Session() as sess:
    init.run() # 初始化
    result = f.eval() # 计算 f
print(result)
```

可以看到，TensorFlow编码具有惰性，`with tf.Session()`之前的代码不会直接运行，而是最后在`Session`中`run`的。

# 3. 节点值的生命周期

这里需要注意的概念是:

> 
多个`run`不会重用已计算过的相同节点的值。 

  什么意思呢？我们来看下面这段代码：

```python
w = tf.constant(3) 
x=w+2 
y=x+5 
z=x*3
with tf.Session() as sess: 
  print(y.eval()) # 10 
  print(z.eval()) # 15
```

这段代码`Session`中会有两个`run`，第一个`run`先计算出w，再计算出x，最后计算出y。第二个`run`不会直接去复用x，而是从头开始，先计算出w，再计算出x，最后计算出z。如果想避免重复计算，必须只使用一个`run`，如下：

```python
with tf.Session() as sess:
  y_val, z_val = sess.run([y, z]) 
  print(y_val) # 10 
  print(z_val) # 15
```

# 4. 实现梯度下降

## 4.1 手动计算梯度下降

梯度下降的理论概念可以参阅： 
[http://blog.csdn.net/u011239443/article/details/77852727#t3](http://blog.csdn.net/u011239443/article/details/77852727#t3)

sklearn中有加利福尼亚的房价数据，我们实践下，使用梯度下降来回归预测加利福尼亚的房价数据：

```python
# -*- coding:utf-8 -*- 
import numpy as np
import tensorflow as tf
from sklearn.datasets import fetch_california_housing
from sklearn.preprocessing import StandardScaler

n_epochs = 1000 # 迭代次数
learning_rate = 0.01

housing = fetch_california_housing()
m,n = housing.data.shape
```

StandardScaler() 将特征进行标准归一化，可参阅：[http://blog.csdn.net/u011239443/article/details/76360294#t2](http://blog.csdn.net/u011239443/article/details/76360294#t2)

```
scaler = StandardScaler()
scaled_housing_data = scaler.fit_transform(housing.data)
scaled_housing_data_plus_bias = np.c_[np.ones((m, 1)), scaled_housing_data]

X = tf.constant(scaled_housing_data_plus_bias,dtype=tf.float32)
y = tf.constant(housing.target.reshape(-1,1),dtype=tf.float32)
theta = tf.Variable(tf.random_uniform([n+1,1],-1.0,1.0)) # 用随机数初始化
y_pred = tf.matmul(X,theta)
error = y_pred - y
mse = tf.reduce_mean(tf.square(error))
gradients = 2/m * tf.matmul(tf.transpose(X),error)
training_op = tf.assign(theta,theta-learning_rate * gradients)

init = tf.global_variables_initializer()

with tf.Session() as sess:
    sess.run(init)
    #for x in theta:
    #    print(x)
    for epoch in range(n_epochs):
        if epoch % 100 == 0:
            print("Epoch", epoch, "MSE =", mse.eval())
        sess.run(training_op)
```

```
Epoch 0 MSE = 6.46781
Epoch 100 MSE = 0.664388
Epoch 200 MSE = 0.543974
Epoch 300 MSE = 0.536818
Epoch 400 MSE = 0.534512
Epoch 500 MSE = 0.532798
Epoch 600 MSE = 0.531383
Epoch 700 MSE = 0.530206
Epoch 800 MSE = 0.529225
Epoch 900 MSE = 0.528407
```

## 4.2 梯度下降优化器

TensorFlow也有直接自己封装好的梯度下降优化器 GradientDescentOptimizer ，我们可以直接使用它来优化 

 mse。我们只需要把上述代码中的：

```python
gradients = 2/m * tf.matmul(tf.transpose(X),error)
training_op = tf.assign(theta,theta-learning_rate * gradients)
```

替换成

```python
optimizer = tf.train.GradientDescentOptimizer(learning_rate=learning_rate)
training_op = optimizer.minimize(mse)
```

```
Epoch 0 MSE = 7.25867
Epoch 100 MSE = 0.694411
Epoch 200 MSE = 0.577526
Epoch 300 MSE = 0.56535
Epoch 400 MSE = 0.55735
Epoch 500 MSE = 0.551041
Epoch 600 MSE = 0.54601
Epoch 700 MSE = 0.541978
Epoch 800 MSE = 0.538734
Epoch 900 MSE = 0.536115
```

# 5. Mini-batch 梯度下降

参阅：[http://blog.csdn.net/u011239443/article/details/78066082#t0](http://blog.csdn.net/u011239443/article/details/78066082#t0)

TensorFlow可以利用 placeholder 将微批数据喂给训练模型。

```python
X = tf.placeholder(tf.float32,(None,n+1),'X')
y = tf.placeholder(tf.float32,(None,1),'y')
batch_size = 100
n_batches = int(np.ceil(m/batch_size))

def fetch_batch(epoch,batch_index,batch_size):
    np.random.seed(epoch*n_batches+batch_index) # 每次调用 都有不同的 随机种子
    indices = np.random.randint(m,size=batch_size) # 去 0 ~ m-1 之间去 batch_size 整数
    X_batch = scaled_housing_data_plus_bias[indices]
    y_batch = housing.target.reshape(-1,1)[indices]
    return X_batch,y_batch

with tf.Session() as sess:
    sess.run(init)

    for epoch in range(n_epochs):
        if epoch%100==0:
            print("Epoch", epoch, "MSE =", mse.eval())

        for batch_index in range(n_batches):
            X_batch,y_batch = fetch_batch(epoch,batch_index,batch_size)
            sess.run(training_op,feed_dict={X:X_batch,y:y_batch})
```

```
Epoch 0 MSE = 12.6052
Epoch 100 MSE = 0.524321
Epoch 200 MSE = 0.524321
Epoch 300 MSE = 0.524321
Epoch 400 MSE = 0.524321
Epoch 500 MSE = 0.524321
Epoch 600 MSE = 0.524321
Epoch 700 MSE = 0.524321
Epoch 800 MSE = 0.524321
Epoch 900 MSE = 0.524321
```

# 6. 保存 & 加载 模型

我们可以使用 Saver 来保存或者加载已经训练好的模型：

```python
saver = tf.train.Saver()
with tf.Session() as sess:
    sess.run(init)

    for epoch in range(n_epochs):
        if epoch%100==0:
            print("Epoch", epoch, "MSE =", mse.eval())

        for batch_index in range(n_batches):
            X_batch,y_batch = fetch_batch(epoch,batch_index,batch_size)
            sess.run(training_op,feed_dict={X:X_batch,y:y_batch})
    save_path = saver.save(sess,'./my_model_final.ckpt')
```

可以看到路径下的文件：

![](http://upload-images.jianshu.io/upload_images/1621805-9cc41a753fd04a3a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

我们可以使用该路径加载模型：

```python
with tf.Session() as sess:
    saver.restore(sess,'./my_model_final.ckpt')
    print(mse.eval())
```

```
INFO:tensorflow:Restoring parameters from ./my_model_final.ckpt
0.524321
```

# 7. 使用 TensorBoard 可视化训练

TensorFlow在训练模型的时候可以将训练过程通过日志保存下来。TensorBoard可以根据这些日志来可视化训练过程。 

首先，我们利用当前时间给日志文件起一个唯一名字：

```python
from datetime import datetime
now = datetime.utcnow().strftime("%Y%m%d%H%M%S")
root_logs = 'tf_logs'
logdir = "{}/run-{}".format(root_logs,now)
```
- 
scalar 用于获取摘要信息，

- 
FileWriter类提供了一种机制，在给定的目录中创建事件文件，并向其添加摘要和事件。该类将异步更新文件内容。这允许训练程序调用方法，直接从训练循环中直接向文件添加数据，而不需要减慢训练的速度。

```
mse_summary = tf.summary.scalar('MSE',mse)
file_writer = tf.summary.FileWriter(logdir,tf.get_default_graph())

with tf.Session() as sess:
    sess.run(init)

    for epoch in range(n_epochs):
        for batch_index in range(n_batches):
            X_batch,y_batch = fetch_batch(epoch,batch_index,batch_size)
            if batch_index % 10 == 0:
                summary_str = mse_summary.eval(feed_dict={X:X_batch,y:y_batch})
                step = epoch * n_batches + batch_index
                file_writer.add_summary(summary_str,step)
            sess.run(training_op, feed_dict={X: X_batch, y: y_batch})
```

目录下生成了文件： 
![](http://upload-images.jianshu.io/upload_images/1621805-2f65c3fbbe27a660.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

命令运行TensorBoard：
`tensorboard --logdir tf_logs/`
![](http://upload-images.jianshu.io/upload_images/1621805-2e04e497ec69933f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

浏览器中打开该链接，可以看到训练过程中mse可视化的展示： 
![](http://upload-images.jianshu.io/upload_images/1621805-ab5d6e46538cc528.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

按GRAPHS，可以看到计算图 
![](http://upload-images.jianshu.io/upload_images/1621805-dd23a973818068fe.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 8.名字空间

当处理更复杂的模型，比如神经网络时，计算图很容易就会变成成千上万个节点。为了避免这种情况，我们可以创建名称范围来分组相关的节点。比如我们把上述代码中的 error 和 mse 发到 “loss”空间里面：

```python
with tf.name_scope("loss") as scope:
    error = y_pred - y
    mse = tf.reduce_mean(tf.square(error), name="mse")
```

使用tensorboard，我们可以看到新的计算图： 
![](http://upload-images.jianshu.io/upload_images/1621805-8d92571c5c6cc9e0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

```python
print(error.op.name)
print(mse.op.name)
```

```
loss/sub
loss/mse
```

# 9. 模块化

我们先试着实现这样功能的代码：两个回归模型，分别喂给ReLU，再将得到的结果相加：

```python
reset_graph()
n_features = 3
X = tf.placeholder(tf.float32,(None,n_features),name='X')
w1 = tf.Variable(tf.random_normal((n_features,1)),name='w1')
w2 = tf.Variable(tf.random_normal((n_features,1)),name='w2')
b1 = tf.Variable(0.0,name='b1')
b2 = tf.Variable(0.0,name='b2')

z1 = tf.add(tf.matmul(X,w1),b1,name='z1')
z2 = tf.add(tf.matmul(X,w2),b2,name='z1')

relu1 = tf.maximum(0.0,z1,name='relu1')
relu2 = tf.maximum(0.0,z2,name='relu1')

output = tf.add(relu1,relu2)

file_writer = tf.summary.FileWriter("logs/relu0", tf.get_default_graph())
```

得到的计算图： 
![](http://upload-images.jianshu.io/upload_images/1621805-0be0d1cf02eebc8f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

看代码，就感觉有好多重复的地方。如果需要10、20个ReLU相加的话，可想而知就会非常冗余。

我们可以使用模块化的方法将其简化：

```python
reset_graph()

def relu(X):
    with tf.name_scope('relu'):
        w_shape = (int(X.get_shape()[1]),1)
        w = tf.Variable(tf.random_normal(w_shape),name='w')
        b = tf.Variable(0.0,name='b')
        z = tf.add(tf.matmul(X,w),b)
        return tf.maximum(0.0,z,name='max')

n_features = 3
X = tf.placeholder(tf.float32,(None,n_features),name='X')
relus = [relu(X) for i in range(10)]
output = tf.add_n(relus)

file_writer = tf.summary.FileWriter("logs/relu2", tf.get_default_graph())
file_writer.close()
```

这里除了使用模块化，还将节点塞入了名字空间’relu’中。模块化会自动生成10个 relu 节点，第一个 relu 节点名字为‘relu’，第二个为‘relu_1’，以此类推。add_n 会将 tensor list 中的元素累加。我们可以看到计算图为：

![](http://upload-images.jianshu.io/upload_images/1621805-7ed36422c0c6c3bb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 10.共享变量

继续上面的代码，如果我们想使用 想给各个 ReLu 设置一个阈值，该怎么实现呢：

```python
reset_graph()

def relu(X):
    with tf.variable_scope('relu',reuse=True):
        threshold = tf.get_variable('threshold')
        w_shape = (int(X.get_shape()[1]),1)
        w = tf.Variable(tf.random_normal(w_shape),name='w')
        b = tf.Variable(0.0,name='b')
        z = tf.add(tf.matmul(X,w),b)
        return tf.maximum(threshold,z,name='max')

n_features = 3
X = tf.placeholder(tf.float32,(None,n_features),name='X')
with tf.variable_scope('relu'):
    threshold = tf.get_variable('threshold',shape=(),initializer=tf.constant_initializer(0.0))
relus = [relu(X) for i in range(10)]
output = tf.add_n(relus)

file_writer = tf.summary.FileWriter("logs/relu3", tf.get_default_graph())
file_writer.close()
```

在 relu 函数外创建一个变量空间‘relu’，在该变量空间里面创建threshold，并初始化。在‘relu’变量空间下，是共享一个threshold的。所以，将relu函数中的变量塞到 在‘relu’变量空间下，设置 

 reuse=True。在‘relu’变量空间下，调用get_variable(‘threshold’)，就能得到共享变量 threshold。计算图如下： 
![](http://upload-images.jianshu.io/upload_images/1621805-aae3fb7d350453ee.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1621805-e7dd694099f756b7?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)


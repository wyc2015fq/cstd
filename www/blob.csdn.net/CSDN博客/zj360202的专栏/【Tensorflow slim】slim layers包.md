# 【Tensorflow slim】slim layers包 - zj360202的专栏 - CSDN博客





2017年11月21日 16:13:41[zj360202](https://me.csdn.net/zj360202)阅读数：1486








tensorflow的操作符集合是十分广泛的，神经网络开发者通常会以更高层的概念，比如"layers", "losses",
 "metrics", and "networks"去考虑模型。一个层，比如卷积层、全连接层或bn层，要比一个单独的tensorflow操作符更抽象，并且通常会包含若干操作符。此外，和原始操作符不同，一个层经常（不总是）有一些与自己相关的变量（可调参数）。例如，在神经网络中，一个卷积层由许多底层操作符组成：



1. 创建权重、偏置变量

2. 将来自上一层的数据和权值进行卷积

3. 在卷积结果上加上偏置

4. 应用激活函数





```python
input = ...  
with tf.name_scope('conv1_1') as scope:  
  kernel = tf.Variable(tf.truncated_normal([3, 3, 64, 128], dtype=tf.float32,  
                                           stddev=1e-1), name='weights')  
  conv = tf.nn.conv2d(input, kernel, [1, 1, 1, 1], padding='SAME')  
  biases = tf.Variable(tf.constant(0.0, shape=[128], dtype=tf.float32),  
                       trainable=True, name='biases')  
  bias = tf.nn.bias_add(conv, biases)  
  conv1 = tf.nn.relu(bias, name=scope)
```
为了缓解重复这些代码，TF-Slim在更抽象的神经网络层的层面上提供了大量方便使用的操作符。比如，将上面的代码和TF-Slim响应的代码调用进行比较：





```python
input = ...  
net = slim.conv2d(input, 128, [3, 3], scope='conv1_1')
```
TF-Slim提供了标准接口用于组建神经网络，包括：


|Layer|TF-Slim|
|----|----|
|BiasAdd|[slim.bias_add](https://www.tensorflow.org/code/tensorflow/contrib/layers/python/layers/layers.py)|
|BatchNorm|[slim.batch_norm](https://www.tensorflow.org/code/tensorflow/contrib/layers/python/layers/layers.py)|
|Conv2d|[slim.conv2d](https://www.tensorflow.org/code/tensorflow/contrib/layers/python/layers/layers.py)|
|Conv2dInPlane|[slim.conv2d_in_plane](https://www.tensorflow.org/code/tensorflow/contrib/layers/python/layers/layers.py)|
|Conv2dTranspose (Deconv)|[slim.conv2d_transpose](https://www.tensorflow.org/code/tensorflow/contrib/layers/python/layers/layers.py)|
|FullyConnected|[slim.fully_connected](https://www.tensorflow.org/code/tensorflow/contrib/layers/python/layers/layers.py)|
|AvgPool2D|[slim.avg_pool2d](https://www.tensorflow.org/code/tensorflow/contrib/layers/python/layers/layers.py)|
|Dropout|[slim.dropout](https://www.tensorflow.org/code/tensorflow/contrib/layers/python/layers/layers.py)|
|Flatten|[slim.flatten](https://www.tensorflow.org/code/tensorflow/contrib/layers/python/layers/layers.py)|
|MaxPool2D|[slim.max_pool2d](https://www.tensorflow.org/code/tensorflow/contrib/layers/python/layers/layers.py)|
|OneHotEncoding|[slim.one_hot_encoding](https://www.tensorflow.org/code/tensorflow/contrib/layers/python/layers/layers.py)|
|SeparableConv2|[slim.separable_conv2d](https://www.tensorflow.org/code/tensorflow/contrib/layers/python/layers/layers.py)|
|UnitNorm|[slim.unit_norm](https://www.tensorflow.org/code/tensorflow/contrib/layers/python/layers/layers.py)|
TF-Slim也提供了两个元运算符----repeat和stack，允许用户可以重复地使用相同的运算符。例如，VGG网络的一个片段，这个网络在两个池化层之间就有许多卷积层的堆叠：


```python
net = ...  
net = slim.conv2d(net, 256, [3, 3], scope='conv3_1')  
net = slim.conv2d(net, 256, [3, 3], scope='conv3_2')  
net = slim.conv2d(net, 256, [3, 3], scope='conv3_3')  
net = slim.max_pool2d(net, [2, 2], scope='pool2')
```
一种减少这种代码重复的方法是使用for循环：

```python
net = ...  
for i in range(3):  
  net = slim.conv2d(net, 256, [3, 3], scope='conv3_' % (i+1))  
net = slim.max_pool2d(net, [2, 2], scope='pool2')
```



若使用TF-Slim的repeat操作符，代码看起来会更简洁：




```python
net = slim.repeat(net, 3, slim.conv2d, 256, [3, 3], scope='conv3')  
net = slim.max_pool2d(net, [2, 2], scope='pool2')
```
slim.repeat不但可以在一行中使用相同的参数，而且还能智能地展开scope，即每个后续的slim.conv2d调用所对应的scope都会追加下划线及迭代数字。更具体地讲，上面代码的scope分别为 'conv3/conv3_1',
 'conv3/conv3_2' and 'conv3/conv3_3'.
除此之外，TF-Slim的slim.stack操作符允许调用者用不同的参数重复使用相同的操作符是创建一个stack或网络层塔。slim.stack也会为每个创建的操作符生成一个新的tf.variable_scope。例如，下面是一个简单的方法去创建MLP：





```python
# Verbose way:  
x = slim.fully_connected(x, 32, scope='fc/fc_1')  
x = slim.fully_connected(x, 64, scope='fc/fc_2')  
x = slim.fully_connected(x, 128, scope='fc/fc_3')  
  
# Equivalent, TF-Slim way using slim.stack:  
slim.stack(x, slim.fully_connected, [32, 64, 128], scope='fc')
```
在这个例子中，slim.stack调用`slim.fully_connected` 三次，前一个层的输出是下一层的输入。而每个网络层的输出通道数从32变到64，再到128.
 同样，我们可以用stack简化一个多卷积层塔：

```python
# Verbose way:  
x = slim.conv2d(x, 32, [3, 3], scope='core/core_1')  
x = slim.conv2d(x, 32, [1, 1], scope='core/core_2')  
x = slim.conv2d(x, 64, [3, 3], scope='core/core_3')  
x = slim.conv2d(x, 64, [1, 1], scope='core/core_4')  
  
# Using stack:  
slim.stack(x, slim.conv2d, [(32, [3, 3]), (32, [1, 1]), (64, [3, 3]), (64, [1, 1])], scope='core')
```









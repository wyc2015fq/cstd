
# AlexNet - 郭云飞的专栏 - CSDN博客


2017年09月28日 13:10:38[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：29146


AlexNet是2012年ImageNet竞赛冠军获得者Hinton和他的学生Alex Krizhevsky设计的。也是在那年之后，更多的更深的神经网路被提出，比如优秀的vgg,GoogleLeNet。其官方提供的数据模型，准确率达到57.1%,top 1-5 达到80.2%. 这项对于传统的机器学习分类算法而言，已经相当的出色。
![](https://img-blog.csdn.net/20170928131553966?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
上图所示是caffe中alexnet的网络结构，上图采用是两台GPU服务器，所有会看到两个流程图。下边把AlexNet的网络结构示意一下：
![](https://img-blog.csdn.net/20170928132129480?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
简化的结构为：
![](https://img-blog.csdn.net/20170928132503250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**AlexNet为啥取得比较好的结果呢？**
1. 使用了*Relu*激活函数。
**Relu**函数：*f(x)=max(0,x)*
![](https://img-blog.csdn.net/20170928133941802?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

基于ReLU的深度卷积网络比基于*tanh*和*sigmoid*的网络训练快数倍，下图是一个基于CIFAR-10的四层卷积网络在tanh和ReLU达到25%的training error的迭代次数：
![](https://img-blog.csdn.net/20170928134202542?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
2. 标准化（*Local Response Normalization*）
使用ReLU f(x)=max(0,x)后，你会发现激活函数之后的值没有了tanh、sigmoid函数那样有一个值域区间，所以一般在ReLU之后会做一个normalization，LRU就是稳重提出（这里不确定，应该是提出？）一种方法，在神经科学中有个概念叫“Lateral inhibition”，讲的是活跃的神经元对它周边神经元的影响。
![](https://pic3.zhimg.com/v2-80acfa6067be50f4d97db91caf82b03e_b.jpg)
3. Dropout
Dropout也是经常说的一个概念，能够比较有效地防止神经网络的过拟合。 相对于一般如线性模型使用正则的方法来防止模型过拟合，而在神经网络中Dropout通过修改神经网络本身结构来实现。对于某一层神经元，通过定义的概率来随机删除一些神经元，同时保持输入层与输出层神经元的个人不变，然后按照神经网络的学习方法进行参数更新，下一次迭代中，重新随机删除一些神经元，直至训练结束。
![](https://pic2.zhimg.com/v2-0acd6f26ca6c3d6a9eaa68f341634f59_b.jpg)
4. 数据增强（*data augmentation*）
在深度学习中，当数据量不够大时候，一般有4解决方法：
>>*data augmentation——*人工增加训练集的大小——通过平移, 翻转, 加噪声等方法从已有数据中创造出一批"新"的数据
>>*Regularization——*数据量比较小会导致模型过拟合, 使得训练误差很小而测试误差特别大. 通过在*Loss Function*后面加上正则项可以抑制过拟合的产生. 缺点是引入了一个需要手动调整的hyper-parameter。
>>*Dropout——*也是一种正则化手段. 不过跟以上不同的是它通过随机将部分神经元的输出置零来实现
>>*Unsupervised Pre-training——*用Auto-Encoder或者RBM的卷积形式一层一层地做无监督预训练, 最后加上分类层做有监督的Fine-Tuning
**AlexNet的TensorFlow实现（仅参考）：**

```python
# -*- coding=UTF-8 -*-
import sys
import os
import random
import cv2
import math
import time
import numpy as np
import tensorflow as tf
import linecache
import string
import skimage
import imageio
# 输入数据
import input_data
mnist = input_data.read_data_sets("/tmp/data/", one_hot=True)
# 定义网络超参数
learning_rate = 0.001
training_iters = 200000
batch_size = 64
display_step = 20
# 定义网络参数
n_input = 784  # 输入的维度
n_classes = 10 # 标签的维度
dropout = 0.8  # Dropout 的概率
# 占位符输入
x = tf.placeholder(tf.types.float32, [None, n_input])
y = tf.placeholder(tf.types.float32, [None, n_classes])
keep_prob = tf.placeholder(tf.types.float32)
# 卷积操作
def conv2d(name, l_input, w, b):
    return tf.nn.relu(tf.nn.bias_add( \
    tf.nn.conv2d(l_input, w, strides=[1, 1, 1, 1], padding='SAME'),b) \
    , name=name)
# 最大下采样操作
def max_pool(name, l_input, k):
    return tf.nn.max_pool(l_input, ksize=[1, k, k, 1], \
    strides=[1, k, k, 1], padding='SAME', name=name)
# 归一化操作
def norm(name, l_input, lsize=4):
    return tf.nn.lrn(l_input, lsize, bias=1.0, alpha=0.001 / 9.0, beta=0.75, name=name)
# 定义整个网络 
def alex_net(_X, _weights, _biases, _dropout):
    _X = tf.reshape(_X, shape=[-1, 28, 28, 1]) # 向量转为矩阵
    # 卷积层
    conv1 = conv2d('conv1', _X, _weights['wc1'], _biases['bc1'])
    # 下采样层
    pool1 = max_pool('pool1', conv1, k=2)
    # 归一化层
    norm1 = norm('norm1', pool1, lsize=4)
    # Dropout
    norm1 = tf.nn.dropout(norm1, _dropout)
    # 卷积
    conv2 = conv2d('conv2', norm1, _weights['wc2'], _biases['bc2'])
    # 下采样
    pool2 = max_pool('pool2', conv2, k=2)
    # 归一化
    norm2 = norm('norm2', pool2, lsize=4)
    # Dropout
    norm2 = tf.nn.dropout(norm2, _dropout)
    # 卷积
    conv3 = conv2d('conv3', norm2, _weights['wc3'], _biases['bc3'])
    # 下采样
    pool3 = max_pool('pool3', conv3, k=2)
    # 归一化
    norm3 = norm('norm3', pool3, lsize=4)
    # Dropout
    norm3 = tf.nn.dropout(norm3, _dropout)
    # 全连接层，先把特征图转为向量
    dense1 = tf.reshape(norm3, [-1, _weights['wd1'].get_shape().as_list()[0]]) 
    dense1 = tf.nn.relu(tf.matmul(dense1, _weights['wd1']) + _biases['bd1'], name='fc1') 
    # 全连接层
    dense2 = tf.nn.relu(tf.matmul(dense1, _weights['wd2']) + _biases['bd2'], name='fc2') # Relu activation
    # 网络输出层
    out = tf.matmul(dense2, _weights['out']) + _biases['out']
    return out
# 存储所有的网络参数
weights = {
    'wc1': tf.Variable(tf.random_normal([3, 3, 1, 64])),
    'wc2': tf.Variable(tf.random_normal([3, 3, 64, 128])),
    'wc3': tf.Variable(tf.random_normal([3, 3, 128, 256])),
    'wd1': tf.Variable(tf.random_normal([4*4*256, 1024])),
    'wd2': tf.Variable(tf.random_normal([1024, 1024])),
    'out': tf.Variable(tf.random_normal([1024, 10]))
}
biases = {
    'bc1': tf.Variable(tf.random_normal([64])),
    'bc2': tf.Variable(tf.random_normal([128])),
    'bc3': tf.Variable(tf.random_normal([256])),
    'bd1': tf.Variable(tf.random_normal([1024])),
    'bd2': tf.Variable(tf.random_normal([1024])),
    'out': tf.Variable(tf.random_normal([n_classes]))
}
# 构建模型
pred = alex_net(x, weights, biases, keep_prob)
# 定义损失函数和学习步骤
cost = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(pred, y))
optimizer = tf.train.AdamOptimizer(learning_rate=learning_rate).minimize(cost)
# 测试网络
correct_pred = tf.equal(tf.argmax(pred,1), tf.argmax(y,1))
accuracy = tf.reduce_mean(tf.cast(correct_pred, tf.float32))
# 初始化所有的共享变量
init = tf.initialize_all_variables()
# 开启一个训练
with tf.Session() as sess:
    sess.run(init)
    step = 1
    # Keep training until reach max iterations
    while step * batch_size < training_iters:
        batch_xs, batch_ys = mnist.train.next_batch(batch_size)
        # 获取批数据
        sess.run(optimizer, feed_dict={x: batch_xs, y: batch_ys, keep_prob: dropout})
        if step % display_step == 0:
            # 计算精度
            acc = sess.run(accuracy, feed_dict={x: batch_xs, y: batch_ys, keep_prob: 1.})
            # 计算损失值
            loss = sess.run(cost, feed_dict={x: batch_xs, y: batch_ys, keep_prob: 1.})
            print "Iter " + str(step*batch_size) + ", Minibatch Loss= " + "{:.6f}".format(loss) + ", Training Accuracy= " + "{:.5f}".format(acc)
        step += 1
    print "Optimization Finished!"
    # 计算测试精度
    print "Testing Accuracy:", sess.run(accuracy, feed_dict={x: mnist.test.images[:256], y: mnist.test.labels[:256], keep_prob: 1.})
```







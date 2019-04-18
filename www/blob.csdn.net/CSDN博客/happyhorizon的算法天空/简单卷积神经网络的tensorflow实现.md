# 简单卷积神经网络的tensorflow实现 - happyhorizon的算法天空 - CSDN博客
2017年09月08日 14:09:51[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：4370

# 卷积神经网络概述
一般一个卷积神经网络由多个卷积层构成，在卷基层内部通常会有如下几个操作：
1）图像通过多个卷积核滤波，添加偏置，提取局部特征每个卷积核会映射出一个新的2D图像。
2）卷积核的滤波结果输出到激活函数中，激活函数通常选ReLU【书，参考37】
3）对激活函数的结果进行池化操作，池化就是对图像分割成不同的小区域后取平均值或最大值。一般取最大值。
上述几个步骤就构成了最常见的卷积层。在池化的后面还可以加上batch normalization等操作。
一个卷积层中可以有不同的卷积核，而每一个卷积核都对应一个滤波后映射出的新图像，同一个新图像的每一个像素都来自完全相同的卷积核。这种卷积核的权值共享可以有效降低模型负责度，减轻过拟合，减少计算量。
# tensorflow实现
## 卷积神经网络结构
建立卷积神经网络对手写数字识别问题进行优化，构建由两个卷积层（包含池化层），两个全连接层构成的卷积神经网络。输入图像是28×28的单通道数据，输出是10×1的one_hot编码的向量。
第一层：卷积核大小是[5,5]，输入通道1，输出通道32，padding选择SAME模式，激活函数为relu。
第二层：池化层，池化核大小是[2,2]，步长[2,2]。
第三层：卷积核大小是[5,5]，输入通道32，输出通道64，padding选择SAME模式，激活函数为relu。
第四层：池化层，设置同上。
第五层：全连接层，首先将图像数据矩阵flatten化，变成1维向量，输出维度是1024， 之后dropout掉一定的数据防止过拟合
第六层：全连接层，输出维度10，激活函数为softmax
损失函数：交叉熵
具体实现如下：
### 导入包和数据
首先导入需要的包和数据
```
import tensorflow as tf
from tensorflow.examples.tutorials.mnist import input_data
mnist = input_data.read_data_sets('MNIST_DATA/', one_hot=True)
```
### 定义卷积核
卷积核是指定维数的张量，卷积核的大小为：[filter_height, filter_width, in_channels, out_channels]
这里用tf.Variable()定义卷积核，定义的时候会指定初始化的操作。
```
def weight_variable(shape):
    initial = tf.truncated_normal(shape,stddev=0.1)
    weight= tf.Variable(initial_value=initial)
return weight
```
### 定义偏置
```
def bias_variable(shape):
    initial = tf.constant(0.1,shape=shape)
    bias = tf.Variable(initial_value=initial)
    return bias
```
### 对输入做卷积操作
```
def conv_op(in_tensor, kernel, strides=[1,1,1,1], padding=’SAME’, name=’conv’):
    conv_out = tf.nn.conv2d(in_tensor,kernel,strides, padding,name=name)
return conv_out
```
输入的张量维度是[batch， in_height，in_width，in_channels]
在具体操作中，程序会将输入张量展开为[batch, out_height, out_width, filter_hight*filter_width*in_channels]
并将卷积核展开为[filter_height*filter_width*in_channels, out_channels ]
输入张量展开后与卷积核的展开相乘，得到的结果的维度是：[batch, out_height, out_width, out_channels]，和输入张量的维度保持一致。也就是说，在tensorflow构建的图中，张量始终都是以始终是：[batch， height，width，channels]的维度流动。
步长strides通常定义为strides=[1, dh, dw, 1]， 也就是strides[0]=strides[3]=1. dh和dw分别是垂直和水平方向卷积核移动的步长。
padding就是是否要做边界填充，如果要，padding=‘SAME’，如果不用，padding=‘VALID’
### 叠加上偏置并输出到激活函数
`h_conv1 = tf.nn.relu(conv_op(x_image, W_conv1)+b_conv1)`
之后输入到激活函数，直接调用tensorflow中的tf.nn.relu()函数就可以，激活函数的输出依然是：[batch, out_height, out_width, out_channels]
`tf.nn.relu(features, name = None)`
输入参数：
- features: 一个Tensor。数据类型必须是：float32，float64，int32，int64，uint8，int16，int8。
- name: （可选）为这个操作取一个名字。
输出参数：
- 一个Tensor，数据类型和features相同。
在tensorflow中，加法运算有好几种，例如专门的偏置叠加tf.bias_add,还有tf.add,也可以直接用a+b,具体区别见后。
### 池化层
池化操作是：对输入的张量进行区域扫描求最大值或者平均值。一般求最大值。tensorflow有tf.nn.max_pool(）函数可以方便地实现池化操作。池化尺寸kh×kw, 步长是dh×dw，padding设置为‘SAME’。
```
def max_pool_2x2(in_tensor, ksize=[1,2,2,1], strides=[1,2,2,1],padding=’SAME’,name=’max_pooling’):
    max_pool = tf.nn.max_pool(in_tensor, ksize, strides, padding, name=name)
return max_pool
```
这里ksize=[1,kh,kd,1], strides=[1,dh,dw,1]
### 目标函数：交叉熵
` cross_entropy = tf.reduce_mean(-tf.reduce_sum(y_*tf.log(y_conv),reduction_indices=[1]))`
自此，就定义好了前向计算的所有操作。后向计算采用AdamOptimizer函数，学习率1e-4
最后，给出卷积操作的实现代码：
```
#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Jul  9 12:16:52 2017
mnist by simple cnn
@author: 
"""
import tensorflow as tf
from tensorflow.examples.tutorials.mnist import input_data
mnist = input_data.read_data_sets('MNIST_DATA/', one_hot=True)
sess = tf.InteractiveSession()
# ======================================================== #
#                       SIMPLE CNN
# ======================================================== #
# define conv kernel
def weight_variable(shape):
    initial = tf.truncated_normal(shape,stddev=0.1)
    weight = tf.Variable(initial_value=initial)
    return weight
# define conv bias
def bias_variable(shape):
    initial = tf.constant(0.1,shape=shape)
    bias = tf.Variable(initial_value=initial)
    return bias
# define a simple conv operation 
def conv_op(in_tensor, kernel, strides=[1,1,1,1], padding='SAME'):
    conv_out = tf.nn.conv2d(in_tensor, kernel, strides=strides, padding=padding)
    return conv_out
# define max pooling operation
def max_pool_2x2(in_tensor,ksize=[1,2,2,1],strides=[1,2,2,1],padding='SAME'):
    max_pool = tf.nn.max_pool(in_tensor, ksize, strides, padding)
    return max_pool
def simple_cnn():
    x = tf.placeholder(tf.float32, [None, 784])
    y_ = tf.placeholder(tf.float32, [None, 10])
    x_image = tf.reshape(x,[-1, 28, 28, 1]) # 28*28 pic of 1 channel
    # cnn structure
    w1 = [5,5,1,32]
    b1 = [32]
    w2 = [5,5,32,64]
    b2 = [64]
    wfc1 = [7*7*64,1024]
    bfc1 = [1024]
    wfc2 = [1024,10]
    bfc2 = [10]
    # 1st layer
    W_conv1 = weight_variable(w1)
    b_conv1 = bias_variable(b1)
    h_conv1 = tf.nn.relu(conv_op(x_image, W_conv1)+b_conv1)
    h_pool1 = max_pool_2x2(h_conv1)
    # 2nd layer
    W_conv2 = weight_variable(w2)
    b_conv2 = bias_variable(b2)
    h_conv2 = tf.nn.relu(conv_op(h_pool1, W_conv2)+b_conv2)
    h_pool2 = max_pool_2x2(h_conv2)
    # fc1
    h_pool2_flat = tf.reshape(h_pool2,[-1,7*7*64])
    W_fc1 = weight_variable(wfc1)
    b_fc1 = bias_variable(bfc1)
    h_fc1 = tf.nn.relu(tf.matmul(h_pool2_flat, W_fc1)+b_fc1)
    # drop out
    keep_prob = tf.placeholder(tf.float32)
    h_fc1_drop = tf.nn.dropout(h_fc1,keep_prob)
    # fc2
    W_fc2 = weight_variable(wfc2)
    b_fc2 = bias_variable(bfc2)
    y_conv = tf.nn.softmax(tf.matmul(h_fc1_drop, W_fc2)+b_fc2)
    # loss function
    cross_entropy = tf.reduce_mean(-tf.reduce_sum(y_*tf.log(y_conv),reduction_indices=[1]))
    train_step = tf.train.AdamOptimizer(1e-4).minimize(cross_entropy)
    # estimate accuarcy
    correct_prediction = tf.equal(tf.arg_max(y_conv,1), tf.arg_max(y_,1))
    accuarcy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))    
    tf.global_variables_initializer().run()
    for i in range(20000):
        batch = mnist.train.next_batch(50)
        if i%100 == 0:
            train_accuracy = accuarcy.eval(feed_dict={x:batch[0],y_:batch[1],keep_prob:1.0}) # testify, or prediction
            print('step %d, training accuarcy %g'%(i, train_accuracy))
        train_step.run(feed_dict={x:batch[0],y_:batch[1],keep_prob:0.5})
    print("test accuracy %g"%accuarcy.eval(feed_dict={x:mnist.test.images,y_:mnist.test.labels,keep_prob:1.0}))
    return
if __name__ == '__main__':
    simple_cnn()
```
# tensorflow的一些细节点：
## padding
padding的选项：
- valid就是指卷积核是以整个卷积核为基础从边沿对齐图像的边沿开始，任何边沿到达图像另一边时就停止卷积运算；
- same则是以卷积核的中间点为基础，中间点扫过真个图像才结束。自然，same的模式下，卷积后的输出和原输入的维度是一致的。
## tf.cast
估计模型的准确度时，用到了函数tf.cast
tf.cast(x, dtype, name=None)数据类型转换，将x或者x.values转换为dtype
a=tf.constant(value=[1.1,2.3],dtype=tf.float32)
b=tf.cast(a,dtype=tf.int32)
sess = tf.Session()
print(sess.run(a))  #输出[ 1.10000002  2.29999995]
print(sess.run(b))  #输出[1 2]
## tf.add(a,b)、tf.bias_add(a,b)和 a+b
#### 1) tf.add(a, b) 与 a+b
在神经网络前向传播的过程中，经常可见如下两种形式的代码：
tf.add(tf.matmul(x, w), b)
tf.matmul(x, w) + b
简而言之，就是 tf.add(a, b) 与 a + b二者的区别，类似的也有，tf.assign 与 =（赋值运算符）的差异。
在计算精度上，二者并没有差别。运算符重载的形式a+b，会在内部转换为，a.**add**(b)，而a.**add**(b)会再一次地映射为tf.add。
#### 2. tf.nn.bias_add 与 tf.add
tf.nn.bias_add 是 tf.add 的一个特例，也即 tf.add 支持的操作比 tf.nn.bias_add 更多。二者均支持 broadcasting（广播机制），也即两个操作数最后一个维度保持一致。
除了支持最后一个维度保持一致的两个操作数相加外，tf.add 还支持第二个操作数是一维的情况。
参考：
《tensorflow实战》
[http://blog.csdn.net/wangjian1204/article/details/53291619](http://blog.csdn.net/wangjian1204/article/details/53291619)
In tensorflow what is the difference between tf.add and operator (+)?（[https://stackoverflow.com/questions/37900780/in-tensorflow-what-is-the-difference-between-tf-add-and-operator）](https://stackoverflow.com/questions/37900780/in-tensorflow-what-is-the-difference-between-tf-add-and-operator%EF%BC%89)

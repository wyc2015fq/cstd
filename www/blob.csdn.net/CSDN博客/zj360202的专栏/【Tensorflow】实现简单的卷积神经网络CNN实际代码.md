# 【Tensorflow】实现简单的卷积神经网络CNN实际代码 - zj360202的专栏 - CSDN博客





2017年04月20日 16:06:57[zj360202](https://me.csdn.net/zj360202)阅读数：7812













TensorFlow是谷歌基于DistBelief进行研发的第二代[人工智能](https://baike.so.com/doc/2952526-3114987.html)[学习系统](https://baike.so.com/doc/10020706-10368631.html)，其命名来源于本身的运行原理。Tensor(张量)意味着N维数组，Flow(流)意味着基于数据流图的计算，TensorFlow为张量从流图的一端流动到另一端计算过程。TensorFlow是将复杂的数据结构传输至人工智能神经网中进行分析和处理过程的系统。


TensorFlow可被用于[语音识别](https://baike.so.com/doc/1662577-1757537.html)或[图像识别](https://baike.so.com/doc/6527187-6740919.html)等多项机器深度学习领域，对2011年开发的深度学习基础架构DistBelief进行了各方面的改进，它可在小到一部智能手机、大到数千台数据中心服务器的各种设备上运行。TensorFlow将完全开源，任何人都可以用。

原生接口文章


- [【Tensorflow】tf.placeholder函数](http://blog.csdn.net/zj360202/article/details/70243127)
- [【TensorFlow】tf.nn.conv2d是怎样实现卷积的](http://blog.csdn.net/zj360202/article/details/70243424)
- [【TensorFlow】tf.nn.max_pool实现池化操作](http://blog.csdn.net/zj360202/article/details/70243836)
- [【Tensorflow】tf.nn.relu函数](http://blog.csdn.net/zj360202/article/details/70256545)
- [【Tensorflow】tf.reshape
 函数](http://blog.csdn.net/zj360202/article/details/70256835)
- [【Tensorflow】tf.nn.dropout函数](http://blog.csdn.net/zj360202/article/details/70257048)
- [【Tensorflow】tf.argmax函数](http://blog.csdn.net/zj360202/article/details/70259999)
- [【Tensorflow】tf.cast
 类型转换 函数](http://blog.csdn.net/zj360202/article/details/70260265)
- [【Tensorflow】tf.train.AdamOptimizer函数](http://blog.csdn.net/zj360202/article/details/70263110)
- [【Tensorflow】tf.Graph()函数](http://blog.csdn.net/zj360202/article/details/78539464)
- [【TensorFlow】tf.nn.softmax_cross_entropy_with_logits的用法](http://blog.csdn.net/zj360202/article/details/78582895)

- [【Tensorflow】tf.dynamic_partition
 函数 分拆数组](http://blog.csdn.net/zj360202/article/details/78642340)

原生接口实例



- [【Tensorflow】实现简单的卷积神经网络CNN实际代码](http://blog.csdn.net/zj360202/article/details/70265414)
- [【Tensorflow
 实战】实现欧式距离](http://blog.csdn.net/zj360202/article/details/78623609)

slim接口文章



- [【Tensorflow】tensorflow.contrib.slim
 包](http://blog.csdn.net/zj360202/article/details/78585197)

- [【Tensorflow
 slim】 slim.arg_scope的用法](http://blog.csdn.net/zj360202/article/details/78590285)

- [【Tensorflow
 slim】slim.data包](http://blog.csdn.net/zj360202/article/details/78591068)

- [【Tensorflow
 slim】slim evaluation 函数](http://blog.csdn.net/zj360202/article/details/78593236)

- [【Tensorflow
 slim】slim layers包](http://blog.csdn.net/zj360202/article/details/78593626)

- [【Tensorflow
 slim】slim learning包](http://blog.csdn.net/zj360202/article/details/78594089)
- [【Tensorflow
 slim】slim losses包](http://blog.csdn.net/zj360202/article/details/78594359)

- [【Tensorflow
 slim】slim nets包](http://blog.csdn.net/zj360202/article/details/78595102)

- [【Tensorflow
 slim】slim variables包](http://blog.csdn.net/zj360202/article/details/78595220)

- [【Tensorflow
 slim】slim metrics包](http://blog.csdn.net/zj360202/article/details/78595315)
slim
 实例



- [【Tensorflow
 slim 实战】写MobileNet](http://blog.csdn.net/zj360202/article/details/78623567)

- [【Tensorflow
 slim 实战】写Inception-V4 Inception-ResNet-v2结构](http://blog.csdn.net/zj360202/article/details/78627217)
kera
 接口文章



- [【Tensorflow
 keras】Keras:基于Theano和TensorFlow的深度学习库](http://blog.csdn.net/zj360202/article/details/78645039)

- [【Tensorflow
 keras】轻量级深度学习框架 Keras简介](http://blog.csdn.net/zj360202/article/details/78646969)

tensorflow使用过程中的辅助接口或通过tensorflow实现的批量操作接口



- [将非RGB图片转换为RGB图片](http://blog.csdn.net/zj360202/article/details/78539040)

- [【opencv】python3
 将图片生成视频文件](http://blog.csdn.net/zj360202/article/details/71561925)

- [【opencv】selective_search函数](http://blog.csdn.net/zj360202/article/details/70847144)





=========================================================================





此处使用的是MNIST,预期结果是99.2%；

![](https://img-blog.csdn.net/20170420172350293?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemozNjAyMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


每一个Tensor或是Operation都会构建自己的graph，

然后在调用各自的eval或run的时候就会按graph执行，

不过对于构建的graph是什么样子，为什么没有操作手工初始化的几步，通过优化最小损失不断的优化内存的w以达到优化正确率的效果，还有一些没有太想明白，有大神了解的，还望指教



```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Time    : 2017/4/19 14:11
# @Author  : zhangjian
# @Site    : 
# @File    : less04.py
# @Software: PyCharm Community Edition

from tensorflow.examples.tutorials.mnist import input_data
import tensorflow as tf

mnist = input_data.read_data_sets("MNIST_data/",one_hot=True)

############################################################################################
# tf.InteractiveSession 配置完了以后，由tf.Tensor.eval或tf.Operation.run 的方式来生效
############################################################################################
sess = tf.InteractiveSession()

'''
weight_variable     权重变量
@:param shape       形状
@:return Variable   返回变量

通过给定形状，标准差的正态分布生成一个随机值
由于此返回结果需要后续不断变更，此处返回类型为tf.Variable
'''
def weight_variable(shape):
    ############################################################################################
    # tf.truncated_normal 截尾正态分布
    # 参数 shape 形状           是一个Tensor或是python数组，用来表示正态分布有几维和对应维有几个特征，比如：[3, 4]表示有两维，第一维有3个特征，第二维有4个特征
    # 参数 stddev 标准差         默认是1
    # 返回 指定形状               指定标准差的正态分布中的一个随机值生成的一个Tensor
    ############################################################################################
    initial = tf.truncated_normal(shape,stddev=0.1)
    ############################################################################################
    # tf.Variable 变量定义  变量是构造流程图的基本元素
    # 参数 initial_value 值            任何类型的值都可以
    # 返回 tf.Variable
    ############################################################################################
    return tf.Variable(initial)

'''
bias_variable       偏移变量
@:param shape       形状
@:return Variable   返回变量
'''
def bias_variable(shape):
    ############################################################################################
    # tf.constant 常量定义
    # 参数 value 值            默认返回当前值，如果shape有定义的话，就是一个由此值形成的多维数组
    # 参数 shape 形状           默认是None
    # 返回 Tensor
    ############################################################################################
    initial = tf.constant(0.1,shape=shape)
    return tf.Variable(initial)

'''
conv2d              卷积
@:param x           输入图像
@:param w           卷积核
@:return Tensor     做过卷积后的图像
'''
def conv2d(x, w):
    ############################################################################################
    # tf.nn.conv2d 卷积函数
    # 参数 input 输入图像             输入数据也是四维[图片数量, 图片高度, 图片宽度, 图像通道数][batch, height, width, channels]
    # 参数 filter 卷积核             四维[卷积核的高度，卷积核的宽度，图像通道数，卷积核个数]
    # 参数 strides 卷积核移动量     四维[图片数量, 图片高度, 图片宽度, 图像通道数]
    # 参数 padding 边缘处理方式     SAME和VALID,SAME就是可以在外围补0再卷积，VALID不能补0
    # 返回 Tensor
    ############################################################################################
    return tf.nn.conv2d(x, w, strides=[1, 1, 1, 1], padding='SAME')

'''
max_pool_2x2       用2x2模板做池化
@:param x          需要池化的对象
@:return Tensor    返回
'''
def max_pool_2x2(x):
    ############################################################################################
    # tf.nn.conv2d 卷积函数
    # 参数 value 输入图像             四维[图片数量, 图片高度, 图片宽度, 图像通道数]
    # 参数 ksize 池化窗口             四维[卷积核的高度，卷积核的宽度，图像通道数，卷积核个数]
    # 参数 strides 卷积核移动量     四维[图片数量, 图片高度, 图片宽度, 图像通道数]，一般不对图片数量和图像通道数进行池化，所以都是1
    # 参数 padding 边缘处理方式     SAME和VALID,SAME就是可以在外围补0再卷积，VALID不能补0
    # 返回 Tensor
    ############################################################################################
    return tf.nn.max_pool(x, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding='SAME')

############################################################################################
# tf.placeholder 形参定义
# 参数 dtype 数据类型
# 参数 shape 形状，默认是None，
############################################################################################
x = tf.placeholder(tf.float32, [None, 784])
y_ = tf.placeholder(tf.float32, [None, 10])
############################################################################################
# tf.reshape 重定形状
# 参数 tensor 输入数据
# 参数 shape 形状                按此shape生成相应数组，但-1是特例，表示有此维度，但是数值不定
# 返回 Tensor
############################################################################################
x_image = tf.reshape(x, [-1, 28, 28, 1])

w_conv1 = weight_variable([5, 5, 1, 32])
b_conv1 = bias_variable([32])
############################################################################################
# tf.nn.relu RELU函数  = max(0,features)
# 参数 features 输入特征Tensor
# 返回 Tensor
############################################################################################
h_conv1 = tf.nn.relu(conv2d(x_image, w_conv1) + b_conv1)
h_pool1 = max_pool_2x2(h_conv1)

w_conv2 = weight_variable([5, 5, 32, 64])
b_conv2 = bias_variable([64])
h_conv2 = tf.nn.relu(conv2d(h_pool1, w_conv2) + b_conv2)
h_pool2 = max_pool_2x2(h_conv2)

w_fc1 = weight_variable([7 * 7 * 64, 1024])
b_fc1 = bias_variable([1024])
h_pool2_flat = tf.reshape(h_pool2, [-1, 7 * 7 * 64])
h_fc1 = tf.nn.relu(tf.matmul(h_pool2_flat, w_fc1) + b_fc1)

keep_prob = tf.placeholder(tf.float32)
############################################################################################
# tf.nn.dropout(x, keep_prob, noise_shape=None, seed=None, name=None)   防止过拟合   在对输入的数据进行一定的取舍，从而降低过拟合
# 参数 x 输入数据
# 参数 keep_prob 保留率             对输入数据保留完整返回的概率
# 返回 Tensor
############################################################################################
h_fc1_drop = tf.nn.dropout(h_fc1,keep_prob)

w_fc2 = weight_variable([1024, 10])
b_fc2 = bias_variable([10])
############################################################################################
# tf.matmul(a, b): 矩阵相乘  a * b
# 参数 a 矩阵Tensor
# 参数 b 矩阵Tensor
# 返回 Tensor
############################################################################################
############################################################################################
# tf.nn.softmax(logits, dim=-1, name=None): SoftMax函数  softmax = exp(logits) / reduce_sum(exp(logits), dim)
# 参数 logits 输入            一般输入是logit函数的结果
# 参数 dim 卷积核             指定是第几个维度，默认是-1，表示最后一个维度
# 返回 Tensor
############################################################################################
y_conv = tf.nn.softmax(tf.matmul(h_fc1_drop, w_fc2) + b_fc2)

############################################################################################
# tf.nn.reduce_sum(input_tensor, axis=None, keep_dims=False, name=None, reduction_indices=None) 求和函数
# 参数 input_tensor 输入数据             可以是值，也可以是多维矩阵
# 参数 axis 求和方式                     默认是全求和；如果是0，就是按列求和；如果1，就是按行求和
# 参数 keep_dims 是否保留原有维度样式    True表示是，False表示不是
# 返回 Tensor
############################################################################################
############################################################################################
# tf.reduce_mean(input_tensor, axis=None, keep_dims=False, name=None, reduction_indices=None)  均值函数
# 参数 input_tensor 输入数据             可以是值，也可以是多维矩阵
# 参数 axis 求和方式                     默认是全求和；如果是0，就是按列求和；如果1，就是按行求和
# 参数 keep_dims 是否保留原有维度样式    True表示是，False表示不是
# 返回 Tensor
############################################################################################
cross_entropy = tf.reduce_mean(-tf.reduce_sum(y_ * tf.log(y_conv), reduction_indices=[1]))
############################################################################################
# tf.train.AdamOptimizer Adam优化算法
# __init__(learning_rate=0.001, beta1=0.9, beta2=0.999, epsilon=1e-08, use_locking=False, name='Adam')
# 返回 Optimizer
# 是一个寻找全局最优点的优化算法，引入了二次方梯度校正。相比于基础SGD算法，1.不容易陷于局部优点。2.速度更快
############################################################################################
############################################################################################
# tf.train.Optimizer.minimize 优化算法之最小化函数 主要参数是loss
# 参数 loss 损失量
# 返回 Operation
# minimize需要run唤起
############################################################################################
train_step = tf.train.AdamOptimizer(1e-4).minimize(cross_entropy)

############################################################################################
# tf.argmax(input, axis=None, name=None, dimension=None) 对矩阵按行或列进行最大值下标提取
# 参数 input 输入
# 参数 axis axis轴         0表示按列，1表示按行
# 参数 name 名称
# 参数 dimension维度       和axis功能一样，默认axis取值优先。新加的字段
# 返回 Tensor
############################################################################################
correct_prediction = tf.equal(tf.argmax(y_conv, 1), tf.argmax(y_, 1))
############################################################################################
# tf.cast(x, dtype, name=None) 类型转换
# 参数 x 输入
# 参数 dtype 转换后的类型
# 返回 Tensor
############################################################################################
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))

############################################################################################
# tf.global_variables_initializer 初始化所有全局变量
# 返回 Operation
############################################################################################
tf.global_variables_initializer().run()       #启动Session
for i in range(20000):
    ############################################################################################
    # Datasets.train.next_batch 批量处理记录数
    # 返回 [image,label]
    ############################################################################################
    batch = mnist.train.next_batch(50)
    if i%100 == 0:
        ############################################################################################
        # Tensor.eval Tensor的执行函数，只能在session启动后面执行
        # 参数 feed_dict  形参列表
        # 返回 Tensor
        ############################################################################################
        train_accuracy = accuracy.eval(feed_dict={x:batch[0], y_:batch[1], keep_prob: 1.0})
        print("step %d, training accuracy %g" %(i, train_accuracy))
    ############################################################################################
    # Operation.run 执行函数 == tf.get_default_session().run(op).
    # 参数 feed_dict  形参列表
    ############################################################################################
    train_step.run(feed_dict={x:batch[0], y_:batch[1], keep_prob: 0.5})

print("test accuracy %g" %accuracy.eval(feed_dict={x:mnist.test.images, y_:mnist.test.labels, keep_prob: 1.0}))
```










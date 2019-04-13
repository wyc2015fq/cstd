
# tensorflow笔记：多层CNN代码分析 - Multiangle's Notepad - CSDN博客


2016年10月03日 18:05:44[multiangle](https://me.csdn.net/u014595019)阅读数：42765标签：[tensorflow																](https://so.csdn.net/so/search/s.do?q=tensorflow&t=blog)[cnn																](https://so.csdn.net/so/search/s.do?q=cnn&t=blog)[卷积神经网络																](https://so.csdn.net/so/search/s.do?q=卷积神经网络&t=blog)[深度学习																](https://so.csdn.net/so/search/s.do?q=深度学习&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=卷积神经网络&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=cnn&t=blog)个人分类：[python																](https://blog.csdn.net/u014595019/article/category/5636921)[机器学习&深度学习																](https://blog.csdn.net/u014595019/article/category/3051069)[
							](https://blog.csdn.net/u014595019/article/category/5636921)
所属专栏：[深度学习&tensorflow笔记](https://blog.csdn.net/column/details/13414.html)
[
																	](https://so.csdn.net/so/search/s.do?q=cnn&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=tensorflow&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=tensorflow&t=blog)

tensorflow笔记系列：
（一）[ tensorflow笔记：流程，概念和简单代码注释](http://blog.csdn.net/u014595019/article/details/52677412)
（二）[tensorflow笔记：多层CNN代码分析](http://blog.csdn.net/u014595019/article/details/52728886)
（三）[tensorflow笔记：多层LSTM代码分析](http://blog.csdn.net/u014595019/article/details/52759104)
（四）[tensorflow笔记：常用函数说明](http://blog.csdn.net/u014595019/article/details/52805444)
（五）[tensorflow笔记：模型的保存与训练过程可视化](http://blog.csdn.net/u014595019/article/details/53912710)
（六）[tensorflow笔记：使用tf来实现word2vec](http://blog.csdn.net/u014595019/article/details/54093161)
---
在之前的[tensorflow笔记：流程，概念和简单代码注释](http://blog.csdn.net/u014595019/article/details/52677412)文章中，已经大概解释了tensorflow的大概运行流程，并且提供了一个mnist数据集分类器的简单实现。当然，因为结构简单，最后的准确率在91%左右。似乎已经不低了？其实这个成绩是非常不理想的。现在mnist的准确率天梯榜已经被刷到了99.5%以上。为了进一步提高准确率，官网还提供了一个多层的CNN分类器的代码。相比之前的一层神经网络，这份代码的主要看点倒不是多层，而是CNN，也就是卷积神经网络。
CNN的具体内容不再详述，概述可以参考[这里](http://blog.csdn.net/zouxy09/article/details/8781543)，详细信息可以参见[Convolutional Networks](http://www.deeplearningbook.org/contents/convnets.html)。一般来说，CNN网络的前几层为卷积层和采样层（或者说池化层），在若干层卷积和池化以后，还有若干层全连接层（也就是传统神经网络），最后输出分类信息。大概的结构示意图如下图所示
![此处输入图片的描述](https://img-my.csdn.net/uploads/201304/10/1365562321_2400.jpg)
可以看到**，CNN相比与传统神经网络，最大的区别就是引入了卷积层和池化层**。这也是我们在代码中要着重看的地方。
在下面的代码中，卷积是使用tf.nn.conv2d, 池化使用tf.nn.max_pool,下面来详细的讲解一下这两个函数的用法。
---
**tf.nn.conv2d**
这个函数的功能是：给定4维的input和filter，计算出一个2维的卷积结果。函数的定义为：
```python
def
```
```python
conv2d
```
```python
(input, filter, strides, padding, use_cudnn_on_gpu=None,
           data_format=None, name=None)
```
```python
:
```
前几个参数分别是input, filter, strides, padding, use_cudnn_on_gpu, …下面来一一解释
**input**：待卷积的数据。格式要求为一个张量，**[batch, in_height, in_width, in_channels]**.
分别表示 批次数，图像高度，宽度，输入通道数。
**filter**： 卷积核。格式要求为**[filter_height, filter_width, in_channels, out_channels]**.
分别表示 卷积核的高度，宽度，输入通道数，输出通道数。
**strides**:一个长为4的list. 表示每次卷积以后卷积窗口在input中滑动的距离
**padding**：有SAME和VALID两种选项，表示是否要保留图像边上那一圈不完全卷积的部分。如果是SAME，则保留
**use_cudnn_on_gpu**：是否使用cudnn加速。默认是True
---
**tf.nn.max_pool**
进行最大值池化操作,而avg_pool 则进行平均值池化操作.函数的定义为：
```python
def
```
```python
max_pool
```
```python
(value, ksize, strides, padding, data_format=
```
```python
"NHWC"
```
```python
, name=None)
```
```python
:
```
**value:**一个4D张量，格式为**[batch, height, width, channels]**，与conv2d中input格式一样
**ksize:**长为4的list,表示池化窗口的尺寸
**strides:**池化窗口的滑动值，与conv2d中的一样
**padding:**与conv2d中用法一样。
---
具体的代码注释如下：
```python
# Copyright 2015 The TensorFlow Authors. All Rights Reserved.
```
```python
#
```
```python
# Licensed under the Apache License, Version 2.0 (the "License");
```
```python
# you may not use this file except in compliance with the License.
```
```python
# You may obtain a copy of the License at
```
```python
#
```
```python
#     http://www.apache.org/licenses/LICENSE-2.0
```
```python
#
```
```python
# Unless required by applicable law or agreed to in writing, software
```
```python
# distributed under the License is distributed on an "AS IS" BASIS,
```
```python
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
```
```python
# See the License for the specific language governing permissions and
```
```python
# limitations under the License.
```
```python
# ==============================================================================
```
```python
"""A very simple MNIST classifier.
See extensive documentation at
http://tensorflow.org/tutorials/mnist/beginners/index.md
"""
```
```python
from
```
```python
__future__
```
```python
import
```
```python
absolute_import
```
```python
from
```
```python
__future__
```
```python
import
```
```python
division
```
```python
from
```
```python
__future__
```
```python
import
```
```python
print_function
```
```python
# Import data
```
```python
from
```
```python
tensorflow.examples.tutorials.mnist
```
```python
import
```
```python
input_data
```
```python
import
```
```python
tensorflow
```
```python
as
```
```python
tf
flags = tf.app.flags
FLAGS = flags.FLAGS
flags.DEFINE_string(
```
```python
'data_dir'
```
```python
,
```
```python
'/tmp/data/'
```
```python
,
```
```python
'Directory for storing data'
```
```python
)
```
```python
# 第一次启动会下载文本资料，放在/tmp/data文件夹下
```
```python
print(FLAGS.data_dir)
mnist = input_data.read_data_sets(FLAGS.data_dir, one_hot=
```
```python
True
```
```python
)
```
```python
def
```
```python
weight_variable
```
```python
(shape)
```
```python
:
```
```python
initial = tf.truncated_normal(shape, stddev=
```
```python
0.1
```
```python
)
```
```python
# 变量的初始值为截断正太分布
```
```python
return
```
```python
tf.Variable(initial)
```
```python
def
```
```python
bias_variable
```
```python
(shape)
```
```python
:
```
```python
initial = tf.constant(
```
```python
0.1
```
```python
, shape=shape)
```
```python
return
```
```python
tf.Variable(initial)
```
```python
def
```
```python
conv2d
```
```python
(x, W)
```
```python
:
```
```python
"""
    tf.nn.conv2d功能：给定4维的input和filter，计算出一个2维的卷积结果
    前几个参数分别是input, filter, strides, padding, use_cudnn_on_gpu, ...
    input   的格式要求为一个张量，[batch, in_height, in_width, in_channels],批次数，图像高度，图像宽度，通道数
    filter  的格式为[filter_height, filter_width, in_channels, out_channels]，滤波器高度，宽度，输入通道数，输出通道数
    strides 一个长为4的list. 表示每次卷积以后在input中滑动的距离
    padding 有SAME和VALID两种选项，表示是否要保留不完全卷积的部分。如果是SAME，则保留
    use_cudnn_on_gpu 是否使用cudnn加速。默认是True
    """
```
```python
return
```
```python
tf.nn.conv2d(x, W, strides=[
```
```python
1
```
```python
,
```
```python
1
```
```python
,
```
```python
1
```
```python
,
```
```python
1
```
```python
], padding=
```
```python
'SAME'
```
```python
)
```
```python
def
```
```python
max_pool_2x2
```
```python
(x)
```
```python
:
```
```python
"""
    tf.nn.max_pool 进行最大值池化操作,而avg_pool 则进行平均值池化操作
    几个参数分别是：value, ksize, strides, padding,
    value:  一个4D张量，格式为[batch, height, width, channels]，与conv2d中input格式一样
    ksize:  长为4的list,表示池化窗口的尺寸
    strides: 窗口的滑动值，与conv2d中的一样
    padding: 与conv2d中用法一样。
    """
```
```python
return
```
```python
tf.nn.max_pool(x, ksize=[
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
2
```
```python
,
```
```python
1
```
```python
],
                          strides=[
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
2
```
```python
,
```
```python
1
```
```python
], padding=
```
```python
'SAME'
```
```python
)
sess = tf.InteractiveSession()
x = tf.placeholder(tf.float32, [
```
```python
None
```
```python
,
```
```python
784
```
```python
])
x_image = tf.reshape(x, [-
```
```python
1
```
```python
,
```
```python
28
```
```python
,
```
```python
28
```
```python
,
```
```python
1
```
```python
])
```
```python
#将输入按照 conv2d中input的格式来reshape，reshape
```
```python
"""
# 第一层
# 卷积核(filter)的尺寸是5*5, 通道数为1，输出通道为32，即feature map 数目为32
# 又因为strides=[1,1,1,1] 所以单个通道的输出尺寸应该跟输入图像一样。即总的卷积输出应该为?*28*28*32
# 也就是单个通道输出为28*28，共有32个通道,共有?个批次
# 在池化阶段，ksize=[1,2,2,1] 那么卷积结果经过池化以后的结果，其尺寸应该是？*14*14*32
"""
```
```python
W_conv1 = weight_variable([
```
```python
5
```
```python
,
```
```python
5
```
```python
,
```
```python
1
```
```python
,
```
```python
32
```
```python
])
```
```python
# 卷积是在每个5*5的patch中算出32个特征，分别是patch大小，输入通道数目，输出通道数目
```
```python
b_conv1 = bias_variable([
```
```python
32
```
```python
])
h_conv1 = tf.nn.elu(conv2d(x_image, W_conv1) + b_conv1)
h_pool1 = max_pool_2x2(h_conv1)
```
```python
"""
# 第二层
# 卷积核5*5，输入通道为32，输出通道为64。
# 卷积前图像的尺寸为 ?*14*14*32， 卷积后为?*14*14*64
# 池化后，输出的图像尺寸为?*7*7*64
"""
```
```python
W_conv2 = weight_variable([
```
```python
5
```
```python
,
```
```python
5
```
```python
,
```
```python
32
```
```python
,
```
```python
64
```
```python
])
b_conv2 = bias_variable([
```
```python
64
```
```python
])
h_conv2 = tf.nn.elu(conv2d(h_pool1, W_conv2) + b_conv2)
h_pool2 = max_pool_2x2(h_conv2)
```
```python
# 第三层 是个全连接层,输入维数7*7*64, 输出维数为1024
```
```python
W_fc1 = weight_variable([
```
```python
7
```
```python
*
```
```python
7
```
```python
*
```
```python
64
```
```python
,
```
```python
1024
```
```python
])
b_fc1 = bias_variable([
```
```python
1024
```
```python
])
h_pool2_flat = tf.reshape(h_pool2, [-
```
```python
1
```
```python
,
```
```python
7
```
```python
*
```
```python
7
```
```python
*
```
```python
64
```
```python
])
h_fc1 = tf.nn.elu(tf.matmul(h_pool2_flat, W_fc1) + b_fc1)
keep_prob = tf.placeholder(tf.float32)
```
```python
# 这里使用了drop out,即随机安排一些cell输出值为0，可以防止过拟合
```
```python
h_fc1_drop = tf.nn.dropout(h_fc1, keep_prob)
```
```python
# 第四层，输入1024维，输出10维，也就是具体的0~9分类
```
```python
W_fc2 = weight_variable([
```
```python
1024
```
```python
,
```
```python
10
```
```python
])
b_fc2 = bias_variable([
```
```python
10
```
```python
])
y_conv=tf.nn.softmax(tf.matmul(h_fc1_drop, W_fc2) + b_fc2)
```
```python
# 使用softmax作为多分类激活函数
```
```python
y_ = tf.placeholder(tf.float32, [
```
```python
None
```
```python
,
```
```python
10
```
```python
])
cross_entropy = tf.reduce_mean(-tf.reduce_sum(y_ * tf.log(y_conv), reduction_indices=[
```
```python
1
```
```python
]))
```
```python
# 损失函数，交叉熵
```
```python
train_step = tf.train.AdamOptimizer(
```
```python
1e-4
```
```python
).minimize(cross_entropy)
```
```python
# 使用adam优化
```
```python
correct_prediction = tf.equal(tf.argmax(y_conv,
```
```python
1
```
```python
), tf.argmax(y_,
```
```python
1
```
```python
))
```
```python
# 计算准确度
```
```python
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
sess.run(tf.initialize_all_variables())
```
```python
# 变量初始化
```
```python
for
```
```python
i
```
```python
in
```
```python
range(
```
```python
20000
```
```python
):
    batch = mnist.train.next_batch(
```
```python
50
```
```python
)
```
```python
if
```
```python
i%
```
```python
100
```
```python
==
```
```python
0
```
```python
:
```
```python
# print(batch[1].shape)
```
```python
train_accuracy = accuracy.eval(feed_dict={
            x:batch[
```
```python
0
```
```python
], y_: batch[
```
```python
1
```
```python
], keep_prob:
```
```python
1.0
```
```python
})
        print(
```
```python
"step %d, training accuracy %g"
```
```python
%(i, train_accuracy))
    train_step.run(feed_dict={x: batch[
```
```python
0
```
```python
], y_: batch[
```
```python
1
```
```python
], keep_prob:
```
```python
0.5
```
```python
})
print(
```
```python
"test accuracy %g"
```
```python
%accuracy.eval(feed_dict={
    x: mnist.test.images, y_: mnist.test.labels, keep_prob:
```
```python
1.0
```
```python
}))
```
---



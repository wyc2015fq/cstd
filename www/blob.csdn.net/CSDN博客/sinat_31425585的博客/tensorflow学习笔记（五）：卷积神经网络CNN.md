# tensorflow学习笔记（五）：卷积神经网络CNN - sinat_31425585的博客 - CSDN博客
2018年07月05日 17:38:05[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：105
    在早期的图像识别研究中，最大的挑战是如何设计特征，特征提取过程依赖于人的经验。但是在图像中，我们很难根据人为理解来提取出有效而丰富的特征。在深度学习之前，我们必须借助于SIFT、HOG等算法提取出具有良好区分性的特征，再结合SVM等机器学习算法进行图像识别。
    然而，SIFT这类算法提取的特征还是存在一定局限性，在ImageNet比赛中，最好的错误率为26%以上，采用卷积神经网络的方法则可以得到更好的方法。卷积神经网络不需要将特征提取和分类训练两个过程分开，训练的过程中，自动地提取了最有效的特征。与SIFT等算法类似，CNN训练的模型同样对缩放、旋转等因素具有不变性，泛化性能较强。
    与传统神经网络不同之处在于，传统的神经网络为全连接网络，存在参数过多和梯度弥散等问题。卷积神经网络则可以利用空间结构关系减少需要学习的参数，从而提高反向传播算法的训练效率。
    例如：一张图片大小为1000x1000的灰度图，输入数据的维度就是100万，假设连接一个相同大小的隐藏层（100万个隐含节点），那么将会产生100万x100万=一万亿个连接，仅一个全连接层就需要训练一万亿个参数。如果采用卷积操作，如采用10x10大小的卷积核对图像进行卷积操作，那么无论图像多大，我们需要训练的参数仅仅为10x10=100个参数，这样，通过这种权值共享的方式，就能大大的降低需要训练的参数量。
    卷积神经网络的核心就是：局部连接、权值共享、池化层中的降采样（降维）。
    卷积神经网络中，主要组成部分有三个：卷积层（CONV ）、激活层（ACTIVATION ）、池化层（POOL）
    一、卷积层
![](https://img-blog.csdn.net/20180802015815791?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
    卷积操作，需要注意几个参数：
    （1）卷积核大小：f x f
    （2）卷积核个数：n
    （3）步长（stride）：s
    （4）填充（padding）：p
     卷积操作的步长s、卷积核大小f和填充方式p，决定了卷积操作后feature map的大小；卷积核个数n，决定了卷积操作后feature map的深度（depth）。
      填充方式主要有两种：valid和same
      valid，即进行卷积操作时，不对图像进行padding，p = 0；same，即进行卷积操作时，对图像进行padding，保证卷积操作后feature map大小与image尺寸一致：
      n + 2*p + 1 - f = n    ->    p = (f - 1) / 2， 为了方便做padding，卷积核大小一般为奇数。
      进行卷积操作后，feature map 大小为：
![](https://img-blog.csdn.net/2018080202110693?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
     二、激活层（ACTIVATION）
     使用激活层的原因是将卷积神经网络的输出做非线性映射。因为，如果不进行非线性映射，仅仅卷积层（CONV）+池化层（POOL）的叠加组合，最后得到的网络相当于是线性叠加，叠加再多层，也仅仅能解决线性可分问题，使用激活层进行非线性映射之后，可以大大提升网络模型的表达能力。
     常见的激活函数有： Sigmoid、Tanh、Relu、Leaky Relu、ELU、Maxout等。
    三、池化层（POOL）
     池化层有两种形式：max-pooling和average-pooling
     用的最多的是max-pooling，max-pooling的优点有四个（参考：[http://blog.sciencenet.cn/blog-395089-861786.html](http://blog.sciencenet.cn/blog-395089-861786.html)）：
     (1) 层次地宽恕局部的位置扭曲；
     (2) 增加感受野的大小的同时，不一定要增加卷积核的大小,因为大卷积核在计算上很昂贵；
     (3) 减少特征检测的密度来宽恕特征模板匹配的误差；
     (4) 允许局部漏失(譬如因遮挡而造成的部件的缺失)。
     工作原理为：
![](https://img-blog.csdn.net/20180802023652946?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
    进行max-pooling操作后，feature map大小的计算公式为：
![](https://img-blog.csdn.net/20180802023922705?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/201808020240392?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
    以上所有图片摘自Andrew Ng 的deeplearning.ai课程PPT
    用tensorflow实现一个卷积神经网络：
**  1、数据读入：**
```python
import tensorflow as tf
from tensorflow.examples.tutorials.mnist import input_data
tf.device('/gpu:0')
# 读入数据
mnist = input_data.read_data_sets("MNIST_data/", one_hot = True)
# 交互式会话
sets = tf.InteractiveSession()
```
**2、定义权重、偏置、卷积和池化操作：**
```python
# 权重变量定义
def weight_variable(shape):
    initial = tf.truncated_normal(shape, stddev = 0.1)
    return tf.Variable(initial)
# 偏置变量定义
def bias_variable(shape):
    initial = tf.constant(0.1, shape = shape)
    return tf.Variable(initial)
# 卷积操作定义
def conv2d(x, w):
    return tf.nn.conv2d(x, w, strides=[1, 1, 1, 1], padding = 'SAME')
# 最大池化操作定义
def max_pool_2x2(x):
    # 2x2 的最大池化，步长为：2，即无交叉池化
    return tf.nn.max_pool(x, ksize = [1, 2, 2, 1], strides = [1, 2, 2, 1], padding = 'SAME')
```
**   3、定义数据传入接口**
```python
# 定义数据传入接口：数据+标签
x = tf.placeholder(tf.float32, [None, 784])
y_ = tf.placeholder(tf.float32, [None, 10])
```
** 4、定义网络结构**
```python
# 将数据转换成图片
x_image = tf.reshape(x, [-1, 28, 28, 1]) # -1：数目未知 大小：28x28  通道数：1
# 第一层卷积层：卷积->relu->pooling 
w_conv1 = weight_variable([5, 5, 1, 32]) # 卷积核大小：5x5  通道数：1  卷积核数目：32个   
b_conv1 = bias_variable([32])
h_conv1 = tf.nn.relu(conv2d(x_image, w_conv1) + b_conv1)  # 28 x 28：padding = 'SAME'
h_pool1 = max_pool_2x2(h_conv1) # 28 / 2 = 14
# 第二层卷积层：卷积->relu->pooling
w_conv2 = weight_variable([5, 5, 32, 64])
b_conv2 = bias_variable([64])
h_conv2 = tf.nn.relu(conv2d(h_pool1, w_conv2) + b_conv2) # 14 x 14
h_pool2 = max_pool_2x2(h_conv2) # 14 / 2 = 7
# 第一个全连接层
w_fc1 = weight_variable([7 * 7 * 64, 1024])
b_fc1 = bias_variable([1024])
# 展开成一个向量
h_pool2_flat = tf.reshape(h_pool2, [-1, 7 * 7 * 64])
h_fc1 = tf.nn.relu(tf.matmul(h_pool2_flat, w_fc1) + b_fc1) # h_pool2_float * w_fc1 + b_fc1
# 定义dropout
keep_prob = tf.placeholder(tf.float32)
h_fc1_drop = tf.nn.dropout(h_fc1, keep_prob)
# 第二个全连接层
w_fc2 = weight_variable([1024, 10])
b_fc2 = bias_variable([10])
y_conv = tf.nn.softmax(tf.matmul(h_fc1_drop, w_fc2) + b_fc2)
```
**  5、定义损失函数及优化方式**
```python
# 定义损失函数：交叉熵
cross_entropy = tf.reduce_mean(-tf.reduce_sum(y_*tf.log(y_conv), reduction_indices = [1]))
# 定义优化方法为：Adam
train_step = tf.train.AdamOptimizer(1e-4).minimize(cross_entropy)
# 计算准确率
correct_prediction = tf.equal(tf.argmax(y_conv, 1), tf.argmax(y_, 1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
```
**6、训练并测试**
```python
# 训练
for i in range(20000):
    batch = mnist.train.next_batch(50)
    if i % 100 == 0:
        train_accuracy = accuracy.eval(feed_dict = {x: batch[0], y_: batch[1], keep_prob: 1.0})
        print("step %d, training accuracy %g" % (i, train_accuracy))
        
    train_step.run(feed_dict = {x: batch[0], y_: batch[1], keep_prob: 0.5})
 
 # 打印最终的准确率
print("test accuracy %g" % accuracy.eval(feed_dict = {x: mnist.test.images, y_: mnist.test.labels, keep_prob: 1.0}))
```
**7、输出结果：  **
```python
step 18700, training accuracy 1
step 18800, training accuracy 1
step 18900, training accuracy 1
step 19000, training accuracy 1
step 19100, training accuracy 1
step 19200, training accuracy 1
step 19300, training accuracy 1
step 19400, training accuracy 1
step 19500, training accuracy 1
step 19600, training accuracy 1
step 19700, training accuracy 1
step 19800, training accuracy 1
step 19900, training accuracy 1
test accuracy 0.9916
```
参考资料：
《Tensorflow实战》：黄文监、唐源等。
[http://blog.sciencenet.cn/blog-395089-861786.html](http://blog.sciencenet.cn/blog-395089-861786.html)
  Andrew Ng的deeplearning.ai 课程

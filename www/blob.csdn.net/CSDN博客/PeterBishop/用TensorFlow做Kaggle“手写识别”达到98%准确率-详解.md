# 用TensorFlow做Kaggle“手写识别”达到98%准确率-详解 - PeterBishop - CSDN博客





2018年12月01日 12:55:00[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：64
个人分类：[TensorFlow](https://blog.csdn.net/qq_40061421/article/category/8461142)









> 
这是一个TensorFlow的系列文章，本文是第三篇，在这个系列中，你讲了解到机器学习的一些基本概念、TensorFlow的使用，并能实际完成手写数字识别、图像分类、风格迁移等实战项目。




**上一期，我们用Tensorflow实现了Kaggle的手写识别项目，但准确率比较低，只有92%，这次我们打算把识别的准确率提升到98%以上。**

为什么不是上次说的提升到99%以上呢？因为92%到98%是比较容易的，而再从98%到99%是要费不少功夫的，一篇文章难以承载这么多内容，所以将会分成两篇文章，首先是从92%到98%，下一次是从98%到99%。

不要小看提升1%，越往后面，难度就越大。如果我们做到99%准确率，在Kaggle的手写识别这个项目上，也就进入了前25%了，可以说入门了。

## **一、回顾上期**

上期我们学习了**梯度下降、神经网络、损失函数、交叉熵**等概念，然后用42000张图片数据训练了一个简单的神经网络，准确度92%。可以说，这只是一个Hello World。

![](https://upload-images.jianshu.io/upload_images/280127-037094d69146cd45.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/720/format/webp)

Hello World

## 二、如何进行改进

首先，这次我们将使用卷积神经网络来进行图片识别。众所周知，卷积神经网络对于图片识别是非常有效的。

这里我打算这样来构建这个卷积神经网络：

卷积层1+池化层1+卷积层2+池化层2+全连接1+Dropout层+输出层

> 
然而，什么是卷积神经网络？什么是卷积层、池化层、全连接层？Dropout又是什么鬼？


**1 什么是卷积神经网络？**

我们人看到一幅图像，眨眼之间就知道图像中有什么，图像中的主体在干什么。但计算机不同，计算机看到的每一副图像都是一个数字矩阵。那我们怎么让计算机从一个个数字矩阵中得到有用的信息呢，比如边缘，角点？更甚一点，怎么让计算机理解图像呢？

对图像进行卷积，就是接近目标的第一步。

图像在计算机里的表示可能是这样的：



![](https://upload-images.jianshu.io/upload_images/280127-ae7288be70fc6c8b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/387/format/webp)

一张图片

对图像卷积，就是求卷积核作用在图像后，得到的图像对于该卷积核的累加数值。这些累加的数值可以代表这个图片的一些特征。

如果是针对猫进行识别，人可能知道猫头，猫尾巴等特征。CNN对图片进行处理后，也会学习到一些特征，它可能不知道猫头、猫尾巴这些特征，但也会识别出一些我们可能看不出来的特征，CNN通过这些学习到的特征去做判断。

**2 什么是卷积层？**

卷积层的作用是指对图片的矩阵进行卷积运算，得到一些数值，作为图片的某些特征

**3 什么是池化层？**

池化曾的作用是对上层的数据进行采样，也就是只留下一部分，这样的作用是可以缩小数据量和模糊特征。

**4 什么是全连接层？**

全连接层就是连在最后的分类器。前面卷积层和池化层进行处理后，得到了很多的特征，全连接层使用这些特征进行分类。比如识别数字，那就是对0~9的十个类别进行分类。

**5 Dropout是什么？**

Dropout层是为了防止CNN对训练样本过拟合，而导致处理新样本的时候效果不好，采取的丢弃部分激活参数的处理方式。

这里对这些概念的解释都是比较简单的，如果希望详细了解，可以看知乎的这个链接：

[CNN卷积神经网络是什么？](https://link.jianshu.com?t=https://www.zhihu.com/question/52668301)

## 三、 代码实现

![](https://upload-images.jianshu.io/upload_images/280127-a4baab0985192ff7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/729/format/webp)

![](https://upload-images.jianshu.io/upload_images/280127-60feb938b0ee750a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/727/format/webp)

1 标签的处理

![](https://upload-images.jianshu.io/upload_images/280127-7dca5c4c8380c12c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/767/format/webp)

2 把数据分为训练集和验证集

![](https://upload-images.jianshu.io/upload_images/280127-1a414aac6ead3e9c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/771/format/webp)

3 定义处理数据的函数

![](https://upload-images.jianshu.io/upload_images/280127-8c2721d9269c0405.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/982/format/webp)

4 定义网络的结构

![](https://upload-images.jianshu.io/upload_images/280127-4e7ebdb95c4f5da6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/966/format/webp)

5 定义各类参数

![](https://upload-images.jianshu.io/upload_images/280127-d31bc4a4df16405e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/984/format/webp)

6 进行训练

## 四、生成结果

这里迭代20个周期：



![](https://upload-images.jianshu.io/upload_images/280127-4299e397edbfd605.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/588/format/webp)

7 验证集上的准确度

**然后我们使用这个模型对Kaggle的测试集进行预测，并生成cvs格式的结果**

![](https://upload-images.jianshu.io/upload_images/280127-745e91d3c2c238b4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)

8 生成结果

这里建议跑30轮以上，因为在验证集上有98.35%准确率，上传到Kaggle往往就只有百分之九十七点几的准确率了

```python
import numpy as np
import tensorflow as tf
import pandas as pd
import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'
# 1 加载数据集
train = pd.read_csv('train.csv')
test = pd.read_csv('test.csv')

# 2 把图片数据取出来，进行处理
x_train = train.iloc[:, 1:].values
x_train = x_train.astype(np.float)
x_test = test.iloc[:, :].values
x_test = x_test.astype(np.float)

# 3 给到的图片灰度值在0-255，这里将图片的信息控制在0~1之间
x_train = np.multiply(x_train, 1.0 / 255)
x_test = np.multiply(x_test, 1.0 / 255)

# 4 计算图片的长和高，下面会用到
image_size = x_train.shape[1]
image_width = image_height = np.ceil(np.sqrt(image_size)).astype(np.uint8)

# 5 把数据集的标签结果取出来
labels_train = train.iloc[:, 0].values
label_count = np.unique(labels_train).shape[0]


# 写一个对Label进行one-hot处理的函数
def dense_to_ont_hot(labels_dense, num_classes):
    num_labels = labels_dense.shape[0]
    index_offset = np.arange(num_labels) * num_classes
    labels_one_hot = np.zeros((num_labels, num_classes))
    labels_one_hot.flat[index_offset + labels_dense.ravel()] = 1
    return labels_one_hot


# 6 对Label进行one-hot处理
labels = dense_to_ont_hot(labels_train, label_count)
labels = labels.astype(np.uint8)

# 7 设置批次大小，求得批次量
batch_size = 128
n_batch = int(len(x_train) / batch_size)

# 8 定义两个placeholder,用来承载数据，因为每个图片都是一个784维数据，所以x是784列；
#  因为要把图片识别为0-9的10个数字，也就是10个标签，所以y是10列
x = tf.placeholder(tf.float32, [None, 784])
y = tf.placeholder(tf.float32, [None, 10])


# 9 定义几个处理函数
def weight_variable(shape):
    # 初始化权重，正态分布，标准方差为0.1
    initial = tf.truncated_normal(shape, stddev=0.1)
    return tf.Variable(initial)


def bias_variable(shape):
    # 初始化偏置值，设置非零避免死神经元
    initial = tf.constant(0.1, shape=shape)
    return tf.Variable(initial)


def conv2d(x, w):
    # 卷积不改变输入的shape
    return tf.nn.conv2d(x, w, strides=[1, 1, 1, 1], padding='SAME')


# 对Tensorflow的池化进行封装
def max_pool_2x2(x):
    return tf.nn.max_pool(x, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding='SAME')


# 10 把输入变换成一个4d的张量，第二三个对应的是图片的长和宽，第四个参数是颜色
x_image = tf.reshape(x, [-1, 28, 28, 1])

# 11 计算32个特征，每3*3patch，第一二个参数指的是patch的size,第三个参数是输入的
#   channelss,第四个参数是输出的channels
W_conv1 = weight_variable([3, 3, 1, 32])

# 12 偏差的shape应该和输出的shape一致
b_conv1 = bias_variable([32])

# 28*28的图片卷积时步长为1，随意卷积后大小不变，按2*2最大值池化，相当于从2*2块中提取一个最大值
# 所以池化后大小为[28/2,28/2]=[14,14],第二次池化后为[7,7]

# 13 对数据进行卷积操作
h_conv1 = tf.nn.relu(conv2d(x_image, W_conv1) + b_conv1)

# 14 对结果做池化，max_pool_2x2之后，图片变成14*14
h_pool1 = max_pool_2x2(h_conv1)

# 15 在以前的基础上，生成了64个特征
W_conv2 = weight_variable([6, 6, 32, 64])
b_conv2 = bias_variable([64])
h_conv2 = tf.nn.relu(conv2d(h_pool1, W_conv2) + b_conv2)

# 16 max_pool_2x2之后，图片变成7*7
h_pool2 = max_pool_2x2(h_conv2)
h_pool2_flat = tf.reshape(h_pool2, [-1, 7 * 7 * 64])

# 17 构造一个全连接的神经网络，1024个神经元
w_fc1 = weight_variable([7 * 7 * 64, 1024])
b_fc1 = bias_variable([1024])
h_fc1 = tf.nn.relu(tf.matmul(h_pool2_flat, w_fc1) + b_fc1)

# 18 做Droupout操作
keep_prob = tf.placeholder(tf.float32)
h_fc1_drop = tf.nn.dropout(h_fc1, keep_prob)

# 19 把1024个神经元的输入变为一个10维输出
w_fc2 = weight_variable([1024, 10])
b_fc2 = bias_variable([10])
y_conv = tf.matmul(h_fc1_drop, w_fc2) + b_fc2

predictions = tf.nn.softmax(y_conv)
# 20 创建损失函数，以交叉熵的平均值为衡量
loss = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits_v2(labels=y, logits=y_conv))

# 21 用梯度下降法优化参数
train_step_1 = tf.train.AdadeltaOptimizer(0.1).minimize(loss)

# 22 计算准确度
correct_prediction = tf.equal(tf.argmax(y, 1), tf.argmax(y_conv, 1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))

# 23 设置保存模型的文件名参数
global_step = tf.Variable(0, name='global_step', trainable=False)
# saver = tf.train.Saver()

# 24 初始化变量
init = tf.global_variables_initializer()

saver = tf.train.Saver()

with tf.Session() as sess:
    # 25 初始化
    sess.run(init)

    # 这是载入以前训练好的模型的语句，有需要采用，注意把文件名改成成绩比较好的周期
    # saver.restore(sess,'model.ckpt-12')

    # 迭代20周期
    for epoch in range(20):
        print('epoch', epoch + 1)
        for batch in range(n_batch):
            # 27 每次取出一个数据进行训练
            batch_x = x_train[(batch) * batch_size:(batch + 1) * batch_size]
            batch_y = labels[(batch) * batch_size:(batch + 1) * batch_size]

            # 28 [重要] 这是最终运行整个训练模型的语句
            sess.run(train_step_1, feed_dict={x: batch_x, y: batch_y, keep_prob: 0.5})

        batch_x = x_train[n_batch * batch_size:]
        batch_y = labels[n_batch * batch_size:]

        # 28 [重要] 这是最终运行整个训练模型的语句
        sess.run(train_step_1, feed_dict={x: batch_x, y: batch_y, keep_prob: 0.5})

    # 保存训练模型
    saver.save(sess, './model.ckpt')

with tf.Session() as sess1:
    saver.restore(sess1, './model.ckpt')
    # 29 计算预测
    # 一次预测28000个内存不足，故分批预测
    test_batch_size = 28000
    test_n_batch = int(len(x_test) / test_batch_size)
    for batch in range(test_n_batch):
        filename = 'ConvNN' + str(batch) + '.csv'
        test_batch_x = x_test[batch * test_batch_size:(batch + 1) * test_batch_size]
        myPrediction = sess1.run(predictions, feed_dict={x: test_batch_x, keep_prob: 1.0})
        label_test = np.argmax(myPrediction, axis=1)
        pd.DataFrame(label_test).to_csv(filename)
```






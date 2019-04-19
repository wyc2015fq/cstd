# tensorflow学习笔记（二）：softmax Regression 识别手写数字 - sinat_31425585的博客 - CSDN博客
2018年07月03日 12:15:13[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：82
    采用MNIST（Mixed Natianal Institute of Standards an technology database）库，每个样本为28x28大小的灰度图片，这些图片只包含灰度值信息，这些样本图片空白部分像素值为0，有笔记的部分按照颜色的深浅在0到1之间取值。由于MNIST数据集比较简单，丢弃图片的空间信息，将28x28的图片变成784维的一维向量。对应标签为10维向量，对应0~9.
例如：
0 对应于[1, 0, 0, 0, 0, 0, 0, 0, 0, 0]
1对应于[0, 1, 0, 0, 0, 0, 0, 0, 0, 0]
2对应于[0, 0, 1, 0, 0, 0, 0, 0, 0, 0]
3对应于[0, 0, 0, 1, 0, 0, 0, 0, 0, 0]
4对应于[0, 0, 0, 0, 1, 0, 0, 0, 0, 0]
5对应于[0, 0, 0, 0, 0, 1, 0, 0, 0, 0]
6对应于[0, 0, 0, 0, 0, 0, 1, 0, 0, 0]
7对应于[0, 0, 0, 0, 0, 0, 0, 1, 0, 0]
8对应于[0, 0, 0, 0, 0, 0, 0, 0, 1, 0]
9对应于[0, 0, 0, 0, 0, 0, 0, 0, 0, 1]
    不难发现，样本对应标签为一个10维向量，每个样本对应标签只有一个为1，其余为0，这就是one-hot编码。
    MNIST数据，可以直接从tensorflow的examples.tutorials.mnist进行导入：
```python
import tensorflow as tf
from tensorflow.examples.tutorials.mnist import input_data
# 将MNIST数据集下载到MNIST_data文件夹下
mnist = input_data.read_data_sets("MNIST_data/", one_hot = True)
print(mnist.train.images.shape, mnist.train.labels.shape)
print(mnist.test.images.shape, mnist.test.labels.shape)
print(mnist.validation.images.shape, mnist.validation.labels.shape)
```
    输出结果为：
```python
Extracting MNIST_data/train-images-idx3-ubyte.gz
Extracting MNIST_data/train-labels-idx1-ubyte.gz
Extracting MNIST_data/t10k-images-idx3-ubyte.gz
Extracting MNIST_data/t10k-labels-idx1-ubyte.gz
(55000, 784) (55000, 10)
(10000, 784) (10000, 10)
(5000, 784) (5000, 10)
```
    可以看到训练过程采用55000张图片作为训练样本集，10000张图片作为测试样本集，5000张图片作为验证样本集：总计60000张训练样本，10000张测试样本。
    softmax即软最大值，按照我的理解就是相对于现有所有响应而言的最大值。
    在进行训练之前，需要定义两个placeholder作为数据的传入接口，然后定义损失函数及对应优化方法：
```python
sess = tf.InteractiveSession()
# x 为数据输入
x = tf.placeholder(tf.float32, [None, 784])
w = tf.Variable(tf.zeros([784, 10]))
b = tf.Variable(tf.zeros([10]))
# softmax
y = tf.nn.softmax(tf.matmul(x, w) + b)
# y_ 为数据的真实标签
y_ = tf.placeholder(tf.float32, [None, 10])
cross_entropy = tf.reduce_mean(-tf.reduce_sum(y_ * tf.log(y), reduction_indices = [1]))
# 定义优化算法进行训练
train_step = tf.train.GradientDescentOptimizer(0.5).minimize(cross_entropy)
# 全局参数初始化
tf.global_variables_initializer().run()
```
    最后就是训练，依次按照批次读入MNIST数据，进行训练：
```python
# 开始迭代执行训练
for i in range(1000):
    # 每次读入100个样本
    batch_xs, batch_ys = mnist.train.next_batch(100)
    # 将样本数据读入对应placeholder
    train_step.run({x: batch_xs, y_: batch_ys})
    correct_prediction = tf.equal(tf.argmax(y, 1), tf.argmax(y_, 1))
    accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
    print(accuracy.eval({x: mnist.test.images, y_: mnist.test.labels}))
```
    这里方便看训练效果，每次训练结束后，打印出对应准确率accuracy。
    结果如下：
```python
0.9198
0.9188
0.9174
0.919
0.9137
0.9176
0.9184
0.914
0.918
0.9173
0.9157
```
    最后，能够达到91.57%的准确率。
参考文献：
《Tensorflow实战》：黄文监、唐源等。

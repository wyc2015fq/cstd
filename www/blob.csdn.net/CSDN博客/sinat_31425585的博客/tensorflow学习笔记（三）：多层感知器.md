# tensorflow学习笔记（三）：多层感知器 - sinat_31425585的博客 - CSDN博客
2018年07月03日 18:16:58[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：113
    与softmax Regression不同之处在于：
    1、加入一层隐藏层；
    2、训练时，采用dropout（随机失活）策略
    随机失活，其实就是在训练的过程中，随机的将一些神经元的响应进行抑制，不让它激活，使得训练过程中，每一次训练仅仅是网络中的部分神经元，最后训练好的神经网络相当于是多个神经网络的集成，能够很好降低过拟合的风险。
```python
from tensorflow.examples.tutorials.mnist import input_data
import tensorflow as tf
tf.device('/gpu:0')
# 1、导入数据
mnist = input_data.read_data_sets("MNIST_data/", one_hot = True)
sess = tf.InteractiveSession()
# 2、定义模型结构
# 定义三层神经网络：输入层、隐藏层和输出层，神经元个数
in_units = 784
h1_units = 300
out_units = 10
# 定义隐藏层的权重和偏置
w1 = tf.Variable(tf.truncated_normal([in_units, h1_units], stddev = 0.1))
b1 = tf.Variable(tf.zeros([h1_units]))
# 定义输出层的权重和偏置
w2 = tf.Variable(tf.zeros([h1_units, out_units]))
b2 = tf.Variable(tf.zeros([out_units]))
# 数据输入接口
x = tf.placeholder(tf.float32, [None, in_units])
# dropout比例
keep_prob = tf.placeholder(tf.float32)
hidden1 = tf.nn.relu(tf.matmul(x, w1) + b1)
hidden1_drop = tf.nn.dropout(hidden1, keep_prob)
y = tf.nn.softmax(tf.matmul(hidden1_drop, w2) + b2)
y_ = tf.placeholder(tf.float32, [None, 10])
# 3、定义损失函数及优化方法
cross_entropy = tf.reduce_mean(-tf.reduce_sum(y_ * tf.log(y), reduction_indices = [1]))
train_step = tf.train.AdagradOptimizer(0.3).minimize(cross_entropy)
tf.global_variables_initializer().run()
# 4、训练
for i in range(3000):
    batch_xs, batch_ys = mnist.train.next_batch(100)
    train_step.run({x: batch_xs, y_: batch_ys, keep_prob: 0.75})
    correct_prediction = tf.equal(tf.argmax(y, 1), tf.argmax(y_, 1))
    accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
    print(accuracy.eval({x: mnist.test.images, y_: mnist.test.labels, keep_prob: 1.0}))
```
    结果如下：
```python
0.979
0.9796
0.9795
0.9791
0.9782
0.9794
0.9788
0.9788
0.9794
0.9788
0.9794
0.9796
```
    最后能够达到97.96%的准确率。
    参考资料：
《Tensorflow实战》：黄文监、唐源等。

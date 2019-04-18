# TensorFlow指南（三）——深度神经网络（初级） - Soul Joy Hub - CSDN博客

2017年04月19日 17:04:34[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：4810


[http://blog.csdn.net/u011239443/article/details/70224863](http://blog.csdn.net/u011239443/article/details/70224863)

由于本系列博文主要专注于Tensorflow本身，所以还是老样子不会过多讲解神经网络的理论知识。 

可以参阅这篇博文来先理解下神经网络：[http://blog.csdn.net/u011239443/article/details/76680704](http://blog.csdn.net/u011239443/article/details/76680704)

关于神经网络调参，可以参阅：[http://blog.csdn.net/u011239443/article/details/78088602](http://blog.csdn.net/u011239443/article/details/78088602)

# 实现前馈神经网络

```python
import tensorflow as tf
from numpy.random import RandomState

# 每次我们训练的时候 只喂8条数据进去
batch_size = 8

"""
random_normal 生成随机多维数组
def random_normal(shape,
                  mean=0.0,
                  stddev=1.0,
                  dtype=dtypes.float32,
                  seed=None,
                  name=None):
如 w1 就生成了 2×3 的随机数组，数值发布为平均数为0，标准差为1

这里,我们准备构建的神经网络为：
输入层为 2个节点
隐藏层（只有一层）为 3个节点

w1 和 w2 
"""
w1 = tf.Variable(tf.random_normal([2, 3], stddev=1, seed=1))
w2 = tf.Variable(tf.random_normal([3, 1], stddev=1, seed=1))

"""
这里的 placeholder 可以看成一个入口，
我们不希望把如此巨大的数据量全都一下子装进TensorFlow里，这极大的增加了开销。
我们更希望是，每次喂一部分的数据，然后每次更新优化优化权值。
x 是特征数据的入口，就2维
y_ 是label数据的入口
None 代表行数任意
"""
x = tf.placeholder(tf.float32, shape=(None, 2), name='x-input')
y_ = tf.placeholder(tf.float32, shape=(None, 1), name='y-input')

# 矩阵相乘，得到预测结果 y
a = tf.matmul(x, w1)
y = tf.matmul(a, w2)

# 损失函数，后面会讲
cross_entropy = -tf.reduce_mean(y_ * tf.log(tf.clip_by_value(y, 1e-10, 1.0)))

"""
 创建优化器,
 0.001 是学习率
 优化目标三 最小化 cross_entropy
"""
train_step = tf.train.AdamOptimizer(0.001).minimize(cross_entropy)

# 随机生成数据
rdm = RandomState(1)
dataset_size = 128
X = rdm.rand(dataset_size, 2)
Y = [[int(x1 + x2 < 1)] for (x1, x2) in X]

"""
TF的会话需要关闭，
但若我们使用

with tf.Session() as sess:
    .....
    .....

的编码写法，就不用担心忘记关闭会话了。
"""
with tf.Session() as sess:
    #  创建变量初始化的操作，并run它
    init_op = tf.global_variables_initializer()
    sess.run(init_op)
    """
    于是便执行了 tf.Variable ，w1 w2 被初始化了。我们来看下打印结果：
    [[-0.81131822  1.48459876  0.06532937]
     [-2.44270396  0.0992484   0.59122431]]
 [[-0.81131822]
 [ 1.48459876]
 [ 0.06532937]]
    """
    print sess.run(w1)
    print sess.run(w2)

    # 迭代次数
    STEPS = 5000

    for i in range(STEPS):
        # 截取数据
        start = (i * batch_size) % dataset_size
        end = min(start + batch_size, dataset_size)

       # 喂数据给 x 和 y_,并 run 训练
        sess.run(train_step, feed_dict={x: X[start:end], y_: Y[start:end]})

      """
      每训练 1000 次
      我们打印出整个数据集的 cross_entropy

     Atter 0 training step(s),cross entropy on all data is 0.0674925 
Atter 1000 training step(s),cross entropy on all data is 0.0163385 
Atter 2000 training step(s),cross entropy on all data is 0.00907547 
Atter 3000 training step(s),cross entropy on all data is 0.00714436 
Atter 4000 training step(s),cross entropy on all data is 0.00578471 

    我们可以看到随着优化，cross entropy越来越低

      """
        if i % 1000 == 0:
            total_cross_entropy = sess.run(cross_entropy,feed_dict={x: X, y_: Y})
            print("Atter %d training step(s),cross entropy on all data ""is %g " % (i, total_cross_entropy))

    """
    最后 看一下 w1 w2 优化调整后的结果：

    [[-1.9618274   2.58235407  1.68203783]
     [-3.4681716   1.06982327  2.11788988]]
[[-1.8247149 ]
 [ 2.68546653]
 [ 1.41819501]]

    """
    print sess.run(w1)
    print sess.run(w2)
```

## 损失函数

接下来我们看看上述代码中的损失函数：

```python
cross_entropy = -tf.reduce_mean(y_ * tf.log(tf.clip_by_value(y, 1e-10, 1.0)))
```

`cross_entropy`，交叉熵，用于计算两个概率分布之间的距离：

$\large H(p,q) = -\sum_{x}p(x)logq(x) $

这里我们之所以可以使用它作为损失函数，是因为我们这里把二分类`label`简单的看作值是`1`的概率。 

这里的`p(x)`就是`y_`,`label`数据。这里的 `logq(x)`就是`tf.log(tf.clip_by_value(y, 1e-10, 1.0))`，预测的结果。`tf.clip_by_value(y, 1e-10, 1.0)`是为了将y中的取值限制在`[1e-10, 1.0]`。最后我们再使用`tf.reduce_mean`对其求一个平均值，并取负数，便得到了损失函数。

# MNIST数字识别

先给出完整的程序代码的，然后我们再逐步解析：

[https://github.com/xiaoyesoso/TensorFlowinAction/blob/master/InActionB1/chapter5/5_2_1.py](https://github.com/xiaoyesoso/TensorFlowinAction/blob/master/InActionB1/chapter5/5_2_1.py)

重构后的代码： 

mnist_inference：[https://github.com/xiaoyesoso/TensorFlowinAction/blob/master/InActionB1/chapter5/mnist_inference_5_5.py](https://github.com/xiaoyesoso/TensorFlowinAction/blob/master/InActionB1/chapter5/mnist_inference_5_5.py)

mnist_train：[https://github.com/xiaoyesoso/TensorFlowinAction/blob/master/InActionB1/chapter5/mnist_train_5_5.py](https://github.com/xiaoyesoso/TensorFlowinAction/blob/master/InActionB1/chapter5/mnist_train_5_5.py)

mnist_eval：[https://github.com/xiaoyesoso/TensorFlowinAction/blob/master/InActionB1/chapter5/mnist_eval_5_5.py](https://github.com/xiaoyesoso/TensorFlowinAction/blob/master/InActionB1/chapter5/mnist_eval_5_5.py)

# 入口

我们先看到程序的最后：

```python
def main(argv=None):
    mnist = input_data.read_data_sets("/home/soso/MNIST_data", one_hot=True)
    train(mnist)

if __name__ == '__main__':
    tf.app.run()
```

`if __name__ == '__main__':`为`Tensorflow`提供的主程序的人口，`tf.app.run()`会调用`main`函数。

`input_data.read_data_sets`会自动帮你下载MNIST数字数据集，返回数据集mnist。该数据集用`28×28（=784）`的像素矩阵来存储图片数字，所以我们会设置参数`INPUT_NODE = 784`，矩阵元素取值范围在`[0,1]`,0代表白色，1代表黑色。`label`是有长度为10的`0/1`向量表示数字`0~9`，因此参数`OUTPUT_NODE = 10`。如，向量第4个位置为1，其他位置都为0，则该数字为3。

# 训练

train：

```python
# 输入
    x = tf.placeholder(tf.float32, [None, INPUT_NODE], name='x-input')
    # label
    y_ = tf.placeholder(tf.float32, [None, OUTPUT_NODE], name='y-input')

    # 输出层——掩藏层1 的权值
    weights1 = tf.Variable(tf.truncated_normal([INPUT_NODE, LAYER1_NONE], stddev=0.1))
    # 输出层——掩藏层1 表达式中的 常数值
    biases1 = tf.Variable(tf.constant(0.1, shape=[LAYER1_NONE]))

   # 掩藏层1——输出层 的权值
    weight2 = tf.Variable(tf.truncated_normal([LAYER1_NONE, OUTPUT_NODE], stddev=0.1))
    # 掩藏层1——输出层 表达式中的 常数值
    biases2 = tf.Variable(tf.constant(0.1, shape=[OUTPUT_NODE]))
    # 构建出预测结果
 y = inference(x, None, weights1, biases1, weight2, biases2)
```

我们来看下`inference`函数，由于`avg_class`为`None`，于是运行一下：

```python
# 这里使用relu作为激活函数，max（x,0）
       # 构建出掩藏层1
    layer1 = tf.nn.relu(tf.matmul(input_tensor, weights1) + biases1)
    # 构建出预测结果
    return tf.matmul(layer1, weights2) + biases2
```

## 滑动平均模型

在讲解后续的`train`函数之前，我们来降下滑动平均模型。滑动平均模型，是一个可以使得模型在测试数据上更加健壮的方法。滑动平均模型会为对每个变量(`variable`)维护一个影子变量(`shadow variable`)，`shadow variable`初始值等于相应`variable`的初始值，但每次运行`variable`更新时，`shadow variable`会更新：

$\large \color{blue}{shadow\_variable = decay × shadow\_variable + (1 - decay) × variable}$

`decay`的初始值会接近于1（如，0.999）,实际`decay`也具有根据参数`step`动态设置为：

$\large \color{blue}{min\{decay,\frac{1 + step}{10 + step}\}}$

`trian`:

```python
global_step = tf.Variable(0, trainable=False)
        '''
        tf.train.ExponentialMovingAverage：
        传入 decay ， MOVING_AVERAGE_DECAY = 0.99
        传入 step ， global_step = 0
        构建出 variables_averages
        '''    
    variables_averages = tf.train.ExponentialMovingAverage(MOVING_AVERAGE_DECAY, global_step)
    variables_averages_op = variables_averages.apply(tf.trainable_variables())

     average_y = inference(x, variables_averages,weights1, biases1, weight2, biases2)
```

我们看下`inference`函数中另外的个分支：

```python
'''
    与另个分支的区别在于
    先使用 avg_class.average 
    转为 shadow_variable
    再进行构造
    '''
    layer1 = tf.nn.relu(tf.matmul(input_tensor, avg_class.average(weights1)) + avg_class.average(biases1))
    return tf.matmul(layer1,avg_class.average(weights2)) +avg_class.average(biases2)
```

## 损失函数

我们得到的预测结果`y`其实不一定是一个概率分布（即，各个label概率累加和不为1）。这里调动`tf.nn.sparse_softmax_cross_entropy_with_logits`来构建损失函数，还帮我们使用Softmax回归来处理得到符合概率分布结果：

$\large \color{blue}{ softmax(y_i) = \frac{e^{y_i}}{\sum^n_{j=1}e^{y_j}}}$

`trian`:

```python
# 注意，这里API 发生过变化
    cross_entropy = tf.nn.sparse_softmax_cross_entropy_with_logits(logits=y,labels=tf.argmax(y_, 1))
    # 一个batch 数据损失函数的平均值
    cross_entropy_mean = tf.reduce_mean(cross_entropy)
```

### 正则化

为了防止过拟合，我们在优化时并不是直接优化$\color{blue}{J(\theta)}$（这里即`cross_entropy_mean`），而是优化$\color{blue}{J(\theta) + \lambda R(w)}$。其中$\color{blue}{R(w)}$刻画的是模型的复杂程度，而$\color{blue}{ \lambda}$表示模型复杂损失在总损失中的比例，通常$\color{blue}{ \lambda}$会接近于0，因为若$\color{blue}{ \lambda}$太大会导致欠拟合。

我们来看下几种正则化的公式：
- $\large L1: \color{blue}{ R(w) = \sum_i|w_i|}$
- $\large L2: \color{blue}{ R(w) = \sum_iw_i^2}$
- $\large L1和L2同时使用: \color{blue}{ R(w) = \sum_i \alpha |w_i| +(1-\alpha)w_i^2}$

```python
'''
    这里使用的是L2
    lambda为，REGULARIZATION_RATE = 0.0001
    '''
    regularizer = tf.contrib.layers.l2_regularizer(REGULARIZATION_RATE)
    regularization = regularizer(weights1) + regularizer(weight2)
       # 构建出新的 损失函数
    loss = cross_entropy_mean + regularization
```

## 学习率

学习率可以理解为做优化时的步长（如，做梯度下降往坡度最大方向走的步长）。当学习率太小，则需要进行非常轮学习;但当学习率太大，则会导致结果在极值两侧来回移动，无法达到最优解。我们这里调用了`tf.train.exponential_decay`，得到了衰减的学习率，公式如下：

$\large \color{blue}{ decayed\_learning\_rate = learning\_rate * decay\_rate^{global\_step / dacay\_steps} }$

根据公式我们可以看出，学习率在不断的减小。这样就可以使得模型在刚开始学习时，大幅度的学习，再接近极值时小幅度的移动。

```python
'''
     learning rate = LEARNING_RATE_BASE = 0.8
     dacay step = mnist.train.num_examples / BATCH_SIZE
     decay rate = LEARNING_RATE_DACAY = 0.99

    '''
     learning_rate = tf.train.exponential_decay(LEARNING_RATE_BASE, global_step,mnist.train.num_examples / BATCH_SIZE, LEARNING_RATE_DACAY)
```

# 评测

我们继续看接下来的`train`函数：

```python
# 使用随机梯度下降，优化loss
    train_step = tf.train.GradientDescentOptimizer(learning_rate).minimize(loss, global_step=global_step)

    # train_op 包含了两个操作 train_step 和 variables_averages_op
    train_op = tf.group(train_step, variables_averages_op)

    # 构建 正确率
    correct_prediction = tf.equal(tf.argmax(average_y, 1), tf.argmax(y_, 1))
    #  构建一个batch的平均正确率
    accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))

    # 开始 tf 会话
    with tf.Session() as sess:
        # 初始化变量 
        tf.initialize_all_variables().run()

    # 验证集
        validate_feed = {x: mnist.validation.images,y_: mnist.validation.labels}
        # 测试集
        test_feed = {x: mnist.test.images, y_: mnist.test.labels}

    # TRAINING_STEPS = 30000
        for i in range(TRAINING_STEPS):
            # 每训练1000轮，打印出验证集的准确率
            if i % 1000 == 0:
                validate_acc = sess.run(accuracy, feed_dict=validate_feed)
                print("After %d training step(s),v acc is %g"% (i, validate_acc))

            # BATCH_SIZE = 100
            xs, ys = mnist.train.next_batch(BATCH_SIZE)
            sess.run(train_op, feed_dict={x: xs, y_: ys})

        # 打印最终的准确率
        test_acc = sess.run(accuracy, feed_dict=test_feed)
print("After %d training step(s),test accuray is %g"% (TRAINING_STEPS, test_acc))
```

## 输出结果

```
After 0 training step(s),v acc is 0.0434
After 1000 training step(s),v acc is 0.9768
After 2000 training step(s),v acc is 0.9806
After 3000 training step(s),v acc is 0.9838
After 4000 training step(s),v acc is 0.9834
After 5000 training step(s),v acc is 0.9838
After 6000 training step(s),v acc is 0.9842
After 7000 training step(s),v acc is 0.9836
After 8000 training step(s),v acc is 0.9838
After 9000 training step(s),v acc is 0.9844
After 10000 training step(s),v acc is 0.9854
After 11000 training step(s),v acc is 0.9852
After 12000 training step(s),v acc is 0.9846
After 13000 training step(s),v acc is 0.986
After 14000 training step(s),v acc is 0.9858
After 15000 training step(s),v acc is 0.985
After 16000 training step(s),v acc is 0.9856
After 17000 training step(s),v acc is 0.9862
After 18000 training step(s),v acc is 0.986
After 19000 training step(s),v acc is 0.9858
After 20000 training step(s),v acc is 0.9862
After 21000 training step(s),v acc is 0.9858
After 22000 training step(s),v acc is 0.9864
After 23000 training step(s),v acc is 0.9866
After 24000 training step(s),v acc is 0.986
After 25000 training step(s),v acc is 0.9862
After 26000 training step(s),v acc is 0.986
After 27000 training step(s),v acc is 0.9868
After 28000 training step(s),v acc is 0.9864
After 29000 training step(s),v acc is 0.9866
After 30000 training step(s),test accuray is 0.9842
```

![这里写图片描述](https://img-blog.csdn.net/20170623191207625?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


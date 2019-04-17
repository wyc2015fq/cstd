# 在tensorflow中实现完整的机器学习或神经网络算法的步骤 - 墨流觞的博客 - CSDN博客





2018年09月13日 14:01:09[墨氲](https://me.csdn.net/dss_dssssd)阅读数：187








# 在tensorflow中实现完整的机器学习或神经网络算法的步骤

无赖的我又来了，歌词分享环节哦

> 
《不染》 

  一场回忆 生生灭灭 了了心扉 

  再回首浅尝心酒余味
本篇主要讲解如何在tensorflow中实现一个完整的机器学习算法，以下是主要步骤： 

 1. 实现神经网络结构或机器学习算法函数。 

 2. 定义误差函数 

 3. 定义训练函数 

 4. 定义算法评估函数 

 5. 利用以上几个函数在sess中实现完整的程序
读到这，可能有点晕，没关系，下面用例子来介绍： 

下面在mnist数据集上利用逻辑回归模型实现手写字体的识别，讲述整个实现过程。
- 
逻辑回归模型的实现 

将单层网络($y=wx+b$,逻辑回归)的输出经过softmax分类函数输出结果

网络结构 
![](./1.png)
softmax： 
![](./2.png)
tensorflow softmax函数 [tf.nn.softmax函数说明](http://www.tensorfly.cn/tfdoc/api_docs/python/nn.html#softmax)

```python
def inference(input_x):
'''
@param: input_x: [batch, 28*28]
return: out: [batch, 10]
'''
w_init = tf.random_normal_initializer(stddev=(2.0 / 784) ** 0.5)
W = tf.get_variable('W', [784, 10], initializer=w_init)

b_init = tf.constant_initializer(value=0)
b = tf.get_variable('b', initializer=b_init)


# y = wx + b

output = tf.matmul(input_x, W) + b

# out = softmax(output)


# out = tf.nn.softmax(output)


return output
```
- 
定义误差函数 

在这里使用交叉熵来定义误差函数。

[交叉熵说明](https://blog.csdn.net/chaipp0607/article/details/73392175),包括定义，例子演示以及tensorflow实现。

**WARNING**: This op expects unscaled logits, since it performs a softmax on logits internally for efficiency. Do not call this op with the output of softmax, as it will produce incorrect results

在使用此函数时，不要再先前的网络层中调用tf.nn.softmax,因为此函数内部会在logits上调用softmax函数。否则会产生错误结果

```python
def loss(output, y):
'''
return : Tensor [1]
'''
xentropy = tf.nn.softmax_cross_entropy_with_logits(logits=output, labels=y)
loss = tf.reduce_mean(xentropy)
return loss
```

- 
定义训练函数

```python
def training(loss, learning_rate, global_step=None):
optimizer = tf.train.GradientDescentOptimizer(learning_rate=learning_rate)

train_op = optimizer.minimize(loss, global_step=global_step)

return train_op
```

- 
定义评估函数 


$accuracy = \frac{识别正确的个数}{总的个数}$

用于在验证集或测试集上评估算法表现

```python
def evalute(output, y):

# 其实说实话，这个操作完全没必要，看一下softmax函数的定义就知道，所以就不要了


# softmax_out = tf.nn.softmax(output)   # delete this operation



# tf.argmax(x, axis)关于axis。 对于二维来说axis=0 是 列； axis=1 是 行

correct_prediction = tf.equal(tf.argmax(output, 1),
                              tf.argmax(y, 1) )


# cast是类型转换函数

accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))


return accuracy
```

- 
tensorflow程序搭建


```python
import tensorflow as tf
#需要用到的数据集 ，通过下面三行代码导入
from tensorflow.examples.tutorials.mnist import input_data
data_dir = 'temp'
mnist = input_data.read_data_sets(data_dir, one_hot=True)


# parameters
learning_rate = 0.001
training_epochs = 1000
batch_size = 100
display_step = 1

with tf.Graph().as_default():
    mnist data image of shape 28*28=784
    x = tf.placeholder(dtype=tf.float32, shape=[None, 784])

    # 0-9 digits recognition => 10 classes
    y = tf.placeholder(dtype=tf.float32, shape=[None, 10])

    # 算法输出
    output = inference(x)
    # 误差函数
    cost = loss(output, y)

    # 维护训练代数的参数，通过trainable设为不可训练参数
    global_step = tf.Variable(0, name='global_step', trainable=False)

    # 定义训练和评估运算
    train_op = training(cost, global_step,global_step=global_step)
    eval_op = evaluate(output, y)

    with tf.Session() as sess:
        # 必须要有的第一步
        init_op = tf.global_variables_initializer()
        sess.run(init_op)

        # 开始循环训练
        for epoch in range(training_epochs):

            total_batch = int(mnist.train.num_examples/batch_size)

            # Loop over all batches
            for i in range(total_batch):
            mbatch_x, mbatch_y = mnist.train.next_batch(batch_size)
            # Fit training using batch data
            feed_dict = {x : mbatch_x, y : mbatch_y}
            #TODO: 训练算法
            sess.run(train_op, feed_dict=feed_dict)

            #TODO: 在验证集上评估算法
            # Display logs per epoch step
            if epoch % display_step == 0:
            val_feed_dict = {
            x : mnist.validation.images,
            y : mnist.validation.labels
            }
            accuracy = sess.run(eval_op, feed_dict=val_feed_dict)
            print "Validation Error:", (1 - accuracy)

print ("Optimization Finished!")
test_feed_dict = {
                x : mnist.test.images,
                y : mnist.test.labels
                }
accuracy = sess.run(eval_op, feed_dict=test_feed_dict)
print ("Test Accuracy:", accuracy)
```










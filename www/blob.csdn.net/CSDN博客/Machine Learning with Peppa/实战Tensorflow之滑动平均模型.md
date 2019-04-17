# 实战Tensorflow之滑动平均模型 - Machine Learning with Peppa - CSDN博客





2018年09月26日 21:31:42[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：284








本篇文章参考《Tensorflow实战Google深度学习框架》一书

### 目的

在Tensorflow的教程里面，使用梯度下降算法训练神经网络时，都会提到一个使模型更加健壮的策略，即滑动平均模型。本文基于最近一段时间的学习，记录一下自己的理解。

### 基本思想

在使用梯度下降算法训练模型时，每次更新权重时，为每个权重维护一个影子变量，该影子变量随着训练的进行，会最终稳定在一个接近真实权重的值的附近。那么，在进行预测的时候，使用影子变量的值替代真实变量的值，可以得到更好的结果。

### 操作步骤

1 训练阶段：为每个可训练的权重维护影子变量，并随着迭代的进行更新；

2 预测阶段：使用影子变量替代真实变量值，进行预测。

在采用随机梯度下降算法训练神经网络时，使用 tf.train.ExponentialMovingAverage 滑动平均操作的意义在于提高模型在测试数据上的健壮性（robustness）。tensorflow 下的 tf.train.ExponentialMovingAverage 需要提供一个衰减率（decay）。该衰减率用于控制模型更新的速度。该衰减率用于控制模型更新的速度，ExponentialMovingAverage 对每一个（待更新训练学习的）变量（variable）都会维护一个影子变量（shadow variable）。影子变量的初始值就是这个变量的初始值，

![](https://img-blog.csdn.net/2018092814082153?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM5NTIxNTU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



由上述公式可知， decay 控制着模型更新的速度，越大越趋于稳定。实际运用中，decay 一般会设置为十分接近 1 的常数（0.99或0.999）。为了使得模型在训练的初始阶段更新得更快，ExponentialMovingAverage 还提供了 num_updates 参数来动态设置 decay 的大小：

![](https://img-blog.csdn.net/20180928140756948?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM5NTIxNTU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



下面是一个使用滑动平均模型的例子：

```python
"""
Created on Thu Sep 27 21:32:47 2018

@author: Administrator
"""

import tensorflow as tf
from tensorflow.examples.tutorials.mnist import input_data


def forward_prop(input_data, avg_class, w1,b1,w2,b2):
    '''without avg_class, it use the variable'''
    if avg_class == None:
        a1 = tf.nn.relu(tf.matmul(input_data,w1) + b1)
        z2 = tf.matmul(a1, w2) + b2
    else:
        #first use avg_class, then forward prop
        a1 = tf.nn.relu(tf.matmul(input_data,avg_class.average(w1)) + 
                        avg_class.average(b1))
        z2 = tf.matmul(a1,avg_class.average(w2)) + avg_class.average(b2)
        
    return z2

def init_parameters(n_x,n_h,n_y):
    w1 = tf.Variable(tf.truncated_normal([n_x, n_h], stddev=0.1))
    b1 = tf.Variable(tf.constant(0.1, shape=[n_h]))
    w2 = tf.Variable(tf.truncated_normal([n_h, n_y], stddev=0.1))
    b2 = tf.Variable(tf.constant(0.1, shape=[n_y]))
    
    paras = [w1,b1,w2,b2]
    return paras
    

            
def train_model(data):
    '''train model in dataset'''
    
    learning_rate = 0.8
    rate_decay = 0.99
    regular_rate = 0.001
    training_step = 5000
    moving_avg_decay = 0.99
    batch_size = 100
    
    X, Y = data.train.next_batch(batch_size)
    
    print("X shape: {}".format(X.shape))
    
    n_x = X.shape[1]
    n_y = Y.shape[1]
    n_h = 250
    
    xs = tf.placeholder(tf.float32,[None, n_x], name='x_inp')
    ys = tf.placeholder(tf.float32,[None, n_y], name='y_inp')
    
    parameters = init_parameters(n_x,n_h,n_y)
    w1 = parameters[0]
    b1 = parameters[1]
    w2 = parameters[2]
    b2 = parameters[3]
    
    y_pred = forward_prop(xs, None, w1,b1,w2,b2)
    global_step = tf.Variable(0, trainable=False)
    var_avg = tf.train.ExponentialMovingAverage(moving_avg_decay,global_step)
    
    var_avg_op = var_avg.apply(tf.trainable_variables())
    
    y_avg = forward_prop(xs, var_avg,w1,b1,w2,b2)
    
    cross_ent = tf.nn.sparse_softmax_cross_entropy_with_logits(
            logits=y_pred, labels=tf.argmax(ys,1))
    cross_entropy = tf.reduce_mean(cross_ent)
    
    reg = tf.contrib.layers.l2_regularizer(regular_rate)
    regular = reg(w1) + reg(w2)
    
    loss = cross_entropy + regular
    learning_rate = tf.train.exponential_decay(
            learning_rate, global_step, data.train.num_examples/batch_size,
            rate_decay)
    
    train_step = tf.train.GradientDescentOptimizer(learning_rate).minimize(
            loss, global_step=global_step)
    train_op = tf.group(train_step, var_avg_op)
    
    correct = tf.equal(tf.argmax(y_avg,1),tf.argmax(ys,1))
    accuracy = tf.reduce_mean(tf.cast(correct,tf.float32))
    
    with tf.Session() as sess:
        tf.global_variables_initializer().run()
        
        validate_feed = {xs:data.validation.images,
                         ys:data.validation.labels}
        
        test_feed = {xs:data.test.images,ys:data.test.labels}
        
        for i in range(training_step):
            if i % 1000 == 0:
                validate_acc = sess.run(accuracy,feed_dict=validate_feed)
                print("After %d steps, accuracy using avg_model is %g" %
                      (i, validate_acc))
            
            x_,y_ = data.train.next_batch(batch_size)
            sess.run(train_op,feed_dict={xs:x_,ys:y_})
    
        test_acc = sess.run(accuracy, feed_dict=test_feed)
        print("After %d steps, test accuracy is: %g" %(training_step, test_acc))
#
#def main(argv=None):
#    
#    mnist = input_data.read_data_sets("mnist_data", one_hot=True)
#    print("training data size:", mnist.train.num_examples)
#    
#    train_model(mnist)
    
if __name__ =='__main__':
    mnist = input_data.read_data_sets("mnist_data", one_hot=True)
    print("training data size:", mnist.train.num_examples)
    
    train_model(mnist)
```

### 结果

不使用滑动平均模型

```
After 0 training steps, validation accuracy is 0.16740000247955322
After 1000 training steps, validation accuracy is 0.9747997522354126
After 2000 training steps, validation accuracy is 0.9775997400283813
After 3000 training steps, validation accuracy is 0.9811996817588806
After 4000 training steps, validation accuracy is 0.9805997014045715
after 5000 steps, test accuracy is 0.9790000915527344
```

使用滑动平均模型

```
After 0 training steps, validation accuracy is 0.16499999165534973
After 1000 training steps, validation accuracy is 0.9763997197151184
After 2000 training steps, validation accuracy is 0.9829997420310974
After 3000 training steps, validation accuracy is 0.9825997352600098
After 4000 training steps, validation accuracy is 0.9843996167182922
after 5000 steps, test accuracy is 0.9821001291275024
```

从结果看，移动滑动平均模型后，在验证和测试集上都又快又好。

### 总结

1 滑动平均模型在梯段下降算法上才会有好的结果，别的优化算法没有这个现象，没见到合理的解释。

2 优化的方法有很多，可能这个可以作为最后的提高健壮性的错失。




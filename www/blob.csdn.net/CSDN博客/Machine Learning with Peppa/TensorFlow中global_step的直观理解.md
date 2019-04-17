# TensorFlow中global_step的直观理解 - Machine Learning with Peppa - CSDN博客





2018年09月28日 16:38:36[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：132








global_step在滑动平均、优化器、指数衰减学习率等方面都有用到，这个变量的实际意义非常好理解：代表全局步数，比如在多少步该进行什么操作，现在神经网络训练到多少轮等等，类似于一个钟表。

根据代码可以发现global_step的初始化值是0:
`  global_step=tf.Variable(0, trainable=False) `


这个初始化代码十分简单，但也是问题所在。如果global_step直接赋值为0了，还怎么表示全局的步数？

当助教的时候，有一个学妹问我关于global_step的操作问题，我一时没回答上来，global_step到底是怎么完成自动加1的。



关于这个问题在CSDN查到一篇文章[《tensorflow中的关键字global_step使用》](http://blog.csdn.net/uestc_c2_403/article/details/72403833)

稍微改了一下人家的代码：

```python
import tensorflow as tf

import numpy as np


x = tf.placeholder(tf.float32, shape=[None, 1], name='x')

y = tf.placeholder(tf.float32, shape=[None, 1], name='y')

w = tf.Variable(tf.constant(0.0))


global_steps = tf.Variable(0, trainable=False)



learning_rate = tf.train.exponential_decay(0.1, global_steps, 10, 2, staircase=False)

loss = tf.pow(w*x-y, 2)


train_step = tf.train.GradientDescentOptimizer(learning_rate).minimize(loss,global_step=global_steps)


with tf.Session() as sess:

sess.run(tf.global_variables_initializer())

for i in range(10):

sess.run(train_step, feed_dict={x:np.linspace(1,2,10).reshape([10,1]),

y:np.linspace(1,2,10).reshape([10,1])})

print(sess.run(learning_rate))

print(sess.run(global_steps))
```



输出为：



0.107177

1

0.11487

2

0.123114

3

0.131951

4

0.141421

5

0.151572

6

0.16245

7

0.17411

8

0.186607

9

0.2

10

这里的global_tep确实完成自动加1了，但具体为什么自动加一，原文章里面也没有提到。

经过多次修改代码验证之后得出，如果把

train_step = tf.train.GradientDescentOptimizer(learning_rate).minimize(loss,global_step=global_steps)

后面部分的global_step=global_steps去掉，global_step的自动加一就会失效，输出如下：

0.1

0

0.1

0

0.1

0

0.1

0

0.1

0

0.1

0

0.1

0

0.1

0

0.1

0

0.1

0


因为指数衰减的学习率是伴随global_step的变化而衰减的，所以当global_step不改变时，学习率也变成一个定值。

综上所述：损失函数优化器的minimize()中global_step=global_steps能够提供global_step自动加一的操作。

这里有个额外的疑虑说明：global_steps是等号右边，在编程语言里面通常当作定值（即不会受函数影响）赋值给等号左边的global_step。然而，在这个优化器里面能够使得右边的变量自动加一。这确实是编程语言里面少见的，也是需要特别注意的。

---

以上是原文

后期解答: tf.Variable定义的变量是**tf自定义的数据结构，以张量形式存在于网络当中**。这也是w, b以及global_steps能被后台改变的原因。上面那个问题也是这个原因，global_steps在右边，但它不是简单的value，而是特殊的数据结构(tensorflow框架使然)。




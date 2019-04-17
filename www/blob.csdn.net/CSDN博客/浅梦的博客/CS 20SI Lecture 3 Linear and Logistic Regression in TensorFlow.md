# CS 20SI|Lecture 3 Linear and Logistic Regression in TensorFlow - 浅梦的博客 - CSDN博客





2017年02月04日 21:52:12[浅梦s](https://me.csdn.net/u012151283)阅读数：1756








本章主要是利用前两章学的内容实现一下线性回归和逻辑回归。

# Linear Regression in TensorFlow

由于格式的问题，建议使用PC端浏览器观看！

## Solution

直接贴代码

```python
"""
Simple linear regression example in TensorFlow
This program tries to predict the number of thefts from 
the number of fire in the city of Chicago
"""
import numpy as np
import matplotlib.pyplot as plt
import tensorflow as tf
import xlrd
from __future__ import print_function
```

```python
DATA_FILE = '../data/fire_theft.xls'
# Step 1: read in data from the .xls file
book = xlrd.open_workbook(DATA_FILE, encoding_override='utf-8')
sheet = book.sheet_by_index(0)
data = np.asarray([sheet.row_values(i) for i in range(1, sheet.nrows)])
n_samples = sheet.nrows - 1
```

### Phase 1: Assemble the graph

```python
# Step 2: create placeholders for input X (number of fire) and label Y (number of theft)
X = tf.placeholder(tf.float32,name = 'X')
Y = tf.placeholder(tf.float32,name = 'Y')

# Step 3: create weight and bias, initialized to 0
# name your variables w and b
w = tf.Variable(0.0,name = 'weight')
b = tf.Variable(0.0,name = 'bias')

# Step 4: predict Y (number of theft) from the number of fire
# name your variable Y_predicted
Y_predicted = w * X + b

# Step 5: use the square error as the loss function
# name your variable loss
loss = tf.square(Y_predicted - Y)
# Step 6: using gradient descent with learning rate of 0.01 to minimize loss
optimizer = tf.train.GradientDescentOptimizer(learning_rate=0.001).minimize(loss)
```

### Phase 2: Train our model

```python
with tf.Session() as sess:
    # Step 7: initialize the necessary variables, in this case, w and b
    # TO - DO   
    sess.run(tf.global_variables_initializer())
    # Step 8: train the model
    for i in range(100): # run 100 epochs
        total_loss = 0
        for x, y in data:
            # Session runs optimizer to minimize loss and fetch the value of loss
            # TO DO: write sess.run()
            _,l = sess.run([optimizer,loss],feed_dict = {X:x,Y:y})
            total_loss += l
        print ("Epoch {0}: {1}".format(i, total_loss/n_samples))
    w_value,b_value = sess.run([w,b])
```

### plot the results

```python
#plot the results
X, Y = data.T[0], data.T[1]
plt.plot(X, Y, 'bo', label='Real data')
plt.plot(X, X * w_value + b_value, 'r', label='Predicted data')
plt.legend()
plt.show()
```

![这里写图片描述](https://img-blog.csdn.net/20170204214434425?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## Analyze the code

```python
optimizer ​=​ tf​.​train​.​GradientDescentOptimizer​(​learning_rate​=​0.01​).​minimize​(​loss) 
sess​.​run​(​optimizer​,​ feed_dict​={​X​:​ x​,​ Y​:​y​})
```

思考两个问题 

1. 为什么train_op可以作为fetches list的成员传递给Session.run()? 

2. TensorFlow如何知道应该对哪些变量进行更新？ 

任何op都可以作为参数是传递给fetches，而`train_op`的优化目标为最小化`loss`,所以TensorFlow会寻找`loss`所依赖的所有的`trainable`变量进行更新。
## Optimizers

默认情况下，优化器会训练所有的目标函数所依赖的可训练的`trainable`变量。如果不希望训练某个变量，可以在声明变量的到时候将`trainable`参数设置为`False`。 

举个例子，比如说在使用学习率随迭代次数降低的优化器的时候，我们不希望`global_step`被优化器改变。

```python
global_step = tf.Variable(0, trainable=False, dtype=tf.int32) 
learning_rate = 0.01 * 0.99 ** tf.cast(global_step, tf.float32) 
increment_step = global_step.assign_add(1) 
optimizer = tf.GradientDescentOptimizer(learning_rate) # learning rate can be a tensor
```

我们也可以要求优化器对特定的变量求导，或者修改求导的计算公式。 

下面的例子是说明如何修改求导求导公式使得新的导数是原来的导数数值减去1.

```python
# create an optimizer. 
optimizer ​=​ ​GradientDescentOptimizer​(​learning_rate​=​0.1) 
# compute the gradients for a list of variables. 
grads_and_vars ​=​ opt​.​compute_gradients​(​loss​,​ ​<​list of variables​>) 
# grads_and_vars is a list of tuples (gradient, variable).  Do whatever you need to the 'gradient' part, for example, subtract each of them by 1. 
subtracted_grads_and_vars ​=​ ​[(​gv​[​0​]​ ​-​ ​1.0​,​ gv​[​1​])​ ​for​ gv ​in​ grads_and_vars] 

# ask the optimizer to apply the subtracted gradients. 
optimizer​.​apply_gradients​(​subtracted_grads_and_vars)
```

More on computing gradients  
`optimizer`类会在计算图上自动求导，但是新的优化器的创造者或者专家用户可以调用下面的更底层的函数。 
`tf​.​gradients​(​ys​,​ xs​,​ grad_ys​=​None​,​ name​=​'gradients'​, colocate_gradients_with_ops​=​False​,​ gate_gradients​=​False​,​ aggregation_method​=​None​)`

This method constructs symbolic partial derivatives of sum of ys w.r.t. x in xs. ys and xs are each a Tensor or a list of tensors. grad_ys is a list of Tensor, holding the gradients received by the ys. The list must be the same length as ys.  
*Technical detail​*: This is especially useful when training only parts of a model. For example, we can use tf.gradients() for to take the derivative G of the loss w.r.t. to the middle layer. Then we use an optimizer to minimize the difference between the middle layer output M and M + G. This only updates the lower half of the network. 
## List of optimizers

TensorFlow封装了一些优化器，更具体的说明请查看官方文档。

```
tf.train.GradientDescentOptimizer
tf.train.AdadeltaOptimizer 
tf.train.AdagradOptimizer 
tf.train.AdagradDAOptimizer 
tf.train.MomentumOptimizer
tf.train.AdamOptimizer
tf.train.FtrlOptimizer
tf.train.ProximalGradientDescentOptimizer
tf.train.ProximalAdagradOptimizer
tf.train.RMSPropOptimizer
```

关于不同的梯度下降优化算法之间的比较，可以查看这篇博客[http://sebastianruder.com/optimizing-gradient-descent/](http://sebastianruder.com/optimizing-gradient-descent/)

结论如下：

> 
RMSprop is an extension of Adagrad that deals with its radically diminishing learning rates. It is identical to Adadelta, except that Adadelta uses the RMS of parameter updates in the numerator update rule. Adam, finally, adds bias-correction and momentum to RMSprop. Insofar, RMSprop, Adadelta, and Adam are very similar algorithms that do well in similar circumstances. Kingma et al. [15] show that its bias-correction helps Adam slightly outperform RMSprop towards the end of optimization as gradients become sparser. Insofar, Adam might be the best overall choice.” 


总的来说，如果你对不同算法的优缺点不是很熟悉，那么就使用 
`tf.train.AdamOptimizer`。

# Logistic Regression in TensorFlow

## Solution

这个我就直接贴课程github上的代码了，和前面的线性回归大同小异，主要是多了softmax，交叉熵等函数。

```python
"""
Simple logistic regression model to solve OCR task 
with MNIST in TensorFlow
MNIST dataset: yann.lecun.com/exdb/mnist/

"""

import tensorflow as tf
import numpy as np
from tensorflow.examples.tutorials.mnist import input_data
import time
# Define paramaters for the model
learning_rate = 0.01
batch_size = 128
n_epochs = 30

# Step 1: Read in data
# using TF Learn's built in function to load MNIST data to the folder data/mnist
mnist = input_data.read_data_sets('/data/mnist', one_hot=True) 

# Step 2: create placeholders for features and labels
# each image in the MNIST data is of shape 28*28 = 784
# therefore, each image is represented with a 1x784 tensor
# there are 10 classes for each image, corresponding to digits 0 - 9. 
# each lable is one hot vector.
X = tf.placeholder(tf.float32, [batch_size, 784], name='X_placeholder') 
Y = tf.placeholder(tf.float32, [batch_size, 10], name='Y_placeholder')

# Step 3: create weights and bias
# w is initialized to random variables with mean of 0, stddev of 0.01
# b is initialized to 0
# shape of w depends on the dimension of X and Y so that Y = tf.matmul(X, w)
# shape of b depends on Y
w = tf.Variable(tf.random_normal(shape=[784, 10], stddev=0.01), name='weights')
b = tf.Variable(tf.zeros([1, 10]), name="bias")

# Step 4: build model
# the model that returns the logits.
# this logits will be later passed through softmax layer
logits = tf.matmul(X, w) + b 

# Step 5: define loss function
# use cross entropy of softmax of logits as the loss function
entropy = tf.nn.softmax_cross_entropy_with_logits(logits, Y, name='loss')
loss = tf.reduce_mean(entropy) # computes the mean over all the examples in the batch

# Step 6: define training op
# using gradient descent with learning rate of 0.01 to minimize loss
optimizer = tf.train.GradientDescentOptimizer(learning_rate).minimize(loss)

with tf.Session() as sess:
    # to visualize using TensorBoard
    writer = tf.summary.FileWriter('./my_graph/03/logistic_reg', sess.graph)

    start_time = time.time()
    sess.run(tf.global_variables_initializer()) 
    n_batches = int(mnist.train.num_examples/batch_size)
    for i in range(n_epochs): # train the model n_epochs times
        total_loss = 0

        for _ in range(n_batches):
            X_batch, Y_batch = mnist.train.next_batch(batch_size)
            _, loss_batch = sess.run([optimizer, loss], feed_dict={X: X_batch, Y:Y_batch}) 
            total_loss += loss_batch
        print 'Average loss epoch {0}: {1}'.format(i, total_loss/n_batches)

    print 'Total time: {0} seconds'.format(time.time() - start_time)

    print('Optimization Finished!') # should be around 0.35 after 25 epochs

    # test the model
    n_batches = int(mnist.test.num_examples/batch_size)
    total_correct_preds = 0
    for i in range(n_batches):
        X_batch, Y_batch = mnist.test.next_batch(batch_size)
        _, loss_batch, logits_batch = sess.run([optimizer, loss, logits], feed_dict={X: X_batch, Y:Y_batch}) 
        preds = tf.nn.softmax(logits_batch)
        correct_preds = tf.equal(tf.argmax(preds, 1), tf.argmax(Y_batch, 1))
        accuracy = tf.reduce_sum(tf.cast(correct_preds, tf.float32)) # need numpy.count_nonzero(boolarr) :(
        total_correct_preds += sess.run(accuracy)   

    print 'Accuracy {0}'.format(total_correct_preds/mnist.test.num_examples)

    writer.close()
```

附上课程slide上的一张计算图，可以看到计算图的架构非常复杂，下一章就会讲述如何使我们的神经网络架构结构化，具有更清晰的结构！ 
![这里写图片描述](https://img-blog.csdn.net/20170204221348206?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)






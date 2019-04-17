# 【Tensorflow】逻辑斯特回归（Logistic Regression）的简单实现 - CD's Coding - CSDN博客





2018年10月07日 02:30:53[糖果天王](https://me.csdn.net/okcd00)阅读数：236








### Introduction

为了简单的介绍和讲解一下Tensorflow的基本操作，

我决定做一个简单的逻辑斯特回归实现与代码讲解，

但不太会用Markdown的方式来展现一个JupyterNotebook，

姑且就按照“说明—实例”的方式来解释逐个代码块好了。
### Import packages

```python
```python
# coding: utf-8
# ============================================================================
#   Copyright (C) 2017 All rights reserved.
#
#   filename : Logistic_Regression.py
#   author   : chendian / okcd00@qq.com
#   date     : 2018-09-26
#   desc     : Tensorflow Logistic Regression Tutorial
#   
# ============================================================================
from __future__ import print_function
import os
os.environ["CUDA_VISIBLE_DEVICES"] = ""

import sys
import math
import numpy as np
import tensorflow as tf
import matplotlib.pyplot as plt
```
```

### Loading training data from sklearn

> 
如果需要使用 sklearn 第三方库中自带的数据集，这里我列出了三种，方便调用与测试


**sklearn的安装**

```
$ pip install sklearn
```

**调用方法**

```python
```python
data = load_data(name='moons')
data = load_data(name='circles')
data = load_data(name='linear')
```
```

如果安装sklearn有困难，也可以直接从文件读取：

```python
```python
data = load_data(name='moons', True)
data = load_data(name='circles', True)
data = load_data(name='linear', True)
```
```

**基本实现**

```python
```python
# use data from sklearn package
def load_moons():
    from sklearn.datasets import make_moons
    np.random.seed(0)
    X, y = make_moons(800, noise=0.2)
    print ("dataset shape:", X.shape)
    
    # return train validate test sets 
    return [(X[0:600,],y[0:600,]), (X[600:800,],y[600:800,])]

def load_circles():
    from sklearn.datasets import make_circles
    np.random.seed(0)
    X, y = make_circles(800, noise=0.2, factor=0.5, random_state=2)
    print ("dataset shape:", X.shape)
    
    # return train validate test sets 
    return [(X[0:600,],y[0:600,]), (X[600:800,],y[600:800,])]

def load_linear():
    from sklearn.datasets import make_classification
    np.random.seed(0)
    X, y = make_classification(
        800, n_features=2, n_redundant=0, n_informative=1,
        random_state=1, n_clusters_per_class=1)
    print ("dataset shape:", X.shape)
    
    # return train validate test sets 
    return [(X[0:600,],y[0:600,]), (X[600:800,],y[600:800,])]

def load_data(name='moons', load_directly=False):
    _datasets={
        'moons': load_moons,
        'linear': load_linear,
        'circles': load_circles,
    }
    try:
        ret = pickle.load(open('./{}.pkl'.format(name), 'r')) if load_directly else _datasets[name]()
    except Exception as e:
        print("set name as 'moons', 'linear' or 'circles',\n or check your files' existence")
        print(e)
    return ret
```
```

### Define network

> 
此处给出的是逻辑回归(Logistic Regression)的神经网络结构


对于输入向量x，其属于类别i的概率为：
$$P\left ( Y=i\mid x,W,b \right ) =softmax_i\left ( Wx+b \right ) = \frac{e^{W_ix+b_i}}{\sum_{j}e^{W_jx+b_j}} $$

模型对于输入向量x的预测结果y_pred是所有类别的预测中概率值最大的，即
$$y_{pred}=argmax_iP\left ( Y=i\mid x,W,b \right )$$

在LR模型中，需要求解的参数为权重矩阵W和偏置向量b，为了求解模型的两个参数，首先必须定义损失函数。对于上述的多类别Logistic回归，可以藉由Log似然函数作为其损失函数（**负对数似然** 注意取负）：
$$L\left ( \theta =\left \{ W,b \right \},D \right )=\sum_{i=0}^{\left | D \right |}log\left ( P\left ( Y=y^{\left ( i \right )}\mid x^{\left ( i \right )},W,b  \right ) \right )$$

P.S.

代码中的 $Wx$ 实际实现为 $xW$，效果为：

将 x 的末维度 `n_features` 在矩阵乘法后转化为 `n_classes`。

代码中使用的`softmax_cross_entropy(y_truth, logits)`效果为:

先将`logits`作`softmax`操作获得`y_pred`，然后使用`y_truth`和`y_pred`作负对数似然。
```python
```python
class LogisticRegression():
    def __init__(self, n_in, n_out):
        self.X = tf.placeholder(tf.float32, [None, n_in], name='X')
        self.y = tf.placeholder(tf.int32, [None], name='y')
        self.init_variables(n_in, n_out)
        
    def init_variables(self, n_in, n_out):
        # n_in means n_features
        # n_out means n_classes
        self.W = tf.Variable(
            initial_value=tf.constant(0.0, shape=[n_in, n_out]),
            dtype=tf.float32, name='weight')
        self.b = tf.Variable(
            initial_value=tf.constant(0.0, shape=[n_out]),
            dtype=tf.float32, name='bias')

    def softmax(self, logits):
        # softmax = tf.exp(logits) / tf.reduce_sum(tf.exp(logits), axis)
        return tf.nn.softmax(logits, -1)
        
    def negative_log_likelihood(self, y_pred, y):
    	# Deprecated.
        prob = self.sigmoid(y_pred)
        positive_likelihood = tf.log(prob) * y.reshape(-1, 1)
        negative_likelihood = tf.log(1 - prob) * (1 - y.reshape(-1, 1))
        log_likelihood = positive_likelihood + negative_likelihood
        return -tf.reduce_mean(log_likelihood)
    
    def get_network(self):
        hidden = tf.matmul(self.X, self.W) + self.b
        self.y_pred = tf.argmax(self.softmax(hidden), axis=-1)  
        return self.y_pred, hidden
    
    def get_loss(self, hidden):
        # self.loss = self.negative_log_likelihood(y_pred, y)
        # self.loss = tf.nn.sigmoid_cross_entropy_with_logits(pred, y)
        onehot_labels = tf.one_hot(self.y, depth=2)
        self.loss = tf.losses.softmax_cross_entropy(onehot_labels=onehot_labels, logits=hidden)
        return tf.reduce_mean(self.loss)
    
    def gen_input(self, data_x, data_y=None):
        feed_dict = {}
        feed_dict[self.X] = data_x
        if data_y is not None:    
            self.y_truth = data_y
            feed_dict[self.y] = data_y
        return feed_dict
    
    def errors(self, y_pred, y_truth=None):
        if y_truth is None:
            y_truth = self.y
            not_equal_counts = tf.abs(y_pred - y_truth)
            return tf.reduce_mean(not_equal_counts)
        else:
            not_equal_counts = abs(y_pred - y_truth)
            return np.mean(not_equal_counts)
```
```

### Define optimizer

> 
因为深度学习常见的是对于梯度的优化，也就是说，

优化器最后其实就是各种对于梯度下降算法的优化。


常见的优化器有 SGD，RMSprop，Adagrad，Adadelta，Adam 等，

此处实例中使用的是随机梯度下降（Stochastic gradient descent），

因为大多数机器学习任务就是最小化损失，在损失定义的情况下，后面的工作就交给优化器处理即可
```python
```python
def sgd_optimization(datasets, learning_rate=0.10, n_epochs=50, draw_freq=10):
    train_set_x, train_set_y = datasets[0]
    test_set_x,  test_set_y  = datasets[1]
    classifier = LogisticRegression(n_in=2, n_out=2)  # Classifier
    
    def get_model_train():
        with tf.name_scope('train'):
            y_pred, hidden = classifier.get_network()
            loss = classifier.get_loss(hidden)
            return y_pred, loss
    
    def get_model_test():
        with tf.name_scope('test'):
            y_pred, hidden = classifier.get_network()
            return y_pred
        
    train_output = get_model_train()  # y_pred, loss
    test_output = get_model_test()  # y_pred
    optimizer = tf.train.GradientDescentOptimizer(learning_rate).minimize(train_output[-1])
    
    init = tf.global_variables_initializer()
    sess = tf.Session()
    sess.run(init)
    
    def call_model(data_x, data_y=None, name=None):
        # generate data_y for placeholder while testing
        if data_y is None:
            data_y = np.zeros(data_x.shape[:-1])
            
        if name == 'test':
            ret = sess.run(  # return y_pred
                test_output,
                feed_dict=classifier.gen_input(data_x, data_y))
        else:  # name == 'train'
            _, ret = sess.run(  # return y_pred, loss
                [optimizer, train_output], 
                feed_dict=classifier.gen_input(data_x, data_y))
        return ret
    
    epoch = 0
    while epoch < n_epochs:
        # draw a figure every 'draw_freq' times
        if epoch % draw_freq == 0:
            # print(train_set_x, train_set_y)
            plot_decision_boundary(
                lambda x: call_model(x)[0], 
                train_set_x, train_set_y)
        
        # print error/cost per epoch
        train_pred, loss = call_model(
            train_set_x, train_set_y, 'train')
        train_error = classifier.errors(
            y_pred=train_pred, y_truth=train_set_y)
        
        test_pred = call_model(
            test_set_x,  test_set_y, 'test')
        test_error = classifier.errors(
            y_pred=test_pred, y_truth=test_set_y)

        print ("epoch is %d, train error %f, test error %f" % (
            epoch, train_error, test_error))
        epoch += 1

    # draw a figure at last        
    plot_decision_boundary(
        lambda x: call_model(x)[0], 
        train_set_x, train_set_y)
    sess.close()


def plot_decision_boundary(pred_func, train_set_x, train_set_y):
    # Draw figures as Matlab 
    x_min, x_max = train_set_x[:, 0].min() - .5, train_set_x[:, 0].max() + .5
    y_min, y_max = train_set_x[:, 1].min() - .5, train_set_x[:, 1].max() + .5
    h = 0.01
    xx, yy = np.meshgrid(np.arange(x_min, x_max, h), np.arange(y_min, y_max, h))
    grid_input = np.c_[xx.ravel(), yy.ravel()].reshape([-1, 2])
    Z = pred_func(grid_input)
    plt.contourf(xx, yy, Z.reshape(xx.shape), cmap=plt.cm.Spectral)
    plt.scatter(train_set_x[:, 0], train_set_x[:, 1], c=train_set_y, cmap=plt.cm.Spectral)
    plt.show()
```
```

### Start Training

> 
对于模型选择输入数据（data），设置参数（迭代次数，每N次绘制一次）


```python
```python
if __name__=="__main__":
    data = load_data('moons')
    sgd_optimization(data, learning_rate=0.1, n_epochs=200, draw_freq=25)
```
```

`dataset shape: (800, 2)`
![01](https://img-blog.csdn.net/20181007022541284?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29rY2QwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```
epoch is 0, train error 0.513333, test error 0.460000
epoch is 1, train error 0.513333, test error 0.165000
epoch is 2, train error 0.248333, test error 0.160000
epoch is 3, train error 0.201667, test error 0.175000
epoch is 4, train error 0.215000, test error 0.185000
epoch is 5, train error 0.206667, test error 0.185000
epoch is 6, train error 0.200000, test error 0.185000
epoch is 7, train error 0.200000, test error 0.185000
epoch is 8, train error 0.201667, test error 0.190000
epoch is 9, train error 0.203333, test error 0.190000
epoch is 10, train error 0.198333, test error 0.195000
epoch is 11, train error 0.198333, test error 0.195000
epoch is 12, train error 0.201667, test error 0.195000
epoch is 13, train error 0.201667, test error 0.195000
epoch is 14, train error 0.198333, test error 0.190000
epoch is 15, train error 0.198333, test error 0.190000
epoch is 16, train error 0.198333, test error 0.190000
epoch is 17, train error 0.195000, test error 0.190000
epoch is 18, train error 0.195000, test error 0.185000
epoch is 19, train error 0.195000, test error 0.185000
epoch is 20, train error 0.193333, test error 0.180000
epoch is 21, train error 0.191667, test error 0.180000
epoch is 22, train error 0.191667, test error 0.180000
epoch is 23, train error 0.191667, test error 0.180000
epoch is 24, train error 0.190000, test error 0.180000
epoch is 25, train error 0.191667, test error 0.175000
epoch is 26, train error 0.193333, test error 0.170000
epoch is 27, train error 0.193333, test error 0.170000
epoch is 28, train error 0.191667, test error 0.165000
epoch is 29, train error 0.191667, test error 0.165000
epoch is 30, train error 0.193333, test error 0.160000
epoch is 31, train error 0.193333, test error 0.160000
epoch is 32, train error 0.193333, test error 0.160000
epoch is 33, train error 0.193333, test error 0.160000
epoch is 34, train error 0.193333, test error 0.160000
epoch is 35, train error 0.193333, test error 0.160000
epoch is 36, train error 0.193333, test error 0.160000
epoch is 37, train error 0.195000, test error 0.160000
epoch is 38, train error 0.195000, test error 0.160000
epoch is 39, train error 0.195000, test error 0.155000
epoch is 40, train error 0.195000, test error 0.155000
epoch is 41, train error 0.195000, test error 0.155000
epoch is 42, train error 0.193333, test error 0.155000
epoch is 43, train error 0.190000, test error 0.160000
epoch is 44, train error 0.190000, test error 0.160000
epoch is 45, train error 0.188333, test error 0.160000
epoch is 46, train error 0.188333, test error 0.160000
epoch is 47, train error 0.188333, test error 0.155000
epoch is 48, train error 0.186667, test error 0.155000
epoch is 49, train error 0.186667, test error 0.155000
```

![03](https://img-blog.csdn.net/20181007022654799?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29rY2QwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```
epoch is 50, train error 0.185000, test error 0.165000
epoch is 51, train error 0.185000, test error 0.160000
epoch is 52, train error 0.190000, test error 0.155000
epoch is 53, train error 0.188333, test error 0.155000
epoch is 54, train error 0.190000, test error 0.155000
epoch is 55, train error 0.190000, test error 0.155000
epoch is 56, train error 0.191667, test error 0.160000
epoch is 57, train error 0.190000, test error 0.160000
epoch is 58, train error 0.191667, test error 0.155000
epoch is 59, train error 0.191667, test error 0.160000
epoch is 60, train error 0.191667, test error 0.160000
epoch is 61, train error 0.190000, test error 0.155000
epoch is 62, train error 0.188333, test error 0.155000
epoch is 63, train error 0.188333, test error 0.155000
epoch is 64, train error 0.188333, test error 0.155000
epoch is 65, train error 0.188333, test error 0.155000
epoch is 66, train error 0.188333, test error 0.155000
epoch is 67, train error 0.188333, test error 0.155000
epoch is 68, train error 0.188333, test error 0.155000
epoch is 69, train error 0.186667, test error 0.155000
epoch is 70, train error 0.185000, test error 0.155000
epoch is 71, train error 0.185000, test error 0.155000
epoch is 72, train error 0.183333, test error 0.155000
epoch is 73, train error 0.181667, test error 0.155000
epoch is 74, train error 0.181667, test error 0.155000
epoch is 75, train error 0.181667, test error 0.155000
epoch is 76, train error 0.183333, test error 0.145000
epoch is 77, train error 0.183333, test error 0.145000
epoch is 78, train error 0.185000, test error 0.150000
epoch is 79, train error 0.185000, test error 0.150000
epoch is 80, train error 0.183333, test error 0.150000
epoch is 81, train error 0.183333, test error 0.150000
epoch is 82, train error 0.183333, test error 0.150000
epoch is 83, train error 0.181667, test error 0.150000
epoch is 84, train error 0.181667, test error 0.150000
epoch is 85, train error 0.181667, test error 0.150000
epoch is 86, train error 0.181667, test error 0.150000
epoch is 87, train error 0.180000, test error 0.150000
epoch is 88, train error 0.180000, test error 0.150000
epoch is 89, train error 0.180000, test error 0.150000
epoch is 90, train error 0.180000, test error 0.150000
epoch is 91, train error 0.178333, test error 0.150000
epoch is 92, train error 0.178333, test error 0.150000
epoch is 93, train error 0.180000, test error 0.150000
epoch is 94, train error 0.176667, test error 0.150000
epoch is 95, train error 0.176667, test error 0.145000
epoch is 96, train error 0.176667, test error 0.145000
epoch is 97, train error 0.176667, test error 0.145000
epoch is 98, train error 0.175000, test error 0.145000
epoch is 99, train error 0.175000, test error 0.145000
```

![05](https://img-blog.csdn.net/20181007022710769?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29rY2QwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```
epoch is 100, train error 0.175000, test error 0.140000
epoch is 101, train error 0.175000, test error 0.140000
epoch is 102, train error 0.176667, test error 0.140000
epoch is 103, train error 0.178333, test error 0.140000
epoch is 104, train error 0.181667, test error 0.145000
epoch is 105, train error 0.180000, test error 0.145000
epoch is 106, train error 0.178333, test error 0.145000
epoch is 107, train error 0.178333, test error 0.145000
epoch is 108, train error 0.178333, test error 0.145000
epoch is 109, train error 0.178333, test error 0.145000
epoch is 110, train error 0.176667, test error 0.145000
epoch is 111, train error 0.178333, test error 0.145000
epoch is 112, train error 0.176667, test error 0.145000
epoch is 113, train error 0.175000, test error 0.145000
epoch is 114, train error 0.170000, test error 0.150000
epoch is 115, train error 0.170000, test error 0.150000
epoch is 116, train error 0.170000, test error 0.150000
epoch is 117, train error 0.170000, test error 0.150000
epoch is 118, train error 0.170000, test error 0.150000
epoch is 119, train error 0.170000, test error 0.150000
epoch is 120, train error 0.170000, test error 0.150000
epoch is 121, train error 0.170000, test error 0.150000
epoch is 122, train error 0.170000, test error 0.150000
epoch is 123, train error 0.170000, test error 0.150000
epoch is 124, train error 0.168333, test error 0.150000
epoch is 125, train error 0.171667, test error 0.135000
epoch is 126, train error 0.173333, test error 0.135000
epoch is 127, train error 0.173333, test error 0.135000
epoch is 128, train error 0.173333, test error 0.135000
epoch is 129, train error 0.173333, test error 0.140000
epoch is 130, train error 0.173333, test error 0.140000
epoch is 131, train error 0.170000, test error 0.140000
epoch is 132, train error 0.170000, test error 0.140000
epoch is 133, train error 0.173333, test error 0.145000
epoch is 134, train error 0.173333, test error 0.145000
epoch is 135, train error 0.170000, test error 0.145000
epoch is 136, train error 0.170000, test error 0.145000
epoch is 137, train error 0.170000, test error 0.145000
epoch is 138, train error 0.170000, test error 0.145000
epoch is 139, train error 0.168333, test error 0.145000
epoch is 140, train error 0.166667, test error 0.145000
epoch is 141, train error 0.166667, test error 0.145000
epoch is 142, train error 0.166667, test error 0.145000
epoch is 143, train error 0.166667, test error 0.145000
epoch is 144, train error 0.165000, test error 0.145000
epoch is 145, train error 0.163333, test error 0.145000
epoch is 146, train error 0.163333, test error 0.145000
epoch is 147, train error 0.165000, test error 0.145000
epoch is 148, train error 0.165000, test error 0.145000
epoch is 149, train error 0.165000, test error 0.145000
```

![07](https://img-blog.csdn.net/20181007022727891?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29rY2QwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```
epoch is 150, train error 0.170000, test error 0.135000
epoch is 151, train error 0.168333, test error 0.135000
epoch is 152, train error 0.168333, test error 0.135000
epoch is 153, train error 0.165000, test error 0.135000
epoch is 154, train error 0.165000, test error 0.135000
epoch is 155, train error 0.165000, test error 0.135000
epoch is 156, train error 0.160000, test error 0.135000
epoch is 157, train error 0.161667, test error 0.135000
epoch is 158, train error 0.161667, test error 0.135000
epoch is 159, train error 0.165000, test error 0.140000
epoch is 160, train error 0.166667, test error 0.140000
epoch is 161, train error 0.166667, test error 0.140000
epoch is 162, train error 0.168333, test error 0.145000
epoch is 163, train error 0.166667, test error 0.145000
epoch is 164, train error 0.165000, test error 0.145000
epoch is 165, train error 0.163333, test error 0.145000
epoch is 166, train error 0.163333, test error 0.145000
epoch is 167, train error 0.163333, test error 0.145000
epoch is 168, train error 0.163333, test error 0.145000
epoch is 169, train error 0.160000, test error 0.145000
epoch is 170, train error 0.160000, test error 0.145000
epoch is 171, train error 0.160000, test error 0.145000
epoch is 172, train error 0.160000, test error 0.145000
epoch is 173, train error 0.160000, test error 0.145000
epoch is 174, train error 0.160000, test error 0.145000
epoch is 175, train error 0.165000, test error 0.130000
epoch is 176, train error 0.163333, test error 0.135000
epoch is 177, train error 0.160000, test error 0.135000
epoch is 178, train error 0.160000, test error 0.135000
epoch is 179, train error 0.160000, test error 0.135000
epoch is 180, train error 0.158333, test error 0.135000
epoch is 181, train error 0.158333, test error 0.135000
epoch is 182, train error 0.156667, test error 0.135000
epoch is 183, train error 0.156667, test error 0.135000
epoch is 184, train error 0.158333, test error 0.135000
epoch is 185, train error 0.158333, test error 0.135000
epoch is 186, train error 0.158333, test error 0.135000
epoch is 187, train error 0.160000, test error 0.135000
epoch is 188, train error 0.160000, test error 0.135000
epoch is 189, train error 0.160000, test error 0.135000
epoch is 190, train error 0.158333, test error 0.140000
epoch is 191, train error 0.160000, test error 0.140000
epoch is 192, train error 0.160000, test error 0.145000
epoch is 193, train error 0.160000, test error 0.145000
epoch is 194, train error 0.160000, test error 0.145000
epoch is 195, train error 0.160000, test error 0.145000
epoch is 196, train error 0.160000, test error 0.145000
epoch is 197, train error 0.160000, test error 0.145000
epoch is 198, train error 0.160000, test error 0.145000
epoch is 199, train error 0.160000, test error 0.145000
```

![09](https://img-blog.csdn.net/20181007022741377?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29rY2QwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)








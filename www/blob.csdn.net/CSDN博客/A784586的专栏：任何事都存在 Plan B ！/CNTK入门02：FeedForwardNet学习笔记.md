# CNTK入门02：FeedForwardNet学习笔记 - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年05月09日 17:54:24[QuJack](https://me.csdn.net/A784586)阅读数：1428








**CNTK例子FeedForwardNet学习笔记**


例子目录：Tutorials\NumpyInterop\FeedForwardNet.py

In FeedForwardNet.py  we show a minimal example how to train a CNTK network using numpy arraysas
 input.


Python文件代码：


```python
# Copyright (c) Microsoft. All rights reserved.

# Licensed under the MIT license. See LICENSE.md file in the project root
# for full license information.
# ==============================================================================

import numpy as np
import sys
import os
from cntk.device import cpu, try_set_default_device
from cntk import Trainer
from cntk.learners import sgd, learning_rate_schedule, UnitType
from cntk.ops import input, sigmoid
from cntk.losses import cross_entropy_with_softmax
from cntk.metrics import classification_error
from cntk.logging import ProgressPrinter
#os.path.abspath(__file__)返回的是.py文件的绝对路径
#os.path.dirname 返回不带.py文件的目录；
#os.path.join 拼接路径
#sys.path.append 添加要搜索的目录
abs_path = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(abs_path, "..", "Examples", "common"))
from nn import fully_connected_classifier_net

# make sure we get always the same "randomness"
np.random.seed(0)
#sample_size 样例大小；feature_dim 特征维度；num_classes类别数；
def generate_random_data(sample_size, feature_dim, num_classes):
    # Create synthetic data using NumPy.
	#产生sample_size*1的矩阵；矩阵元素由low--high之间的随机整数填充；
    Y = np.random.randint(size=(sample_size, 1), low=0, high=num_classes)
    #print("Y:",Y)
    # Make sure that the data is separable
	#np.random.randn(row,col)从标准正态分布中返回一个或多个样本值,产生矩阵row*col;
    X = (np.random.randn(sample_size, feature_dim) + 3) * (Y + 1)
    #print(X)
    X = X.astype(np.float32)
    # converting class 0 into the vector "1 0 0",
    # class 1 into vector "0 1 0", ...
    class_ind = [Y == class_number for class_number in range(num_classes)]
    #print("class_ind",class_ind)
    Y = np.asarray(np.hstack(class_ind), dtype=np.float32)
    return X, Y
"""	
if __name__ == '__main__':
	xx,yy=generate_random_data(5,2,2)
	print(xx)
	print(yy)

"""
# Creates and trains a feedforward classification model
def ffnet():
    input_dim = 2#输入的数据维度；
    num_output_classes = 2#标签类别
    num_hidden_layers = 2#隐藏层数
    hidden_layers_dim = 50#隐藏层维度

    # Input variables denoting the features and label data
    feature = input((input_dim), np.float32)
    label = input((num_output_classes), np.float32)

    # Instantiate the feedforward classification model
	#初始化FFC前馈分类器模型
    netout = fully_connected_classifier_net(
        feature, num_output_classes, hidden_layers_dim, num_hidden_layers, sigmoid)

    ce = cross_entropy_with_softmax(netout, label)#交叉熵
    pe = classification_error(netout, label)#分类的度量

    lr_per_minibatch=learning_rate_schedule(0.5, UnitType.minibatch)
    # Instantiate the trainer object to drive the model training
    learner = sgd(netout.parameters, lr=lr_per_minibatch)#sgd随机梯度下降
    progress_printer = ProgressPrinter(128)
    trainer = Trainer(netout, (ce, pe), learner, progress_printer)

    # Get minibatches of training data and perform model training
    minibatch_size = 25
#训练
    for i in range(1024):
        features, labels = generate_random_data(
            minibatch_size, input_dim, num_output_classes)
        # Specify the mapping of input variables in the model to actual
        # minibatch data to be trained with
        trainer.train_minibatch({feature: features, label: labels})

    trainer.summarize_training_progress()
#测试
    test_features, test_labels = generate_random_data(
        minibatch_size, input_dim, num_output_classes)
    avg_error = trainer.test_minibatch(
        {feature: test_features, label: test_labels})
    return avg_error

if __name__ == '__main__':
    # Specify the target device to be used for computing, if you do not want to
    # use the best available one, e.g.
    # try_set_default_device(cpu())

    error = ffnet()
    print(" error rate on an unseen minibatch %f" % error)
```


该代码相关的另外一个文件nn中相关的代码：


```python
import numpy as np
from cntk.ops import *
from cntk.initializer import glorot_uniform, he_normal

#计算一次：偏向+权重乘积和
def linear_layer(input_var, output_dim):
    times_param = parameter(shape=(list(input_var.shape)+[output_dim]), init=glorot_uniform())
    bias_param = parameter(shape=(output_dim), init=0)

    t = times(input_var, times_param)
    return bias_param + t

#特征，隐藏层维度，非线性激活函数；
#通过激活函数，计算得到一值；
def fully_connected_layer(input, output_dim, nonlinearity):
    p = linear_layer(input, output_dim)
    return nonlinearity(p)

# Defines a multilayer feedforward classification model

#input特征，类别，隐藏层维度，隐藏层数，非线性函数sigmoid函数
def fully_connected_classifier_net(input, num_output_classes, hidden_layer_dim, num_hidden_layers, nonlinearity):
    r = fully_connected_layer(input, hidden_layer_dim, nonlinearity)
	#有多少隐藏层，循环计算多少次
    for i in range(1, num_hidden_layers):
        r = fully_connected_layer(r, hidden_layer_dim, nonlinearity)

    return linear_layer(r, num_output_classes)#返回没有通过激活函数的Ij
```







# MNIST数据集简介 - PeterBishop - CSDN博客





2018年12月15日 14:27:57[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：20
个人分类：[TensorFlow](https://blog.csdn.net/qq_40061421/article/category/8461142)









# MNIST Dataset Introduction

大多数示例使用手写数字的MNIST数据集。 该数据集包含60,000个用于培训的示例和10,000个用于测试的示例。 这些数字已经标准化，并以固定大小的图像（28x28像素）为中心，其值为0到1.为简单起见，每个图像都被展平并转换为784个特征（28 * 28）的一维 numpy数组）。

## Overview

![MNIST Digits](https://camo.githubusercontent.com/b06741b45df8ffe29c7de999ab2ec4ff6b2965ba/687474703a2f2f6e657572616c6e6574776f726b73616e64646565706c6561726e696e672e636f6d2f696d616765732f6d6e6973745f3130305f6469676974732e706e67)

## Usage

在我们的示例中，我们使用TensorFlow input_data.py脚本来加载该数据集。 它对于管理和处理我们的数据非常有用：
- 
数据集下载

- 
将整个数据集加载到numpy数组中：


```python
# Import MNIST
from tensorflow.examples.tutorials.mnist import input_data
mnist = input_data.read_data_sets("/tmp/data/", one_hot=True)

# Load data
X_train = mnist.train.images
Y_train = mnist.train.labels
X_test = mnist.test.images
Y_test = mnist.test.labels
```
- next_batch函数，可以遍历整个数据集并仅返回所需的数据集样本部分（以节省内存并避免加载整个数据集）。

```python
# Get the next 64 images array and labels
batch_X, batch_Y = mnist.train.next_batch(64)
```

Link: [http://yann.lecun.com/exdb/mnist/](http://yann.lecun.com/exdb/mnist/)




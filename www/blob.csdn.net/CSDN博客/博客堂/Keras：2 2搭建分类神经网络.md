# Keras：2.2搭建分类神经网络 - 博客堂 - CSDN博客





2017年08月26日 17:20:37[最小森林](https://me.csdn.net/u012052268)阅读数：712标签：[神经网络																[分类																[mnist																[keras](https://so.csdn.net/so/search/s.do?q=keras&t=blog)
个人分类：[深度学习](https://blog.csdn.net/u012052268/article/category/6969928)







- [Keras22搭建分类神经网络Classifier](#keras22搭建分类神经网络classifier)- [任务综述](#0-任务综述)
- [数据预处理](#1-数据预处理)
- [建立神经网络](#2-建立神经网络)- [1网络结构](#21网络结构)
- [2优化器](#22优化器)
- [3激活模型](#23激活模型)

- [训练网络](#3-训练网络)
- [测试模型](#4-测试模型)
- [代码总结带自己注释](#5-代码总结带自己注释)





# Keras：2.2搭建分类神经网络Classifier

## 0 任务综述

今天用 Keras 来构建一个分类神经网络，用到的数据集是 MNIST，就是 0 到 9 这几个数字的图片数据集。

如图： 
![image](http://wiki.jikexueyuan.com/project/tensorflow-zh/images/MNIST.png)
Keras 自身就有 MNIST 这个数据包，再分成训练集和测试集。x 是一张张图片，y 是每张图片对应的标签，即它是哪个数字。

下面开始任务：首先是导入必要的包

```python
import numpy as np
np.random.seed(1337)  # for reproducibility
from keras.datasets import mnist
from keras.utils import np_utils
from keras.models import Sequential
from keras.layers import Dense, Activation
from keras.optimizers import RMSprop
```
- models.Sequential，用来一层一层一层的去建立神经层；
- layers.Dense 意思是这个神经层是全连接层。
- layers.Activation 激励函数。
- optimizers.RMSprop 优化器采用 - RMSprop，加速神经网络训练方法。

## 1 数据预处理

首先装载需要的数据：

```
# download the mnist to the path '~/.keras/datasets/' 如果第一次运行，程序会自动去网上下载

(X_train, y_train), (X_test, y_test) = mnist.load_data()

# 数据的格式： X shape (60,000 28x28), y shape (10,000)
```

但是我们想预处理数据，使之更好用。 

1. 对于X来说：

我们将其3维变2维（(60000,28,28)变(60000，784)）：X_train.reshape(X_train.shape[0], -1)

（这里-1的意思是：懒的写维度，np自己计算去）

再归一化：/ 255.
- 对于Y来说

我们将其1x1维，变为1x10维。one-hot

数据预处理代码如下：

```python
# data pre-processing
X_train = X_train.reshape(X_train.shape[0], -1) / 255.   # normalize
X_test = X_test.reshape(X_test.shape[0], -1) / 255.      # normalize
y_train = np_utils.to_categorical(y_train, num_classes=10)
y_test = np_utils.to_categorical(y_test, num_classes=10)

print(X_train[1].shape)
"""
(784,)
"""

print(y_train[:3])
"""
[[ 0.  0.  0.  0.  0.  1.  0.  0.  0.  0.]
 [ 1.  0.  0.  0.  0.  0.  0.  0.  0.  0.]
 [ 0.  0.  0.  0.  1.  0.  0.  0.  0.  0.]]
"""
```

## 2 建立神经网络

### 2.1网络结构

在回归网络中用到的是 model.add 一层一层添加神经层，今天的方法是直接在模型的里面加多个神经层。好比一个水管，一段一段的，数据是从上面一段掉到下面一段，再掉到下面一段。

第一段就是加入 Dense 神经层。32 是输出的维度，784 是输入的维度。 第一层传出的数据有 32 个 feature，传给激励单元，激励函数用到的是 relu 函数。 经过激励函数之后，就变成了非线性的数据。 

然后再把这个数据传给下一个神经层，这个 Dense 我们定义它有 10 个输出的 feature。同样的，此处不需要再定义输入的维度，因为它接收的是上一层的输出。 接下来再输入给下面的 softmax 函数，用来分类。

```
# 另外一种创建模型的方法
model = Sequential([
    Dense(32, input_dim=784),
    Activation('relu'),
    Dense(10),
    Activation('softmax'),
])
```

### 2.2优化器

接下来用 RMSprop 作为优化器，它的参数包括学习率等，可以通过修改这些参数来看一下模型的效果。

```
# Another way to define your optimizer
rmsprop = RMSprop(lr=0.001, rho=0.9, epsilon=1e-08, decay=0.0)
```

### 2.3激活模型

接下来用 model.compile 激励神经网络。

优化器，可以是默认的，也可以是我们在上一步定义的。 损失函数，分类和回归问题的不一样，用的是交叉熵。 metrics，里面可以放入需要计算的 cost，accuracy，score 等。

```
# We add metrics to get more results you want to see
# 我们添加指标以获得更多您想要查看的结果

model.compile(optimizer=rmsprop,
              loss='categorical_crossentropy',
              metrics=['accuracy'])
```

## 3 训练网络

这里用到的是 fit 函数，把训练集的 x 和 y 传入之后，epoch，训练的轮数， 表示把整个数据训练多少次，batch_size 每批处理32个。

```python
print('Training ------------')
# Another way to train the model
model.fit(X_train, y_train, epoch=2, batch_size=32)

"""
Training ------------
Epoch 1/2
60000/60000 [==============================] - 2s - loss: 0.3506 - acc: 0.9025     
Epoch 2/2
60000/60000 [==============================] - 2s - loss: 0.1995 - acc: 0.9421   
"""
```

## 4 测试模型

接下来就是用测试集来检验一下模型，方法和回归网络中是一样的，运行代码之后，可以输出 accuracy 和 loss。

```python
print('\nTesting ------------')
# Evaluate the model with the metrics we defined earlier
loss, accuracy = model.evaluate(X_test, y_test)

print('test loss: ', loss)
print('test accuracy: ', accuracy)

"""
Testing ------------
 9760/10000 [============================>.] - ETA: 0s

test loss:  0.1724540345
test accuracy:  0.9489
"""
```

## 5 代码总结（带自己注释）

代码光看不行，必须自己敲一遍：

```python
import numpy as np
np.random.seed(1337)  # 让每次的随机数相似
from keras.datasets import mnist # 数据集合
from keras.utils import np_utils # keras自带的numpy
from keras.models import Sequential
from keras.layers import Dense, Activation
from keras.optimizers import RMSprop

## 1 数据预处理
# download the mnist to the path '~/.keras/datasets/' if it is the first time to be called
# X shape (60,000 28x28), y shape (10,000, )
(X_train, y_train), (X_test, y_test) = mnist.load_data()

# 我们将X3维变2维
X_train = X_train.reshape(X_train.shape[0],-1)/255.
X_test = X_test.reshape(X_test.shape[0],-1)/255.
# 将Y进行one-hot化
y_train = np_utils.to_categorical(y_train,10)
y_test = np_utils.to_categorical(y_test,10)
print(X_train.shape)
print(y_train[:2])

## 2 建立模型
model = Sequential([
    Dense(32,input_dim=784),
    Activation('relu'),
    Dense(10),
    Activation('softmax'),
])
# 自己定制 优化器
rmsprop = RMSprop(lr=0.001,rho=0.9,epsilon=1e-8,decay=0.0)
# 激活神经网络
model.compile(optimizer=rmsprop,
              loss='categorical_crossentropy',
              metrics=['accuracy'])

## 3 训练模型
print('Training----------------')
model.fit(X_train,y_train,epochs=2,batch_size=32)

## 4测试模型
print('\ntesting---------------------')
loss,accuracy = model.evaluate(X_test,y_test)

print('test loss:',loss)
print('test accuracy:',accuracy)
```](https://so.csdn.net/so/search/s.do?q=mnist&t=blog)](https://so.csdn.net/so/search/s.do?q=分类&t=blog)](https://so.csdn.net/so/search/s.do?q=神经网络&t=blog)





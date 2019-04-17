# Keras：2.3 keras实现卷积神经网络 - 博客堂 - CSDN博客





2017年10月07日 15:17:07[最小森林](https://me.csdn.net/u012052268)阅读数：1279










- [3 keras实现卷积神经网络](#23-keras实现卷积神经网络)- [导入数据](#1-导入数据)
- [构造模型](#2-构造模型)
- [训练](#3-训练)
- [所有代码](#4-所有代码)





# 2.3 keras实现卷积神经网络

CNN（Convolutional Neural Networks）卷积神经网络在 keras 上的代码实现。 用到的数据集还是MNIST。不同的是这次用到的层比较多，导入的模块也相应增加了一些。

下面直接进入代码部分：

## 1 导入数据

```python
import numpy as np
np.random.seed(1337)  # for reproducibility
from keras.datasets import mnist
from keras.utils import np_utils
from keras.models import Sequential
from keras.layers import Dense, Activation, Convolution2D, MaxPooling2D, Flatten
from keras.optimizers import Adam

# download the mnist to the path '~/.keras/datasets/' if it is the first time to be called
# X shape (60,000 28x28), y shape (10,000, )
(X_train, y_train), (X_test, y_test) = mnist.load_data()
```

数据预处理

```python
# data pre-processing
X_train = X_train.reshape(-1, 1,28, 28)/255.
X_test = X_test.reshape(-1, 1,28, 28)/255.
y_train = np_utils.to_categorical(y_train, num_classes=10)
y_test = np_utils.to_categorical(y_test, num_classes=10)
```

## 2 构造模型

开始构造模型

```
# Another way to build your CNN
model = Sequential()
```

添加第一层卷基层

```python
# Conv layer 1 output shape (32, 28, 28)
model.add(Convolution2D(
    batch_input_shape=(64, 1, 28, 28),
    filters=32, 
    kernel_size=5,  # 滤波器边长
    strides=1,      # 步数
    padding='same',     # Padding method
    data_format='channels_first', # 以128x128的RGB图像为例，“channels_first”应将数据组织为（3,128,128），而“channels_last”应将数据组织为（128,128,3）。
))

model.add(Activation('relu'))
```

第一层 pooling（池化，下采样），分辨率长宽各降低一半，输出数据shape为（32，14，14）

```
# Pooling layer 1 (max pooling) output shape (32, 14, 14)
model.add(MaxPooling2D(
    pool_size=2,
    strides=2,
    padding='same',    # Padding method
    data_format='channels_first',
))
```

第二层卷积和第二层pooling

```
# Conv layer 2 output shape (64, 14, 14)
model.add(Convolution2D(64, 5, strides=1, padding='same', data_format='channels_first'))
model.add(Activation('relu'))

# Pooling layer 2 (max pooling) output shape (64, 7, 7)
model.add(MaxPooling2D(2, 2, 'same', data_format='channels_first'))
```

经过以上处理之后数据shape为（64，7，7），需要将数据抹平成一维，再添加全连接层1

```
model.add(Flatten())
model.add(Dense(1024))
model.add(Activation('relu'))
```

添加全连接层2（即输出层）

```
model.add(Dense(10))
model.add(Activation('softmax'))
```

## 3 训练

设置adam优化方法，loss函数, metrics方法来观察输出结果

```bash
model.compile(optimizer=adam,
              loss='categorical_crossentropy',
              metrics=['accuracy'])
```

开始训练模型
`model.fit(X_train, y_train, epoch=1, batch_size=32,)`
输出test的loss和accuracy结果

![image](https://morvanzhou.github.io/static/results/keras/2-3-1.png)

## 4 所有代码：




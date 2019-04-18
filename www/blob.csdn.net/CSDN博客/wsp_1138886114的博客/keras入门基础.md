# keras入门基础 - wsp_1138886114的博客 - CSDN博客





2018年10月24日 14:25:25[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：77











### 文章目录
- [1. 关于Keras](#1_Keras_3)
- [2. Keras的模块结构](#2_Keras_16)
- [3. 使用Keras搭建一个神经网络](#3_Keras_19)
- [4. 主要概念](#4__22)
- [5. 第一个示例](#5__52)
- [6. 第二个示例](#6__151)



关于Keras 可查看官方文档：[https://keras-cn.readthedocs.io/en/latest/](https://keras-cn.readthedocs.io/en/latest/)

转载自 [https://blog.csdn.net/zdy0_2004/article/details/74736656](https://blog.csdn.net/zdy0_2004/article/details/74736656)


#### 1. 关于Keras

        1）简介          

         Keras是由纯python编写的基于theano/tensorflow的深度学习框架。

         Keras是一个高层神经网络API，支持快速实验，能够把你的idea迅速转换为结果，如果有如下需求，可以优先选择Keras：

                a）简易和快速的原型设计（keras具有高度模块化，极简，和可扩充特性）

                b）支持CNN和RNN，或二者的结合

c）无缝CPU和GPU切换

         2）设计原则

               a）用户友好：Keras是为人类而不是天顶星人设计的API。用户的使用体验始终是我们考虑的首要和中心内容。Keras遵循减少认知困难的最佳实践：Keras提供一致而简洁的API， 能够极大减少一般应用下用户的工作量，同时，Keras提供清晰和具有实践意义的bug反馈。

               b）模块性：模型可理解为一个层的序列或数据的运算图，完全可配置的模块可以用最少的代价自由组合在一起。具体而言，网络层、损失函数、优化器、初始化策略、激活函数、正则化方法都是独立的模块，你可以使用它们来构建自己的模型。

              c）易扩展性：添加新模块超级容易，只需要仿照现有的模块编写新的类或函数即可。创建新模块的便利性使得Keras更适合于先进的研究工作。

              d）与Python协作：Keras没有单独的模型配置文件类型（作为对比，caffe有），模型由python代码描述，使其更紧凑和更易debug，并提供了扩展的便利性。

#### 2. Keras的模块结构

![](http://images2015.cnblogs.com/blog/1119747/201707/1119747-20170707133635659-888158147.png)

#### 3. 使用Keras搭建一个神经网络

![](http://images2015.cnblogs.com/blog/1119747/201707/1119747-20170707133932722-715494711.png)

#### 4. 主要概念

   1）符号计算

        Keras的底层库使用Theano或TensorFlow，这两个库也称为Keras的后端。无论是Theano还是TensorFlow，都是一个“符号式”的库。符号计算首先定义各种变量，然后建立一个“计算图”,计算图规定了各个变量之间的计算关系。

       符号计算也叫数据流图，其过程如下(gif图不好打开，所以用了静态图，数据是按图中黑色带箭头的线流动的)：

![](http://images2015.cnblogs.com/blog/1119747/201707/1119747-20170707135253581-1586562235.png)

     2）张量

          张量(tensor)，可以看作是向量、矩阵的自然推广，用来表示广泛的数据类型。张量的阶数也叫维度。

          0阶张量,即标量,是一个数。

          1阶张量,即向量,一组有序排列的数

          2阶张量,即矩阵,一组向量有序的排列起来

          3阶张量，即立方体，一组矩阵上下排列起来

          4阶张量......

          依次类推

        重点：关于维度的理解

          假如有一个10长度的列表，那么我们横向看有10个数字，也可以叫做10维度，纵向看只能看到1个数字，那么就叫1维度。注意这个区别有助于理解Keras或者神经网络中计算时出现的维度问题。

    3）数据格式(data_format)

        目前主要有两种方式来表示张量：

        a) th模式或channels_first模式，Theano和caffe使用此模式。

        b）tf模式或channels_last模式，TensorFlow使用此模式。


        下面举例说明两种模式的区别：

         对于100张RGB3通道的16×32（高为16宽为32）彩色图，

         th表示方式：（100,3,16,32）

         tf表示方式：（100,16,32,3）

         唯一的区别就是表示通道个数3的位置不一样。
     4）模型

          Keras有两种类型的模型，序贯模型（Sequential）和函数式模型（Model），函数式模型应用更为广泛，序贯模型是函数式模型的一种特殊情况。

          a）序贯模型（Sequential):单输入单输出，一条路通到底，层与层之间只有相邻关系，没有跨层连接。这种模型编译速度快，操作也比较简单

          b）函数式模型（Model）：多输入多输出，层与层之间任意连接。这种模型编译速度慢。
#### 5. 第一个示例

          这里也采用介绍神经网络时常用的一个例子：手写数字的识别。

          在写代码之前，基于这个例子介绍一些概念，方便大家理解。

       PS：可能是版本差异的问题，官网中的参数和示例中的参数是不一样的，官网中给出的参数少，并且有些参数支持，有些不支持。所以此例子去掉了不支持的参数，并且只介绍本例中用到的参数。

          1）Dense(500,input_shape=(784,))

               a）Dense层属于网络层-->常用层中的一个层

               b） 500表示输出的维度，完整的输出表示：(*,500)：即输出任意个500维的数据流。但是在参数中只写维度就可以了，比较具体输出多少个是有输入确定的。换个说法，Dense的输出其实是个N×500的矩阵。

              c）input_shape(784,) 表示输入维度是784(28×28，后面具体介绍为什么)，完整的输入表示：(*,784)：即输入N个784维度的数据

         2）Activation('tanh')

              a）Activation：激活层

              b）'tanh' ：激活函数

         3）Dropout(0.5)

              在训练过程中每次更新参数时随机断开一定百分比（rate）的输入神经元，防止过拟合。

         4）数据集

           数据集包括60000张28×28的训练集和10000张28×28的测试集及其对应的目标数字。如果完全按照上述数据格式表述，以tensorflow作为后端应该是（60000,28,28,3），因为示例中采用了mnist.load_data()获取数据集，所以已经判断使用了tensorflow作为后端，因此数据集就变成了(60000,28,28),那么input_shape(784,)应该是input_shape(28,28，)才对，但是在这个示例中这么写是不对的，需要转换成(60000,784),才可以。为什么需要转换呢？

![](http://images2015.cnblogs.com/blog/1119747/201707/1119747-20170707145150519-637435869.png)

              如上图，训练集(60000,28,28)作为输入，就相当于一个立方体，而输入层从当前角度看就是一个平面，立方体的数据流怎么进入平面的输入层进行计算呢？所以需要进行黄色箭头所示的变换，然后才进入输入层进行后续计算。至于从28*28变换成784之后输入层如何处理，就不需要我们关心了。(喜欢钻研的同学可以去研究下源代码)。

         并且，Keras中输入多为(nb_samples, input_dim)的形式：即(样本数量，输入维度)。

        
5) 代码示例


```python
from keras.models import Sequential
from keras.layers.core import Dense, Dropout, Activation
from keras.optimizers import SGD
from keras.datasets import mnist
import numpy as np


'''
    第一步：选择模型
'''
model = Sequential()
'''
   第二步：构建网络层
'''
model.add(Dense(500,input_shape=(784,)))  # 输入层，28*28=784
model.add(Activation('tanh'))             # 激活函数是tanh
model.add(Dropout(0.5))                   # 采用50%的dropout

model.add(Dense(500))                     # 隐藏层节点500个
model.add(Activation('tanh'))
model.add(Dropout(0.5))

model.add(Dense(10))                      # 输出结果是10个类别，所以维度是10
model.add(Activation('softmax'))          # 最后一层用softmax作为激活函数

'''
   第三步：编译
'''
sgd = SGD(lr=0.01, decay=1e-6,
          momentum=0.9, nesterov=True)    # 优化函数，设定学习率（lr）等参数
model.compile(loss='categorical_crossentropy',
              optimizer=sgd)   # 使用交叉熵作为loss函数

'''
   第四步：训练
   .fit的一些参数
   batch_size：对总的样本数进行分组，每组包含的样本数量
   epochs ：训练次数
   shuffle：是否把数据随机打乱之后再进行训练
   validation_split：拿出百分之多少用来做交叉验证
   verbose：屏显模式 0：不输出  1：输出进度  2：输出每次的训练结果
'''
(X_train, y_train), (X_test, y_test) = mnist.load_data() 
# 使用Keras自带的mnist工具读取数据（第一次需要联网）
# 由于mist的输入数据维度是(num, 28, 28)，这里需要把后面的维度直接拼起来变成784维  
X_train = X_train.reshape(X_train.shape[0], X_train.shape[1] * X_train.shape[2]) 
X_test = X_test.reshape(X_test.shape[0], X_test.shape[1] * X_test.shape[2])  
Y_train = (np.arange(10) == y_train[:, None]).astype(int)
Y_test = (np.arange(10) == y_test[:, None]).astype(int)

model.fit(X_train,Y_train,batch_size=200,epochs=50,shuffle=True,verbose=0,validation_split=0.3)
model.evaluate(X_test, Y_test, batch_size=200, verbose=0)

'''
    第五步：输出
'''
print("test set")
scores = model.evaluate(X_test,Y_test,batch_size=200,verbose=0)
print("")
print("The test loss is %f" % scores)
result = model.predict(np.asarray(X_test),batch_size=200,verbose=0)

result_max = np.argmax(result, axis = 1)
test_max = np.argmax(Y_test, axis = 1)

result_bool = np.equal(result_max, test_max)
true_num = np.sum(result_bool)
print("")
print("The accuracy of the model is :",true_num/(len(result_bool)*1.0))
```

上述代码运行可能会出现数据集下载报错，多运行几次可以解决。

数据集大小约11M

运行结果：
```python
The test loss is 0.246470
The accuracy of the model is : 0.9248
```

#### 6. 第二个示例

这次采用激活函数：relu，优化器采用;RMSprop,其他略有改动

```python
from __future__ import print_function
import keras
from keras.datasets import mnist
from keras.models import Sequential
from keras.layers import Dense, Dropout
from keras.optimizers import RMSprop

batch_size = 128
num_classes = 10
epochs = 20

"""
    一、数据预处理
"""
# 数据读取： shuffled and split between train and test sets
(x_train, y_train), (x_test, y_test) = mnist.load_data()
x_train = x_train.reshape(60000, 784)
x_test = x_test.reshape(10000, 784)

x_train = x_train.astype('float32')
x_test = x_test.astype('float32')

# 输入的 x 变成 60,000*784 的数据，标准化到【0，1】
x_train /= 255
x_test /= 255
print(x_train.shape[0], 'train samples')
print(x_test.shape[0], 'test samples')

y_train = keras.utils.to_categorical(y_train, num_classes)
y_test = keras.utils.to_categorical(y_test, num_classes)


"""
    二、创建模型并训练、预测
"""
model = Sequential()  # 序贯式模型
model.add(Dense(512, activation='relu', input_shape=(784,)))
model.add(Dropout(0.2))
model.add(Dense(512, activation='relu'))
model.add(Dropout(0.2))
model.add(Dense(10, activation='softmax'))

model.summary()  # 打印出模型概况

# compile the model
model.compile(loss='categorical_crossentropy',  # 对数损失
              optimizer=RMSprop(),
              metrics=['accuracy'])

# train the model
history = model.fit(x_train, y_train,
                    batch_size=batch_size,
                    epochs=epochs,
                    verbose=1,  # 日志显示
                    validation_data=(x_test, y_test))  # fit将模型训练epochs轮

# test the model
score = model.evaluate(x_test, y_test, verbose=0)  # evaluate函数按batch计算在某些输入数据上模型的误差

print('Test loss:', score[0])
print('Test accuracy:', score[1])

输出：
Using TensorFlow backend.
60000 train samples
10000 test samples
_________________________________________________________________
Layer (type)                 Output Shape              Param #
=================================================================
dense_1 (Dense)              (None, 512)               401920
_________________________________________________________________
dropout_1 (Dropout)          (None, 512)               0
_________________________________________________________________
dense_2 (Dense)              (None, 512)               262656
_________________________________________________________________
dropout_2 (Dropout)          (None, 512)               0
_________________________________________________________________
dense_3 (Dense)              (None, 10)                5130
=================================================================
Total params: 669,706
Trainable params: 669,706
Non-trainable params: 0
_________________________________________________________________
Train on 60000 samples, validate on 10000 samples

60000/60000 [==============================] - 18s 301us/step - loss: 0.2434 - acc: 0.9237 - val_loss: 0.1479 - val_acc: 0.9537
Epoch 2/20
60000/60000 [==============================] - 15s 254us/step - loss: 0.1027 - acc: 0.9691 - val_loss: 0.0794 - val_acc: 0.9759
Epoch 3/20
60000/60000 [==============================] - 16s 262us/step - loss: 0.0771 - acc: 0.9768 - val_loss: 0.0760 - val_acc: 0.9779
Epoch 4/20
60000/60000 [==============================] - 16s 265us/step - loss: 0.0608 - acc: 0.9819 - val_loss: 0.0724 - val_acc: 0.9799
Epoch 5/20
60000/60000 [==============================] - 17s 275us/step - loss: 0.0510 - acc: 0.9849 - val_loss: 0.0777 - val_acc: 0.9794
Epoch 6/20
60000/60000 [==============================] - 16s 262us/step - loss: 0.0447 - acc: 0.9868 - val_loss: 0.0711 - val_acc: 0.9811
Epoch 7/20
60000/60000 [==============================] - 16s 262us/step - loss: 0.0386 - acc: 0.9887 - val_loss: 0.0852 - val_acc: 0.9793
Epoch 8/20
60000/60000 [==============================] - 16s 264us/step - loss: 0.0344 - acc: 0.9899 - val_loss: 0.0967 - val_acc: 0.9812
Epoch 9/20
60000/60000 [==============================] - 16s 273us/step - loss: 0.0347 - acc: 0.9902 - val_loss: 0.0846 - val_acc: 0.9815
Epoch 10/20
60000/60000 [==============================] - 15s 247us/step - loss: 0.0269 - acc: 0.9922 - val_loss: 0.0926 - val_acc: 0.9821
Epoch 11/20
60000/60000 [==============================] - 16s 264us/step - loss: 0.0280 - acc: 0.9919 - val_loss: 0.0865 - val_acc: 0.9827
Epoch 12/20
60000/60000 [==============================] - 15s 244us/step - loss: 0.0257 - acc: 0.9926 - val_loss: 0.0961 - val_acc: 0.9817
Epoch 13/20
60000/60000 [==============================] - 15s 256us/step - loss: 0.0237 - acc: 0.9928 - val_loss: 0.1064 - val_acc: 0.9815
Epoch 14/20
60000/60000 [==============================] - 16s 261us/step - loss: 0.0223 - acc: 0.9939 - val_loss: 0.1030 - val_acc: 0.9823
Epoch 15/20
60000/60000 [==============================] - 16s 261us/step - loss: 0.0220 - acc: 0.9942 - val_loss: 0.1001 - val_acc: 0.9825
Epoch 16/20
60000/60000 [==============================] - 16s 274us/step - loss: 0.0210 - acc: 0.9948 - val_loss: 0.1117 - val_acc: 0.9809
Epoch 17/20
60000/60000 [==============================] - 15s 257us/step - loss: 0.0192 - acc: 0.9946 - val_loss: 0.1027 - val_acc: 0.9833
Epoch 18/20
60000/60000 [==============================] - 15s 255us/step - loss: 0.0169 - acc: 0.9953 - val_loss: 0.1150 - val_acc: 0.9828
Epoch 19/20
60000/60000 [==============================] - 16s 275us/step - loss: 0.0164 - acc: 0.9953 - val_loss: 0.1319 - val_acc: 0.9801
Epoch 20/20
60000/60000 [==============================] - 14s 239us/step - loss: 0.0192 - acc: 0.9951 - val_loss: 0.1148 - val_acc: 0.9805
Test loss: 0.1148101102249917
Test accuracy: 0.9805
```








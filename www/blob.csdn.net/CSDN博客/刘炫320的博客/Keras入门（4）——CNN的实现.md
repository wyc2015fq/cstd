# Keras入门（4）——CNN的实现 - 刘炫320的博客 - CSDN博客
2018年01月25日 11:20:48[刘炫320](https://me.csdn.net/qq_35082030)阅读数：7054
# 0. 写在前面
之前，我们把环境和HelloWorld都进行了详细的介绍。接下来，我们要迎接第一个真正意义上的程序了，上一节中，我们使用了多层感知机来实现了最基本的神经网络模型，下面我们才进入最经典的卷积神经网络模型的构建。
对于一个可执行的神经网络模型，那么它应当具有以下几个步骤：
- 数据预处理
- 数据输入
- 模型构建
- 模型编译
- 模型训练与评估
- 模型的可视化
当然，如果你还希望它能应用到其他系统之中，那么可能需要另外的3个步骤：
- 模型保存
- 模型加载
- 模型预测
本文将顺着这个顺序来进行讲解，因此我们这里不赘述数据预处理和数据输入，如果喜欢numpy的可以参考《[numpy读取文件为列表](http://blog.csdn.net/heyijia0327/article/details/42506063)》和《[numpy文件存取](https://www.cnblogs.com/dmir/p/5009075.html)》；而如果是喜欢Pandas的，请看某小皮的《[pandas数据输入输出](http://blog.csdn.net/pipisorry/article/details/52208727)》。
# 2.CNN
我们从模型的构建开始说起，我们在第二讲中，讲述了一个最简单的模型，多层感知机（MLP），那么这里我们讲解另一个深度学习系统中的常用的模型CNN。
## 2.1 CNN实例
下面就是一个CNN的典型例子，他就是对一个$100\times 100\times 3$的图片进行一个10分类的问题。
```python
import numpy as np
import keras
from keras.models import Sequential
from keras.layers import Dense, Dropout, Flatten
from keras.layers import Conv2D, MaxPooling2D
from keras.optimizers import SGD
#这一行新加的，用于导入绘图包
from keras.utils import plot_model
# 生成数据
#生成100张图片，每张图片100*100大小，是3通道的。
x_train = np.random.random((100, 100, 100, 3))
y_train = keras.utils.to_categorical(np.random.randint(10, size=(100, 1)), num_classes=10)
x_test = np.random.random((20, 100, 100, 3))
y_test = keras.utils.to_categorical(np.random.randint(10, size=(20, 1)), num_classes=10)
model = Sequential()
#一层卷积层，包含了32个卷积核，大小为3*3
model.add(Conv2D(32, (3, 3), activation='relu', input_shape=(100, 100, 3)))
model.add(Conv2D(32, (3, 3), activation='relu'))
#一个最大池化层，池化大小为2*2
model.add(MaxPooling2D(pool_size=(2, 2)))
#遗忘层，遗忘速率为0.25
model.add(Dropout(0.25))
#添加一个卷积层，包含64个卷积和，每个卷积和仍为3*3
model.add(Conv2D(64, (3, 3), activation='relu'))
model.add(Conv2D(64, (3, 3), activation='relu'))
#来一个池化层
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Dropout(0.25))
#压平层
model.add(Flatten())
#来一个全连接层
model.add(Dense(256, activation='relu'))
#来一个遗忘层
model.add(Dropout(0.5))
#最后为分类层
model.add(Dense(10, activation='softmax'))
sgd = SGD(lr=0.01, decay=1e-6, momentum=0.9, nesterov=True)
model.compile(loss='categorical_crossentropy', optimizer=sgd)
model.fit(x_train, y_train, batch_size=32, epochs=10)
score = model.evaluate(x_test, y_test, batch_size=32)
#这一行新加的，用于绘图
plot_model(model, to_file='modelcnn.png',show_shapes=True)
model.save("CNN.model")
```
这里，我们主要讲解整个框架内的各个层。
## 2.2 卷积层
卷积是深度学习中的主力，最开始的时候深度学习除了全链接层外，最重要的就是卷积了。卷积分为一维卷积，二维卷积和三维卷积。
### 一维卷积
一维卷积的函数声明为：
```python
keras.layers.convolutional.Conv1D(
filters, #卷积核的数目（即输出的维度）
kernel_size,#整数或由单个整数构成的list/tuple，卷积核的空域或时域窗长度 
strides=1, #整数或由单个整数构成的list/tuple，为卷积的步长。任何不为1的strides均与任何不为1的dilation_rate均不兼容
padding='valid', #补0策略，为“valid”, “same” 或“causal”，
#“causal”这是1维卷积独有的，它一次卷积只会卷积一个新样本，其余的均为之前的样本，尤其是边缘时，则需要补卷积长度-1个0。目的是为了保证时序性，也就是你不能看到未来发生的事情才对。
#“valid”这是说只从边界开始卷积，不进行补0。
#“same”对边界也进行补0，但是保证输入维度与输出维度相同。
dilation_rate=1, #整数或由单个整数构成的list/tuple，指定dilated convolution中的膨胀比例。任何不为1的dilation_rate均与任何不为1的strides均不兼容。
activation=None, #激活函数，为预定义的激活函数名（参考激活函数），或逐元素（element-wise）的Theano函数。如果不指定该参数，将不会使用任何激活函数（即使用线性激活函数：a(x)=x）
use_bias=True, #布尔值，是否使用偏置项
kernel_initializer='glorot_uniform', #权值初始化方法，为预定义初始化方法名的字符串，或用于初始化权重的初始化器。
bias_initializer='zeros', #权值初始化方法，为预定义初始化方法名的字符串，或用于初始化权重的初始化器。
kernel_regularizer=None, #施加在权重上的正则项
bias_regularizer=None, #施加在偏置向量上的正则项
activity_regularizer=None, #施加在输出上的正则项
kernel_constraint=None, #施加在权重上的约束项
bias_constraint=None#施加在偏置上的约束项
)
```
对于卷积中的Padding我做了进一步的说明，因为原文的说明实在是搞不懂。
### 二维卷积
二维卷积则要熟悉的多了，其函数声明为：
```python
keras.layers.convolutional.Conv2D(
filters,
kernel_size,
strides=(1, 1),#单个整数或由两个整数构成的list/tuple，卷积核的宽度和长度。如为单个整数，则表示在各个空间维度的相同长度。
padding='valid',
data_format=None, #字符串，“channels_first”或“channels_last”之一，代表图像的通道维的位置。
#“channels_last”对应原本的“tf”，
#“channels_first”对应原本的“th”。
#以128x128的RGB图像为例，“channels_first”应将数据组织为（3,128,128），而“channels_last”应将数据组织为（128,128,3）。
#该参数的默认值是~/.keras/keras.json中设置的值，若从未设置过，则为“channels_last”。
dilation_rate=(1, 1), #单个整数或由两个整数构成的list/tuple，为卷积的步长。如为单个整数，则表示在各个空间维度的相同步长。任何不为1的strides均与任何不为1的dilation_rate均不兼容
activation=None, 
use_bias=True, 
kernel_initializer='glorot_uniform', 
bias_initializer='zeros', 
kernel_regularizer=None, 
bias_regularizer=None, 
activity_regularizer=None, 
kernel_constraint=None, 
bias_constraint=None)
```
我们仅对2维和1维中的不同进行了注释。一般来讲，只进行一维和二维卷积，当然还有很多其他特殊的卷积，我们这里没遇到，就不再赘述。
## 2.2 池化层
当然卷积层完毕之后紧接着的应该是池化操作了。池化的原理我们就不多讲，因为我们这里是Keras入门，神经网络入门请出门左转。但是池化的目的就是为了降维，池化一般有2种，第一种是最大池化，即挑选侯选池中最大的一个作为代表，第二种是平均池化，即把侯选池中所有数据求个平均值。 
下面是最大池化的函数声明：
```python
#1维最大池化
keras.layers.pooling.MaxPooling1D(
pool_size=2,#整数，池化窗口大小 
strides=None, #整数或None，下采样因子，例如设2将会使得输出shape为输入的一半，若为None则默认值为pool_size。
padding='valid'#‘valid’或者‘same’
)
#2维最大池化
keras.layers.pooling.MaxPooling2D(
pool_size=(2, 2),
 strides=None, 
data_format=None#代表图像的通道维的位置。
)
#对于时间信号的全局最大池化
keras.layers.pooling.GlobalMaxPooling1D()
#为空域信号施加全局最大值池化
keras.layers.pooling.GlobalMaxPooling2D(dim_ordering='default')
```
而平均池化也是和最大池化相同的，这里就不再介绍。
## 2.3 压平层
压平层属于核心层，是一个比较特殊的层。它主要是把多维的输入一维化，常用在从卷积层到全连接层的过渡。Flatten不影响batch的大小。其函数声明如下：
```python
keras.layers.core.Flatten()
```
这层没有什么好说的，完全自动化。但是其实我们需要考虑的是它如何进行变化的。 
以Keras官方例子：
```python
model = Sequential()
model.add(Convolution2D(64, 3, 3,
            border_mode='same',
            input_shape=(3, 32, 32)))
# now: model.output_shape == (None, 64, 32, 32)
model.add(Flatten())
# now: model.output_shape == (None, 65536)
```
可以看到这里的例子是一个输入为32*32大小的3个样例。（这里应该是这样，这就成了channel_first模式了）卷积后，应当是64个核心，每个核心32$\times$32大小。经过压平后，就是65536了。剩下的就都是我们认识的激活层、遗忘层和分类层了。不再介绍。
## 2.4 模型可视化
下面我们给出整个模型的样子，通过改图，我们可以清晰的看到每层的输入和输出的样子。当然，如果你愿意还可以使用model.summary()来获取关于模型更详细的信息。
![这里写图片描述](https://img-blog.csdn.net/20180125111107445?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzUwODIwMzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
从图中我们可以看到，该模型是如何把一个$100\times100\times3$的模型逐渐的变成一个10分类的的问题的，很神奇的过程吧。

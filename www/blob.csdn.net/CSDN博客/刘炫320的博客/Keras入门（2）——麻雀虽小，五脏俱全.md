# Keras入门（2）——麻雀虽小，五脏俱全 - 刘炫320的博客 - CSDN博客
2017年08月14日 21:45:14[刘炫320](https://me.csdn.net/qq_35082030)阅读数：7688
# 1. 什么是Keras
不知什么时候，突然对于Keras是什么产生了困惑。Keras中文为克拉斯，相传也是铜管乐器。
其实，Keras的名字源于希腊古典史诗《奥德赛》里的牛角之门，是真实事物进出梦境和现实的地方。《奥德赛》里面说，象牙之门内只是一场无法应验的梦境，唯有走进牛角之门奋斗的人，能够拥有真正的回报。其用意不可谓不深刻。
但事实上，Keras只是深度学习建模的一个上层建筑，其后端可以灵活使用CNTK、TensorFlow或者Theano。这样就可以避免不同深度学习框架的差异而集中于建模过程。并且可以进行CPU和GPU之间的无缝切换。
# 2. Hello World
在之前的一个快速入门里，我们只是大概的讲述了如何安装Keras环境以及一个简易的Demo，这次我们将会详细讲述一个真正的Hello World.下面我们先看这个示例：
```python
##2.1引用包
from keras.models import Sequential
from keras.layers import Dense, Dropout, Activation
from keras.optimizers import SGD
from keras import metrics
import keras
##2.2生成数据
# Generate dummy data
import numpy as np
x_train = np.random.random((1000, 20))
y_train = keras.utils.to_categorical(np.random.randint(10, size=(1000, 1)), num_classes=10)
x_test = np.random.random((100, 20))
y_test = keras.utils.to_categorical(np.random.randint(10, size=(100, 1)), num_classes=10)
##2.3构建模型
model = Sequential()
# Dense(64) is a fully-connected layer with 64 hidden units.
# in the first layer, you must specify the expected input data shape:
# here, 20-dimensional vectors.
model.add(Dense(64, activation='relu', input_dim=20))
model.add(Dropout(0.5))
model.add(Dense(64, activation='relu'))
model.add(Dropout(0.5))
model.add(Dense(10, activation='softmax'))
sgd = SGD(lr=0.01, decay=1e-6, momentum=0.9, nesterov=True)
model.compile(loss='categorical_crossentropy',
              optimizer=sgd,
              metrics=[metrics.categorical_accuracy,metrics.mae])
##2.4训练模型
model.fit(x_train, y_train,
          epochs=20,
          batch_size=128)
##2.5评估模型
score=model.evaluate(x_test, y_test, batch_size=128)
print(score);
```
先别急，我们先统一说一下上述这段代码的作用是，通过深度学习方法实现10分类问题的训练及测试过程。具体来讲，就是生成一个1000条训练样例，每个样例20维，再使用同样20维的测试数据100条，通过一个序列模型使用SGD优化算法进行训练，其模型层数不多，3个全连接层和2个放弃层。就这样一个简单的深度学习（算不上深度）模型就搭建完毕了。不过不用担心，我们会在接下来的部分详细介绍这部分代码，俗话说，麻雀虽小，五脏俱全。
## 2.1 引用包
先来回顾一下这部分代码:
```python
#引入序列模型
from keras.models import Sequential
#引入全连接层、放弃层、激活层（激活层没有直接用到，但是在全连接层里间接用到了。）
from keras.layers import Dense, Dropout, Activation
#引入SGD优化算法
from keras.optimizers import SGD
#引入了metrics评估模块
from keras import metrics
#引入了keras
import keras
```
这样一写，我们就清楚很多了，这是编码的必备。
## 2.2生成数据
还是先来回顾一下这部分代码：
```python
# Generate dummy data
#使用numpy来模拟生成数据
import numpy as np
#生成一个1000*20维的向量
x_train = np.random.random((1000, 20))
#生成一个1000*10维的向量
y_train = keras.utils.to_categorical(np.random.randint(10, size=(1000, 1)), num_classes=10)
#同上
x_test = np.random.random((100, 20))
y_test = keras.utils.to_categorical(np.random.randint(10, size=(100, 1)), num_classes=10)
```
别急，我知道这里可能有些难以理解。 
尤其是`keras.utils.to_categorical`这个方法，源码中，它是这样写的：
> 
Converts a class vector (integers) to binary class matrix. 
  E.g. for use with categorical_crossentropy.
也就是说它是对于一个类型的容器（整型）的转化为二元类型矩阵。比如用来计算多类别交叉熵来使用的。
其参数也很简单：
> 
def to_categorical(y, num_classes=None): 
  Arguments 
  y: class vector to be converted into a matrix 
  (integers from 0 to num_classes). 
  num_classes: total number of classes.
说的很明白了，y就是待转换容器（其类型为从0到类型数目），而num_classes则是类型的总数。
这样这一句就比较容易理解了：
```python
#先通过np生成一个1000*1维的其值为0-9的矩阵，然后再通过```keras.utils.to_categorical```方法获取成一个1000*10维的二元矩阵。
y_train = keras.utils.to_categorical(np.random.randint(10, size=(1000, 1)), num_classes=10)
```
说了这么多，其实就是使用onehot对类型标签进行编码。下面的也都是这样解释。
## 2.3构建模型
这部分代码如下:
```python
#构建序列模型
model = Sequential()
# Dense(64) is a fully-connected layer with 64 hidden units.
# in the first layer, you must specify the expected input data shape:
# here, 20-dimensional vectors.
#第一层为全连接层，隐含单元数为64，激活函数为relu，在第一层中一定要指明输入的维度。
model.add(Dense(64, activation='relu', input_dim=20))
#放弃层，将在训练过程中每次更新参数时随机断开一定百分比（rate）的输入神经元，Dropout层用于防止过拟合。这里是断开50%的输入神经元。
model.add(Dropout(0.5))
model.add(Dense(64, activation='relu'))
model.add(Dropout(0.5))
model.add(Dense(10, activation='softmax'))
#实例化优化算法为sgd优化算法
sgd = SGD(lr=0.01, decay=1e-6, momentum=0.9, nesterov=True)
#对模型进行预编译，其损失函数为多类别交叉熵，优化算法为sgd,评估方法为多类别准确度和平均绝对误差。
model.compile(loss='categorical_crossentropy',
              optimizer=sgd,
              metrics=[metrics.categorical_accuracy,metrics.mae])
```
这里才是核心，我们将会分开来讲。
### 2.3.1模型
首先这里接触到了model，model在Keras里有两种，一种是序贯模型，一种是函数式模型。
- 
序贯模型 
序贯模型是多个网络层的线性堆叠，也就是“一条路走到黑”。这也是非常常用的和傻瓜式的方法。
- 
函数式（Functional）模型 
这已经是高级阶段了，目前我们只能摘取[Keras中文文档](http://keras-cn.readthedocs.io/en/latest/getting_started/functional_API/)中的简介来告诉大家，在后面我们会单独讲解。 
我们起初将Functional一词译作泛型，想要表达该类模型能够表达任意张量映射的含义，但表达的不是很精确，在Keras 2里我们将这个词改译为“函数式”，对函数式编程有所了解的同学应能够快速get到该类模型想要表达的含义。函数式模型称作Functional，但它的类名是Model，因此我们有时候也用Model来代表函数式模型。 
Keras函数式模型接口是用户定义多输出模型、非循环有向模型或具有共享层的模型等复杂模型的途径。一句话，只要你的模型不是类似VGG一样一条路走到黑的模型，或者你的模型需要多于一个的输出，那么你总应该选择函数式模型。函数式模型是最广泛的一类模型，序贯模型（Sequential）只是它的一种特殊情况。
### 2.3.2添加层
代码如下：
```python
model.add(Dense(64, activation='relu', input_dim=20))
```
这就是序贯模型的添加层数的方法，很简单。这里添加的是Keras的常用层中的全连接层（Dense）。它是最常用的层，也是多层感知机（MLP）的基本构成单位。 
下面我们来简单的介绍一下目前用到的几个层，他们都属于常用层（Core）。
#### 2.3.2.1. Dense层
这个层的声明如下：
```python
keras.layers.core.Dense(units, activation=None, use_bias=True, kernel_initializer='glorot_uniform', bias_initializer='zeros', kernel_regularizer=None, bias_regularizer=None, activity_regularizer=None, kernel_constraint=None, bias_constraint=None, **kwargs)
```
Dense就是常用的全连接层，所实现的运算是output = activation(dot(input, kernel)+bias)。其中activation是逐元素计算的激活函数，kernel是本层的权值矩阵，bias为偏置向量，只有当use_bias=True才会添加。 
如果本层的输入数据的维度大于2，则会先被压为与kernel相匹配的大小。 
但是我们更关注的是参数的含义：
- units：大于0的整数，代表该层的输出维度。
- activation：激活函数，为预定义的激活函数名（参考激活函数），或逐元素（element-wise）的Theano函数。如果不指定该参数，将不会使用任何激活函数（即使用线性激活函数：a(x)=x）
- use_bias: 布尔值，是否使用偏置项
- kernel_initializer：权值初始化方法，为预定义初始化方法名的字符串，或用于初始化权重的初始化器。参考initializers
- bias_initializer：权值初始化方法，为预定义初始化方法名的字符串，或用于初始化权重的初始化器。参考initializers
- kernel_regularizer：施加在权重上的正则项，为Regularizer对象
- bias_regularizer：施加在偏置向量上的正则项，为Regularizer对象
- activity_regularizer：施加在输出上的正则项，为Regularizer对象
- kernel_constraints：施加在权重上的约束项，为Constraints对象
- bias_constraints：施加在偏置上的约束项，为Constraints对象
而这里我们看到了一个不同，那就是输入的单元`input_dim`没有出现在定义中，其实它藏在了`**kwargs`中，因为源码中这样写道：
```python
if 'input_shape' not in kwargs and 'input_dim' in kwargs:
            kwargs['input_shape'] = (kwargs.pop('input_dim'),)
```
这里就有出现问题了，什么是`input_dim`和`input_shape`。他们的区别在于，如果是2D张量，则可以使用`input_dim`即可，但是如果是三维的话，可能就需要`input_length`来帮忙了，具体来讲，他们之间的联系是：
```python
input_dim = input_shape(input_dim,)
input_dim, input_length = input_shape(input_length, input_dim,)
#像我们上面例子中的就是一个20维的数据，但是如果是识别手写体的话，你的原始数据是二维的，比如这个是 （28， 28） = 784，这个是mnist的手写数据。2维数据可以看成是1维的，那1维的shape就是（28*28，）了。
```
#### 2.3.2.2. Activation层
这个激活函数在本例中没有出现，其实不是，它间接的在Dense层中出现过了，预设的激活层常见的有以下几种：
- 
softmax 
这是归一化的多分类，可以把K维实数域压缩到（0，1）的值域中，并且使得K个数值和为1。
- 
sigmoid 
这时归一化的二元分类，可以把K维实数域压缩到近似为0，1二值上。
- 
relu 
这也是常用的激活函数，它可以把K维实数域映射到[0,inf)区间。
- 
tanh 
这时三角双曲正切函数，它可以把K维实数域映射到(-1,1)区间。
还有其他激活函数我们就不一一介绍了。
#### 2.3.2.3. Dropout层
它的原型为：
`keras.layers.core.Dropout(rate, noise_shape=None, seed=None)`
正如上面所述，为输入数据施加Dropout。Dropout将在训练过程中每次更新参数时随机断开一定百分比（rate）的输入神经元，Dropout层用于防止过拟合。
其参数含义如下：
- 
rate：0~1的浮点数，控制需要断开的神经元的比例。
- 
noise_shape：整数张量，为将要应用在输入上的二值Dropout mask的shape，例如你的输入为(batch_size, timesteps, features)，并且你希望在各个时间步上的Dropout mask都相同，则可传入noise_shape=(batch_size, 1, features)。
- 
seed：整数，使用的随机数种子 
还有几个常用层，我们会在接下来的讲解中讲到。这里不再赘述。
### 2.3.3优化算法
优化算法最常用的为SGD算法，也就是随机梯度下降算法。这里我们不多讲，因为优化算法我们会单开一章来总结一下所有的优化算法。
这里我们只讲这里用到的SGD，它的原型为：
`keras.optimizers.SGD(lr=0.01, momentum=0.0, decay=0.0, nesterov=False)`
它就是随机梯度下降法，支持动量参数，支持学习衰减率，支持Nesterov动量。这几个参数就是SGD的几个改进，优化算法那章我们会讲到。
其参数含义如下：
- lr：大于0的浮点数，学习率
- momentum：大于0的浮点数，动量参数
- decay：大于0的浮点数，每次更新后的学习率衰减值
- nesterov：布尔值，确定是否使用Nesterov动量
### 2.3.4模型的编译
上面的注释已经讲的很明白了。在训练模型之前，我们需要通过compile来对学习过程进行配置。compile接收三个参数：
- 优化器optimizer：该参数可指定为已预定义的优化器名，如rmsprop、adagrad，或一个Optimizer类的对象，详情见optimizers
- 损失函数loss：该参数为模型试图最小化的目标函数，它可为预定义的损失函数名，如categorical_crossentropy、mse，也可以为一个损失函数。详情见losses
- 指标列表metrics：对分类问题，我们一般将该列表设置为metrics=[‘accuracy’]。指标可以是一个预定义指标的名字,也可以是一个用户定制的函数.指标函数应该返回单个张量,或一个完成metric_name - > metric_value映射的字典.请参考性能评估
下面给出一些样例：
```python
# For a multi-class classification problem
model.compile(optimizer='rmsprop',
              loss='categorical_crossentropy',
              metrics=['accuracy'])
# For a binary classification problem
model.compile(optimizer='rmsprop',
              loss='binary_crossentropy',
              metrics=['accuracy'])
# For a mean squared error regression problem
model.compile(optimizer='rmsprop',
              loss='mse')
# For custom metrics
import keras.backend as K
def mean_pred(y_true, y_pred):
    return K.mean(y_pred)
model.compile(optimizer='rmsprop',
              loss='binary_crossentropy',
              metrics=['accuracy', mean_pred])
```
要注意的是，这里loss和metrics的指标都会在最后的评估中体现。
## 2.4 模型的训练
模型训练都是如出一辙，全是`fit`方法：
```python
fit(self, x, y, batch_size=32, epochs=10, verbose=1, callbacks=None, validation_split=0.0, validation_data=None, shuffle=True, class_weight=None, sample_weight=None, initial_epoch=0)
```
本函数将模型训练nb_epoch轮，其参数有：
- x：输入数据。如果模型只有一个输入，那么x的类型是numpy array，如果模型有多个输入，那么x的类型应当为list，list的元素是对应于各个输入的numpy array
- y：标签，numpy array
- batch_size：整数，指定进行梯度下降时每个batch包含的样本数。训练时一个batch的样本会被计算一次梯度下降，使目标函数优化一步。
- epochs：整数，训练的轮数，每个epoch会把训练集轮一遍。
- verbose：日志显示，0为不在标准输出流输出日志信息，1为输出进度条记录，2为每个epoch输出一行记录
- callbacks：list，其中的元素是keras.callbacks.Callback的对象。这个list中的回调函数将会在训练过程中的适当时机被调用，参考回调函数
- validation_split：0~1之间的浮点数，用来指定训练集的一定比例数据作为验证集。验证集将不参与训练，并在每个epoch结束后测试的模型的指标，如损失函数、精确度等。注意，validation_split的划分在shuffle之前，因此如果你的数据本身是有序的，需要先手工打乱再指定validation_split，否则可能会出现验证集样本不均匀。
- validation_data：形式为（X，y）的tuple，是指定的验证集。此参数将覆盖validation_spilt。
- shuffle：布尔值或字符串，一般为布尔值，表示是否在训练过程中随机打乱输入样本的顺序。若为字符串“batch”，则是用来处理HDF5数据的特殊情况，它将在batch内部将数据打乱。
- class_weight：字典，将不同的类别映射为不同的权值，该参数用来在训练过程中调整损失函数（只能用于训练）
- sample_weight：权值的numpy array，用于在训练时调整损失函数（仅用于训练）。可以传递一个1D的与样本等长的向量用于对样本进行1对1的加权，或者在面对时序数据时，传递一个的形式为（samples，sequence_length）的矩阵来为每个时间步上的样本赋不同的权。这种情况下请确定在编译模型时添加了sample_weight_mode=’temporal’。
- initial_epoch: 从该参数指定的epoch开始训练，在继续之前的训练时有用。
fit函数返回一个History的对象，其History.history属性记录了损失函数和其他指标的数值随epoch变化的情况，如果有验证集的话，也包含了验证集的这些指标变化情况。
## 2.5 模型的评估
模型评估使用的是evaluate方法：
```python
evaluate(self, x, y, batch_size=32, verbose=1, sample_weight=None)
```
它最终返回的是一个score,第0维为编译中的loss指标，剩下的就是metrics中的指标了。
参数含义如下:
- x：输入数据，与fit一样，是numpy array或numpy array的list
- y：标签，numpy array
- batch_size：整数，含义同fit的同名参数
- verbose：含义同fit的同名参数，但只能取0或1
- sample_weight：numpy array，含义同fit的同名参数
# 3. 小结
在本文中，我们庖丁解牛般的详细的介绍了一个麻雀的五脏六腑，基本上是解说到了最基础的部分，这也增加了我们对于Keras的理解。在接下来的过程中，我们将会对于每一部分进行一个详细的讲解。
最后致谢《[Keras中文文档](http://keras-cn.readthedocs.io/en/latest/)》、百度知道、Stack Overflow以及各位同行的博客。

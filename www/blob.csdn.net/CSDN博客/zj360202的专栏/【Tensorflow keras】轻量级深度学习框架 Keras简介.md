# 【Tensorflow keras】轻量级深度学习框架 Keras简介 - zj360202的专栏 - CSDN博客





2017年11月27日 17:17:10[zj360202](https://me.csdn.net/zj360202)阅读数：1280








Keras是基于Theano的一个深度学习框架，它的设计参考了Torch，用Python语言编写，是一个高度模块化的神经网络库，支持GPU和CPU。

使用文档在这：http://keras.io/，

中文文档在这：http://keras-cn.readthedocs.io/en/latest/ ，

这个框架是2015年流行起来的，使用中遇到的困惑或者问题可以提交到github：https://github.com/fchollet/keras。
Keras主要包括14个模块包，可参见文档https://keras.io/layers/ ，下面主要对Models、Layers、Initializations、Activations、Objectives、Optimizers、Preprocessing、metrics八个模块包展开介绍。


**1. Models包：keras.models**
这是Keras中最主要的一个模块，用于对各个组件进行组装。
详细说明：http://keras.io/models/

```python
from keras.models import Sequential
model = Sequential()  # 初始化模型
model.add(...)  # 可使用add方法组装组件
```

**2. Layers包：keras.layers**
该模块主要用于生成神经网络层，包含多种类型，如Core
 layers、Convolutional layers、recurrent layers、advanced_activations layers、normalization layers、embeddings layers等。
其中Core layers里面包含了flatten（CNN的全连接层之前需要把二维特征图flatten成为一维的）、reshape（CNN输入时将一维的向量弄成二维的）、dense（隐藏层）。
Convolutional layers层包含Theano的Convolution2D的封装等。
详细说明：http://keras.io/layers/

```python
from keras.layers import Dense  # Dense表示BP层
model.add(Dense(input_dim=3,output_dim=5))  # 加入隐含层
```

**3. Initializations包：keras.initializations**
该模块主要负责对模型参数（权重）进行初始化，初始化方法包括：uniform、lecun_uniform、normal、orthogonal、zero、glorot_normal、he_normal等。
详细说明：http://keras.io/initializations/

```python
model.add(Dense(input_dim=3,output_dim=5,init='uniform')) #加入带初始化（uniform）的隐含层
```

**4. Activations包：keras.activations、keras.layers.advanced_activations（新激活函数）**
该模块主要负责为神经层附加激活函数，如linear、sigmoid、hard_sigmoid、tanh、softplus、softmax、relu以及LeakyReLU、PReLU等比较新的激活函数。
详细说明：http://keras.io/activations/

```python
model.add(Dense(input_dim=3, output_dim=5, activation='sigmoid'))  # 加入带激活函数（sigmoid）的隐含层
等价于：
model.add(Dense(input_dim=3, output_dim=5))
model.add(Activation('sigmoid'))
```

**5. Objectives包：keras.objectives**
该模块主要负责为神经网络附加损失函数，即目标函数。如mean_squared_error，mean_absolute_error
 ，squared_hinge，hinge，binary_crossentropy，categorical_crossentropy等，其中binary_crossentropy，categorical_crossentropy是指logloss。
注：目标函数的设定是在模型编译阶段。
详细说明：http://keras.io/objectives/

```python
model.compile(loss='binary_crossentropy', optimizer='sgd') #loss是指目标函数
```

**6. Optimizers包：keras.optimizers**
该模块主要负责设定神经网络的优化方法，如最基本的随机梯度下降SGD,另外还有Adagrad、Adadelta、RMSprop、Adam，一些新的方法以后也会被不断添加进来。
详细说明：http://keras.io/optimizers/

```python
keras.optimizers.SGD(lr=0.01, momentum=0.9, decay=0.9, nesterov=False)
```

上面的代码是SGD的使用方法，lr表示学习速率,momentum表示动量项，decay是学习速率的衰减系数(每个epoch衰减一次),Nesterov的值是False或者True，表示使不使用Nesterov momentum。




```python
model = Sequential()
model.add(Dense(64, init='uniform', input_dim=10))
model.add(Activation('tanh'))
model.add(Activation('softmax'))
sgd = SGD(lr=0.01, decay=1e-6, momentum=0.9, nesterov=True)
model.compile(loss='mean_squared_error', optimizer=sgd)  #指优化方法sgd
model.compile(loss='binary_crossentropy', optimizer='sgd')
```

**7. Preprocessing包：keras.preprocessing**
数据预处理模块，包括序列数据的处理、文本数据的处理和图像数据的处理等。对于图像数据的处理，keras提供了ImageDataGenerator函数,实现数据集扩增，对图像做一些弹性变换，比如水平翻转，垂直翻转，旋转等。


**8. metrics包：keras.metrics**
与sklearn中metrics包基本相同，主要包含一些如binary_accuracy、mae、mse等的评价方法。

```python
predict = model.predict_classes(test_x)       #输出预测结果
keras.metrics.binary_accuracy(test_y, predict)  #计算预测精度
```




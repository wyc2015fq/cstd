# Tensorflow keras入门教程 - DumpDoctorWang的博客 - CSDN博客





2018年11月04日 21:08:31[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：358











### 目录
- [1、Tensorflow与Keras](#1TensorflowKeras_2)
- [2、安装内置Keras的Tensorflow](#2KerasTensorflow_7)
- [3、Tensorflow内置的Keras教程](#3TensorflowKeras_10)
- [3.1、导入tf.keras](#31tfkeras_12)
- [3.2、创建一个简单的模型](#32_26)
- [3.2.1、顺序模型（Sequential model）](#321Sequential_model_27)
- [3.2.2、设置keras层（layer）](#322keraslayer_40)
- [3.3、训练和评估](#33_68)
- [3.3.1、配置训练](#331_69)
- [3.3.2、使用NumPy数据作为训练数据](#332NumPy_114)
- [3.3.3、使用 tf.data作为训练数据](#333_tfdata_146)
- [3.3.4、评估和预测](#334_174)
- [3.4、构建高级模型](#34_194)
- [3.4.1、函数式API](#341API_195)
- [3.4.2、模型子类化（Model subclassing）](#342Model_subclassing_232)
- [3.4.3、自定义层](#343_277)
- [3.5、回调](#35_337)
- [3.6、保存与载入](#36_358)
- [3.6.1、仅保存权重](#361_359)
- [3.6.2、仅保存配置](#362_378)
- [3.6.3、保存整个模型](#363_403)
- [3.7、Eager execution](#37Eager_execution_425)
- [3.8、分布式训练](#38_430)
- [3.8.1 、Estimators](#381_Estimators_431)
- [3.8.2、多GPU训练](#382GPU_447)




# 1、Tensorflow与Keras

Tensorflow和Keras原本是两个深度学习的框架。Keras有着更高级的API，构建模型要比Tensorflow简单许多；Keras有许多后端(backend)可以选，Tensorflow就是其中一种后端。

2017年01月17日，Keras的作者、谷歌AI研究员Francois Chollet宣布了一条激动人心的消息：Keras将会成为第一个被添加到TensorFlow核心中的高级别框架，这将会让Keras变成Tensorflow的默认API。也就是说Tensorflow内置Keras了。

# 2、安装内置Keras的Tensorflow

最新版Tensorflow已经内置了Keras模块（最新的Tensorflow版本是1.11.0）,其对应的Keras的版本为2.1.6-tf，只要安装了Tensorflow 1.11.0及以上版本，就已经能使用Tensorflow内置的Keras了。

# 3、Tensorflow内置的Keras教程

此部分文档是官分文档的翻译，根据自己的理解进行了翻译，帮助大家入门。原文链接[Tensorflow Keras](https://www.tensorflow.org/guide/keras)

## 3.1、导入tf.keras

[*tf.keras*](https://www.tensorflow.org/api_docs/python/tf/keras)是[*Keras API*](https://keras.io/)的Tensorflow实现，是一个用于构建和训练模型的高级API，其中包括对TensorFlow特定功能的一流支持，例如[*Eager Execution*](https://www.tensorflow.org/guide/keras#eager_execution)，[*tf.data Pipeline*](https://www.tensorflow.org/api_docs/python/tf/data)和[*Estimators*](https://www.tensorflow.org/guide/estimators)。 [tf.keras](https://www.tensorflow.org/api_docs/python/tf/keras)使TensorFlow更易于使用，而不会牺牲灵活性和性能。

导入*tf.keras*的代码为：

```python
import tensorflow as tf
from tensorflow.keras import layers
# 打印Tensorflow和内置的Keras版本
print(tf.VERSION)
print(tf.keras.__version__)
```

*tf.keras*可以运行任何与Keras兼容的代码，但请记住：
- 最新TensorFlow版本中的*tf.keras*版本可能与PyPI的最新keras版本不同。 可打印*tf.keras.version*来检查版本。
- [保存模型的权重](https://www.tensorflow.org/guide/keras#weights_only)时，*tf.keras*默认为[检查点格式](https://www.tensorflow.org/guide/checkpoints)( checkpoint format)。 要使用HDF5保存权重时，传入参数*save_format =‘h5’*。

## 3.2、创建一个简单的模型

### 3.2.1、顺序模型（Sequential model）

在Keras中，您可以组装层（Layer）来构建模型（Model）。 最常见的模型类型是层的堆叠：[*tf.keras.Sequential*](https://www.tensorflow.org/api_docs/python/tf/keras/models/Sequential)模型。

构建一个简单的，完全连接的网络（即多层感知器）：

```python
model = tf.keras.Sequential()
# 添加一个有64个单元全连接层到模型
# 顺便说一句，densely-connected layer=fully-connected layer
model.add(layers.Dense(64, activation='relu'))
# 再添加一个
model.add(layers.Dense(64, activation='relu'))
# 添加一个有10个输出单元的softmax层
model.add(layers.Dense(10, activation='softmax'))
```

### 3.2.2、设置keras层（layer）

[*tf.keras.layers*](https://www.tensorflow.org/api_docs/python/tf/keras/layers)一些常见的构造函数参数：
- *activation*：设置层的激活函数。 此参数由内置函数的名称或可调用对象指定。 默认情况下，不使用任何激活。
- *kernel_initializer*和*bias_initializer*：分别指定层的核（原文是Kernel，可以理解为weights）和偏置（bias）的初始化器（initializer）。 参数是名称或可调用对象。 默认为“Glorot uniform”初始化器。
- *kernel_regularizer*和*bias_regularizer*：分别指定层的核（Kernel）和偏置（bias）的正则化方案，例如L1或L2正则化。 默认情况下，不使用正则化。

下面使用构造函数参数实例化[*tf.keras.layers.Dense*](https://www.tensorflow.org/api_docs/python/tf/keras/layers/Dense)的一些例子：

```python
# 创建一个sigmoid层:
layers.Dense(64, activation='sigmoid')
# 另外一种方法
layers.Dense(64, activation=tf.sigmoid)

# 定义一个线性层(linear layer)，核矩阵（kernel matrix）使用一个因子为0.01的L1正则化器
layers.Dense(64, kernel_regularizer=tf.keras.regularizers.l1(0.01))

# 定义一个线性层，偏置向量（ bias vector）使用一个因子为0.01的L2正则化器
layers.Dense(64, bias_regularizer=tf.keras.regularizers.l2(0.01))

# 定义了一个线性层，核使用的初始化器为orthogonal（核会被初始化为一个随机正交矩阵）
layers.Dense(64, kernel_initializer='orthogonal')

# 定义了一个线性层，偏置向量的初始化去器为constant（偏置向量的所有元素都会被初始化为2.0）
layers.Dense(64, bias_initializer=tf.keras.initializers.constant(2.0))
```

## 3.3、训练和评估

### 3.3.1、配置训练

构建模型后，通过调用[*compile*](https://www.tensorflow.org/api_docs/python/tf/keras/Model#compile)函数编译（原文为configure，我这里翻译为编译）：

```python
model = tf.keras.Sequential([
# 添加一个有64个单元的全连接层，激活函数为relu
layers.Dense(64, activation='relu'),
# 再添加一个
layers.Dense(64, activation='relu'),
# 添加一个有10个输出单元的softmax层
layers.Dense(10, activation='softmax')])

model.compile(optimizer=tf.train.AdamOptimizer(0.001),
              loss='categorical_crossentropy',
              metrics=['accuracy'])
```

*tf.keras.Model.compile*有三个重要参数（建议参考下[*Keras compile document*](https://keras.io/getting-started/sequential-model-guide/#compilation)）：
- *optimizer*：指定优化器。 从[*tf.train*](https://www.tensorflow.org/api_docs/python/tf/train)模块传递优化器实例，例如[*tf.train.AdamOptimizer*](https://www.tensorflow.org/api_docs/python/tf/train/AdamOptimizer)，[*tf.train.RMSPropOptimizer*](https://www.tensorflow.org/api_docs/python/tf/train/RMSPropOptimizer)或[*tf.train.GradientDescentOptimizer*](https://www.tensorflow.org/api_docs/python/tf/train/GradientDescentOptimizer)。
- *loss*：在优化期间最小化的函数。 常见的选择包括均方误差（*mse*），*categorical_crossentropy*和*binary_crossentropy*。 损失函数由名称或通过从[*tf.keras.losses*](https://www.tensorflow.org/api_docs/python/tf/keras/losses)模块传递可调用对象来指定。
- *metrics*：设置训练中要输出的指标(原文为metrics，我这里翻译为指标)组成的列表，上面的代码中就只有准确率（accuracy）。 指标是来自[*tf.keras.metrics*](https://www.tensorflow.org/api_docs/python/tf/keras/metrics)模块的字符串名称或可调用对象组成的list。

以下显示了编译模型的几个示例：

```python
# 编译均方误差回归模型（a model for mean-squared error regression）
model.compile(optimizer=tf.train.AdamOptimizer(0.01),
              loss='mse',       # 最小均方误差（mean squared error）
              metrics=['mae'])  # 平均绝对误差（mean absolute error）

# 编译一个分类模型（a model for categorical classification）
# 多元分类问题例子，比如手写数字识别
model.compile(optimizer=tf.train.RMSPropOptimizer(0.01),
              loss=tf.keras.losses.categorical_crossentropy,
              metrics=[tf.keras.metrics.categorical_accuracy])
# 和上面的相同
model.compile(optimizer='rmsprop',
              loss='categorical_crossentropy',
              metrics=['accuracy'])

# 二元分类例子
model.compile(optimizer='rmsprop',
              loss='binary_crossentropy',
              metrics=['accuracy'])
```

**可以注意到，上面的程序中第一个的*loss*和*metrics*传的是字符串，第二种传的是可调用的对象。**

### 3.3.2、使用NumPy数据作为训练数据

对于小型数据集，请使用内存中的NumPy数组来训练和评估模型。 使用*fit*方法将模型“拟合”到训练数据：

```python
import numpy as np

data = np.random.random((1000, 32))
labels = np.random.random((1000, 10))

model.fit(data, labels, epochs=10, batch_size=32)
```

[*tf.keras.Model.fit*](https://www.tensorflow.org/api_docs/python/tf/keras/Model#fit)有三个重要参数：
- *epoch*：训练多少个epoch。 一个epoch是对整个训练数据集的一次训练（这是以较小的批次完成的）。
- *batch_size*：当传递NumPy数据时，模型将数据分成较小的批次（batch），并在训练期间训练这些批次。 此整数指定每个批次的大小。 请注意，如果样本总数不能被批次大小整除，则最后一批可能会更小。
- *validation_data*：在对模型进行原型设计时，若要监控其在某些验证数据集上的性能。 传递由（输入，标签）组成的元组， 模型在每个epoch的末尾显示损失和指标。

这是使用*validation_data*的示例：

```python
import numpy as np

data = np.random.random((1000, 32))
labels = np.random.random((1000, 10))

val_data = np.random.random((100, 32))
val_labels = np.random.random((100, 10))

model.fit(data, labels, epochs=10, batch_size=32,
          validation_data=(val_data, val_labels))
```

### 3.3.3、使用 tf.data作为训练数据

若要使用大型数据集或多设备训练，[要使用*Dateset API*](https://www.tensorflow.org/guide/datasets) ，将[*tf.data.Dataset*](https://www.tensorflow.org/api_docs/python/tf/data/Dataset)实例传递给fit方法：

```python
# 实例化玩具数据集实例：
dataset = tf.data.Dataset.from_tensor_slices((data, labels))
dataset = dataset.batch(32)
dataset = dataset.repeat()

# 在数据集上调用`fit`时，不要忘记指定`steps_per_epoch`
model.fit(dataset, epochs=10, steps_per_epoch=30)
```

这里，*fit*方法函数*steps_per_epoch*参数 - 这是模型在训练集上训练一遍（也就是一个epoch）的训练步数。 由于Dataset生成批次数据，因此此代码段不需要*batch_size*。

如果要传入验证集：

```python
dataset = tf.data.Dataset.from_tensor_slices((data, labels))
dataset = dataset.batch(32).repeat()

val_dataset = tf.data.Dataset.from_tensor_slices((val_data, val_labels))
val_dataset = val_dataset.batch(32).repeat()

model.fit(dataset, epochs=10, steps_per_epoch=30,
          validation_data=val_dataset,
          validation_steps=3)
```

### 3.3.4、评估和预测

[*tf.keras.Model.evaluate*](https://www.tensorflow.org/api_docs/python/tf/keras/Model#evaluate)和[*tf.keras.Model.predict*](https://www.tensorflow.org/api_docs/python/tf/keras/Model#predict)函数可以使用NumPy数组和*tf.data.Dataset*作为参数。

如果要评估（evaluate）所提供数据的模型的损失和指标（也就是深度学习中评估测试集的损失loss和前面compile函数中metrics指定的指标），代码如下：

```python
data = np.random.random((1000, 32))
labels = np.random.random((1000, 10))

model.evaluate(data, labels, batch_size=32)

model.evaluate(dataset, steps=30)
```

NumPy数组作为参数，输出模型最后一层的输出（也就是预测一个样本）：

```python
result = model.predict(data, batch_size=32)
print(result.shape)
```

## 3.4、构建高级模型

### 3.4.1、函数式API

[*tf.keras.Sequential*](https://www.tensorflow.org/api_docs/python/tf/keras/models/Sequential)模型是一个简单的层的堆叠，不能构建任意模型。 使用[Keras函数式API](https://keras.io/getting-started/functional-api-guide/)构建复杂的模型，例如：
- 多输入模型，
- 多输出模型，
- 具有共享层的模型（同一层被调用多次），
- 具有非顺序数据流的模型（例如，residual connections）。

使用函数式API构建模型的方法如下：
- 调用层实例，并且返回张量（tensor）。
- 输入张量和输出张量用于定义[*tf.keras.Model*](https://www.tensorflow.org/api_docs/python/tf/keras/models/Model)实例。
- 这个模型的训练就像*Sequential*模型一样。

以下示例使用函数式API构建一个简单，全连接（fully-connected）的网络：

```python
inputs = tf.keras.Input(shape=(32,))  # Returns a placeholder tensor

# 层的实例可调用，参数为tensor，返回一个tensor
x = layers.Dense(64, activation='relu')(inputs)
x = layers.Dense(64, activation='relu')(x)
predictions = layers.Dense(10, activation='softmax')(x)
```

实例化指定输入和输出的模型

```python
model = tf.keras.Model(inputs=inputs, outputs=predictions)

# 编译这个步骤指定了训练的配置（the training configuration）
model.compile(optimizer=tf.train.RMSPropOptimizer(0.001),
              loss='categorical_crossentropy',
              metrics=['accuracy'])

# 训练5个epoch
model.fit(data, labels, batch_size=32, epochs=5)
```

### 3.4.2、模型子类化（Model subclassing）

通过继承*tf.keras.Model*并定义自己的前向传播来构建完全可自定义的模型。 在`__init__`函数中创建图层并将它们设置为类实例的属性。 在*call*函数中定义前向传播。

当启用Eager Execution时，模型子类化特别有用（原文说很有用，但我还不知道）。
- **关键点： 虽然模型子类化提供了灵活性，但其代价是更高的复杂性和更多的编码错误可能性。 如果可能，请选择函数式API（也就是3.4.1节中提到的方法）。**

以下示例显示了继承*tf.keras.Model*，使用自定义前向传播：

```python
class MyModel(tf.keras.Model):

  def __init__(self, num_classes=10):
    super(MyModel, self).__init__(name='my_model')
    self.num_classes = num_classes
    # 在__init__函数中定义层
    self.dense_1 = layers.Dense(32, activation='relu')
    self.dense_2 = layers.Dense(num_classes, activation='sigmoid')

  def call(self, inputs):
    # 在call函数中定义前向传播
    # 使用在__init__中定义的层
    x = self.dense_1(inputs)
    return self.dense_2(x)

  def compute_output_shape(self, input_shape):
    # 如果你需要把这个子类化的模型当作一个函数式模型的一部分，那么你需要重载这个函数。
    # 否则，这个函数是可选的
    shape = tf.TensorShape(input_shape).as_list()
    shape[-1] = self.num_classes
    return tf.TensorShape(shape)
```

实例化上述模型的代码为：

```python
model = MyModel(num_classes=10)

model.compile(optimizer=tf.train.RMSPropOptimizer(0.001),
              loss='categorical_crossentropy',
              metrics=['accuracy'])

model.fit(data, labels, batch_size=32, epochs=5)
```

### 3.4.3、自定义层

要自定义层，需要继承 [*tf.keras.layers.Layer*](https://www.tensorflow.org/api_docs/python/tf/keras/layers/Layer)并且实现如下几个函数：
- *build*：创建图层的权重。 使用*add_weight*函数来创建。
- *call*：定义前向传播。
- *compute_output_shape*：计算在给定的输入的shape时，计算出输出的shape。
- 可以通过实现*get_config*方法和*from_config*方法来序列化层。不过是可选的。

下面是自定义层的示例，它实现了一个使输入（Input）和核（Kernel）矩阵相乘（matmul）：

```python
class MyLayer(layers.Layer):

  def __init__(self, output_dim, **kwargs):
    self.output_dim = output_dim
    super(MyLayer, self).__init__(**kwargs)

  def build(self, input_shape):
    shape = tf.TensorShape((input_shape[1], self.output_dim))
    # 为此层创建一个可训练的权重
    self.kernel = self.add_weight(name='kernel',
                                  shape=shape,
                                  initializer='uniform',
                                  trainable=True)
    # 确保在函数结束时调用下面的语句
    super(MyLayer, self).build(input_shape)

  def call(self, inputs):
    # 这里定义了这层要实现的操作，也就是前向传播的操作
    return tf.matmul(inputs, self.kernel)

  def compute_output_shape(self, input_shape):
    # 计算输出tensor的shape
    shape = tf.TensorShape(input_shape).as_list()
    shape[-1] = self.output_dim
    return tf.TensorShape(shape)

  def get_config(self):
    base_config = super(MyLayer, self).get_config()
    base_config['output_dim'] = self.output_dim
    return base_config

  @classmethod
  def from_config(cls, config):
    return cls(**config)
```

使用自定义层：

```python
model = tf.keras.Sequential([
    MyLayer(10),
    layers.Activation('softmax')])

model.compile(optimizer=tf.train.RMSPropOptimizer(0.001),
              loss='categorical_crossentropy',
              metrics=['accuracy'])

model.fit(data, labels, batch_size=32, epochs=5)
```

## 3.5、回调

回调是传递给模型的对象，用于在训练模型期间自定义和扩展其行为。 可以编写自己的自定义回调，或使用以下内置的[*tf.keras.callbacks*](https://www.tensorflow.org/api_docs/python/tf/keras/callbacks)：
- [*tf.keras.callbacks.ModelCheckpoint*](https://www.tensorflow.org/api_docs/python/tf/keras/callbacks/ModelCheckpoint)：定期保存模型的检查点（checkpoint）。
- [*tf.keras.callbacks.LearningRateScheduler*](https://www.tensorflow.org/api_docs/python/tf/keras/callbacks/LearningRateScheduler)：动态改变学习率。
- [*tf.keras.callbacks.EarlyStopping*](https://www.tensorflow.org/api_docs/python/tf/keras/callbacks/EarlyStopping)：在校验集的性能停止提升时，中断训练。
- [*tf.keras.callbacks.TensorBoard*](https://www.tensorflow.org/api_docs/python/tf/keras/callbacks/TensorBoard)：使用TensorBoard监控模型的行为。

若要使用[*tf.keras.callbacks.Callback*](https://www.tensorflow.org/api_docs/python/tf/keras/callbacks/Callback)，请将其传递给模型的*fit*方法：

```python
callbacks = [
  # 如果`val_loss`在超过两个epoch都没有提升，那么中断训练
  tf.keras.callbacks.EarlyStopping(patience=2, monitor='val_loss'),
  # 把TensorBoard的日志写入文件夹`./logs`
  tf.keras.callbacks.TensorBoard(log_dir='./logs')
]
model.fit(data, labels, batch_size=32, epochs=5, callbacks=callbacks,
          validation_data=(val_data, val_labels))
```

## 3.6、保存与载入

### 3.6.1、仅保存权重

参考[*tf.keras.Model.save_weights*](https://www.tensorflow.org/api_docs/python/tf/keras/Model#save_weights)

```python
# 把权重保存为TensorFlow Checkpoint文件
model.save_weights('./weights/my_model')

# 载入权重。要求模型和保存权重的模型具有相同的架构
model.load_weights('./weights/my_model')
```

如果要把权重保存为Keras的HDF5格式，可使用如下代码

```python
# 把权重保存为HDF5这种格式的文件
model.save_weights('my_model.h5', save_format='h5')

# 载入权重
model.load_weights('my_model.h5')
```

### 3.6.2、仅保存配置

模型的配置（原文为model’s configuration，可以理解为模型的结构）是可以保存的。 这可以在没有任何权重的情况下序列化模型体系结构。 即使没有定义原始模型的代码，保存的配置也可以重新创建和初始化相同的模型。 Keras支持JSON和YAML序列化格式：

把模型序列化为json字符串

```python
json_string = model.to_json()
print(json_string)
```

从json字符串恢复模型:

```python
fresh_model = tf.keras.models.model_from_json(json_string)
```

把模型序列化为YAML格式：

```python
yaml_string = model.to_yaml()
print(yaml_string)
```

从YAML字符串恢复模型:

```python
fresh_model = tf.keras.models.model_from_yaml(yaml_string)
```
- **注意：子类化模型不可序列化，因为它们的体系结构由*call*函数中的Python代码定义。**

### 3.6.3、保存整个模型

如果保存模型结构与权重（甚至还包括优化器的配置），下次可以恢复模型结构和权重接着训练且不需要访问原始代码。

```python
# 创建一个简单的模型
model = tf.keras.Sequential([
  layers.Dense(10, activation='softmax', input_shape=(32,)),
  layers.Dense(10, activation='softmax')
])
model.compile(optimizer='rmsprop',
              loss='categorical_crossentropy',
              metrics=['accuracy'])
model.fit(data, labels, batch_size=32, epochs=5)


# 将整个模型保存到HDF5文件
model.save('my_model.h5')

# 重新创建完全相同的模型，包括权重和优化器。
model = tf.keras.models.load_model('my_model.h5')
```

## 3.7、Eager execution

[Eager execution](https://www.tensorflow.org/guide/eager)是一个必要的编程环境，可以立即评估操作。 这对于Keras不是必需的，但是由*tf.keras*支持，对于检查程序和调试很有用。

所有*tf.keras* API都与Eager execution兼容。 虽然可以使用顺序模型和函数式API，但是Eager execution尤其有利于模型子类化和构建自定义层 。

有关使用具有自定义训练和[*tf.GradientTape*](https://www.tensorflow.org/api_docs/python/tf/GradientTape)的Keras模型的示例，请参阅[Eager execution 指南](https://www.tensorflow.org/guide/eager#build_a_model)。
## 3.8、分布式训练

### 3.8.1 、Estimators

[Estimators](https://www.tensorflow.org/guide/estimators) API用于分布式环境的训练模型的API。 可以导出模型进行大型数据集的分布式训练，并得到可以商用的模型。

使用[*tf.keras.estimator.model_to_estimator*](https://www.tensorflow.org/api_docs/python/tf/keras/estimator/model_to_estimator)将模型转换为[*tf.estimator.Estimator*](https://www.tensorflow.org/api_docs/python/tf/estimator/Estimator)对象，就可以使用[*tf.estimator*](https://www.tensorflow.org/api_docs/python/tf/estimator) API训练[*tf.keras.Model*](https://www.tensorflow.org/api_docs/python/tf/keras/models/Model)。 请参阅[Creating Estimators from Keras models](https://www.tensorflow.org/guide/estimators#creating_estimators_from_keras_models)。

```python
model = tf.keras.Sequential([layers.Dense(10,activation='softmax'),
                          layers.Dense(10,activation='softmax')])

model.compile(optimizer=tf.train.RMSPropOptimizer(0.001),
              loss='categorical_crossentropy',
              metrics=['accuracy'])
# 把模型转换为Estimator
estimator = tf.keras.estimator.model_to_estimator(model)
```
- **注意：要检查Estimator的[输入函数](https://www.tensorflow.org/guide/premade_estimators#create_input_functions)和查看数据时，请启用[Eager Execution](https://www.tensorflow.org/guide/eager)。**

### 3.8.2、多GPU训练

*tf.keras*模型可以使用[*tf.contrib.distribute.DistributionStrategy*](https://www.tensorflow.org/api_docs/python/tf/contrib/distribute/DistributionStrategy)在多个GPU上训练。 此API在多个GPU上提供分布式训练，几乎不对现有代码进行任何更改。

目前，[*tf.contrib.distribute.MirroredStrategy*](https://www.tensorflow.org/api_docs/python/tf/contrib/distribute/MirroredStrategy)是唯一受支持的分布式策略。 要将DistributionStrategy与Keras一起使用，请使用[*tf.keras.estimator.model_to_estimator*](https://www.tensorflow.org/api_docs/python/tf/keras/estimator/model_to_estimator)将*tf.keras.Model*转换为*tf.estimator.Estimator*，然后训练Estimator。

以下示例在单个计算机上的多个GPU之间分布式训练*tf.keras.Model*。

首先，定义一个简单的模型：
```python
model = tf.keras.Sequential()
model.add(layers.Dense(16, activation='relu', input_shape=(10,)))
model.add(layers.Dense(1, activation='sigmoid'))

optimizer = tf.train.GradientDescentOptimizer(0.2)

model.compile(loss='binary_crossentropy', optimizer=optimizer)
model.summary()
```

定义输入管线（Input Pipeline）。*input_fn*返回一个*tf.data.Dataset*对象，用于在多个设备之间分配数据 - 每个设备处理输入数据（Input batch）的一部分。

```python
def input_fn():
  x = np.random.random((1024, 10))
  y = np.random.randint(2, size=(1024, 1))
  x = tf.cast(x, tf.float32)
  dataset = tf.data.Dataset.from_tensor_slices((x, y))
  dataset = dataset.repeat(10)
  dataset = dataset.batch(32)
  return dataset
```

接下来，创建一个[*tf.estimator.RunConfig*](https://www.tensorflow.org/api_docs/python/tf/estimator/RunConfig)并将*train_distribute*参数设置为*tf.contrib.distribute.MirroredStrategy*。 创建MirroredStrategy时，可以指定设备列表或设置num_gpus参数。 默认使用所有可用的GPU，如下所示：

```python
strategy = tf.contrib.distribute.MirroredStrategy()
config = tf.estimator.RunConfig(train_distribute=strategy)
```

将Keras模型转换为*tf.estimator.Estimator*实例：

```python
keras_estimator = tf.keras.estimator.model_to_estimator(
  keras_model=model,
  config=config,
  model_dir='/tmp/model_dir')
```

最后，通过提供input_fn和steps参数来训练Estimator实例：

```python
keras_estimator.train(input_fn=input_fn, steps=10)
```






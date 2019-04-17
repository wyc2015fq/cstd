# 终于！Keras官方中文版文档正式发布了 - 知乎
# 



> 今年 1 月 12 日，Keras 作者 François Chollet‏ 在推特上表示因为中文读者的广泛关注，他已经在 GitHub 上展开了一个 Keras 中文文档项目。而昨日，François Chollet‏ 再一次在推特上表示 Keras 官方文档已经基本完成！他非常感谢翻译和校对人员两个多月的不懈努力，也希望 Keras 中文使用者能继续帮助提升文档质量。

这一次发布的是 Keras 官方中文文档，它得到了严谨的校对而提升了整体质量。但该项目还在进行中，虽然目前已经上线了很多 API 文档和使用教程，但仍然有一部分内容没有完成。其实早在官方中文文档出现以前，就有开发者构建了 Keras 的中文文档，而且很多读者都在使用 MoyanZitto 等人构建的中文文档。
- Keras 官方文档：[https://keras.io/zh/](https://link.zhihu.com/?target=https%3A//keras.io/zh/)
- Keras 第三方文档：[http://keras-cn.readthedocs.io/en/latest/](https://link.zhihu.com/?target=http%3A//keras-cn.readthedocs.io/en/latest/)

以下我们将简要介绍这次官方发布的 Keras 文档。
![](https://pic4.zhimg.com/v2-68ff850e089a47eef2b159d1d76d62ab_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='313'></svg>)
Keras 是一个用 Python 编写的高级神经网络 API，它能够以 TensorFlow、CNTK、或者 Theano 作为后端运行。Keras 的开发重点是支持快速的实验。能够以最小的时延把你的想法转换为实验结果，是做好研究的关键。




如果你有如下需求，请选择 Keras：
- 允许简单而快速的原型设计（用户友好，高度模块化，可扩展性）。
- 同时支持卷积神经网络和循环神经网络，以及两者的组合。
- 在 CPU 和 GPU 上无缝运行与切换。

Keras 兼容的 Python 版本: Python 2.7-3.6。

Keras 相对于其它深度学习库非常容易构建：首先它提供一致和简单的 API；其次，它提供独立的、完全可配置的模块构成序列或图表以完成模型；最后，作为新的类和函数，新的模块很容易扩展。这样说可能比较抽象，但正如文档中所描述的，我们甚至在 30 秒就能快速上手 Keras。所以在坑外徘徊或准备入坑 Keras 的小伙伴可以开心地开始你们的 30 秒。




## **快速开始：30 秒上手 Keras**

Keras 的核心数据结构是 model，一种组织网络层的方式。最简单的模型是 Sequential 模型，它是由多网络层线性堆叠的栈。对于更复杂的结构，你应该使用 Keras 函数式 API，它允许构建任意的神经网络图。

Sequential 模型如下所示：


```
from keras.models import Sequential

model = Sequential()
```


可以简单地使用 .add() 来堆叠模型：


```
from keras.layers import Dense

model.add(Dense(units=64, activation='relu', input_dim=100))
model.add(Dense(units=10, activation='softmax'))
```


在完成了模型的构建后, 可以使用 .compile() 来配置学习过程：


```
model.compile(loss='categorical_crossentropy',
              optimizer='sgd',
              metrics=['accuracy'])
```


如果需要，你还可以进一步地配置优化器。Keras 的一个核心原则是使事情变得相当简单，同时又允许用户在需要的时候能够进行完全的控制（终极的控制是源代码的易扩展性）。


```
model.compile(loss=keras.losses.categorical_crossentropy,
              optimizer=keras.optimizers.SGD(lr=0.01, momentum=0.9, nesterov=True))
```


现在，你可以批量地在训练数据上进行迭代了：


```
# x_train and y_train are Numpy arrays --just like in the Scikit-Learn API.
model.fit(x_train, y_train, epochs=5, batch_size=32)
```


或者，你可以手动地将批次的数据提供给模型：

`model.train_on_batch(x_batch, y_batch)`

只需一行代码就能评估模型性能：

`loss_and_metrics = model.evaluate(x_test, y_test, batch_size=128)`

或者对新的数据生成预测：

`classes = model.predict(x_test, batch_size=128)`

构建一个问答系统，一个图像分类模型，一个神经图灵机，或者其他的任何模型，就是这么的快。深度学习背后的思想很简单，那么它们的实现又何必要那么痛苦呢？




## **使用简介**

Keras 模型的使用一般可以分为顺序模型（Sequential）和 Keras 函数式 API，顺序模型是多个网络层的线性堆叠，而 Keras 函数式 API 是定义复杂模型（如多输出模型、有向无环图，或具有共享层的模型）的方法。以下将简要介绍两种模型的使用方法：




**1.Keras 顺序模型**

你可以通过将层的列表传递给 Sequential 的构造函数，来创建一个 Sequential 模型：


```
from keras.models import Sequential
from keras.layers import Dense, Activation

model = Sequential([
    Dense(32, input_shape=(784,)),
    Activation('relu'),
    Dense(10),
    Activation('softmax'),
])
```


也可以使用 .add() 方法将各层添加到模型中：


```
model = Sequential()
model.add(Dense(32, input_dim=784))
model.add(Activation('relu'))
```


如下展示了一个完整的模型，即基于多层感知器 (MLP) 的 softmax 多分类：
![](https://pic4.zhimg.com/v2-bbdfce1f18752ded53f761f12f2bb5cf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='397'></svg>)

```
import keras
from keras.models import Sequential
from keras.layers import Dense, Dropout, Activation
from keras.optimizers import SGD

# 生成虚拟数据
import numpy as np
x_train = np.random.random((1000, 20))
y_train = keras.utils.to_categorical(np.random.randint(10, size=(1000, 1)), num_classes=10)
x_test = np.random.random((100, 20))
y_test = keras.utils.to_categorical(np.random.randint(10, size=(100, 1)), num_classes=10)

model = Sequential()
# Dense(64) 是一个具有 64 个隐藏神经元的全连接层。
# 在第一层必须指定所期望的输入数据尺寸，在这里是一个 20 维的向量。
model.add(Dense(64, activation='relu', input_dim=20))
model.add(Dropout(0.5))
model.add(Dense(64, activation='relu'))
model.add(Dropout(0.5))
model.add(Dense(10, activation='softmax'))

sgd = SGD(lr=0.01, decay=1e-6, momentum=0.9, nesterov=True)
model.compile(loss='categorical_crossentropy',
              optimizer=sgd,
              metrics=['accuracy'])

model.fit(x_train, y_train,
          epochs=20,
          batch_size=128)
score = model.evaluate(x_test, y_test, batch_size=128)
```





**2. Keras 函数式 API **

利用函数式 API，可以轻易地重用训练好的模型：可以将任何模型看作是一个层，然后通过传递一个张量来调用它。注意，在调用模型时，您不仅重用模型的结构，还重用了它的权重。

以下是函数式 API 的一个很好的例子：具有多个输入和输出的模型。函数式 API 使处理大量交织的数据流变得容易。

来考虑下面的模型。我们试图预测 Twitter 上的一条新闻标题有多少转发和点赞数。模型的主要输入将是新闻标题本身，即一系列词语，但是为了增添趣味，我们的模型还添加了其他的辅助输入来接收额外的数据，例如新闻标题的发布的时间等。该模型也将通过两个损失函数进行监督学习。较早地在模型中使用主损失函数，是深度学习模型的一个良好正则方法。

模型结构如下图所示：
![](https://pic2.zhimg.com/v2-e12b2f7870a10ebb5bb48f87912c8c55_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='697' height='861'></svg>)
让我们用函数式 API 来实现它（详细解释请查看中文文档）：


```
from keras.layers import Input, Embedding, LSTM, Dense
from keras.models import Model

# 标题输入：接收一个含有 100 个整数的序列，每个整数在 1 到 10000 之间。
# 注意我们可以通过传递一个 `name` 参数来命名任何层。
main_input = Input(shape=(100,), dtype='int32', name='main_input')

# Embedding 层将输入序列编码为一个稠密向量的序列，每个向量维度为 512。
x = Embedding(output_dim=512, input_dim=10000, input_length=100)(main_input)

# LSTM 层把向量序列转换成单个向量，它包含整个序列的上下文信息
lstm_out = LSTM(32)(x)

auxiliary_output = Dense(1, activation='sigmoid', name='aux_output')(lstm_out)

auxiliary_input = Input(shape=(5,), name='aux_input')
x = keras.layers.concatenate([lstm_out, auxiliary_input])

# 堆叠多个全连接网络层
x = Dense(64, activation='relu')(x)
x = Dense(64, activation='relu')(x)
x = Dense(64, activation='relu')(x)

# 最后添加主要的逻辑回归层
main_output = Dense(1, activation='sigmoid', name='main_output')(x)

model = Model(inputs=[main_input, auxiliary_input], outputs=[main_output, auxiliary_output])

model.compile(optimizer='rmsprop', loss='binary_crossentropy',
              loss_weights=[1., 0.2])


model.fit([headline_data, additional_data], [labels, labels],
          epochs=50, batch_size=32)

model.compile(optimizer='rmsprop',
              loss={'main_output': 'binary_crossentropy', 'aux_output': 'binary_crossentropy'},
              loss_weights={'main_output': 1., 'aux_output': 0.2})

# 然后使用以下方式训练：
model.fit({'main_input': headline_data, 'aux_input': additional_data},
          {'main_output': labels, 'aux_output': labels},
          epochs=50, batch_size=32)
```





以上只是一个简单的案例，Keras 函数式 API 还有非常多的应用案例，包括层级共享、有向无环图和残差网络等顶尖视觉模型，读者可以继续阅读中文文档了解更多。

文档的后一部分更多是描述 Keras 中常用的函数与 API，包括 Keras 模型、层级函数、预处理过程、损失函数、最优化方法、数据集和可视化等。这些 API 和对应实现的功能其实很多时候可以在实际使用的时候再查找，当然最基本的 API 我们还是需要了解的。以下将简要介绍 Keras 模型和层级 API，其它的模块请查阅原中文文档。




## **Keras 模型**

在 Keras 中有两类模型，顺序模型 和 使用函数式 API 的 Model 类模型。这些模型有许多共同的方法：
- model.summary(): 打印出模型概述信息。它是 utils.print_summary 的简捷调用。
- model.get_config(): 返回包含模型配置信息的字典。通过以下代码，就可以根据这些配置信息重新实例化模型：


```
config = model.get_config()
model = Model.from_config(config)
# or, for Sequential:
model = Sequential.from_config(config)
```

- model.get_weights(): 返回模型权重的张量列表，类型为 Numpy array。
- model.set_weights(weights): 从 Nympy array 中为模型设置权重。列表中的数组必须与 get_weights() 返回的权重具有相同的尺寸。
- model.to_json(): 以 JSON 字符串的形式返回模型的表示。请注意，该表示不包括权重，只包含结构。你可以通过以下代码，从 JSON 字符串中重新实例化相同的模型（带有重新初始化的权重）：


```
from keras.models import model_from_json

json_string = model.to_json()
model = model_from_json(json_string)
```

- model.to_yaml(): 以 YAML 字符串的形式返回模型的表示。请注意，该表示不包括权重，只包含结构。你可以通过以下代码，从 YAML 字符串中重新实例化相同的模型（带有重新初始化的权重）：


```
from keras.models import model_from_yaml

yaml_string = model.to_yaml()
model = model_from_yaml(yaml_string)
```

- model.save_weights(filepath): 将模型权重存储为 HDF5 文件。
- model.load_weights(filepath, by_name=False): 从 HDF5 文件（由 save_weights 创建）中加载权重。默认情况下，模型的结构应该是不变的。如果想将权重载入不同的模型（部分层相同），设置 by_name=True 来载入那些名字相同的层的权重。




## **Keras 层级**

所有 Keras 层都有很多共同的函数：
- layer.get_weights(): 以 Numpy 矩阵的形式返回层的权重。
- layer.set_weights(weights): 从 Numpy 矩阵中设置层的权重（与 get_weights 的输出形状相同）。
- layer.get_config(): 返回包含层配置的字典。此图层可以通过以下方式重置：


```
layer = Dense(32)
config = layer.get_config()
reconstructed_layer = Dense.from_config(config)
```


如果一个层具有单个节点 (i.e. 如果它不是共享层), 你可以得到它的输入张量，输出张量，输入尺寸和输出尺寸：
- layer.input
- layer.output
- layer.input_shape
- layer.output_shape

如果层有多个节点，您可以使用以下函数：
- layer.get_input_at(node_index)
- layer.get_output_at(node_index)
- layer.get_input_shape_at(node_index)
- layer.get_output_shape_at(node_index)

这些是 Keras 模型与层级基本的函数，文档的中心内容也是这一部分和下面描述的 API 用途与参数，它包括完整模型所需要的各个模块，包括数据、预处理、网络架构、训练、评估和可视化等。但这一部分我们并不会介绍，因为很多时候我们只有在遇到未知的函数时才会详细查阅。

Keras 官方中文文档，欢迎各位徘徊者入坑。



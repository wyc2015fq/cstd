# Keras实例教程(3) - 白马负金羁 - CSDN博客





2018年06月16日 12:12:49[白马负金羁](https://me.csdn.net/baimafujinji)阅读数：5634








我们在之前的Keras教程中介绍了用Sequential model的形式来搭建神经网络模型的基本方法。然而，Keras中还提供了另外一种基于函数式编程思想的神经网络组建方法，我们称其为functional API。如果你对类似Haskell这样的函数式编程语言比较熟悉的话，那么上手Keras中的functional API是非常容易的。更重要的是，functional API允许你在Keras中以极其简便且直观的方法实现相当复杂的、定制化的神经网络结构。




作为开始，我们用functional API来搭建一个densely-connected network（或称为全连接网络fully-connected network）。当然，根据本系列教程中【1~2】中所介绍的内容，搭建全连接网络，使用Sequential model才应该是最简单的方法。但我们从这个大家已经掌握了的模型切入，更容易通过对比来揭示functional API方法的一些特点。




考虑到读者可能未必熟悉Haskell，这里试图从另外一种对函数式编程提供了一定支持的语言——R中提取一些可以借鉴的东西。粗略来说，R中所有的操作都是函数化的，即使是那些在别的语言（例如C或Java）中通常被视为运算符的加号“+”在R中也是一个函数。所以在R中如果想计算3+4的值，除了在控制台的命令提示符后面直接输入表达式3+4以外，你还可以使用`+`(1, 2)这样的语法。此处，`+`是函数名，而后面括号中给出的则是参数列表。




在Keras中，每个layer instance 都可以被看成是一个函数，其输入是一个tensor，输出也是一个tensor。例如在下面这个实现全连接网络的例子中，你可以看到第一个Dense层的输入是inputs，其输出是x，而且这个x又被当做是第二个Dense层的输入。最初的输入tensor和最后的输出tensor共同定义了模型。而模型的训练方法则跟Sequential model中的情况一致。

```python
from keras.layers import Input, Dense
from keras.models import Model

# This returns a tensor
inputs = Input(shape=(784,))

# a layer instance is callable on a tensor, and returns a tensor
x = Dense(64, activation='relu')(inputs)
x = Dense(64, activation='relu')(x)
predictions = Dense(10, activation='softmax')(x)

# This creates a model that includes
# the Input layer and three Dense layers
model = Model(inputs=inputs, outputs=predictions)
model.compile(optimizer='rmsprop',
              loss='categorical_crossentropy',
              metrics=['accuracy'])
model.fit(data, labels)  # starts training
```

前面我们讲到在Keras中，每个layer instance都可以被看成是一个函数，不仅如此，在Keras中，所有的model也都可以被看成是函数，这样也就使得模型复用变得很方便。例如，其中的model就是前面刚刚建好的模型：

```python
x = Input(shape=(784,))
# This works, and returns the 10-way softmax we defined above.
y = model(x)
```

在接下来的篇幅里，我们将介绍几个利用functional API实现的常用的网络结构，而且你可以看到单纯使用Sequential model来搭建它们其实是很不容易的，但functional API却使得这样一个过程变得相当轻松。




##### “多输入-多输出”模型




考虑下图所示的这个网络模型，注意到它有两个输入x_in和y_in，以及两个输出x_out和y_out，在中间有一个Merge层，将两路输入融合在一起。Keras中为一类特殊的layer提供了API，这类layer就是所谓的Merge Layers，你可以参阅相关文档【2】来了解可用的融合方法。


![](https://img-blog.csdn.net/2018061602183264?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2JhaW1hZnVqaW5qaQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


而在本例中所使用的融合方法是concatenate，即简单相连，也就是图中示意的情形。这时的API为

- keras.layers.concatenate(inputs, axis=-1)


来看一下具体的代码实现：


```python
from keras.layers import concatenate

x_in = Input(shape=(100,), name='x_in')
y_in = Input(shape=(100,), name='y_in')

# a layer instance is callable on a tensor, and returns a tensor
x = Dense(64, activation='relu')(x_in)
y = Dense(64, activation='relu')(y_in)

z = concatenate([x, y])

x = Dense(1, activation='sigmoid', name='x_out')(z)
y = Dense(10, activation='softmax', name='y_out')(z)
```

要定义一个多输入或多输出的模型， you just need to specify a list （containing inputs and outputs）:


```python
model = Model(inputs=[x_in, y_in], outputs=[x, y])

model.summary()
```
下面输入的模型概况和之前给出的图示是吻合的，可见我们的创建是正确的。

![](https://img-blog.csdn.net/20180616020258140?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2JhaW1hZnVqaW5qaQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


接下来进入模型compile的阶段，具体来说你可以有两个选择，其一是by passing in lists of losses and loss weights:


```python
from keras.utils import to_categorical

import numpy as np
data = np.random.random((1000, 100))
xs = np.random.randint(2, size=(1000, 1))
ys = np.random.randint(10, size=(1000, 1))

model.compile(optimizer='rmsprop', loss=['binary_crossentropy', 'categorical_crossentropy'],
              loss_weights=[1., 0.2])

model.fit([data, data], [xs, to_categorical(ys)],
          epochs=1, batch_size=32)
```
其二，你也可以使用字典 (refering to the names of the output tensors)：

```python
model.compile(optimizer='rmsprop',
              loss={'x_out': 'binary_crossentropy', 'y_out': 'categorical_crossentropy'},
              loss_weights={'x_out': 1., 'y_out': 0.2})

# And trained it via:
model.fit({'x_in': data, 'y_in': data},
          {'x_out': xs, 'y_out': to_categorical(ys)},
          epochs=1, batch_size=32)
```

除了【2】中已经定义好的一些融合层实现方法，如果你想自己定义一些融合层该怎么办呢？例如，下面给出的是求exp(-|| left-right ||)的函数，其中|| left-right ||表示 left和right 之间的曼哈顿距离。


```python
def exponent_neg_manhattan_distance(left, right):
    return K.exp(-K.sum(K.abs(left-right), axis=1, keepdims=True))
```

Keras中提供有一个Merge层，因为它同样是基于函数式编程来实现自定义的layer的，所以如果要将两路输入 left和right 以上面自定义的融合方法exponent_neg_manhattan_distance来加以融合，那么你可以采用下面的代码：


```python
malstm_distance = Merge(mode=lambda x: exponent_neg_manhattan_distance(x[0], x[1]), \
                        output_shape=lambda x: (x[0][0], 1))([left_output, right_output])
```

然而，你可能会得到如下这样的一个警告：UserWarning: The `Merge` layer is deprecated and will be removed after 08/2017. Use instead layers from `keras.layers.merge`, e.g. `add`, `concatenate`, etc. 也就是说Merge已经被废弃了，那么如何在没有Merge的情况下实现上面的功能呢？此时，你可以使用Lambda层，其定义如下：
- keras.layers.Lambda(function, output_shape=None, mask=None, arguments=None)


你也可以参考文献【3】来了解关于Lambda层的更多细节。下面给出的是用Lambda改写之后的，融合层实现方法：


```python
def exponent_neg_manhattan_distance(v):
    return K.exp(-K.sum(K.abs(v[0]-v[1]), axis=1, keepdims=True))

def out_shape(shapes):
    return (shapes[0][0],1)

malstm_distance = Lambda(exponent_neg_manhattan_distance, output_shape=out_shape)([left_output, right_output])
```


##### 共享层（Shared layers）




另外一个可以展现functional API强大能力的例子就是shared layers的实现。一方面shared layers的引入会大大缩减模型中所需的权重数量，另一方面，在某些特殊的网络结构中（例如Siamese），共享层也是必须的。但是，在Tensorflow中编写shared layers是比较麻烦的，好在我们有Keras中的functional API。来看一个具体的例子：


```python
tweet_a = Input(shape=(280, 256))
tweet_b = Input(shape=(280, 256))

# This layer can take as input a matrix
# and will return a vector of size 64
shared_lstm = LSTM(64)

# When we reuse the same layer instance
# multiple times, the weights of the layer
# are also being reused
# (it is effectively *the same* layer)
encoded_a = shared_lstm(tweet_a)
encoded_b = shared_lstm(tweet_b)

# We can then concatenate the two vectors:
merged_vector = keras.layers.concatenate([encoded_a, encoded_b], axis=-1)

# And add a logistic regression on top
predictions = Dense(1, activation='sigmoid')(merged_vector)

# We define a trainable model linking the
# tweet inputs to the predictions
model = Model(inputs=[tweet_a, tweet_b], outputs=predictions)
```

如果用图形来展示搭建的神经网络，则有下图，其中[tweet_a→encoded_a]和[tweet_b→encoded_b]共享一组相同的权重，也就是同一个 shared layer。


![](https://img-blog.csdn.net/20180616040142518?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2JhaW1hZnVqaW5qaQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


##### 层“节点”的概念

你可以把层（layer）看成是连接input tensor 与 output tensor 的节点。每次调用一个layer时，都相当于是添加了一个node。只要这个layer只接入了一个input，就不会有混淆，那么此时调用 .output 方法就会返回该层的输出，所以下面的代码是可以正确执行的。


```python
a = Input(shape=(280, 256))

lstm = LSTM(32)
encoded_a = lstm(a)

assert lstm.output == encoded_a
```
但对于本文前面介绍的那种多输入的模型来说，就会出现问题，例如：

```python
a = Input(shape=(280, 256))
b = Input(shape=(280, 256))

lstm = LSTM(32)
encoded_a = lstm(a)
encoded_b = lstm(b)

lstm.output
```

执行结果会导致下面的错误提示：


>> AttributeError: Layer lstm_1 has multiple inbound nodes,
hence the notion of "layer output" is ill-defined.

Use `get_output_at(node_index)` instead.

需要指出的是When you are calling the same layer multiple times, that layer owns multiple nodes indexed as 0, 1, 2... 所以你只要把代码改成像下面这样就没问题了。


```python
assert lstm.get_output_at(0) == encoded_a
assert lstm.get_output_at(1) == encoded_b
```
最后，一个介绍Keras中函数式API的视频可以从链接【5】中访问到，供有兴趣的读者参考。







##### 参考文献

【1】https://keras.io/getting-started/functional-api-guide/

【2】https://keras.io/layers/merge/

【3】https://keras.io/layers/core/#lambda

【4】[参考代码](https://github.com/knathanieltucker/a-bit-of-deep-learning-and-keras/blob/master/notebooks/Functional.ipynb)


【5】[视频链接](https://www.youtube.com/watch?v=elOLCEJV-dc&t=57s)








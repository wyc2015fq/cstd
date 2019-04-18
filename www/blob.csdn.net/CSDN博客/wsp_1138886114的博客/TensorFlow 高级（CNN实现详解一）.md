# TensorFlow 高级（CNN实现详解一） - wsp_1138886114的博客 - CSDN博客





2018年08月05日 13:08:10[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：980








## Tensorflow 实现 CNN

```
TensorFlow使用计算图模型来构建神经网络。其主要流程是先建立好整个网络的计算图模型，然后再导入数据进行计算。
一个TensorFlow计算图包含以下几个部分：
  ● Placeholder: 占位符，用来读取用户输入与输出；
  ● Variable:    模型的变量，也称为参数，在计算过程中逐步优化；
  ● Model:       使用的神经网络模型，也可以使用一些简单的计算；
  ● Cost Function： 代价函数，也称损失函数，如何计算模型的误差；
  ● Optimizer：  优化器，使用哪种优化策略来降低损失。
```

#### 1、创建变量

> 
```
卷积神经网络中有两类变量，权重和偏置项。以下为初始化这两种变量的函数，其中对权重参数采用随机生成其符合 
正态分布的随机值，对偏置项初始化为常量0.05。
```

```python
def new_weights(shape):  
    return tf.Variable(tf.truncated_normal(shape, stddev=0.05))    # 初始化为随机值

def new_biases(length):
    return tf.Variable(tf.constant(0.05, shape=[length]))          # 初始化为常数
```


#### 2 创建卷积

> 
```
这个函数创建了一个卷积层。输入为4维的tensor，维度如下：
图像数量
图像高度
图像宽度
通道数
输出同样是一个4维的tensor，维度如下：
图像数量，与输入相同
图像高度，如果使用2x2 pooling，高宽都除以2
图像宽度，同上
由卷积层生成的通道数
```

```python
def new_conv_layer(input,             # 前一层.
                  num_input_channels, # 前一层通道数
                  filter_size,        # 卷积核尺寸
                  num_filters,        # 卷积核数目
                  use_pooling=True):  # 使用 2x2 max-pooling.

    # 卷积核权重的形状，由TensorFlow API决定
    shape = [filter_size, filter_size, num_input_channels, num_filters]

    # 根据跟定形状创建权重
    weights = new_weights(shape=shape)

    # 创建新的偏置，每个卷积核一个偏置
    biases = new_biases(length=num_filters)

    # 创建卷积层。注意stride全设置为1。
    # 第1个和第4个必须是1，因为第1个是图像的数目，第4个是图像的通道。
    # 第2和第3指定和左右、上下的步长。
    # padding设置为'SAME' 意味着给图像补零，以保证前后像素相同。
    layer = tf.nn.conv2d(input=input,
                        filter=weights,
                        strides=[1, 1, 1, 1],
                        padding='SAME')

    # 给卷积层的输出添加一个偏置，每个卷积通道一个偏置值
    layer += biases

    # 是否使用pooling
    if use_pooling:
        # 这是 2x2 max-pooling, 表明使用 2x2 的窗口，选择每一窗口的最大值作为该窗口的像素，
        # 然后移动2格到下一窗口。
        layer = tf.nn.max_pool(value=layer,
                              ksize=[1, 2, 2, 1],
                              strides=[1, 2, 2, 1],
                              padding='SAME')

    # Rectified Linear Unit (ReLU).
    # 对每个输入像素x，计算 max(x, 0)，把负数的像素值变为0.
    # 这一步为原输出添加了一定的非线性特性，允许我们学习更加复杂的函数。
    layer = tf.nn.relu(layer)

    # 注意 relu 通常在pooling前执行，但是由于 relu(max_pool(x)) == max_pool(relu(x))，
    # 我们可以通过先max_pooling再relu省去75%的计算。

    # 返回结果层和权重，结果层用于下一层输入，权重用于显式输出
    return layer, weights
```


#### 3 展平操作

> 
一个卷积层的输出为4维度的tensor。我们需要在卷积层后添加一个全连接层，首先得将4维的  

      tensor展平为2维的tensor，这样才能直接输入到全连接层。 

```python
def flatten_layer(layer):
    # 获取输入层的形状，
    # layer_shape == [num_images, img_height, img_width, num_channels]
    layer_shape = layer.get_shape()

    # 特征数量: img_height * img_width * num_channels
    # 可以使用TensorFlow内建操作计算.
    num_features = layer_shape[1:4].num_elements()

    # 将形状重塑为 [num_images, num_features].
    # 注意只设定了第二个维度的尺寸为num_filters，第一个维度为-1，保证第一个维度num_images不变
    # 展平后的层的形状为:
    # [num_images, img_height * img_width * num_channels]
    layer_flat = tf.reshape(layer, [-1, num_features])

    return layer_flat, num_features
```


### 4 创建全连接层

```python
def new_fc_layer(input,         # 前一层.
                num_inputs,     # 前一层输入维度
                num_outputs,    # 输出维度
                use_relu=True): # 是否使用RELU

    # 新的权重和偏置.
    weights = new_weights(shape=[num_inputs, num_outputs])
    biases = new_biases(length=num_outputs)

    # 计算 y = wx + b
    layer = tf.matmul(input, weights) + biases

    # 是否使用RELU
    if use_relu:
        layer = tf.nn.relu(layer)

    return layer
```

### 5 定义占位符

> 
```
占位符(Placeholder)为输入与输出占据位置，这些输入输出一般在不同的轮次都会有所变化。
由于TensorFlow先构图再计算，所以需要使用占位符为输入和输出预留位置。
```

```bash
# 原始输入
x = tf.placeholder(tf.float32, shape=[None, img_size_flat], name='x')          

 # 转换为2维图像
x_image = tf.reshape(x, [-1, img_size, img_size, num_channels])                 

 # 原始输出
y_true = tf.placeholder(tf.float32, shape=[None, num_classes], name='y_true') 

 # 转换为真实类别，与之前的使用placeholder不同
y_true_cls = tf.argmax(y_true, axis=1)
```


### 6 定义卷积层

> 
###### 卷积层1

```php
layer_conv1, weights_conv1 = \
    new_conv_layer(input=x_image,                    # 输入图像
                  num_input_channels=num_channels,   # 输入通道数
                  filter_size=filter_size1,          # 卷积核尺寸
                  num_filters=num_filters1,          # 卷积核数目
                  use_pooling=True)
print(layer_conv1)
Tensor("Relu:0", shape=(?, 14, 14, 16), dtype=float32)
输入为(?, 28, 28, 1)的图像，其中?为图像数量。可以看到，第一个卷积层的输入为(?, 14, 14, 16)的tensor， 
即14x14像素的16通道图像。
```

###### 卷积层2

```
layer_conv2, weights_conv2 = \
    new_conv_layer(input=layer_conv1,
                   num_input_channels=num_filters1,
                   filter_size=filter_size2,
                   num_filters=num_filters2,
                   use_pooling=True)
print(layer_conv2)
Tensor("Relu_1:0", shape=(?, 7, 7, 36), dtype=float32)
```


#### 7 定义展平层

```
展平层将第二个卷积层展平为二维tensor。

layer_flat, num_features = flatten_layer(layer_conv2)
print(layer_flat)
Tensor("Reshape_1:0", shape=(?, 1764), dtype=float32)
```

#### 8 定义全连接层

> 
###### 全连接层1

```php
layer_fc1 = new_fc_layer(input=layer_flat,          # 展平层输出
                         num_inputs=num_features,   # 输入特征维度
                         num_outputs=fc_size,       # 输出特征维度
                         use_relu=True)
print(layer_fc1)
Tensor("Relu_2:0", shape=(?, 128), dtype=float32)
```

##### 全连接层2

```php
layer_fc2 = new_fc_layer(input=layer_fc1,           # 上一全连接层
                         num_inputs=fc_size,        # 输入特征维度
                         num_outputs=num_classes,   # 输出类别数
                         use_relu=False)
print(layer_fc2)
Tensor("add_4:0", shape=(?, 10), dtype=float32)
输出为(?, 10)的二维tensor，意在判定输入图像属于哪一类, 注意该层未使用relu， 
因为将要输入到后续的softmax中。
```


### 9 预测类别

```
第二个全连接层估计输入的图像属于某一类别的程度，这个估计有些粗糙， 
需要添加一个softmax层归一化为概率表示。

y_pred = tf.nn.softmax(layer_fc2)              # softmax归一化
y_pred_cls = tf.argmax(y_pred, axis=1)         # 真实类别
```

### 10 定义代价函数

```
这里以交叉熵为代价函数或损失函数。

cross_entropy = tf.nn.softmax_cross_entropy_with_logits(logits=layer_fc2,labels=y_true)
cost = tf.reduce_mean(cross_entropy)
```

### 11 选择优化方法

```
使用自适应的梯度下降优化法，Adam。
optimizer = tf.train.AdamOptimizer(learning_rate=1e-4).minimize(cost)
```

### 12 模型性能度量

```
correct_prediction = tf.equal(y_pred_cls, y_true_cls)
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
```




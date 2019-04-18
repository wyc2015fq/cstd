# 【Tensorflow】tf.contrib.slim - YZXnuaa的博客 - CSDN博客
2018年03月29日 11:37:56[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：1616
可能很多tensorflow的老版本玩家没见过这个东西，slim这个模块是在16年新推出的，其主要目的是来做所谓的“代码瘦身”。
但事实上它已经成为我比较喜欢，甚至是比较常用的模块，github上面大部分tensorflow的工程都会涉及到它，不得不说，撇开Keras，TensorLayer，tfLearn这些个高级库不谈，光用tensorflow能不能写出简洁的代码？当然行，有slim就够了！
惟一的缺点是slim这玩意的中文的文档几乎绝迹。所以国内还是Keras，tensorLayer这些官方文档比较完备的高级库的天下。
## 一.简介
slim被放在tensorflow.contrib这个库下面，导入的方法如下：
[?](https://www.2cto.com/kf/201706/649266.html#)
```
```java
import
```
```java
tensorflow.contrib.slim as slim
```
```
这样我们就可以使用slim了，既然说到了，先来扒一扒tensorflow.contrib这个库，tensorflow官方对它的描述是：此目录中的任何代码未经官方支持，可能会随时更改或删除。每个目录下都有指定的所有者。它旨在包含额外功能和贡献，最终会合并到核心TensorFlow中，但其接口可能仍然会发生变化，或者需要进行一些测试，看是否可以获得更广泛的接受。所以slim依然不属于原生tensorflow。
那么什么是slim？slim到底有什么用？
slim是一个使构建，训练，评估神经网络变得简单的库。它可以消除原生tensorflow里面很多重复的模板性的代码，让代码更紧凑，更具备可读性。另外slim提供了很多计算机视觉方面的著名模型（VGG, AlexNet等），我们不仅可以直接使用，甚至能以各种方式进行扩展。
slim的子模块及功能介绍：
arg_scope: provides a new scope named arg_scope that allows a user to define default arguments for specific operations within that scope.
除了基本的namescope，variabelscope外，又加了argscope，它是用来控制每一层的默认超参数的。（后面会详细说）
data: contains TF-slim's dataset definition, data providers, parallel_reader, and decoding utilities.
貌似slim里面还有一套自己的数据定义，这个跳过，我们用的不多。
evaluation: contains routines for evaluating models.
评估模型的一些方法，用的也不多
layers: contains high level layers for building models using tensorflow.
这个比较重要，slim的核心和精髓，一些复杂层的定义
learning: contains routines for training models.
一些训练规则
losses: contains commonly used loss functions.
一些loss
metrics: contains popular evaluation metrics.
评估模型的度量标准
nets: contains popular network definitions such as VGG and AlexNet models.
包含一些经典网络，VGG等，用的也比较多
queues: provides a context manager for easily and safely starting and closing QueueRunners.
文本队列管理，比较有用。
regularizers: contains weight regularizers.
包含一些正则规则
variables: provides convenience wrappers for variable creation and manipulation.
这个比较有用，我很喜欢slim管理变量的机制
具体子库就这么多拉，接下来干货时间！
## 二.slim定义模型
slim中定义一个变量的示例：
[?](https://www.2cto.com/kf/201706/649266.html#)
```
```java
# Model Variables
```
```java
weights = slim.model_variable(
```
```java
'weights'
```
```java
,
```
```java
```
```java
shape=[
```
```java
10
```
```java
,
```
```java
10
```
```java
,
```
```java
3
```
```java
,
```
```java
3
```
```java
],
```
```java
```
```java
initializer=tf.truncated_normal_initializer(stddev=
```
```java
0.1
```
```java
),
```
```java
```
```java
regularizer=slim.l2_regularizer(
```
```java
0.05
```
```java
),
```
```java
```
```java
device=
```
```java
'/CPU:0'
```
```java
)
```
```java
model_variables = slim.get_model_variables()
```
```java
# Regular variables
```
```java
my_var = slim.variable(
```
```java
'my_var'
```
```java
,
```
```java
```
```java
shape=[
```
```java
20
```
```java
,
```
```java
1
```
```java
],
```
```java
```
```java
initializer=tf.zeros_initializer())
```
```java
regular_variables_and_model_variables = slim.get_variables()
```
```
如上，变量分为两类：模型变量和局部变量。局部变量是不作为模型参数保存的，而模型变量会再save的时候保存下来。这个玩过tensorflow的人都会明白，诸如global_step之类的就是局部变量。slim中可以写明变量存放的设备，正则和初始化规则。还有获取变量的函数也需要注意一下，get_variables是返回所有的变量。
slim中实现一个层：
首先让我们看看tensorflow怎么实现一个层，例如卷积层：
[?](https://www.2cto.com/kf/201706/649266.html#)
```
```java
input = ...
```
```java
with tf.name_scope(
```
```java
'conv1_1'
```
```java
) as scope:
```
```java
```
```java
kernel = tf.Variable(tf.truncated_normal([
```
```java
3
```
```java
,
```
```java
3
```
```java
,
```
```java
64
```
```java
,
```
```java
128
```
```java
], dtype=tf.float32,
```
```java
```
```java
stddev=1e-
```
```java
1
```
```java
), name=
```
```java
'weights'
```
```java
)
```
```java
```
```java
conv = tf.nn.conv2d(input, kernel, [
```
```java
1
```
```java
,
```
```java
1
```
```java
,
```
```java
1
```
```java
,
```
```java
1
```
```java
], padding=
```
```java
'SAME'
```
```java
)
```
```java
```
```java
biases = tf.Variable(tf.constant(
```
```java
0.0
```
```java
, shape=[
```
```java
128
```
```java
], dtype=tf.float32),
```
```java
```
```java
trainable=True, name=
```
```java
'biases'
```
```java
)
```
```java
```
```java
bias = tf.nn.bias_add(conv, biases)
```
```java
```
```java
conv1 = tf.nn.relu(bias, name=scope)
```
```
然后slim的实现：
[?](https://www.2cto.com/kf/201706/649266.html#)
```
```java
input = ...
```
```java
net = slim.conv2d(input,
```
```java
128
```
```java
, [
```
```java
3
```
```java
,
```
```java
3
```
```java
], scope=
```
```java
'conv1_1'
```
```java
)
```
```
但这个不是重要的，因为tenorflow目前也有大部分层的简单实现，这里比较吸引人的是slim中的repeat和stack操作：
假设定义三个相同的卷积层：
[?](https://www.2cto.com/kf/201706/649266.html#)
```
```java
net = ...
```
```java
net = slim.conv2d(net,
```
```java
256
```
```java
, [
```
```java
3
```
```java
,
```
```java
3
```
```java
], scope=
```
```java
'conv3_1'
```
```java
)
```
```java
net = slim.conv2d(net,
```
```java
256
```
```java
, [
```
```java
3
```
```java
,
```
```java
3
```
```java
], scope=
```
```java
'conv3_2'
```
```java
)
```
```java
net = slim.conv2d(net,
```
```java
256
```
```java
, [
```
```java
3
```
```java
,
```
```java
3
```
```java
], scope=
```
```java
'conv3_3'
```
```java
)
```
```java
net = slim.max_pool2d(net, [
```
```java
2
```
```java
,
```
```java
2
```
```java
], scope=
```
```java
'pool2'
```
```java
)
```
```
在slim中的repeat操作可以减少代码量：
[?](https://www.2cto.com/kf/201706/649266.html#)
```
```java
net = slim.repeat(net,
```
```java
3
```
```java
, slim.conv2d,
```
```java
256
```
```java
, [
```
```java
3
```
```java
,
```
```java
3
```
```java
], scope=
```
```java
'conv3'
```
```java
)
```
```java
net = slim.max_pool2d(net, [
```
```java
2
```
```java
,
```
```java
2
```
```java
], scope=
```
```java
'pool2'
```
```java
)
```
```
而stack是处理卷积核或者输出不一样的情况：
假设定义三层FC：
[?](https://www.2cto.com/kf/201706/649266.html#)
```
```java
# Verbose way:
```
```java
x = slim.fully_connected(x,
```
```java
32
```
```java
, scope=
```
```java
'fc/fc_1'
```
```java
)
```
```java
x = slim.fully_connected(x,
```
```java
64
```
```java
, scope=
```
```java
'fc/fc_2'
```
```java
)
```
```java
x = slim.fully_connected(x,
```
```java
128
```
```java
, scope=
```
```java
'fc/fc_3'
```
```java
)
```
```
使用stack操作：
[?](https://www.2cto.com/kf/201706/649266.html#)
```
```java
slim.stack(x, slim.fully_connected, [
```
```java
32
```
```java
,
```
```java
64
```
```java
,
```
```java
128
```
```java
], scope=
```
```java
'fc'
```
```java
)
```
```
同理卷积层也一样：
[?](https://www.2cto.com/kf/201706/649266.html#)
```
```java
# 普通方法:
```
```java
x = slim.conv2d(x,
```
```java
32
```
```java
, [
```
```java
3
```
```java
,
```
```java
3
```
```java
], scope=
```
```java
'core/core_1'
```
```java
)
```
```java
x = slim.conv2d(x,
```
```java
32
```
```java
, [
```
```java
1
```
```java
,
```
```java
1
```
```java
], scope=
```
```java
'core/core_2'
```
```java
)
```
```java
x = slim.conv2d(x,
```
```java
64
```
```java
, [
```
```java
3
```
```java
,
```
```java
3
```
```java
], scope=
```
```java
'core/core_3'
```
```java
)
```
```java
x = slim.conv2d(x,
```
```java
64
```
```java
, [
```
```java
1
```
```java
,
```
```java
1
```
```java
], scope=
```
```java
'core/core_4'
```
```java
)
```
```java
# 简便方法:
```
```java
slim.stack(x, slim.conv2d, [(
```
```java
32
```
```java
, [
```
```java
3
```
```java
,
```
```java
3
```
```java
]), (
```
```java
32
```
```java
, [
```
```java
1
```
```java
,
```
```java
1
```
```java
]), (
```
```java
64
```
```java
, [
```
```java
3
```
```java
,
```
```java
3
```
```java
]), (
```
```java
64
```
```java
, [
```
```java
1
```
```java
,
```
```java
1
```
```java
])], scope=
```
```java
'core'
```
```java
)
```
```
slim中的argscope：
如果你的网络有大量相同的参数，如下：
[?](https://www.2cto.com/kf/201706/649266.html#)
```
```java
net = slim.conv2d(inputs,
```
```java
64
```
```java
, [
```
```java
11
```
```java
,
```
```java
11
```
```java
],
```
```java
4
```
```java
, padding=
```
```java
'SAME'
```
```java
,
```
```java
```
```java
weights_initializer=tf.truncated_normal_initializer(stddev=
```
```java
0.01
```
```java
),
```
```java
```
```java
weights_regularizer=slim.l2_regularizer(
```
```java
0.0005
```
```java
), scope=
```
```java
'conv1'
```
```java
)
```
```java
net = slim.conv2d(net,
```
```java
128
```
```java
, [
```
```java
11
```
```java
,
```
```java
11
```
```java
], padding=
```
```java
'VALID'
```
```java
,
```
```java
```
```java
weights_initializer=tf.truncated_normal_initializer(stddev=
```
```java
0.01
```
```java
),
```
```java
```
```java
weights_regularizer=slim.l2_regularizer(
```
```java
0.0005
```
```java
), scope=
```
```java
'conv2'
```
```java
)
```
```java
net = slim.conv2d(net,
```
```java
256
```
```java
, [
```
```java
11
```
```java
,
```
```java
11
```
```java
], padding=
```
```java
'SAME'
```
```java
,
```
```java
```
```java
weights_initializer=tf.truncated_normal_initializer(stddev=
```
```java
0.01
```
```java
),
```
```java
```
```java
weights_regularizer=slim.l2_regularizer(
```
```java
0.0005
```
```java
), scope=
```
```java
'conv3'
```
```java
)
```
```
然后我们用arg_scope处理一下：
[?](https://www.2cto.com/kf/201706/649266.html#)
```
```java
with slim.arg_scope([slim.conv2d], padding=
```
```java
'SAME'
```
```java
,
```
```java
```
```java
weights_initializer=tf.truncated_normal_initializer(stddev=
```
```java
0.01
```
```java
)
```
```java
```
```java
weights_regularizer=slim.l2_regularizer(
```
```java
0.0005
```
```java
)):
```
```java
```
```java
net = slim.conv2d(inputs,
```
```java
64
```
```java
, [
```
```java
11
```
```java
,
```
```java
11
```
```java
], scope=
```
```java
'conv1'
```
```java
)
```
```java
```
```java
net = slim.conv2d(net,
```
```java
128
```
```java
, [
```
```java
11
```
```java
,
```
```java
11
```
```java
], padding=
```
```java
'VALID'
```
```java
, scope=
```
```java
'conv2'
```
```java
)
```
```java
```
```java
net = slim.conv2d(net,
```
```java
256
```
```java
, [
```
```java
11
```
```java
,
```
```java
11
```
```java
], scope=
```
```java
'conv3'
```
```java
)
```
```
是不是一下子就变简洁了？这里额外说明一点，arg_scope的作用范围内，是定义了指定层的默认参数，若想特别指定某些层的参数，可以重新赋值（相当于重写），如上倒数第二行代码。那如果除了卷积层还有其他层呢？那就要如下定义：
[?](https://www.2cto.com/kf/201706/649266.html#)
```
```java
with slim.arg_scope([slim.conv2d, slim.fully_connected],
```
```java
```
```java
activation_fn=tf.nn.relu,
```
```java
```
```java
weights_initializer=tf.truncated_normal_initializer(stddev=
```
```java
0.01
```
```java
),
```
```java
```
```java
weights_regularizer=slim.l2_regularizer(
```
```java
0.0005
```
```java
)):
```
```java
```
```java
with slim.arg_scope([slim.conv2d], stride=
```
```java
1
```
```java
, padding=
```
```java
'SAME'
```
```java
):
```
```java
```
```java
net = slim.conv2d(inputs,
```
```java
64
```
```java
, [
```
```java
11
```
```java
,
```
```java
11
```
```java
],
```
```java
4
```
```java
, padding=
```
```java
'VALID'
```
```java
, scope=
```
```java
'conv1'
```
```java
)
```
```java
```
```java
net = slim.conv2d(net,
```
```java
256
```
```java
, [
```
```java
5
```
```java
,
```
```java
5
```
```java
],
```
```java
```
```java
weights_initializer=tf.truncated_normal_initializer(stddev=
```
```java
0.03
```
```java
),
```
```java
```
```java
scope=
```
```java
'conv2'
```
```java
)
```
```java
```
```java
net = slim.fully_connected(net,
```
```java
1000
```
```java
, activation_fn=None, scope=
```
```java
'fc'
```
```java
)
```
```
写两个arg_scope就行了。
采用如上方法，定义一个VGG也就十几行代码的事了。
[?](https://www.2cto.com/kf/201706/649266.html#)
```
```java
def vgg16(inputs):
```
```java
```
```java
with slim.arg_scope([slim.conv2d, slim.fully_connected],
```
```java
```
```java
activation_fn=tf.nn.relu,
```
```java
```
```java
weights_initializer=tf.truncated_normal_initializer(
```
```java
0.0
```
```java
,
```
```java
0.01
```
```java
),
```
```java
```
```java
weights_regularizer=slim.l2_regularizer(
```
```java
0.0005
```
```java
)):
```
```java
```
```java
net = slim.repeat(inputs,
```
```java
2
```
```java
, slim.conv2d,
```
```java
64
```
```java
, [
```
```java
3
```
```java
,
```
```java
3
```
```java
], scope=
```
```java
'conv1'
```
```java
)
```
```java
```
```java
net = slim.max_pool2d(net, [
```
```java
2
```
```java
,
```
```java
2
```
```java
], scope=
```
```java
'pool1'
```
```java
)
```
```java
```
```java
net = slim.repeat(net,
```
```java
2
```
```java
, slim.conv2d,
```
```java
128
```
```java
, [
```
```java
3
```
```java
,
```
```java
3
```
```java
], scope=
```
```java
'conv2'
```
```java
)
```
```java
```
```java
net = slim.max_pool2d(net, [
```
```java
2
```
```java
,
```
```java
2
```
```java
], scope=
```
```java
'pool2'
```
```java
)
```
```java
```
```java
net = slim.repeat(net,
```
```java
3
```
```java
, slim.conv2d,
```
```java
256
```
```java
, [
```
```java
3
```
```java
,
```
```java
3
```
```java
], scope=
```
```java
'conv3'
```
```java
)
```
```java
```
```java
net = slim.max_pool2d(net, [
```
```java
2
```
```java
,
```
```java
2
```
```java
], scope=
```
```java
'pool3'
```
```java
)
```
```java
```
```java
net = slim.repeat(net,
```
```java
3
```
```java
, slim.conv2d,
```
```java
512
```
```java
, [
```
```java
3
```
```java
,
```
```java
3
```
```java
], scope=
```
```java
'conv4'
```
```java
)
```
```java
```
```java
net = slim.max_pool2d(net, [
```
```java
2
```
```java
,
```
```java
2
```
```java
], scope=
```
```java
'pool4'
```
```java
)
```
```java
```
```java
net = slim.repeat(net,
```
```java
3
```
```java
, slim.conv2d,
```
```java
512
```
```java
, [
```
```java
3
```
```java
,
```
```java
3
```
```java
], scope=
```
```java
'conv5'
```
```java
)
```
```java
```
```java
net = slim.max_pool2d(net, [
```
```java
2
```
```java
,
```
```java
2
```
```java
], scope=
```
```java
'pool5'
```
```java
)
```
```java
```
```java
net = slim.fully_connected(net,
```
```java
4096
```
```java
, scope=
```
```java
'fc6'
```
```java
)
```
```java
```
```java
net = slim.dropout(net,
```
```java
0.5
```
```java
, scope=
```
```java
'dropout6'
```
```java
)
```
```java
```
```java
net = slim.fully_connected(net,
```
```java
4096
```
```java
, scope=
```
```java
'fc7'
```
```java
)
```
```java
```
```java
net = slim.dropout(net,
```
```java
0.5
```
```java
, scope=
```
```java
'dropout7'
```
```java
)
```
```java
```
```java
net = slim.fully_connected(net,
```
```java
1000
```
```java
, activation_fn=None, scope=
```
```java
'fc8'
```
```java
)
```
```java
```
```java
return
```
```java
net
```
```
## 三.训练模型
这个没什么好说的，说一下直接拿经典网络来训练吧。
[?](https://www.2cto.com/kf/201706/649266.html#)
```
```java
import
```
```java
tensorflow as tf
```
```java
vgg = tf.contrib.slim.nets.vgg
```
```java
# Load the images and labels.
```
```java
images, labels = ...
```
```java
# Create the model.
```
```java
predictions, _ = vgg.vgg_16(images)
```
```java
# Define the loss functions and get the total loss.
```
```java
loss = slim.losses.softmax_cross_entropy(predictions, labels)
```
```
是不是超级简单？
关于loss,要说一下定义自己的loss的方法，以及注意不要忘记加入到slim中让slim看到你的loss。
还有正则项也是需要手动添加进loss当中的，不然最后计算的时候就不优化正则目标了。
[?](https://www.2cto.com/kf/201706/649266.html#)
```
```java
# Load the images and labels.
```
```java
images, scene_labels, depth_labels, pose_labels = ...
```
```java
# Create the model.
```
```java
scene_predictions, depth_predictions, pose_predictions = CreateMultiTaskModel(images)
```
```java
# Define the loss functions and get the total loss.
```
```java
classification_loss = slim.losses.softmax_cross_entropy(scene_predictions, scene_labels)
```
```java
sum_of_squares_loss = slim.losses.sum_of_squares(depth_predictions, depth_labels)
```
```java
pose_loss = MyCustomLossFunction(pose_predictions, pose_labels)
```
```java
slim.losses.add_loss(pose_loss) # Letting TF-Slim know about the additional loss.
```
```java
# The following two ways to compute the total loss are equivalent:
```
```java
regularization_loss = tf.add_n(slim.losses.get_regularization_losses())
```
```java
total_loss1 = classification_loss + sum_of_squares_loss + pose_loss + regularization_loss
```
```java
# (Regularization Loss is included in the total loss by
```
```java
default
```
```java
).
```
```java
total_loss2 = slim.losses.get_total_loss()
```
```
## 四.读取保存模型变量
通过以下功能我们可以载入模型的部分变量：
[?](https://www.2cto.com/kf/201706/649266.html#)
```
```java
# Create some variables.
```
```java
v1 = slim.variable(name=
```
```java
"v1"
```
```java
, ...)
```
```java
v2 = slim.variable(name=
```
```java
"nested/v2"
```
```java
, ...)
```
```java
...
```
```java
# Get list of variables to restore (which contains only
```
```java
'v2'
```
```java
).
```
```java
variables_to_restore = slim.get_variables_by_name(
```
```java
"v2"
```
```java
)
```
```java
# Create the saver which will be used to restore the variables.
```
```java
restorer = tf.train.Saver(variables_to_restore)
```
```java
with tf.Session() as sess:
```
```java
```
```java
# Restore variables from disk.
```
```java
```
```java
restorer.restore(sess,
```
```java
"/tmp/model.ckpt"
```
```java
)
```
```java
```
```java
print(
```
```java
"Model restored."
```
```java
)
```
```
除了这种部分变量加载的方法外，我们甚至还能加载到不同名字的变量中。
假设我们定义的网络变量是conv1/weights，而从VGG加载的变量名为vgg16/conv1/weights，正常load肯定会报错（找不到变量名），但是可以这样：
[?](https://www.2cto.com/kf/201706/649266.html#)
```
```java
def name_in_checkpoint(var):
```
```java
```
```java
return
```
```java
'vgg16/'
```
```java
+ var.op.name
```
```java
variables_to_restore = slim.get_model_variables()
```
```java
variables_to_restore = {name_in_checkpoint(var):var
```
```java
for
```
```java
var in variables_to_restore}
```
```java
restorer = tf.train.Saver(variables_to_restore)
```
```java
with tf.Session() as sess:
```
```java
```
```java
# Restore variables from disk.
```
```java
```
```java
restorer.restore(sess,
```
```java
"/tmp/model.ckpt"
```
```java
)
```
```
通过这种方式我们可以加载不同变量名的变量！！

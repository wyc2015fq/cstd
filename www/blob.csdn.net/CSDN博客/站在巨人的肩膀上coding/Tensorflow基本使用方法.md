# Tensorflow基本使用方法 - 站在巨人的肩膀上coding - CSDN博客





2018年04月20日 16:09:35[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：196








使用`TensorFlow`，你必须明白`TensorFlow`：
- 使用图`(graph)`来表示任务
- 被称之为会话`(Session)`的上下文`(context)`中执行图
- 使用`tensor`表示数据
- 通过变量`(Variable)`维护状态
- 使用`feed`和`fetch`可以为任意操作`(arbitrary operation)`赋值或者从其中获取数据

### 综述

`TensorFlow`是一个编程系统，使用图来表示计算任务，图中的节点被称之为`op`（`operation`的缩写），一个`op`获得0个或者多个`tensor`，执行计算，产生0个或多个`tensor`。每个`tensor`是一个类型化的多维数组。例如，你可以将一组图像素集表示为一个四维浮点数数组，这四个维度分别是`[batch, height, width, channels]`。

一个`TensorFlow`图描述了计算的过程，为了进行计算，图必须在`会话`里被启动，`会话`将图的`op`分发到诸如CPU或GPU之类的设备上，同时提供执行`op`的方法，这些方法执行后，将产生的`tensor`返回。在python语言中，返回的`tensor`是`numpy ndarry`对象；在C/C++语言中，返回的是`tensor`是`tensorflow::Tensor`实例。

### 计算图

`Tensorflow`程序通常被组织成一个构建阶段和一个执行阶段，在构建阶段，`op`的执行步骤被描述成为一个图，在执行阶段，使用会话执行图中的`op`。

例如，通常在构建阶段创建一个图来表示和训练神经网络，然后在执行阶段反复执行图中的训练`op`。

`Tensorflow`支持C/C++，python编程语言。目前，`TensorFlow`的python库更易使用，它提供了大量的辅助函数来简化构建图的工作，这些函数尚未被C/C++库支持。

三种语言的会话库`(session libraries)`是一致的。

### 构建图

构件图的第一步是创建源`op (source op)`。源`op`不需要任何输入。源`op`的输出被传递给其它`op`做运算。

python库中，`op`构造器的返回值代表被构造出的`op`输出，这些返回值可以传递给其它`op`作为输入。

`TensorFlow` Python库中有一个默认图`(default graph)`，`op`构造器可以为其增加节点。这个默认图对许多程序来说已经足够用了，可以阅读[`Graph`类](http://wiki.jikexueyuan.com/project/tensorflow-zh/api_docs/python/framework.html)文档，来了解如何管理多个视图。

```python
import tensorflow as tf
# 创建一个常量op， 产生一个1x2矩阵，这个op被作为一个节点
# 加到默认视图中
# 构造器的返回值代表该常量op的返回值
matrix1 = tr.constant([[3., 3.]])

# 创建另一个常量op, 产生一个2x1的矩阵
matrix2 = tr.constant([[2.], [2.]])

# 创建一个矩阵乘法matmul op，把matrix1和matrix2作为输入：
product = tf.matmul(matrix1, matrix2)
```

默认图现在有三个节点，两个`constant() op`和`matmul() op`。为了真正进行矩阵乘法的结果，你必须在会话里启动这个图。

### 在一个会话中启动图

构造阶段完成后，才能启动图。启动图的第一步是创建一个`Session`对象，如果无任何创建参数，会话构造器将无法启动默认图。

欲了解完整的会话API，请阅读[`Session`类](http://wiki.jikexueyuan.com/project/tensorflow-zh/api_docs/python/client.html)。 

```python
# 启动默认图
sess = tf.Session()

# 调用sess的'run()' 方法来执行矩阵乘法op，传入'product'作为该方法的参数
# 上面提到，'product'代表了矩阵乘法op的输出，传入它是向方法表明，我们希望取回
# 矩阵乘法op的输出。
#
#整个执行过程是自动化的，会话负责传递op所需的全部输入。op通常是并发执行的。
#
# 函数调用'run(product)' 触发了图中三个op（两个常量op和一个矩阵乘法op）的执行。
# 返回值'result'是一个numpy 'ndarray'对象。
result = sess.run(product)
print result
# ==>[[12.]]

# 完成任务，关闭会话
sess.close()
```

`Session`对象在使用完成后需要关闭以释放资源，除了显式调用`close`外，也可以使用`with`代码来自动完成关闭动作：

```python
with tf.Session() as sess:
  result = sess.run([product])
  print result
```

在实现上，`Tensorflow`将图形定义转换成分布式执行的操作，以充分利用可以利用的计算资源（如CPU或GPU）。一般你不需要显式指定使用CPU还是GPU，`Tensorflow`能自动检测。如果检测到GPU，`Tensorflow`会尽可能地使用找到的第一个GPU来执行操作。

如果机器上有超过一个可用的GPU，除了第一个外的其他GPU是不参与计算的。为了让`Tensorflow`使用这些GPU，你必须将`op`明确地指派给它们执行。`with...Device`语句用来指派特定的CPU或GPU操作：

```python
with tf.Session() as sess:
  with tf.device("/gpu:1"):
    matrix1 = tf.constant([[3., 3.]])
    matrix2 = tf.constant([[2.], [2.]])
    product = tf.matmul(matrix1, matrix2)
```

设备用字符串进行标识，目前支持的设备包括：
- `/cpu:0`:机器的CPU
- `/gpu:0`:机器的第一个GPU，如果有的话
- `/gpu:1`:机器的的第二个GPU，以此类推

### 交互式使用

文档中的python示例使用一个会话`Session`来启动图，并调用`Session.run()`方法执行操作。

为了便于使用诸如IPython之类的python交互环境，可以使用`InteractiveSession`代替`Session`类，使用`Tensor.eval()`和`Operation.run()`方法代替`Session.run()`。这样可以避免使用一个变量来持有会话：

```python
# 进入一个交互式Tensorflow会话
import tensorflow as tf
sess = tf.InteractiveSession()

x = tf.Variable([1.0, 2.0])
a = tf.constant([3.0, 3.0]);

# 使用初始化器initializer op的run()方法初始化x
x.initializer.run()

# 增加一个减法sub op，从x减去a。运行减法op，输出结果
sud = tf.sub(x, a)
print sub.eval()
# ==>[-2. -1.]
```

### `Tensor`

`Tensorflow`程序使用`tensor`数据结构来代表所有的数据，计算图中，操作间传递数据都是`tensor`。你可以把`Tensorflow`的`tensor`看做是一个`n`维的数组或列表。一个`tensor`包含一个静态类型`rank`和一个`shape`。

#### 阶

在`Tensorflow`系统中，张量的维数被描述为阶。但是张量的阶和矩阵的阶并不是同一个概念。张量的阶是张量维数的一个数量描述，下面的张量（使用python中`list`定义的）就是2阶：

```python
t = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]
```

你可以认为一个二阶张量就是我们平常所说的矩阵，一阶张量可以认为是一个向量。对于一个二阶张量，你可以使用语句`t[i, j]`来访问其中的任何元素。而对于三阶张量你可以通过`t[i, j, k]`来访问任何元素：
|阶|数学实例|python例子|
|----|----|----|
|`0`|纯量（只有大小）|`s=483`|
|`1`|向量（大小和方向）|`v=[1.1, 2.2, 3.3]`|
|`2`|矩阵（数据表）|`m=[[1, 2, 3], [4, 5, 6], [7, 8, 9]]`|
|`3`|`3`阶张量|`t=[[[2], [4], [6]], [[8], [9], [10]], [[11], [12], [13]]]`|
|`n`|`n`阶||

#### 形状

`Tensorflow`文档中使用了三种记号来方便地描述张量的维度：阶，形状以及维数。以下展示了它们之间的关系：
|阶|形状|维数|实例|
|----|----|----|----|
|`0`|`[]`|0-D|一个0维张量，一个纯量|
|`1`|`[D0]`|1-D|一个1维张量的形式`[5]`|
|`2`|`[D0, D1]`|2-D|一个2维张量的形式`[3, 4]`|
|`3`|`[D0, D1, D2]`|3-D|一个3维张量的形式`[1, 4, 3]`|
|`n`|`[D0, D1, ... Dn]`|n-D|一个n维张量的形式`[D0, D1, ..., Dn]`|
数据类型
除了维度，`tensor`有一个数据类型属性。你可以为一个张量指定下列数据类型中的任意一个类型：
|数据类型|python类型|描述|
|----|----|----|
|`DT_FLOAT`|`tf.float32`|32位浮点数|
|`DT_DOUBLE`|`tf.float64`|64位浮点数|
|`DT_INT64`|`tf.int64`|64位有符号整型|
|`DT_INT32`|`tf.int32`|32位有符号整型|
|`DF_INT16`|`tf.int16`|16位有符号整型|
|`DT_INT8`|`tf.int8`|8位有符号整型|
|`DT_UINT8`|`tf.uint8`|8位无符号整型|
|`DT_STRING`|`tf.string`|可变长度的字节数组，每一个张量元素都是一个字节数组|
|`DT_BOOL`|`tf.bool`|布尔型|
|`DT_COMPLEX64`|`tf.complex64`|由32位浮点数组成的负数：实数和虚数|
|`DT_QINT32`|`tf.qint32`|用于量化Ops的32位有符号整型|
|`DT_QINT8`|`tf.qint8`|用于量化Ops的8位有符号整型|
|`DT_QUINT8`|`tf.quint8`|用于量化Ops的8位无符号整型|
变量
在[Variables](http://wiki.jikexueyuan.com/project/tensorflow-zh/how_tos/variables/index.html) 中查看更多细节。变量维护图执行过程中的状态信息。下面的例子演示了如何使用变量实现一个简单的计数器：

```python
# 创建一个变量，初始为标量0
state = tf.Variable(0, name="counter")

# 创建一个op，其作用是使`state`增加1
one = tf.constant(1)
new_value = tf.add(state, one)
update = tf.assign(state, new_value)

# 启动图后，变量必须先经过init op初始化
# 首先先增加一个初始化op到图中
init_op = tf.initialize_all_variables()

# 启动图
with tf.Session() as sess:
  # 运行init op
  sess.run(init_op)
  # 打印 state 的初始值
  print sess.run(state)
  # 运行op， 更新state 并打印
  for _ in range(3):
    sess.run(update)
    print sess.run(state)

# 输出：
# 0
# 1
# 2
# 3
```

代码中`assign()`操作是图所描述的表达式的一部分，正如`add()`操作一样，所以在调用`run()`执行表达式之前，它并不会真正执行赋值操作。

通常会将一个统计模型中的参数表示为一组变量。例如，你可以将一个神经网络的权重作为某个变量存储在一个`tensor`中。在训练过程中，通过反复训练图，更新这个`tensor`。

### `Fetch`

为了取回操作的输出内容，可以在使用`Session`对象的`run()`调用执行图时，传入一些`tensor`，这些`tensor`会帮助你取回结果。在之前的例子里，我们只取回了单个节点`state`，但是你也可以取回多个`tensor`:

```python
input1 = tf.constant(3.0)
input2 = tf.constant(4.0)
input3 = tf.constant(5.0)
intermed = tf.add(input2, input3)
mul = tf.mul(input1, intermed)

with tf.Session() as sess:
  result = sess.run([mul, intermed])
  print result

# print
# [27.0, 9.0]
```

需要获得更多个`tensor`值，在`op`的依次运行中获得（而不是逐个去获得`tenter`）。

### `Feed`

上述示例在计算图中引入`tensor`，以常量或变量的形式存储。`Tensorflow`还提供了`feed`机制，该机制可以临时替代图中的任意操作中的`tensor`可以对图中任何操作提交补丁，直接插入一个`tensor`。

`feed`使用一个`tensor`值临时替换一个操作的输出结果，你可以提供`feed`数据作为`run()`调用的参数。`feed`只在调用它的方法内有效，方法结束，`feed`就会消失。最常见的用例是将某些特殊的操作指定为`feed`操作，标记的方法是使用`tf.placeholder()`为这些操作创建占位符。

```python
input1 = tf.placeholder(tf.types.float32)
input2 = tf.placeholder(tf.types.float32)
output = tf.mul(input1, input2)

with tf.Session() as see:
  print sess.run([output], feed_dict={input:[7.]， input2:[2.]})

# print
# [array([ 14.], dtype=float32)]
```




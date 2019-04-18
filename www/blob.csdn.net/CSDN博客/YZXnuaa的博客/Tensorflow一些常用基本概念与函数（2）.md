# Tensorflow一些常用基本概念与函数（2） - YZXnuaa的博客 - CSDN博客
2018年03月30日 00:22:35[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：127
## 1、tensorflow的基本运作
为了快速的熟悉TensorFlow编程，下面从一段简单的代码开始：
```python
import tensorflow as tf
 #定义‘符号’变量，也称为占位符
 a = tf.placeholder("float")
 b = tf.placeholder("float")
 y = tf.mul(a, b) #构造一个op节点
 sess = tf.Session()#建立会话
 #运行会话，输入数据，并计算节点，同时打印结果
 print sess.run(y, feed_dict={a: 3, b: 3})
 # 任务完成, 关闭会话.
 sess.close()
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
其中tf.mul(a, b)函数便是tf的一个基本的算数运算，接下来介绍跟多的相关函数。
## 2、tf函数
```
TensorFlow 将图形定义转换成分布式执行的操作, 以充分利用可用的计算资源(如 CPU 或 GPU。一般你不需要显式指定使用 CPU 还是 GPU, TensorFlow 能自动检测。如果检测到 GPU, TensorFlow 会尽可能地利用找到的第一个 GPU 来执行操作.
并行计算能让代价大的算法计算加速执行，TensorFlow也在实现上对复杂操作进行了有效的改进。大部分核相关的操作都是设备相关的实现，比如GPU。本文主要涉及的相关概念或操作有以下内容：
```
- 1
- 2
- 3
|操作组|操作|
|----|----|
|Building Graphs|Core graph data structures，Tensor types，Utility functions|
|Inputs and Readers|Placeholders，Readers，Converting，Queues，Input pipeline|
2.1 建立图(Building Graphs)
本节主要介绍建立tensorflow图的相关类或函数
#### * 核心图的数据结构（Core graph data structures）
*tf.Graph*
|操作|描述|
|----|----|
|class tf.Graph|tensorflow中的计算以图数据流的方式表示一个图包含一系列表示计算单元的操作对象以及在图中流动的数据单元以tensor对象表现|
|tf.Graph.__init__()|建立一个空图|
|tf.Graph.as_default()|一个将某图设置为默认图，并返回一个上下文管理器如果不显式添加一个默认图，系统会自动设置一个全局的默认图。所设置的默认图，在模块范围内所定义的节点都将默认加入默认图中|
|tf.Graph.as_graph_def(from_version=None, add_shapes=False)|返回一个图的序列化的GraphDef表示序列化的[GraphDef](https://github.com/tensorflow/tensorflow/blob/r0.10/tensorflow/core/framework/graph.proto)可以导入至另一个图中(使用 import_graph_def())或者使用C++ Session API|
|tf.Graph.finalize()|完成图的构建，即将其设置为只读模式|
|tf.Graph.finalized|返回True，如果图被完成|
|tf.Graph.control_dependencies(control_inputs)|定义一个控制依赖，并返回一个上下文管理器with g.control_dependencies([a, b, c]):# `d` 和 `e` 将在 `a`, `b`, 和`c`执行完之后运行.d = …e = …|
|tf.Graph.device(device_name_or_function)|定义运行图所使用的设备，并返回一个上下文管理器`with g.device('/gpu:0'): ...``with g.device('/cpu:0'): ...`|
|tf.Graph.name_scope(name)|为节点创建层次化的名称，并返回一个上下文管理器|
|tf.Graph.add_to_collection(name, value)|将value以name的名称存储在收集器(collection)中|
|tf.Graph.get_collection(name, scope=None)|根据name返回一个收集器中所收集的值的列表|
|tf.Graph.as_graph_element(obj, allow_tensor=True, allow_operation=True)|返回一个图中与obj相关联的对象，为一个操作节点或者tensor数据|
|tf.Graph.get_operation_by_name(name)|根据名称返回操作节点|
|tf.Graph.get_tensor_by_name(name)|根据名称返回tensor数据|
|tf.Graph.get_operations()|返回图中的操作节点列表|
|tf.Graph.gradient_override_map(op_type_map)|用于覆盖梯度函数的上下文管理器|
```python
#class tf.Graph
#tensorflow运行时需要设置默认的图
g = tf.Graph()
with g.as_default():
  # Define operations and tensors in `g`.
  c = tf.constant(30.0)
  assert c.graph is g
##也可以使用tf.get_default_graph()获得默认图，也可在基础上加入节点或子图
c = tf.constant(4.0)
assert c.graph is tf.get_default_graph()
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
```python
#tf.Graph.as_default
#以下两段代码功能相同
#1、使用Graph.as_default():
g = tf.Graph()
with g.as_default():
  c = tf.constant(5.0)
  assert c.graph is g
#2、构造和设置为默认
with tf.Graph().as_default() as g:
  c = tf.constant(5.0)
  assert c.graph is g
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
```python
#tf.Graph.control_dependencies(control_inputs)
# 错误代码
def my_func(pred, tensor):
  t = tf.matmul(tensor, tensor)
  with tf.control_dependencies([pred]):
    # 乘法操作(op)没有创建在该上下文，所以没有被加入依赖控制
    return t
# 正确代码
def my_func(pred, tensor):
  with tf.control_dependencies([pred]):
    # 乘法操作(op)创建在该上下文，所以被加入依赖控制中
    #执行完pred之后再执行matmul
    return tf.matmul(tensor, tensor)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
```python
# tf.Graph.name_scope(name)
# 一个图中包含有一个名称范围的堆栈，在使用name_scope(...)之后，将压(push)新名称进栈中，
#并在下文中使用该名称
with tf.Graph().as_default() as g:
  c = tf.constant(5.0, name="c")
  assert c.op.name == "c"
  c_1 = tf.constant(6.0, name="c")
  assert c_1.op.name == "c_1"
  # Creates a scope called "nested"
  with g.name_scope("nested") as scope:
    nested_c = tf.constant(10.0, name="c")
    assert nested_c.op.name == "nested/c"
    # Creates a nested scope called "inner".
    with g.name_scope("inner"):
      nested_inner_c = tf.constant(20.0, name="c")
      assert nested_inner_c.op.name == "nested/inner/c"
    # Create a nested scope called "inner_1".
    with g.name_scope("inner"):
      nested_inner_1_c = tf.constant(30.0, name="c")
      assert nested_inner_1_c.op.name == "nested/inner_1/c"
      # Treats `scope` as an absolute name scope, and
      # switches to the "nested/" scope.
      with g.name_scope(scope):
        nested_d = tf.constant(40.0, name="d")
        assert nested_d.op.name == "nested/d"
        with g.name_scope(""):
          e = tf.constant(50.0, name="e")
          assert e.op.name == "e"
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
*tf.Operation*
|操作|描述|
|----|----|
|class tf.Operation|代表图中的一个节点，用于计算tensors数据该类型将由python节点构造器产生(比如tf.matmul())或者Graph.create_op()例如c = tf.matmul(a, b)创建一个Operation类为类型为”MatMul”,输入为’a’,’b’，输出为’c’的操作类|
|tf.Operation.name|操作节点(op)的名称|
|tf.Operation.type|操作节点(op)的类型，比如”MatMul”|
|tf.Operation.inputstf.Operation.outputs|操作节点的输入与输出|
|tf.Operation.control_inputs|操作节点的依赖|
|tf.Operation.run(feed_dict=None, session=None)|在会话(Session)中运行该操作|
|tf.Operation.get_attr(name)|获取op的属性值|
*tf.Tensor*
|操作|描述|
|----|----|
|class tf.Tensor|表示一个由操作节点op产生的值，TensorFlow程序使用tensor数据结构来代表所有的数据, 计算图中, 操作间传递的数据都是 tensor，一个tensor是一个符号handle,里面并没有表示实际数据，而相当于数据流的载体|
|tf.Tensor.dtype|tensor中数据类型|
|tf.Tensor.name|该tensor名称|
|tf.Tensor.value_index|该tensor输出外op的index|
|tf.Tensor.graph|该tensor所处在的图|
|tf.Tensor.op|产生该tensor的op|
|tf.Tensor.consumers()|返回使用该tensor的op列表|
|tf.Tensor.eval(feed_dict=None, session=None)|在会话中求tensor的值需要使用`with sess.as_default()`或者 `eval(session=sess)`|
|tf.Tensor.get_shape()|返回用于表示tensor的shape的类TensorShape|
|tf.Tensor.set_shape(shape)|更新tensor的shape|
|tf.Tensor.device|设置计算该tensor的设备|
```python
#tf.Tensor.get_shape()
c = tf.constant([[1.0, 2.0, 3.0], [4.0, 5.0, 6.0]])
print(c.get_shape())
==> TensorShape([Dimension(2), Dimension(3)])
```
- 1
- 2
- 3
- 4
```python
#现在有个用于图像处理的tensor->image
print(image.get_shape())
==> TensorShape([Dimension(None), Dimension(None), Dimension(3)])
# 假如我们知道数据集中图像尺寸为28 x 28，那么可以设置
image.set_shape([28, 28, 3])
print(image.get_shape())
==> TensorShape([Dimension(28), Dimension(28), Dimension(3)])
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
#### * tensor类型(Tensor types)
*tf.DType*
|操作|描述|
|----|----|
|class tf.DType|数据类型主要包含tf.float16，tf.float16,tf.float32,tf.float64,tf.bfloat16,tf.complex64,tf.complex128,tf.int8,tf.uint8,tf.uint16,tf.int16,tf.int32,tf.int64,tf.bool,tf.string|
|tf.DType.is_compatible_with(other)|判断other的数据类型是否将转变为该DType|
|tf.DType.name|数据类型名称|
|tf.DType.base_dtype|返回该DType的基础DType，而非参考的数据类型(non-reference)|
|tf.DType.as_ref|返回一个基于DType的参考数据类型|
|tf.DType.is_floating|判断是否为浮点类型|
|tf.DType.is_complex|判断是否为复数|
|tf.DType.is_integer|判断是否为整数|
|tf.DType.is_unsigned|判断是否为无符号型数据|
|tf.DType.as_numpy_dtype|返回一个基于DType的numpy.dtype类型|
|tf.DType.maxtf.DType.min|返回这种数据类型能表示的最大值及其最小值|
|tf.as_dtype(type_value)|返回由type_value转变得的相应tf数据类型|
#### * 通用函数（Utility functions）
|操作|描述|
|----|----|
|tf.device(device_name_or_function)|基于默认的图，其功能便为Graph.device()|
|tf.container(container_name)|基于默认的图，其功能便为Graph.container()|
|tf.name_scope(name)|基于默认的图，其功能便为 Graph.name_scope()|
|tf.control_dependencies(control_inputs)|基于默认的图，其功能便为Graph.control_dependencies()|
|tf.convert_to_tensor(value, dtype=None, name=None, as_ref=False)|将value转变为tensor数据类型|
|tf.get_default_graph()|返回返回当前线程的默认图|
|tf.reset_default_graph()|清除默认图的堆栈，并设置全局图为默认图|
|tf.import_graph_def(graph_def, input_map=None,return_elements=None, name=None, op_dict=None,producer_op_list=None)|将graph_def的图导入到python中|
#### * 图收集（Graph collections）
|操作|描述|
|----|----|
|tf.add_to_collection(name, value)|基于默认的图，其功能便为Graph.add_to_collection()|
|tf.get_collection(key, scope=None)|基于默认的图，其功能便为Graph.get_collection()|
#### * 定义新操作节点（Defining new operations）
*tf.RegisterGradient*
|操作|描述|
|----|----|
|class tf.RegisterGradient|返回一个用于寄存op类型的梯度函数的装饰器|
|tf.NoGradient(op_type)|设置操作节点类型op_type的节点没有指定的梯度|
|class tf.RegisterShape|返回一个用于寄存op类型的shape函数的装饰器|
|class tf.TensorShape|表示tensor的shape|
|tf.TensorShape.merge_with(other)|与other合并shape信息，返回一个TensorShape类|
|tf.TensorShape.concatenate(other)|与other的维度相连结|
|tf.TensorShape.ndims|返回tensor的rank|
|tf.TensorShape.dims|返回tensor的维度|
|tf.TensorShape.as_list()|以list的形式返回tensor的shape|
|tf.TensorShape.is_compatible_with(other)|判断shape是否为兼容TensorShape(None)与其他任何shape值兼容|
|class tf.Dimension||
|tf.Dimension.is_compatible_with(other)|判断dims是否为兼容|
|tf.Dimension.merge_with(other)|与other合并dims信息|
|tf.op_scope(values, name, default_name=None)|在python定义op时，返回一个上下文管理器|
```python
#tf.RegisterGradient
#该装饰器只使用于定义一个新的op类型时候，如果一个op有m个输入，n个输出。那么该梯度函数应该设置原始的
#操作类型，以及n个Tensor对象（表示每一个op输出的梯度），以及m个对象(表示每一个op输入的偏梯度)
#以操作节点类型为'Sub'为例，两输入为x,y。为一个输出x-y
@tf.RegisterGradient("Sub")
def _sub_grad(unused_op, grad):
  return grad, tf.neg(grad)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
```python
#tf.op_scope
#定义一个名称为my_op的python操作节点op
def my_op(a, b, c, name=None):
  with tf.op_scope([a, b, c], name, "MyOp") as scope:
    a = tf.convert_to_tensor(a, name="a")
    b = tf.convert_to_tensor(b, name="b")
    c = tf.convert_to_tensor(c, name="c")
    # Define some computation that uses `a`, `b`, and `c`.
    return foo_op(..., name=scope)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
### 2.2 输入和读取器(Inputs and Readers)
本节主要介绍tensorflow中数据的读入相关类或函数
#### * 占位符（Placeholders）
tf提供一种占位符操作，在执行时需要为其提供数据data。
|操作|描述|
|----|----|
|tf.placeholder(dtype, shape=None, name=None)|为一个tensor插入一个占位符eg:x = tf.placeholder(tf.float32, shape=(1024, 1024))|
|tf.placeholder_with_default(input, shape, name=None)|当输出没有fed时，input通过一个占位符op|
|tf.sparse_placeholder(dtype, shape=None, name=None)|为一个稀疏tensor插入一个占位符|
#### * 读取器（Readers）
tf提供一系列读取各种数据格式的类。对于多文件输入，可以使用函数[tf.train.string_input_producer](https://www.tensorflow.org/versions/r0.10/api_docs/python/io_ops.html#string_input_producer)，该函数将创建一个保持文件的FIFO队列，以供reader使用。或者如果输入的这些文件名有相雷同的字符串，也可以使用函数[tf.train.match_filenames_once](https://www.tensorflow.org/versions/r0.10/api_docs/python/io_ops.html#match_filenames_once)。
|操作|描述|
|----|----|
|class tf.ReaderBase|不同的读取器类型的基本类|
|tf.ReaderBase.read(queue, name=None)|返回下一个记录对(key, value),queue为tf文件队列FIFOQueue|
|tf.ReaderBase.read_up_to(queue, num_records, name=None)|返回reader产生的num_records对(key, value)|
|tf.ReaderBase.reader_ref|返回应用在该reader上的Op|
|tf.ReaderBase.reset(name=None)|恢复reader为初始状态|
|tf.ReaderBase.restore_state(state, name=None)|恢复reader为之前的保存状态state|
|tf.ReaderBase.serialize_state(name=None)|返回一个reader解码后产生的字符串tansor|
|class tf.TextLineReader||
|tf.TextLineReader.num_records_produced(name=None)|返回reader已经产生的记录(records )数目|
|tf.TextLineReader.num_work_units_completed(name=None)|返回该reader已经完成的处理的work数目|
|tf.TextLineReader.read(queue, name=None)|返回reader所产生的下一个记录对 (key, value)，该reader可以限定新产生输出的行数|
|tf.TextLineReader.reader_ref|返回应用在该reader上的Op|
|tf.TextLineReader.reset(name=None)|恢复reader为初始状态|
|tf.TextLineReader.restore_state(state, name=None)|恢复reader为之前的保存状态state|
|tf.TextLineReader.serialize_state(name=None)|返回一个reader解码后产生的字符串tansor|
|class tf.WholeFileReader|一个阅读器，读取整个文件，返回文件名称key,以及文件中所有的内容value,该类的方法同上，不赘述|
|class tf.IdentityReader|一个reader，以key和value的形式，输出一个work队列。该类其他方法基本同上|
|class tf.TFRecordReader|读取TFRecord格式文件的reader。该类其他方法基本同上|
|class tf.FixedLengthRecordReader|输出|
#### * 数据转换（Converting）
tf提供一系列方法将各种格式数据转换为tensor表示。
|操作|描述|
|----|----|
|tf.decode_csv(records, record_defaults, field_delim=None, name=None)|将csv转换为tensor，与tf.TextLineReader搭配使用|
|tf.decode_raw(bytes, out_type, little_endian=None, name=None)|将bytes转换为一个数字向量表示，bytes为一个字符串类型的tensor与函数 tf.FixedLengthRecordReader搭配使用，详见[tf的CIFAR-10例子](https://www.tensorflow.org/code/tensorflow/models/image/cifar10/cifar10_input.py)|
选取与要输入的文件格式相匹配的reader，并将文件队列提供给reader的读方法( read method)。读方法将返回文件唯一标识的key，以及一个记录(record)（有助于对出现一些另类的records时debug），以及一个标量的字符串值。再使用一个（或多个）解码器(decoder) 或转换操作(conversion ops)将字符串转换为tensor类型。
```python
#读取文件队列，使用reader中read的方法，返回key与value
filename_queue = tf.train.string_input_producer(["file0.csv", "file1.csv"])
reader = tf.TextLineReader()
key, value = reader.read(filename_queue)
record_defaults = [[1], [1], [1], [1], [1]]
col1, col2, col3, col4, col5 = tf.decode_csv(
    value, record_defaults=record_defaults)
features = tf.pack([col1, col2, col3, col4])
with tf.Session() as sess:
  # Start populating the filename queue.
  coord = tf.train.Coordinator()
  threads = tf.train.start_queue_runners(coord=coord)
  for i in range(1200):
    # Retrieve a single instance:
    example, label = sess.run([features, col5])
  coord.request_stop()
  coord.join(threads)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
#### * Example protocol buffer
提供了一些[Example protocol buffers](https://www.tensorflow.org/code/tensorflow/core/example/example.proto)，tf所[推荐的用于训练样本的数据格式](https://www.tensorflow.org/versions/r0.10/how_tos/reading_data/index.html#standard-tensorflow-format)，它们包含特征信息，[详情可见](https://www.tensorflow.org/code/tensorflow/core/example/feature.proto)。 
这是一种与前述将手上现有的各种数据类型转换为支持的格式的方法，这种方法更容易将网络结构与数据集融合或匹配。这种tensorflow所推荐的数据格式是一个包含tf.train.Example protocol buffers (包含特征[Features](https://www.tensorflow.org/code/tensorflow/core/example/feature.proto)域)的TFRecords文件。 
1、获取这种格式的文件方式为，首先将一般的数据格式填入Example protocol buffer中，再将 protocol buffer序列化为一个字符串，然后使用tf.python_io.TFRecordWriter类的相关方法将字符串写入一个TFRecords文件中，[参见MNIST例子](https://www.tensorflow.org/code/tensorflow/examples/how_tos/reading_data/convert_to_records.py)，将MNIST 数据转换为该类型数据。 
2、读取TFRecords格式文件的方法为，使用tf.TFRecordReader读取器和tf.parse_single_example解码器。parse_single_example操作将 example protocol buffers解码为tensor形式。[参见MNIST例子](https://www.tensorflow.org/code/tensorflow/examples/how_tos/reading_data/fully_connected_reader.py)
|操作|描述|
|----|----|
|class tf.VarLenFeature|解析变长的输入特征feature相关配置|
|class tf.FixedLenFeature|解析定长的输入特征feature相关配置|
|class tf.FixedLenSequenceFeature|序列项目中的稠密(dense )输入特征的相关配置|
|tf.parse_example(serialized, features, name=None, example_names=None)|将一组Example protos解析为tensor的字典形式解析serialized所给予的序列化的一些Example protos返回一个由特征keys映射Tensor和SparseTensor值的字典|
|tf.parse_single_example(serialized, features, name=None, example_names=None)|解析一个单独的Example proto，与tf.parse_example方法雷同|
|tf.decode_json_example(json_examples, name=None)|将JSON编码的样本记录转换为二进制protocol buffer字符串|
```python
#tf.parse_example的使用举例
#输入序列化数据如下： 
serialized = [
  features
    { feature { key: "ft" value { float_list { value: [1.0, 2.0] } } } },
  features
    { feature []},
  features
    { feature { key: "ft" value { float_list { value: [3.0] } } }
]
#那么输出为一个字典(dict),如下：
{"ft": SparseTensor(indices=[[0, 0], [0, 1], [2, 0]],
                    values=[1.0, 2.0, 3.0],
                    shape=(3, 2)) }
#########
#再来看一个例子，给定两个序列化的原始输入样本：
[
  features {
    feature { key: "kw" value { bytes_list { value: [ "knit", "big" ] } } }
    feature { key: "gps" value { float_list { value: [] } } }
  },
  features {
    feature { key: "kw" value { bytes_list { value: [ "emmy" ] } } }
    feature { key: "dank" value { int64_list { value: [ 42 ] } } }
    feature { key: "gps" value { } }
  }
]
#相关参数如下：
example_names: ["input0", "input1"],
features: {
    "kw": VarLenFeature(tf.string),
    "dank": VarLenFeature(tf.int64),
    "gps": VarLenFeature(tf.float32),
}
#那么有如下输出：
{
  "kw": SparseTensor(
      indices=[[0, 0], [0, 1], [1, 0]],
      values=["knit", "big", "emmy"]
      shape=[2, 2]),
  "dank": SparseTensor(
      indices=[[1, 0]],
      values=[42],
      shape=[2, 1]),
  "gps": SparseTensor(
      indices=[],
      values=[],
      shape=[2, 0]),
}
#########
#对于两个样本的输出稠密结果情况
[
  features {
    feature { key: "age" value { int64_list { value: [ 0 ] } } }
    feature { key: "gender" value { bytes_list { value: [ "f" ] } } }
   },
   features {
    feature { key: "age" value { int64_list { value: [] } } }
    feature { key: "gender" value { bytes_list { value: [ "f" ] } } }
  }
]
#我们可以使用以下参数
example_names: ["input0", "input1"],
features: {
    "age": FixedLenFeature([], dtype=tf.int64, default_value=-1),
    "gender": FixedLenFeature([], dtype=tf.string),
}
#期望的结果如下
{
  "age": [[0], [-1]],
  "gender": [["f"], ["f"]],
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
```python
##Example protocol buffer相关使用的例子
#将mnist的数据转换为TFRecords文件格式
import os
import tensorflow as tf
from tensorflow.contrib.learn.python.learn.datasets import mnist
SOURCE_URL = 'http://yann.lecun.com/exdb/mnist/'
TRAIN_IMAGES = 'train-images-idx3-ubyte.gz'  # MNIST filenames
TRAIN_LABELS = 'train-labels-idx1-ubyte.gz'
TEST_IMAGES = 't10k-images-idx3-ubyte.gz'
TEST_LABELS = 't10k-labels-idx1-ubyte.gz'
tf.app.flags.DEFINE_string('directory', '/tmp/data',
                           'Directory to download data files and write the '
                           'converted result')
tf.app.flags.DEFINE_integer('validation_size', 5000,
                            'Number of examples to separate from the training '
                            'data for the validation set.')
FLAGS = tf.app.flags.FLAGS
def _int64_feature(value):
  return tf.train.Feature(int64_list=tf.train.Int64List(value=[value]))
def _bytes_feature(value):
  return tf.train.Feature(bytes_list=tf.train.BytesList(value=[value]))
def convert_to(data_set, name):
  images = data_set.images
  labels = data_set.labels
  num_examples = data_set.num_examples
  if images.shape[0] != num_examples:
    raise ValueError('Images size %d does not match label size %d.' %
                     (images.shape[0], num_examples))
  rows = images.shape[1]
  cols = images.shape[2]
  depth = images.shape[3]
  filename = os.path.join(FLAGS.directory, name + '.tfrecords')
  print('Writing', filename)
  writer = tf.python_io.TFRecordWriter(filename)
  for index in range(num_examples):
    image_raw = images[index].tostring()
    example = tf.train.Example(features=tf.train.Features(feature={
        'height': _int64_feature(rows),
        'width': _int64_feature(cols),
        'depth': _int64_feature(depth),
        'label': _int64_feature(int(labels[index])),
        'image_raw': _bytes_feature(image_raw)}))
    writer.write(example.SerializeToString())
  writer.close()
def main(argv):
  # Get the data.
  data_sets = mnist.read_data_sets(FLAGS.directory,
                                   dtype=tf.uint8,
                                   reshape=False)
  # Convert to Examples and write the result to TFRecords.
  convert_to(data_sets.train, 'train')
  convert_to(data_sets.validation, 'validation')
  convert_to(data_sets.test, 'test')
if __name__ == '__main__':
  tf.app.run()
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
#### * 队列(Queues)
tensorflow提供了几个队列应用，用来将tf计算图与tensors的阶段流水组织到一起。队列是使用tensorflow计算的一个强大的机制，正如其他Tensorflow的元素一样，一个队列也是tf图中的一个节点(node),它是一个有状态的node，就像一个变量：其他节点可以改变其内容。 
我们来看一个简单的例子，如下gif图，我们将创建一个先入先出队列(FIFOQueue)并且将值全设为0，然后我们构建一个图以获取队列出来的元素，对该元素加1操作，并将结果再放入队列末尾。渐渐地，队列中的数字便增加。 
![这里写图片描述](https://img-blog.csdn.net/20160815145525960)
|操作|描述|
|----|----|
|class tf.QueueBase|基本的队列应用类.队列(queue)是一种数据结构，该结构通过多个步骤存储tensors,并且对tensors进行入列(enqueue)与出列(dequeue)操作|
|tf.QueueBase.enqueue(vals, name=None)|将一个元素编入该队列中。如果在执行该操作时队列已满，那么将会阻塞直到元素编入队列之中|
|tf.QueueBase.enqueue_many(vals, name=None)|将零个或多个元素编入该队列中|
|tf.QueueBase.dequeue(name=None)|将元素从队列中移出。如果在执行该操作时队列已空，那么将会阻塞直到元素出列，返回出列的tensors的tuple|
|tf.QueueBase.dequeue_many(n, name=None)|将一个或多个元素从队列中移出|
|tf.QueueBase.size(name=None)|计算队列中的元素个数|
|tf.QueueBase.close(cancel_pending_enqueues=False, name=None)|关闭该队列|
|f.QueueBase.dequeue_up_to(n, name=None)|从该队列中移出n个元素并将之连接|
|tf.QueueBase.dtypes|列出组成元素的数据类型|
|tf.QueueBase.from_list(index, queues)|根据queues[index]的参考队列创建一个队列|
|tf.QueueBase.name|返回最队列下面元素的名称|
|tf.QueueBase.names|返回队列每一个组成部分的名称|
|class tf.FIFOQueue|在出列时依照先入先出顺序，其他方法与tf.QueueBase雷同|
|class tf.PaddingFIFOQueue|一个FIFOQueue ，同时根据padding支持batching变长的tensor|
|class tf.RandomShuffleQueue|该队列将随机元素出列，其他方法与tf.QueueBase雷同|
#### * 文件系统的处理(Dealing with the filesystem)
|操作|描述|
|----|----|
|tf.matching_files(pattern, name=None)|返回与pattern匹配模式的文件名称|
|tf.read_file(filename, name=None)|读取并输出输入文件的整个内容|
#### * 输入管道(Input pipeline)
用于设置输入预取数的管道TF函数，函数 “producer”添加一个队列至图中，同时一个相应用于运行队列中子图(subgraph)的QueueRunner
|操作|描述|
|----|----|
|tf.train.match_filenames_once(pattern, name=None)|保存与pattern的文件列表|
|tf.train.limit_epochs(tensor, num_epochs=None, name=None)|返回一个num_epochs次数，然后报告OutOfRange错误|
|tf.train.input_producer(input_tensor, element_shape=None, num_epochs=None, shuffle=True, seed=None, capacity=32, shared_name=None, summary_name=None, name=None)|为一个输入管道输出input_tensor中的多行至一个队列中|
|tf.train.range_input_producer(limit, num_epochs=None, shuffle=True, seed=None, capacity=32, shared_name=None, name=None)|产生一个从1至limit-1的整数至队列中|
|tf.train.slice_input_producer(tensor_list, num_epochs=None, shuffle=True, seed=None, capacity=32, shared_name=None, name=None)|对tensor_list中的每一个tensor切片|
|tf.train.string_input_producer(string_tensor, num_epochs=None,shuffle=True, seed=None, capacity=32, shared_name=None, name=None)|为一个输入管道输出一组字符串(比如文件名)至队列中|
#### * 在输入管道末端批量打包(Batching at the end of an input pipeline)
该相关函数增添一个队列至图中以将数据一样本打包为batch。它们也会添加 一个QueueRunner，以便执行的已经被填满队列的子图
|操作|描述|
|----|----|
|tf.train.batch(tensors, batch_size, num_threads=1,capacity=32, enqueue_many=False, shapes=None, dynamic_pad=False, allow_smaller_final_batch=False, shared_name=None, name=None)|在输入的tensors中创建一些tensor数据格式的batch，若输入为shape[*, x, y, z]，那么输出则为[batch_size, x, y, z]返回一个列表或者一个具有与输入tensors相同类型tensors的字典|
|tf.train.batch_join(tensors_list, batch_size, capacity=32, enqueue_many=False, shapes=None, dynamic_pad=False, allow_smaller_final_batch=False, shared_name=None, name=None)|将一个tensors的列表添加至一个队列中以创建样本的batcheslen(tensors_list)个线程将启动，线程i将tensors_list[i]的tensors入列tensors_list[i1][j]与tensors_list[i2][j]有相同的类型和shape|
|tf.train.shuffle_batch(tensors, batch_size, capacity, min_after_dequeue, num_threads=1, seed=None, enqueue_many=False, shapes=None, allow_smaller_final_batch=False,shared_name=None, name=None)|使用随机乱序的方法创建batchestensors:用于入列的一个list或者dictcapacity:一个整数，表示队列中元素最大数目|
|tf.train.shuffle_batch_join(tensors_list, batch_size, capacity, min_after_dequeue, seed=None, enqueue_many=False, shapes=None, allow_smaller_final_batch=False, shared_name=None, name=None)|随机乱序的tensors创建batches，其中tensors_list参数为tensors元组或tensors字典的列表len(tensors_list)个线程将启动，线程i将tensors_list[i]的tensors入列tensors_list[i1][j]与tensors_list[i2][j]有相同的类型和shape|
```python
# 一个简单例子，使用tf.train.shuffle_batch创建一个具有32张图像和32个标签的batches.
image_batch, label_batch = tf.train.shuffle_batch(
      [single_image, single_label],
      batch_size=32,
      num_threads=4,
      capacity=50000,
      min_after_dequeue=10000)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
```python
#Batching函数相关例子，以函数tf.train.shuffle_batch为例
#为training, evaluation等操作将样本batching，以下代码使用随机顺序打包样本
def read_my_file_format(filename_queue):
  reader = tf.SomeReader()
  key, record_string = reader.read(filename_queue)
  example, label = tf.some_decoder(record_string)
  processed_example = some_processing(example)
  return processed_example, label
def input_pipeline(filenames, batch_size, num_epochs=None):
  filename_queue = tf.train.string_input_producer(
      filenames, num_epochs=num_epochs, shuffle=True)
  example, label = read_my_file_format(filename_queue)
  # min_after_dequeue defines how big a buffer we will randomly sample
  #   from -- bigger means better shuffling but slower start up and more
  #   memory used.
  # capacity must be larger than min_after_dequeue and the amount larger
  #   determines the maximum we will prefetch.  Recommendation:
  #   min_after_dequeue + (num_threads + a small safety margin) * batch_size
  min_after_dequeue = 10000
  capacity = min_after_dequeue + 3 * batch_size
  example_batch, label_batch = tf.train.shuffle_batch(
      [example, label], batch_size=batch_size, capacity=capacity,
      min_after_dequeue=min_after_dequeue)
  return example_batch, label_batch
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
```python
#如果需要跟多的并行或文件之间的样本乱序操作，可以使用函数tf.train.shuffle_batch_join多实例化reader
def read_my_file_format(filename_queue):
  # 与上例子相同
def input_pipeline(filenames, batch_size, read_threads, num_epochs=None):
  filename_queue = tf.train.string_input_producer(
      filenames, num_epochs=num_epochs, shuffle=True)
  example_list = [read_my_file_format(filename_queue)
                  for _ in range(read_threads)]
  min_after_dequeue = 10000
  capacity = min_after_dequeue + 3 * batch_size
  example_batch, label_batch = tf.train.shuffle_batch_join(
      example_list, batch_size=batch_size, capacity=capacity,
      min_after_dequeue=min_after_dequeue)
  return example_batch, label_batch
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
相关链接：
[1] 安装Tensorflow（Linux ubuntu） [http://blog.csdn.net/lenbow/article/details/51203526](http://blog.csdn.net/lenbow/article/details/51203526)
[2] ubuntu下CUDA编译的GCC降级安装 [http://blog.csdn.net/lenbow/article/details/51596706](http://blog.csdn.net/lenbow/article/details/51596706)
[3] ubuntu手动安装最新Nvidia显卡驱动 [http://blog.csdn.net/lenbow/article/details/51683783](http://blog.csdn.net/lenbow/article/details/51683783)
[4] Tensorflow的CUDA升级，以及相关配置 [http://blog.csdn.net/lenbow/article/details/52118116](http://blog.csdn.net/lenbow/article/details/52118116)
[5] 基于gensim的Doc2Vec简析 [http://blog.csdn.net/lenbow/article/details/52120230](http://blog.csdn.net/lenbow/article/details/52120230)
[6] TensorFlow的分布式学习框架简介 [http://blog.csdn.net/lenbow/article/details/52130565](http://blog.csdn.net/lenbow/article/details/52130565)
[7] Tensorflow一些常用基本概念与函数（1） [http://blog.csdn.net/lenbow/article/details/52152766](http://blog.csdn.net/lenbow/article/details/52152766)


# TensorFlow：tf.get_variable()和tf.Variable() - 小花生的博客 - CSDN博客


2018年07月21日 08:32:50[Peanut_范](https://me.csdn.net/u013841196)阅读数：104


TensorFlow：tf.get_variable()和tf.Variable()
## 1.tf.get_variable函数
函数：tf.get_variable，*获取具有这些参数的现有变量或创建一个新变量。*
**get_variable(name, shape=None, dtype=None, initializer=None, regularizer=None, trainable=True, collections=None, caching_device=None, partitioner=None, validate_shape=True, use_resource=None, custom_getter=None)**
此函数将名称与当前变量范围进行前缀，并执行重用检查。有关重用如何工作的详细说明，请参见变量范围。下面是一个基本示例:
```python
with tf
```
```python
.variable
```
```python
_scope(
```
```python
"foo"
```
```python
):
    v = tf
```
```python
.get
```
```python
_variable(
```
```python
"v"
```
```python
, [
```
```python
1
```
```python
])
```
```python
# v.name == "foo/v:0"
```
```python
w = tf
```
```python
.get
```
```python
_variable(
```
```python
"w"
```
```python
, [
```
```python
1
```
```python
])
```
```python
# w.name == "foo/w:0"
```
```python
with tf
```
```python
.variable
```
```python
_scope(
```
```python
"foo"
```
```python
, reuse=True):
    v1 = tf
```
```python
.get
```
```python
_variable(
```
```python
"v"
```
```python
)
```
```python
# The same as v above.
```
如果初始化器为 None（默认），则将使用在变量范围内传递的默认初始化器。如果另一个也是 None，那么一个 glorot_uniform_initializer 将被使用。初始化器也可以是张量，在这种情况下，变量被初始化为该值和形状。
类似地，如果正则化器是 None（默认），则将使用在变量范围内传递的默认正则符号（如果另一个也是 None，则默认情况下不执行正则化）。
如果提供了分区，则返回 PartitionedVariable。作为张量访问此对象将返回沿分区轴连接的碎片。
一些有用的分区可用。例如：variable_axis_size_partitioner 和 min_max_variable_partitioner。
**参数：**
name：新变量或现有变量的名称。
shape：新变量或现有变量的形状。
dtype：新变量或现有变量的类型（默认为 DT_FLOAT）。
initializer：创建变量的初始化器。
regularizer：一个函数（张量 - >张量或无）；将其应用于新创建的变量的结果将被添加到集合 tf.GraphKeys.REGULARIZATION_LOSSES 中，并可用于正则化。
trainable：如果为 True，还将变量添加到图形集合：GraphKeys.TRAINABLE_VARIABLES。
collections：要将变量添加到其中的图形集合键的列表。默认为 [GraphKeys.LOCAL_VARIABLES]。
caching_device：可选的设备字符串或函数，描述变量应该被缓存以读取的位置。默认为变量的设备，如果不是 None，则在其他设备上进行缓存。典型的用法的在使用该变量的操作所在的设备上进行缓存，通过 Switch 和其他条件语句来复制重复数据删除。
partitioner：（可选）可调用性，它接受要创建的变量的完全定义的 TensorShape 和 dtype，并且返回每个坐标轴的分区列表（当前只能对一个坐标轴进行分区）。
validate_shape：如果为假，则允许使用未知形状的值初始化变量。如果为真，则默认情况下，initial_value 的形状必须是已知的。
use_resource：如果为假，则创建一个常规变量。如果为真，则创建一个实验性的 ResourceVariable，而不是具有明确定义的语义。默认为假（稍后将更改为真）。
custom_getter：可调用的，将第一个参数作为真正的 getter，并允许覆盖内部的 get_variable 方法。custom_getter 的签名应该符合这种方法，但最经得起未来考验的版本将允许更改：def custom_getter(getter, *args, **kwargs)。还允许直接访问所有 get_variable 参数：def custom_getter(getter, name, *args, **kwargs)。创建具有修改的名称的变量的简单标识自定义 getter 是：python def custom_getter(getter, name, *args, **kwargs): return getter(name + ‘_suffix’, *args, **kwargs)
**返回值：**
创建或存在Variable（或者PartitionedVariable，如果使用分区器）。
**可能引发的异常：**
ValueError：当创建新的变量和形状时，在变量创建时违反重用，或当 initializer 的 dtype 和 dtype 不匹配时。在 variable_scope 中设置重用。
**initializer为变量初始化的方法，初始化有如下几种：**
```python
tf
```
```python
.constant
```
```python
_initializer(value=
```
```python
0
```
```python
, dtype=tf
```
```python
.float
```
```python
32)：常量初始化
tf
```
```python
.random
```
```python
_normal_initializer(mean=
```
```python
0.0
```
```python
, stddev=
```
```python
1.0
```
```python
, seed=None, dtype=tf
```
```python
.float
```
```python
32)：正态分布初始化
tf
```
```python
.truncated
```
```python
_normal_initializer(mean=
```
```python
0.0
```
```python
, stddev=
```
```python
1.0
```
```python
, seed=None, dtype=tf
```
```python
.float
```
```python
32)：截取的正态分布初始化
tf
```
```python
.random
```
```python
_uniform_initializer(minval=
```
```python
0
```
```python
, maxval=None, seed=None, dtype=tf
```
```python
.float
```
```python
32)：均匀分布初始化
tf
```
```python
.zeros
```
```python
_initializer(shape, dtype=tf
```
```python
.float
```
```python
32, partition_info=None)：全
```
```python
0
```
```python
常量初始化
tf
```
```python
.ones
```
```python
_initializer(dtype=tf
```
```python
.float
```
```python
32, partition_info=None)：全
```
```python
1
```
```python
常量初始化
tf
```
```python
.uniform
```
```python
_unit_scaling_initializer(factor=
```
```python
1.0
```
```python
, seed=None, dtype=tf
```
```python
.float
```
```python
32)：均匀分布（不指定最小、最大值)初始化
tf
```
```python
.variance
```
```python
_scaling_initializer(scale =
```
```python
1.0
```
```python
, mode =
```
```python
"fan_in"
```
```python
, distribution =
```
```python
"normal"
```
```python
, seed = None, dtype = dtypes
```
```python
.float
```
```python
32)：由mode确定数量的截取的正态分布或均匀分
tf
```
```python
.orthogonal
```
```python
_initializer(gain=
```
```python
1.0
```
```python
, dtype=tf
```
```python
.float
```
```python
32, seed=None)：正交矩阵初始化
tf
```
```python
.glorot
```
```python
_uniform_initializer(seed=None, dtype=tf
```
```python
.float
```
```python
32)：由输入单元节点数和输出单元节点数确定的均匀分布初始化
tf
```
```python
.glorot
```
```python
_normal_initializer(seed=None, dtype=tf
```
```python
.float
```
```python
32)：由输入单元节点数和输出单元节点数确定的截取的正态分布初始化
```
```python
note:
```
```python
tf
```
```python
.get
```
```python
_variable中initializer的初始化不需要指定shape了，已在外面指定
```
## 2.tf.get_variable函数：
函数：tf.Variable，*新建一个变量。*
**Variable(initial_value=None, trainable=True, collections=None, validate_shape=True, caching_device=None, name=None, variable_def=None, dtype=None, expected_shape=None, import_scope=None)**
**initial_value为初始化变量的值，有以下几种方法：**
```python
tf.random_normal(shape, mean=
```
```python
0.0
```
```python
, stddev=
```
```python
1.0
```
```python
, dtype=tf.float32, seed=
```
```python
None
```
```python
, name=
```
```python
None
```
```python
)
tf.truncated_normal(shape, mean=
```
```python
0.0
```
```python
, stddev=
```
```python
1.0
```
```python
, dtype=tf.float32, seed=
```
```python
None
```
```python
, name=
```
```python
None
```
```python
)
tf.random_uniform(shape, minval=
```
```python
0
```
```python
, maxval=
```
```python
None
```
```python
, dtype=tf.float32, seed=
```
```python
None
```
```python
, name=
```
```python
None
```
```python
)
tf.random_shuffle(value, seed=
```
```python
None
```
```python
, name=
```
```python
None
```
```python
)
tf.random_crop(value, size, seed=
```
```python
None
```
```python
, name=
```
```python
None
```
```python
)
tf.multinomial(logits, num_samples, seed=
```
```python
None
```
```python
, name=
```
```python
None
```
```python
)
tf.random_gamma(shape, alpha, beta=
```
```python
None
```
```python
, dtype=tf.float32, seed=
```
```python
None
```
```python
, name=
```
```python
None
```
```python
)
tf.set_random_seed(seed) 设置产生随机数的种子
tf.zeros(shape, dtype=tf.float32, name=
```
```python
None
```
```python
)
tf.zeros_like(tensor, dtype=
```
```python
None
```
```python
, name=
```
```python
None
```
```python
)
tf.ones(shape, dtype=tf.float32, name=
```
```python
None
```
```python
)
tf.ones_like(tensor, dtype=
```
```python
None
```
```python
, name=
```
```python
None
```
```python
)
tf.fill(dims, value, name=
```
```python
None
```
```python
)
tf.constant(value, dtype=
```
```python
None
```
```python
, shape=
```
```python
None
```
```python
, name=
```
```python
'Const'
```
```python
)
```
参考：[https://www.w3cschool.cn/tensorflow_python/](https://www.w3cschool.cn/tensorflow_python/)


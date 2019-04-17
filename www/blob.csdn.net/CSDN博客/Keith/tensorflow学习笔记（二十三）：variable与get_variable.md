# tensorflow学习笔记（二十三）：variable与get_variable - Keith - CSDN博客





2016年12月16日 19:14:23[ke1th](https://me.csdn.net/u012436149)阅读数：61515
所属专栏：[tensorflow学习笔记](https://blog.csdn.net/column/details/13300.html)









# Variable

tensorflow中有两个关于variable的op，`tf.Variable()`与`tf.get_variable()`下面介绍这两个的区别

## tf.Variable与tf.get_variable()

```python
tf.Variable(initial_value=None, trainable=True, collections=None, validate_shape=True, 
caching_device=None, name=None, variable_def=None, dtype=None, expected_shape=None, 
import_scope=None)
```

```python
tf.get_variable(name, shape=None, dtype=None, initializer=None, regularizer=None, 
trainable=True, collections=None, caching_device=None, partitioner=None, validate_shape=True, 
custom_getter=None)
```

## 区别
- 使用`tf.Variable`时，如果检测到命名冲突，系统会自己处理。使用`tf.get_variable()`时，系统不会处理冲突，而会报错

```python
import tensorflow as tf
w_1 = tf.Variable(3,name="w_1")
w_2 = tf.Variable(1,name="w_1")
print w_1.name
print w_2.name
#输出
#w_1:0
#w_1_1:0
```

```python
import tensorflow as tf

w_1 = tf.get_variable(name="w_1",initializer=1)
w_2 = tf.get_variable(name="w_1",initializer=2)
#错误信息
#ValueError: Variable w_1 already exists, disallowed. Did
#you mean to set reuse=True in VarScope?
```
- 基于这两个函数的特性，当我们需要共享变量的时候，需要使用`tf.get_variable()`。在其他情况下，这两个的用法是一样的

## get_variable()与Variable的实质区别

来看下面一段代码：

```python
import tensorflow as tf

with tf.variable_scope("scope1"):
    w1 = tf.get_variable("w1", shape=[])
    w2 = tf.Variable(0.0, name="w2")
with tf.variable_scope("scope1", reuse=True):
    w1_p = tf.get_variable("w1", shape=[])
    w2_p = tf.Variable(1.0, name="w2")

print(w1 is w1_p, w2 is w2_p)
#输出
#True  False
```

看到这，就可以明白官网上说的参数复用的真面目了。由于`tf.Variable()` 每次都在创建新对象，所有`reuse=True` 和它并没有什么关系。对于`get_variable()`，来说，如果已经创建的变量对象，就把那个对象返回，如果没有创建变量对象的话，就创建一个新的。

## random Tensor

可用于赋值给`tf.Variable()`的第一个参数

```python
tf.random_normal(shape, mean=0.0, stddev=1.0, dtype=tf.float32, seed=None, name=None)

tf.truncated_normal(shape, mean=0.0, stddev=1.0, dtype=tf.float32, seed=None, name=None)

tf.random_uniform(shape, minval=0, maxval=None, dtype=tf.float32, seed=None, name=None)

tf.random_shuffle(value, seed=None, name=None)

tf.random_crop(value, size, seed=None, name=None)

tf.multinomial(logits, num_samples, seed=None, name=None)

tf.random_gamma(shape, alpha, beta=None, dtype=tf.float32, seed=None, name=None)

tf.set_random_seed(seed)
```

## constant value tensor

```python
tf.zeros(shape, dtype=tf.float32, name=None)

tf.zeros_like(tensor, dtype=None, name=None)

tf.ones(shape, dtype=tf.float32, name=None)

tf.ones_like(tensor, dtype=None, name=None)

tf.fill(dims, value, name=None)

tf.constant(value, dtype=None, shape=None, name='Const')
```

## initializer

```python
tf.constant_initializer(value=0, dtype=tf.float32)
tf.random_normal_initializer(mean=0.0, stddev=1.0, seed=None, dtype=tf.float32)
tf.truncated_normal_initializer(mean=0.0, stddev=1.0, seed=None, dtype=tf.float32)
tf.random_uniform_initializer(minval=0, maxval=None, seed=None, dtype=tf.float32)
tf.uniform_unit_scaling_initializer(factor=1.0, seed=None, dtype=tf.float32)
tf.zeros_initializer(shape, dtype=tf.float32, partition_info=None)
tf.ones_initializer(dtype=tf.float32, partition_info=None)
tf.orthogonal_initializer(gain=1.0, dtype=tf.float32, seed=None)
```

**参考资料**
[https://www.tensorflow.org/api_docs/python/state_ops/variables#Variable](https://www.tensorflow.org/api_docs/python/state_ops/variables#Variable)
[https://www.tensorflow.org/api_docs/python/state_ops/sharing_variables#get_variable](https://www.tensorflow.org/api_docs/python/state_ops/sharing_variables#get_variable)
[https://www.tensorflow.org/versions/r0.10/api_docs/python/constant_op/](https://www.tensorflow.org/versions/r0.10/api_docs/python/constant_op/)
[https://www.tensorflow.org/api_docs/python/state_ops/](https://www.tensorflow.org/api_docs/python/state_ops/)




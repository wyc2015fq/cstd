# TensorFlow之一—参数初始化 - wsp_1138886114的博客 - CSDN博客





2019年01月25日 21:49:06[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：205











### 文章目录
- [一、初始化为 常量（一维数组，多维数组）](#__2)
- [`tf.constant_initializer(value)`](#tfconstant_initializervalue_10)
- [`tf.zeros_initializer()` 和 `tf.ones_initializer()`](#tfzeros_initializer__tfones_initializer_32)
- [二、初始化为 正太分布](#__58)
- [`tf.random_normal_initializer() 和 tf.truncated_normal_initializer()`](#tfrandom_normal_initializer__tftruncated_normal_initializer_59)
- [三、初始化为 均匀分布](#__96)
- [`tf.random_uniform_initializer()`](#tfrandom_uniform_initializer_97)
- [`tf.uniform_unit_scaling_initializer()`](#tfuniform_unit_scaling_initializer_119)
- [四、初始化为 变尺度正太分布、均匀分布](#__132)
- [五、其他初始化方式](#_190)



在对张量进行计算之前，我们需要对其初始化，主要有如下以下初始化方法，关于详情请查看 [http://www.tensorfly.cn/tfdoc/how_tos/overview.html](http://www.tensorfly.cn/tfdoc/how_tos/overview.html)


### 一、初始化为 常量（一维数组，多维数组）
- **graph**（需要进行计算的任务） 与 **session** （会话的上下文，用于执行图）
- **Tensor**（rank,shape,data types）依次是：张量的维度，数据形状，数据类型
- **Variables** (变量)与**constant** (常量) 的区别
- **variables** 与 **get_variables** 的区别
- **placeholders** 与 **feed_dict**
- **constant** 、**variable**及 **placeholder**的异同 [详情请点击](https://blog.csdn.net/wsp_1138886114/article/details/80686053)。

##### `tf.constant_initializer(value)`

生成一个初始值为常量value的tensor对象。构造函数定义：

```python
def __init__(self, value=0, dtype=dtypes.float32, verify_shape=False):
    self.value = value                    # 指定的常量
    self.dtype = dtypes.as_dtype(dtype)   # 数据类型
    self._verify_shape = verify_shape     # 是否可调tensor的形状，默认可以调整
```

示例：

```python
import tensorflow as tf

value = [0,1,2,3,4,5]
init = tf.constant_initializer(value)

with tf.Session() as session:
    x = tf.get_variable("x",shape=[6],initializer=init)
    x.initializer.run()
    print(x.eval())

输出：[0. 1. 2. 3. 4. 5.]
```

##### `tf.zeros_initializer()` 和 `tf.ones_initializer()`

分别用来初始化全0和全1的tensor对象。此法经常用来初始化偏置项。

```python
import tensorflow as tf

init_zeros = tf.zeros_initializer()
init_ones = tf.ones_initializer()

with tf.Session() as session:
    x= tf.get_variable('x', shape=[4,4], initializer=init_zeros)
    y = tf.get_variable('y', shape=[4,4], initializer=init_ones)
    x.initializer.run()
    y.initializer.run()
    print(x.eval())
    print(y.eval())

输出：
[[0. 0. 0. 0.]
 [0. 0. 0. 0.]
 [0. 0. 0. 0.]
 [0. 0. 0. 0.]]
[[1. 1. 1. 1.]
 [1. 1. 1. 1.]
 [1. 1. 1. 1.]
 [1. 1. 1. 1.]]
```

### 二、初始化为 正太分布

##### `tf.random_normal_initializer() 和 tf.truncated_normal_initializer()`

初始化参数为正太分布在神经网络中应用的最多，可以初始化为**标准正太分布**和**截断正太分布**。

tf.random_normal_initializer() 类来生成一组符合标准正太分布的tensor。

tf.truncated_normal_initializer() 类来生成一组符合截断正太分布的tensor。

这两个类的构造函数定义如下：

```python
def __init__(self, mean=0.0, stddev=1.0, seed=None, dtype=dtypes.float32):
    self.mean = mean                 # 正太分布的均值，默认值0
    self.stddev = stddev             # 正太分布的标准差，默认值1
    self.seed = seed                 # 随机数种子，指定seed的值可以使生成数据复现
    self.dtype = _assert_float_dtype(dtypes.as_dtype(dtype))        # 数据类型
```

示例：

```python
import tensorflow as tf
 
init_random = tf.random_normal_initializer(mean=0.0, stddev=1.0, seed=None, dtype=tf.float32)
init_truncated = tf.truncated_normal_initializer(mean=0.0, stddev=1.0, seed=None, dtype=tf.float32)

x = tf.get_variable('x', shape=[10], initializer=init_random)
y = tf.get_variable('y', shape=[10], initializer=init_truncated)
init = tf.global_variables_initializer()

with tf.Session() as session:
    session.run(init)
    print(session.run(x))
    print(session.run(y))

输出：
[-0.8217834  -1.9128209  -1.3991181   0.34968138 -1.896406   -0.56199044
 -0.23541978 -0.73091364  1.5376586   1.8318678 ]
[ 0.6299984   0.21290737  1.836676    0.46953124 -1.8452135   1.1208061
 -0.47813228  0.8735586  -1.4976063  -0.23585671]
```

### 三、初始化为 均匀分布

##### `tf.random_uniform_initializer()`

**`tf.random_uniform_initializer()`** 生成一组符合均匀分布的tensor。在实际生产环境中用的少。函数定义如下：

```python
def __init__(self, minval=0, maxval=None, seed=None, dtype=dtypes.float32):
    self.minval = minval                 # 最小值
    self.maxval = maxval                 # 最大值
    self.seed = seed                     # 随机数种子
    self.dtype = dtypes.as_dtype(dtype)  # 数据类型
```

示例：

```python
import tensorflow as tf
 
init_uniform = tf.random_uniform_initializer(minval=0, maxval=10, seed=None, dtype=tf.float32)
 
with tf.Session() as sess:
    x = tf.get_variable('x', shape=[5], initializer=init_uniform)
    x.initializer.run()
    print(x.eval())

输出：[9.381029  1.2081659 5.7928753 8.464394  3.7529194]
```

##### `tf.uniform_unit_scaling_initializer()`

**`tf.uniform_unit_scaling_initializer()`** 功能同上一个均匀分布函数，不过不需要指定最大最小值，是通过公式计算出来的（input_size是生成数据的维度，factor是系数）如下：

max_val = math.sqrt(3 / input_size) * factor

min_val = -max_val
构造函数是：

```python
def __init__(self, factor=1.0, seed=None, dtype=dtypes.float32):
    self.factor = factor    # 系数
    self.seed = seed        # 随机数种子
    self.dtype = _assert_float_dtype(dtypes.as_dtype(dtype))  # 数据类型
```

### 四、初始化为 变尺度正太分布、均匀分布

**`tf.variance_scaling_initializer()`** 类可以生成 **截断正太分布** 和 **均匀分布** 的tensor，增加了更多的控制参数。构造函数：

```python
def __init__(self,scale=1.0, mode="fan_in", 
             distribution="normal", seed=None,
             dtype=dtypes.float32):
    if scale <= 0.:
        raise ValueError("`scale` must be positive float.")
    if mode not in {"fan_in", "fan_out", "fan_avg"}:
        raise ValueError("Invalid `mode` argument:", mode)
    distribution = distribution.lower()
    if distribution not in {"normal", "uniform"}:
        raise ValueError("Invalid `distribution` argument:", distribution)
    self.scale = scale               # 尺度缩放
    self.mode = mode                 # 用于控制计算标准差 stddev的值
    self.distribution = distribution # 定义生成的tensor的分布是截断正太分布还是均匀分布
    self.seed = seed
    self.dtype = _assert_float_dtype(dtypes.as_dtype(dtype))
    
"""
distribution 选‘normal’，为截断正太分布，标准差 stddev = sqrt(scale/n), n的取值根据mode的不同设置而不同：
mode = "fan_in"， n为输入单元的结点数；         
mode = "fan_out"，n为输出单元的结点数；
mode = "fan_avg",n为输入和输出单元结点数的平均值;


distribution选 ‘uniform’，为均匀分布的随机数tensor，最大值 max_value和 最小值 min_value 的计算公式：
max_value = sqrt(3 * scale / n)
min_value = -max_value
"""
```

示例：

```python
import tensorflow as tf
 
init_variance_scaling_normal = tf.variance_scaling_initializer(scale=1.0,mode="fan_in",
                                                               distribution="normal",seed=None,dtype=tf.float32)
init_variance_scaling_uniform = tf.variance_scaling_initializer(scale=1.0,mode="fan_in",
                                                                distribution="uniform",seed=None,dtype=tf.float32)

x = tf.get_variable('x',shape=[2,4],initializer=init_variance_scaling_normal)
y = tf.get_variable('y',shape=[2,4],initializer=init_variance_scaling_uniform)


init = tf.global_variables_initializer()

with tf.Session() as session:
    session.run(init)
    print(session.run(x))
    print(session.run(y))

输出：
[[-0.7211887   0.8433075   0.20461018 -0.08919425]
 [ 0.90516967 -0.4645301   0.62844646  1.1735324 ]]
[[ 0.19446981 -1.1852347  -1.201917    1.1782695 ]
 [-1.2183801   0.05404139  1.0391353   0.11931932]]
```

### 五、其他初始化方式

`tf.orthogonal_initializer()` 初始化为**正交矩阵**的随机数，形状最少需要是二维的
`tf.glorot_uniform_initializer()` 初始化为与输入输出节点数相关的均匀分布随机数
`tf.glorot_normal_initializer()` 初始化为与输入输出节点数相关的截断正太分布随机数

示例：
```python
import tensorflow as tf
 
init_orthogonal = tf.orthogonal_initializer(gain=1.0, seed=None, dtype=tf.float32)
init_glorot_uniform = tf.glorot_uniform_initializer()
init_glorot_normal = tf.glorot_normal_initializer()

with tf.Session() as sess:
    x = tf.get_variable('x', shape=[2, 4], initializer=init_orthogonal)
    y = tf.get_variable('y', shape=[8], initializer=init_glorot_uniform)
    z = tf.get_variable('z', shape=[8], initializer=init_glorot_normal)

    x.initializer.run()
    y.initializer.run()
    z.initializer.run()

    print("x:\n", x.eval())
    print("y:\n", y.eval())
    print("z:\n", z.eval())

输出：
x:
 [[-1.18477225e-01 -1.01901613e-01 -9.39625502e-01  3.04438859e-01]
 [-9.47847426e-01  3.31364572e-04  1.99907243e-01  2.48237878e-01]]
y:
 [ 0.3285622  -0.11004734  0.23961973  0.36906475 -0.5068728  -0.18283468
 -0.22346807  0.10944742]
z:
 [-0.6496676   0.22893634  0.00468164  0.3642434   0.18585554  0.06581366
 -0.38038838  0.1071352 ]
```

如下一个小例子

```python
import tensorflow as tf
import numpy as np


x_data = np.float32(np.random.rand(2, 100))       # 生成一个由100个数浮点数组成的二维数组
y_data = np.dot([0.100, 0.200], x_data) + 0.300

# 构造一个线性模型
b = tf.Variable(tf.zeros([1]))
W = tf.Variable(tf.random_uniform([1, 2], -1.0, 1.0))
y = tf.matmul(W, x_data) + b

# 最小化方差
loss = tf.reduce_mean(tf.square(y - y_data))       #损失函数
optimizer = tf.train.GradientDescentOptimizer(0.5) #梯度下降优化器
train = optimizer.minimize(loss)


init = tf.global_variables_initializer()          # 初始化变量

with tf.Session() as sess:
    sess.run(init)
    for step in range(0, 201):
        sess.run(train)
        if step % 20 == 0:                       # 每20步输出
            print(step, sess.run(W), sess.run(b))

输出：
0 [[0.6297652 0.65284  ]] [-0.54393715]
20 [[0.28837553 0.3881851 ]] [0.09904786]
40 [[0.15730362 0.26102772]] [0.23685846]
60 [[0.1177224  0.21948564]] [0.28014672]
80 [[0.10552805 0.20617528]] [0.29375553]
100 [[0.1017318  0.20194992]] [0.2980356]
120 [[0.1005437  0.20061459]] [0.299382]
140 [[0.10017087 0.20019354]] [0.29980558]
160 [[0.10005374 0.20006093]] [0.29993883]
180 [[0.10001688 0.20001915]] [0.29998076]
200 [[0.10000531 0.20000601]] [0.29999396]
```






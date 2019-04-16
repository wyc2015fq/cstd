# Tensorflow中关于参数初始化的方法 - 一个跳popping的quant的博客 - CSDN博客





2018年04月01日 20:42:52[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：5068








在对神经网络模型进行训练的时候，训练的就是模型中的Weight、Bias参数，要想模型效果好，当然参数就要训练到一个好的结果了，因此参数的初始化在训练时也起到了非常重要的作用，好的初始化参数可以加快模型的收敛，尽快得到好的效果，否则容易使模型的收敛变慢或者造成结果的发散。在tensorflow中有很多关于参数初始化的方法，以下内容转自以下链接：

http://www.mamicode.com/info-detail-1835147.html







所有的初始化方法都定义在[`tensorflow/python/ops/init_ops.py`](https://www.github.com/tensorflow/tensorflow/blob/r1.1/tensorflow/python/ops/init_ops.py)

**1、tf.constant_initializer()**

也可以简写为tf.Constant()

初始化为常数，这个非常有用，通常偏置项就是用它初始化的。

由它衍生出的两个初始化方法：

a、 tf.zeros_initializer()， 也可以简写为tf.Zeros()

b、tf.ones_initializer(), 也可以简写为tf.Ones()

例：在卷积层中，将偏置项b初始化为0，则有多种写法：

```
conv1 = tf.layers.conv2d(batch_images, 
                         filters=64,
                         kernel_size=7,
                         strides=2,
                         activation=tf.nn.relu,
                         kernel_initializer=tf.TruncatedNormal(stddev=0.01)
                         bias_initializer=tf.Constant(0),
                        )
```

或者：
`bias_initializer=tf.constant_initializer(0)`
或者：
`bias_initializer=tf.zeros_initializer()`
或者：
`bias_initializer=tf.Zeros()`


例：如何将W初始化成拉普拉斯算子？

```
value = [1, 1, 1, 1, -8, 1, 1, 1，1]
init = tf.constant_initializer(value)
W= tf.get_variable(‘W‘, shape=[3, 3], initializer=init)
```

**2、tf.truncated_normal_initializer()**

或者简写为tf.TruncatedNormal()

生成截断正态分布的随机数，这个初始化方法好像在tf中用得比较多。

它有四个参数（mean=0.0, stddev=1.0, seed=None, dtype=dtypes.float32)，分别用于指定均值、标准差、随机数种子和随机数的数据类型，一般只需要设置stddev这一个参数就可以了。

例：

```
conv1 = tf.layers.conv2d(batch_images, 
                         filters=64,
                         kernel_size=7,
                         strides=2,
                         activation=tf.nn.relu,
                         kernel_initializer=tf.TruncatedNormal(stddev=0.01)
                         bias_initializer=tf.Constant(0),
                        )
```

或者：

```
conv1 = tf.layers.conv2d(batch_images, 
                         filters=64,
                         kernel_size=7,
                         strides=2,
                         activation=tf.nn.relu,
                         kernel_initializer=tf.truncated_normal_initializer(stddev=0.01)
                         bias_initializer=tf.zero_initializer(),
                        )
```



**3、tf.random_normal_initializer()**

可简写为 tf.RandomNormal()

生成标准正态分布的随机数，参数和truncated_normal_initializer一样。

**4、random_uniform_initializer = RandomUniform()**

可简写为tf.RandomUniform()

生成均匀分布的随机数，参数有四个（minval=0, maxval=None, seed=None, dtype=dtypes.float32)，分别用于指定最小值，最大值，随机数种子和类型。

**5、tf.uniform_unit_scaling_initializer()**

可简写为tf.UniformUnitScaling()

和均匀分布差不多，只是这个初始化方法不需要指定最小最大值，是通过计算出来的。参数为（factor=1.0, seed=None, dtype=dtypes.float32)
`max_val = math.sqrt(3 / input_size) * factor`
这里的input_size是指输入数据的维数，假设输入为x, 运算为x * W，则input_size= `W.shape[0]`

它的分布区间为[ -max_val, max_val]

**6、tf.variance_scaling_initializer()**

可简写为tf.VarianceScaling()

参数为（scale=1.0,mode="fan_in",distribution="normal",seed=None，dtype=dtypes.float32)

scale: 缩放尺度（正浮点数）

mode:  "fan_in", "fan_out", "fan_avg"中的一个，用于计算标准差stddev的值。

distribution：分布类型，"normal"或“uniform"中的一个。

当 distribution="normal" 的时候，生成truncated normaldistribution（截断正态分布） 的随机数，其中stddev = sqrt(scale / n) ，n的计算与mode参数有关。

      如果mode = "fan_in"， n为输入单元的结点数；          

      如果mode = "fan_out"，n为输出单元的结点数；

       如果mode = "fan_avg",n为输入和输出单元结点数的平均值。

当distribution="uniform”的时候 ，生成均匀分布的随机数，假设分布区间为[-limit, limit]，则

      limit = sqrt(3 * scale / n)

**7、tf.orthogonal_initializer()**

简写为tf.Orthogonal()

生成正交矩阵的随机数。

当需要生成的参数是2维时，这个正交矩阵是由均匀分布的随机数矩阵经过SVD分解而来。

**8、tf.glorot_uniform_initializer()**

也称之为Xavier uniform initializer，由一个均匀分布（uniform distribution)来初始化数据。

假设均匀分布的区间是[-limit, limit],则

limit=sqrt(6 / (fan_in + fan_out))

其中的fan_in和fan_out分别表示输入单元的结点数和输出单元的结点数。

**9、glorot_normal_initializer（）**

也称之为 Xavier normal initializer. 由一个 truncated normal distribution来初始化数据.

stddev = sqrt(2 / (fan_in + fan_out))

其中的fan_in和fan_out分别表示输入单元的结点数和输出单元的结点数。








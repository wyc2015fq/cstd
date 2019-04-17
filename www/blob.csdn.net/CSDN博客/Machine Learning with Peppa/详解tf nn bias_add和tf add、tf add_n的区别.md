# 详解tf.nn.bias_add和tf.add、tf.add_n的区别 - Machine Learning with Peppa - CSDN博客





2018年10月13日 15:20:35[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：613








tf.add(x,y,name=None)

`x`:a tensor musut be one of                           the following types: `half`, `float32`, `float64`, `uint8`, `int8`, `int16`, `int32`, `int64`, `complex64`, `complex128`, `string`.  

`y`: A `Tensor`. Must have the same type as `x`.

`name`: A name for the operation (optional).

![](https://img-blog.csdn.net/20180426220825714?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl8zODY5ODY0OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```python
import tensorflow as tf


a=tf.constant([[1,1],[2,2],[3,3]],dtype=tf.float32)

b=tf.constant([1,-1],dtype=tf.float32)

c=tf.constant([1],dtype=tf.float32)


with tf.Session() as sess:

print('bias_add:')

print(sess.run(tf.nn.bias_add(a, b)))

#执行下面语句错误

#print(sess.run(tf.nn.bias_add(a, c)))


print('add:')

print(sess.run(tf.add(a, c)))
```

输出结果：

bias_add:

[[ 2. 0.]

[ 3. 1.]

[ 4. 2.]]

add:

[[ 2. 2.]

[ 3. 3.]

[ 4. 4.]]

![](https://img-blog.csdn.net/2018042622094486?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl8zODY5ODY0OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

tf.add_n(inputs,name=None)

函数是实现一个列表的元素的相加。就是输入的对象是一个列表，列表里的元素可以是向量，矩阵等但没有广播功能

例子：

```python
import tensorflow as tf;

import numpy as np;


input1 = tf.constant([1.0, 2.0, 3.0])

input2 = tf.Variable(tf.random_uniform([3]))

output = tf.add_n([input1, input2])


with tf.Session() as sess:

sess.run(tf.initialize_all_variables())

print (sess.run(input1 + input2))

print (sess.run(output))
```

输出：



[ 1.30945706  2.29760814  3.81558323]
[ 1.30945706  2.29760814  3.81558323]



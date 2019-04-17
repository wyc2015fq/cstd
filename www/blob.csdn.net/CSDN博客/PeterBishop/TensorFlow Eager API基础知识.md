# TensorFlow Eager API基础知识 - PeterBishop - CSDN博客





2018年12月15日 15:44:17[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：35








# Basic introduction to TensorFlow's Eager API

简单介绍一下TensorFlow的Eager API入门。
- Author: Aymeric Damien
- Project: [https://github.com/aymericdamien/TensorFlow-Examples/](https://github.com/aymericdamien/TensorFlow-Examples/)

### What is TensorFlow's Eager API ?

*Eager execution is an imperative, define-by-run interface where operations are executed immediately as they are called from Python. This makes it easier to get started with TensorFlow, and can make research and development more intuitive. A vast majority of the TensorFlow API remains the same whether eager execution is enabled or not. As a result, the exact same code that constructs TensorFlow graphs (e.g. using the layers API) can be executed imperatively by using eager execution. Conversely, most models written with Eager enabled can be converted to a graph that can be further optimized and/or extracted for deployment in production without changing code. - Rajat Monga*

More info: [https://research.googleblog.com/2017/10/eager-execution-imperative-define-by.html](https://research.googleblog.com/2017/10/eager-execution-imperative-define-by.html)

```python
from __future__ import absolute_import, division, print_function

import numpy as np
import tensorflow as tf
```

```python
# Set Eager API
print("Setting Eager mode...")
tf.enable_eager_execution()
tfe = tf.contrib.eager
```

```
Setting Eager mode...
```

```python
# 定义常数
print("Define constant tensors")
a = tf.constant(2)
print("a = %i" % a)
b = tf.constant(3)
print("b = %i" % b)
```

Define constant tensors

a = 2

b = 3 

```python
# 不需要调用 tf.Session
print("Running operations, without tf.Session")
c = a + b
print("a + b = %i" % c)
d = a * b
print("a * b = %i" % d)
```

```
Running operations, without tf.Session
a + b = 5
a * b = 6
```

```python
# 完全兼容Numpy
print("Mixing operations with Tensors and Numpy Arrays")

# 定义常数张量
a = tf.constant([[2., 1.],
                 [1., 0.]], dtype=tf.float32)
print("Tensor:\n a = %s" % a)
b = np.array([[3., 0.],
              [5., 1.]], dtype=np.float32)
print("NumpyArray:\n b = %s" % b)
```

```
Mixing operations with Tensors and Numpy Arrays
Tensor:
 a = tf.Tensor(
[[2. 1.]
 [1. 0.]], shape=(2, 2), dtype=float32)
NumpyArray:
 b = [[3. 0.]
 [5. 1.]]
```

```python
#不需要调用 tf.Session
print("Running operations, without tf.Session")

c = a + b
print("a + b = %s" % c)

d = tf.matmul(a, b)
print("a * b = %s" % d)
```

```
Running operations, without tf.Session
a + b = tf.Tensor(
[[5. 1.]
 [6. 1.]], shape=(2, 2), dtype=float32)
a * b = tf.Tensor(
[[11.  1.]
 [ 3.  0.]], shape=(2, 2), dtype=float32)
```

```python
print("Iterate through Tensor 'a':")
for i in range(a.shape[0]):
    for j in range(a.shape[1]):
        print(a[i][j])
```

```
Iterate through Tensor 'a':
tf.Tensor(2.0, shape=(), dtype=float32)
tf.Tensor(1.0, shape=(), dtype=float32)
tf.Tensor(1.0, shape=(), dtype=float32)
tf.Tensor(0.0, shape=(), dtype=float32)
```




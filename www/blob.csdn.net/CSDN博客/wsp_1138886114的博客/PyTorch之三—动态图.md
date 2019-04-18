# PyTorch之三—动态图 - wsp_1138886114的博客 - CSDN博客





2019年02月21日 19:06:06[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：36标签：[PyTorch之三—动态图](https://so.csdn.net/so/search/s.do?q=PyTorch之三—动态图&t=blog)
个人分类：[pytorch](https://blog.csdn.net/wsp_1138886114/article/category/8293033)












### 文章目录
- [一、概述](#_1)
- [二、Torch](#Torch_9)
- [三、动态图效果](#_12)




### 一、概述

目前神经网络框架分为**静态图框架**和**动态图框架**，PyTorch 和 TensorFlow、Caffe 等框架最大的区别就是他们拥有不同的计算图表现形式。

TensorFlow 使用静态图，这意味着我们先定义计算图，然后不断使用它，中间是不能够改变它的计算图的，且定义静态图时需要使用新的特殊语法，这也意味着图设定时**无法使用if、while、for-loop等结构**，而是需要特殊的由框架专门设计的语法，在构建图时，我们需要考虑到所有的情况（即各个if分支图结构必须全部在图中，即使不一定会在每一次运行时使用到），使得静态图异常庞大占用过多显存。

而在 PyTorch 中，它兼容python的各种逻辑控制语法，每次都会重新构建一个新的计算图。通过这次课程，我们会了解静态图和动态图之间的优缺点。

对于使用者来说，两种形式的计算图有着非常大的区别，同时静态图和动态图都有他们各自的优点，比如动态图比较方便debug，使用者能够用任何他们喜欢的方式进行debug，同时非常直观，而静态图是通过先定义后运行的方式，之后再次运行的时候就不再需要重新构建计算图，所以速度会比动态图更快。

### 二、Torch

Torch 自称为神经网络界的 Numpy, 因为他能将 torch 产生的 tensor 放在 GPU 中加速运算 (前提是你有合适的 GPU), 就像 Numpy 会把 array 放在 CPU 中加速运算。Torch和Numpy之间可以进行自由的切换。Pytorch中很多的数学计算与numpy中的数学计算函数是相同（相似）的，可谓直接切换逻辑。最方便的是它的[自动求导](https://blog.csdn.net/wsp_1138886114/article/details/86593370)。

### 三、动态图效果

下面比较while循环语句在TensorFlow和Pytorch中的定义
- **Tensorflow**

TensorFlow 需要将整个图构建成静态的，换句话说，每次运行的时候图都是一样的，是不能够改变的，所以不能直接使用 Python 的 while 循环语句，需要使用辅助函数 **tf.while_loop** 写成 TensorFlow 内部的形式

```python
import tensorflow as tf

first_counter = tf.constant(0)
second_counter = tf.constant(10)

def cond(first_counter, second_counter, *args):
    return first_counter < second_counter

def body(first_counter, second_counter):
    first_counter = tf.add(first_counter, 2)
    second_counter = tf.add(second_counter, 1)
    return first_counter , second_counter

c1, c2 = tf.while_loop(cond, body, [first_counter, second_counter])

with tf.Session() as sess:
    counter_1_res, counter_2_res = sess.run([c1,c2])
print(counter_1_res)
print(counter_2_res)

输出：
20
20
```

PyTorch 的动态图机制，这使得我们能够使用 Python 的 while 写循环，非常方便。代码如下，是不是很直观！

```python
import torch

first_counter = torch.Tensor([0])
second_counter = torch.Tensor([10])

while (first_counter < second_counter)[0]:
    first_counter += 2
    second_counter += 1

print(first_counter)
print(second_counter)

输出：
tensor([20.])
tensor([20.])
```

特别鸣谢
[https://www.cnblogs.com/hellcat/p/8436955.html](https://www.cnblogs.com/hellcat/p/8436955.html)
[https://blog.csdn.net/qjk19940101/article/details/79557866](https://blog.csdn.net/qjk19940101/article/details/79557866)




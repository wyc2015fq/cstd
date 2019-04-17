# tensorflow学习笔记(三十二):conv2d_transpose ("解卷积") - Keith - CSDN博客





2017年02月18日 22:43:22[ke1th](https://me.csdn.net/u012436149)阅读数：29716
个人分类：[tensorflow](https://blog.csdn.net/u012436149/article/category/6461700)

所属专栏：[tensorflow学习笔记](https://blog.csdn.net/column/details/13300.html)









`deconv`解卷积，实际是叫做`conv_transpose`, `conv_transpose`实际是卷积的一个逆向过程，`tf` 中， 编写`conv_transpose`代码的时候，心中想着一个正向的卷积过程会很有帮助。

想象一下我们有一个正向卷积: 
`input_shape = [1,5,5,3]`
`kernel_shape=[2,2,3,1]`
`strides=[1,2,2,1]`
`padding = "SAME"`
那么，卷积激活后，我们会得到 x(就是上面代码的x)。那么，我们已知x，要想得到input_shape 形状的 tensor，我们应该如何使用`conv2d_transpose`函数呢？ 

就用下面的代码

```python
import tensorflow as tf
tf.set_random_seed(1)
x = tf.random_normal(shape=[1,3,3,1])
#正向卷积的kernel的模样
kernel = tf.random_normal(shape=[2,2,3,1])

# strides 和padding也是假想中 正向卷积的模样。当然，x是正向卷积后的模样
y = tf.nn.conv2d_transpose(x,kernel,output_shape=[1,5,5,3],
    strides=[1,2,2,1],padding="SAME")
# 在这里，output_shape=[1,6,6,3]也可以，考虑正向过程，[1,6,6,3]
# 通过kernel_shape:[2,2,3,1],strides:[1,2,2,1]也可以
# 获得x_shape:[1,3,3,1]
# output_shape 也可以是一个 tensor
sess = tf.Session()
tf.global_variables_initializer().run(session=sess)

print(y.eval(session=sess))
```

**conv2d_transpose 中会计算 output_shape 能否通过给定的参数计算出 inputs的维度，如果不能，则报错**

```python
import tensorflow as tf
from tensorflow.contrib import slim

inputs = tf.random_normal(shape=[3, 97, 97, 10])

conv1 = slim.conv2d(inputs, num_outputs=20, kernel_size=3, stride=4)

de_weight = tf.get_variable('de_weight', shape=[3, 3, 10, 20])

deconv1 = tf.nn.conv2d_transpose(conv1, filter=de_weight, output_shape=tf.shape(inputs),
                                 strides=[1, 3, 3, 1], padding='SAME')

# ValueError: Shapes (3, 33, 33, 20) and (3, 25, 25, 20) are not compatible
```

上面错误的意思是：
- conv1 的  shape 是 (3, 25, 25, 20)
- 但是 deconv1 对 conv1 求导的时候，得到的导数 shape 却是 [3, 33, 33, 20]，这个和 `conv1` 的shape 不匹配，当然要报错咯。

```python
import tensorflow as tf
from tensorflow.contrib import slim
import numpy as np

inputs = tf.placeholder(tf.float32, shape=[None, None, None, 3])

conv1 = slim.conv2d(inputs, num_outputs=20, kernel_size=3, stride=4)

de_weight = tf.get_variable('de_weight', shape=[3, 3, 3, 20])

deconv1 = tf.nn.conv2d_transpose(conv1, filter=de_weight, output_shape=tf.shape(inputs),
                                 strides=[1, 3, 3, 1], padding='SAME')

loss = deconv1 - inputs
train_op = tf.train.GradientDescentOptimizer(0.001).minimize(loss)

with tf.Session() as sess:
    tf.global_variables_initializer().run()

    for i in range(10):
        data_in = np.random.normal(size=[3, 97, 97, 3])
        _, los_ = sess.run([train_op, loss], feed_dict={inputs: data_in})
        print(los_)
# InvalidArgumentError (see above for traceback): Conv2DSlowBackpropInput: Size of out_backprop doesn't match computed: actual = 25, computed = 33
```

如果 输入的 shape 有好多 `None` 的话，那就是另外一种 报错方式了，如上所示： 

这个错误的意思是：
- `conv1` 的 shape 第二维或第三维的 shape 是 25
- 但是 deconv1 对 conv1 求导的时候，得到的 倒数 shape 的第二位或第三维却是 33

至于为什么会这样，因为 `deconv` 的计算方式就是 `conv` 求导的计算方式，`conv` 的计算方式，就是 `decov` 求导的方式。

对`deconv` 求导就相当于 拿着 `conv_transpose` 中的参数对 `deconv` 输出的值的导数做卷积。 

## 如何灵活的控制 deconv 的output shape

在 `conv2d_transpose()` 中，有一个参数，叫 `output_shape`， 如果对它传入一个 int list 的话，那么在运行的过程中，`output_shape` 将无法改变（传入`int list`已经可以满足大部分应用的需要），但是如何更灵活的控制 `output_shape` 呢？
- 传入 `tensor`

```python
# 可以用 placeholder
outputs_shape = tf.placeholder(dtype=tf.int32, shape=[4])
deconv1 = tf.nn.conv2d_transpose(conv1, filter=de_weight, output_shape=output_shape,
                                 strides=[1, 3, 3, 1], padding='SAME')

# 可以用 inputs 的shape，但是有点改变
inputs_shape = tf.shape(inputs)
outputs_shape = [inputs_shape[0], inputs_shape[1], inputs_shape[2], some_value]
deconv1 = tf.nn.conv2d_transpose(conv1, filter=de_weight, output_shape=outputs_shape,
                                 strides=[1, 3, 3, 1], padding='SAME')
```





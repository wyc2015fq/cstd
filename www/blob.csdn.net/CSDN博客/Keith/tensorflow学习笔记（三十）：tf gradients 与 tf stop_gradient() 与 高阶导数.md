# tensorflow学习笔记（三十）：tf.gradients 与 tf.stop_gradient() 与 高阶导数 - Keith - CSDN博客





2016年12月28日 09:46:11[ke1th](https://me.csdn.net/u012436149)阅读数：34870
个人分类：[tensorflow](https://blog.csdn.net/u012436149/article/category/6461700)

所属专栏：[tensorflow学习笔记](https://blog.csdn.net/column/details/13300.html)









# gradient

`tensorflow`中有一个计算梯度的函数`tf.gradients(ys, xs)`，要注意的是，`xs`中的`x`必须要与`ys`相关，不相关的话，会报错。 

代码中定义了两个变量`w1`， `w2`， 但`res`只与`w1`相关

```python
#wrong
import tensorflow as tf

w1 = tf.Variable([[1,2]])
w2 = tf.Variable([[3,4]])

res = tf.matmul(w1, [[2],[1]])

grads = tf.gradients(res,[w1,w2])

with tf.Session() as sess:
    tf.global_variables_initializer().run()
    re = sess.run(grads)
    print(re)
```

**错误信息**

TypeError: Fetch argument None has invalid type 

```python
# right
import tensorflow as tf

w1 = tf.Variable([[1,2]])
w2 = tf.Variable([[3,4]])

res = tf.matmul(w1, [[2],[1]])

grads = tf.gradients(res,[w1])

with tf.Session() as sess:
    tf.global_variables_initializer().run()
    re = sess.run(grads)
    print(re)
#  [array([[2, 1]], dtype=int32)]
```

对于`grad_ys`的测试：

```python
import tensorflow as tf

w1 = tf.get_variable('w1', shape=[3])
w2 = tf.get_variable('w2', shape=[3])

w3 = tf.get_variable('w3', shape=[3])
w4 = tf.get_variable('w4', shape=[3])

z1 = w1 + w2+ w3
z2 = w3 + w4

grads = tf.gradients([z1, z2], [w1, w2, w3, w4], grad_ys=[tf.convert_to_tensor([2.,2.,3.]),
                                                          tf.convert_to_tensor([3.,2.,4.])])

with tf.Session() as sess:
    tf.global_variables_initializer().run()
    print(sess.run(grads))
```

```
[array([ 2.,  2.,  3.],dtype=float32),
 array([ 2.,  2.,  3.], dtype=float32), 
 array([ 5.,  4.,  7.], dtype=float32), 
 array([ 3.,  2.,  4.], dtype=float32)]
```
- 可以看出，`grad_ys` 代表的是 `ys` 的头梯度

## tf.stop_gradient()

阻挡节点`BP`的梯度

```python
import tensorflow as tf

w1 = tf.Variable(2.0)
w2 = tf.Variable(2.0)

a = tf.multiply(w1, 3.0)
a_stoped = tf.stop_gradient(a)

# b=w1*3.0*w2
b = tf.multiply(a_stoped, w2)
gradients = tf.gradients(b, xs=[w1, w2])
print(gradients)
#输出
#[None, <tf.Tensor 'gradients/Mul_1_grad/Reshape_1:0' shape=() dtype=float32>]
```

可见，一个`节点`被 `stop`之后，这个节点上的梯度，就无法再向前`BP`了。由于`w1`变量的梯度只能来自`a`节点，所以，计算梯度返回的是`None`。

```python
a = tf.Variable(1.0)
b = tf.Variable(1.0)

c = tf.add(a, b)

c_stoped = tf.stop_gradient(c)

d = tf.add(a, b)

e = tf.add(c_stoped, d)

gradients = tf.gradients(e, xs=[a, b])

with tf.Session() as sess:
    tf.global_variables_initializer().run()
    print(sess.run(gradients))
#输出 [1.0, 1.0]
```

虽然 `c`节点被`stop`了，但是`a，b`还有从`d`传回的梯度，所以还是可以输出梯度值的。

```python
import tensorflow as tf

w1 = tf.Variable(2.0)
w2 = tf.Variable(2.0)
a = tf.multiply(w1, 3.0)
a_stoped = tf.stop_gradient(a)

# b=w1*3.0*w2
b = tf.multiply(a_stoped, w2)

opt = tf.train.GradientDescentOptimizer(0.1)

gradients = tf.gradients(b, xs=tf.trainable_variables())

tf.summary.histogram(gradients[0].name, gradients[0])# 这里会报错，因为gradients[0]是None
#其它地方都会运行正常，无论是梯度的计算还是变量的更新。总觉着tensorflow这么设计有点不好，
#不如改成流过去的梯度为0
train_op = opt.apply_gradients(zip(gradients, tf.trainable_variables()))

print(gradients)
with tf.Session() as sess:
    tf.global_variables_initializer().run()
    print(sess.run(train_op))
    print(sess.run([w1, w2]))
```

## 高阶导数

`tensorflow` 求 高阶导数可以使用 `tf.gradients` 来实现

```python
import tensorflow as tf

with tf.device('/cpu:0'):
    a = tf.constant(1.)
    b = tf.pow(a, 2)
    grad = tf.gradients(ys=b, xs=a) # 一阶导
    print(grad[0])
    grad_2 = tf.gradients(ys=grad[0], xs=a) # 二阶导
    grad_3 = tf.gradients(ys=grad_2[0], xs=a) # 三阶导
    print(grad_3)

with tf.Session() as sess:
    print(sess.run(grad_3))
```

**Note: 有些 op，tf 没有实现其高阶导的计算，例如 tf.add …, 如果计算了一个没有实现 高阶导的 op的高阶导， gradients 会返回 None。**




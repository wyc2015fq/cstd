# tensorflow学习笔记（二十七）：leaky relu - Keith - CSDN博客





2016年12月26日 21:27:24[ke1th](https://me.csdn.net/u012436149)阅读数：17200
所属专栏：[tensorflow学习笔记](https://blog.csdn.net/column/details/13300.html)









# tensorflow leaky relu

在`tensorflow 0.12.0`及之前，都没有内置的`leaky relu`函数，那么我们如何实现`leaky relu`函数呢？

## 方法1

```python
def relu(x, alpha=0., max_value=None):
    '''ReLU.

    alpha: slope of negative section.
    '''
    negative_part = tf.nn.relu(-x)
    x = tf.nn.relu(x)
    if max_value is not None:
        x = tf.clip_by_value(x, tf.cast(0., dtype=_FLOATX),
                             tf.cast(max_value, dtype=_FLOATX))
    x -= tf.constant(alpha, dtype=_FLOATX) * negative_part
    return x
```

## 方法2

```python
x = tf.maximum(alpha*x,x)
```

这两种方法，在`BP`的时候，梯度都会被正确的计算的。 

另外，关于`tf.clip...`函数在`BP`的时候，梯度也是会被正确计算的

```python
import tensorflow as tf
w1 = tf.Variable(0) #0或5时，打印出来1， 2时打印出来1,6时打印出来0，-1时打印出来0
g = tf.clip_by_value(w1, 0, 5)
grad = tf.gradients(g, [w1])

with tf.Session() as sess:
    tf.global_variables_initializer().run()
    print(sess.run(grad))
```

**参考资料**
[https://groups.google.com/a/tensorflow.org/forum/#!topic/discuss/V6aeBw4nlaE](https://groups.google.com/a/tensorflow.org/forum/#!topic/discuss/V6aeBw4nlaE)




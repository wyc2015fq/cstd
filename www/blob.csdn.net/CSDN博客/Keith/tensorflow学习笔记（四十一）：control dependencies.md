# tensorflow学习笔记（四十一）：control dependencies - Keith - CSDN博客





2017年05月14日 23:48:46[ke1th](https://me.csdn.net/u012436149)阅读数：19391
所属专栏：[tensorflow学习笔记](https://blog.csdn.net/column/details/13300.html)









`tf.control_dependencies()`设计是用来控制计算流图的，给图中的某些计算指定顺序。比如：我们想要获取参数更新后的值，那么我们可以这么组织我们的代码。

```python
opt = tf.train.Optimizer().minize(loss)

with tf.control_dependencies([opt]):
  updated_weight = tf.identity(weight)

with tf.Session() as sess:
  tf.global_variables_initializer().run()
  sess.run(updated_weight, feed_dict={...}) # 这样每次得到的都是更新后的weight
```

关于tf.control_dependencies的具体用法，請移步官网[https://www.tensorflow.org/api_docs/python/tf/Graph#control_dependencies](https://www.tensorflow.org/api_docs/python/tf/Graph#control_dependencies),总结一句话就是，在执行某些`op,tensor`之前，某些`op,tensor`得首先被运行。

**需要注意的是，除了 对于`Variable` 的 `identity op` 会在 `control dependence` 后重新计算，其它 `op` 都不会重新计算**， 下面例子说明这个现象

```python
import tensorflow as tf
a = tf.Variable(initial_value=[1.], dtype=tf.float32)
b = a + 3
update_a = tf.assign(a, b)
with tf.control_dependencies([update_a]):
    e = tf.identity(b) + 5
    val = tf.identity(a)

with tf.Session() as sess:
    sess.run(tf.global_variables_initializer())
    print(sess.run([e, val]))
# e=9: update_a 之前的值，可以看出，a的更新并没有使得 b被 重新计算
# val=4 : update_a 之后的值，tf.identity(var) 得到的是 var 更新后的值
```

## 下面说明两种 control_dependencies 不 work 的情况

下面有两种情况，control_dependencies不work，其实并不是它真的不work，而是我们的使用方法有问题。

**第一种情况:**

```python
import tensorflow as tf
w = tf.Variable(1.0)
ema = tf.train.ExponentialMovingAverage(0.9)
update = tf.assign_add(w, 1.0)

ema_op = ema.apply([update])
with tf.control_dependencies([ema_op]):
    ema_val = ema.average(update)

with tf.Session() as sess:
    tf.global_variables_initializer().run()
    for i in range(3):
        print(sess.run([ema_val]))
```

也许你会觉得，在我们 `sess.run([ema_val])`， `ema_op` 都会被先执行，然后再计算`ema_val`，实际情况并不是这样，为什么？

有兴趣的可以看一下源码，就会发现 `ema.average(update)` 不是一个 `op`，它只是从`ema`对象的一个字典中取出键对应的 `tensor` 而已，然后赋值给`ema_val`。这个 `tensor`是由一个在 `tf.control_dependencies([ema_op])` 外部的一个 `op` 计算得来的，所以 `control_dependencies`会失效。解决方法也很简单，看代码：

```python
import tensorflow as tf
w = tf.Variable(1.0)
ema = tf.train.ExponentialMovingAverage(0.9)
update = tf.assign_add(w, 1.0)

ema_op = ema.apply([update])
with tf.control_dependencies([ema_op]):
    ema_val = tf.identity(ema.average(update)) #一个identity搞定

with tf.Session() as sess:
    tf.global_variables_initializer().run()
    for i in range(3):
        print(sess.run([ema_val]))
```

**第二种情况：** 这个情况一般不会碰到，这是我在测试 `control_dependencies` 发现的

```python
import tensorflow as tf
w = tf.Variable(1.0)
ema = tf.train.ExponentialMovingAverage(0.9)
update = tf.assign_add(w, 1.0)

ema_op = ema.apply([update])
with tf.control_dependencies([ema_op]):
    w1 = tf.Variable(2.0)
    ema_val = ema.average(update)

with tf.Session() as sess:
    tf.global_variables_initializer().run()
    for i in range(3):
        print(sess.run([ema_val, w1]))
```

这种情况下，`control_dependencies`也不 work。读取 `w1` 的值并不会触发 `ema_op`， 原因请看代码：

```python
#这段代码出现在Variable类定义文件中第287行，
# 在创建Varible时，tensorflow是移除了dependencies了的
#所以会出现 control 不住的情况
with ops.control_dependencies(None):
	...
```




# tf.trainable_variables方法 - YZXnuaa的博客 - CSDN博客
2018年03月30日 19:00:24[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：2615
# [tf.trainable_variables方法](http://www.cnblogs.com/guqiangjs/p/7805098.html)
```python
import tensorflow as tf
v1 = tf.get_variable('v1', shape=[1])
v2 = tf.get_variable('v2', shape=[1], trainable=False)
with tf.variable_scope('scope1'):
    s1 = tf.get_variable('s1', shape=[1], initializer=tf.random_normal_initializer())
g1=tf.Graph()
g2=tf.Graph()
with g1.as_default():
    g1v1 = tf.get_variable('g1v1', shape=[1])
    g1v2 = tf.get_variable('g1v2', shape=[1], trainable=False)
    g1vs = tf.trainable_variables()
    # [<tf.Variable 'g1v1:0' shape=(1,) dtype=float32_ref>]
    print(g1vs)
with g2.as_default():
    g2v1 = tf.get_variable('g2v1', shape=[1])
    g2v2 = tf.get_variable('g2v2', shape=[1], trainable=False)
    g2vs = tf.trainable_variables()
    # [<tf.Variable 'g2v1:0' shape=(1,) dtype=float32_ref>]
    print(g2vs)
with tf.Session() as sess:
    vs = tf.trainable_variables()
    # [<tf.Variable 'v1:0' shape=(1,) dtype=float32_ref>, <tf.Variable 'scope1/s1:0' shape=(1,) dtype=float32_ref>]
    print(vs)
```
[tf.trainable_variables](https://tensorflow.google.cn/api_docs/python/tf/contrib/framework/get_trainable_variables) 返回所有 **当前计算图中** 在获取变量时未标记 `trainable=False` 的变量集合

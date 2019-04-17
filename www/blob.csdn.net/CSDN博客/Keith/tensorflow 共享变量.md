# tensorflow 共享变量 - Keith - CSDN博客





2016年10月17日 14:54:28[ke1th](https://me.csdn.net/u012436149)阅读数：2684








```python
with tf.variable_scope("try"):
    #先创建两个变量w1， w2
    w2 = tf.get_variable("w1",shape=[2,3,4], dtype=tf.float32)
    w3 = tf.get_variable("w2", shape=[2, 3, 4], dtype=tf.float32)
    #使用reuse_variables 将刚刚创建的两个变量共享
    tf.get_variable_scope().reuse_variables()
    w4 = tf.get_variable("w1", shape=[2, 3, 4], dtype=tf.float32)
    w5 = tf.get_variable("w2", shape=[2, 3, 4], dtype=tf.float32)
    #再进行共享的话，还需要再使用一次reuse_variables()
    tf.get_variable_scope().reuse_variables()
    w6 = tf.get_variable("w1", shape=[2, 3, 4], dtype=tf.float32)
    w7 = tf.get_variable("w2", shape=[2, 3, 4], dtype=tf.float32)
```

这样就可以实现变量共享了




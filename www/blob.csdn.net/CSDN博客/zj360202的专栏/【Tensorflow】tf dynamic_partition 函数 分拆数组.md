# 【Tensorflow】tf.dynamic_partition 函数 分拆数组 - zj360202的专栏 - CSDN博客





2017年11月27日 10:21:11[zj360202](https://me.csdn.net/zj360202)阅读数：3344








拆分Tensor: dynamic_partition(data, partitions, num_partition, name=None)

Tensorflow中文社区提供的展示：![](https://img-blog.csdn.net/20170627172450688?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWl0YXpoaXhpbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


实例



```python
# -*- coding:utf-8 -*-
import tensorflow as tf

x1 = tf.constant([[1,1],[1,1],[1,2],[1,2]], tf.float32)
x2 = tf.constant([[1,3],[1,2],[2,3],[2,4]], tf.float32)

#ones = tf.ones([2,1], dtype=tf.float32)
partitions = [1,0,1,0]

result = tf.dynamic_partition(x1, partitions, 2)

with tf.Session() as sess:
    r = sess.run(result)
    print r[0]
```

结果



```python
[[ 1.  1.]
 [ 1.  2.]]
```






C++接口方式



```cpp
DynamicPartition(const ::tensorflow::Scope & scope, ::tensorflow::Input data, ::tensorflow::Input partitions, int64 num_partitions)
```











# 【Tensorflow 实战】实现欧式距离 - zj360202的专栏 - CSDN博客





2017年11月24日 13:38:00[zj360202](https://me.csdn.net/zj360202)阅读数：2770








场景：通过一组图片和一些模板图片进行匹配，得到每一个图片距离最小的模板

假设数据是[1,1]和[1,2]， 模板是[2,3]和[2,4]



```python
# -*- coding:utf-8 -*-
import tensorflow as tf

x1 = tf.constant([[1,1],[1,1],[1,2],[1,2]], tf.float32)
x2 = tf.constant([[2,3],[2,4],[2,3],[2,4]], tf.float32)

ones = tf.ones([2,1], dtype=tf.float32)

#计算两矩阵对应点相减后的平方
dis = tf.square(x1-x2)
#将矩阵的行向量求和，得到一维列向量
dis_add = tf.matmul(dis, ones)
#将一维向量按模板数切成多维向量
da_reshape = tf.reshape(dis_add, [-1,2])
#得到列向量中最小的结果
result = tf.argmin(da_reshape, axis=1)

with tf.Session() as sess:
    o = sess.run(ones)
    r = sess.run(dis)
    da = sess.run(dis_add)
    da_r = sess.run(da_reshape)
    res = sess.run(result)
    print o,r, da, da_r, res
```


结果





```python
[[ 1.]
 [ 1.]] [[ 1.  4.]
 [ 1.  9.]
 [ 1.  1.]
 [ 1.  4.]] [[  5.]
 [ 10.]
 [  2.]
 [  5.]] [[  5.  10.]
 [  2.   5.]] [0 0]            #[0,0]是结果
```








# tensorflow：卷积中的 SAME VALID padding - Keith - CSDN博客





2016年12月07日 13:30:35[ke1th](https://me.csdn.net/u012436149)阅读数：1679








[http://stackoverflow.com/questions/37674306/what-is-the-difference-between-same-and-valid-padding-in-tf-nn-max-pool-of-t](http://stackoverflow.com/questions/37674306/what-is-the-difference-between-same-and-valid-padding-in-tf-nn-max-pool-of-t)

## SANE padding
- 如果需要 pad 的长度为 偶数， 那么左边右边（上边下边）分别 pad 一半
- 如果需要 pad 的长度为 奇数，平均之后 多出来的那一个 pad 到右边（下边）

数学表达式可以写成：
- left(up) = pad_len // 2
- right(bottom) = pad_len - left(up)

测试代码：

```python
import tensorflow as tf

inp = tf.ones(shape=[1, 5, 5, 1])
# 更改 weight 的size 看看结果是什么样的。
weight = tf.ones(shape=[3, 6, 1, 1]) 

conv = tf.nn.conv2d(input=inp, filter=weight, strides=[1, 1, 1, 1], padding="SAME")
conv = tf.transpose(conv, perm=[0, 3, 1, 2])
with tf.Session() as sess:
    print(sess.run(conv))
```




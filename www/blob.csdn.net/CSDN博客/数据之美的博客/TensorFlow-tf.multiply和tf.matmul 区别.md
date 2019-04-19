# TensorFlow - tf.multiply和tf.matmul 区别 - 数据之美的博客 - CSDN博客
2018年11月18日 16:04:26[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：37
```
# a
# [[1, 2, 3],
#  [4, 5, 6]] a = tf.constant([1, 2, 3, 4, 5, 6], shape=[2, 3])
# b1
# [[ 7,  8],
#  [ 9, 10],
#  [11, 12]] b1 = tf.constant([7, 8, 9, 10, 11, 12], shape=[3, 2])
#b2
#[[ 7  8  9]
# [10 11 12]] b2 = tf.constant([7, 8, 9, 10, 11, 12], shape=[2, 3])
# c矩阵相乘 第一个矩阵的列数（column）等于第二个矩阵的行数（row）
# [[ 58,  64],
#  [139, 154]] c = tf.matmul(a, b1)
# d`数元素各自相乘
#[[ 7 16 27]
# [40 55 72]] d = tf.multiply(a, b2) #维度必须相等 with tf.Session():
    print(d.eval())
--------------------- 
作者：flyfish1986 
来源：CSDN 
原文：https://blog.csdn.net/flyfish1986/article/details/79141763 
版权声明：本文为博主原创文章，转载请附上博文链接！
```

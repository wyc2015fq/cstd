# 【Tensorflow】tf.tile函数 解决矩阵相互N对M进行合并问题 - zj360202的专栏 - CSDN博客





2017年12月22日 12:52:39[zj360202](https://me.csdn.net/zj360202)阅读数：5069








 tf.tile主要的功能就是在tensorflow中对矩阵进行自身进行复制的功能，比如按行进行复制，或是按列进行复制



```python
import tensorflow as tf

a = tf.constant([[1, 2],[2, 3],[3, 4]], dtype=tf.float32) 
b = tf.constant([[4, 4],[5, 5],[6, 6],[7, 7]], dtype=tf.float32) 
c = tf.constant([[2], [2], [2], [2], [2]], dtype=tf.float32)

a_shape = tf.shape(a)
b_shape = tf.shape(b)
tile_a = tf.tile(a, [1, b_shape[0]])  # 如果是a是两维，那[1, b_shape[0]] 表示第一维复制一份，第二维复制b_shape[0]份
tile_b = tf.tile(b, [a_shape[0], 1]) 
tile_a = tf.reshape(tile_a, [-1 ,2])

cartesian_product = tf.concat([tile_a, tile_b], axis=1) 

#w = tf.add(tf.shape(a)[1], tf.shape(b)[1])
cartesian_product_res = tf.reshape(cartesian_product, [-1, 4])
c_shape = tf.shape(cartesian_product_res)
ones = tf.ones([c_shape[0], 1], dtype=tf.float32)
cartesian_product_c = tf.concat([ones, cartesian_product_res], axis=1) 

c_shape = tf.shape(c)
cartesian_product_c_shape = tf.shape(cartesian_product_c)

result_tf = tf.matmul(cartesian_product_c, c)
result_res = tf.reshape(result_tf, [-1, 4])
result_max = tf.argmax(result_res, axis=1)
'''
'''
sess = tf.Session()
#[3 2]
print(sess.run(a_shape))
#[4 2]
print(sess.run(b_shape))
#[5 1]
print(sess.run(c_shape))
#[12  5]
print(sess.run(cartesian_product_c_shape))
'''
[[ 1.  2.]
 [ 1.  2.]
 [ 1.  2.]
 [ 1.  2.]
 [ 2.  3.]
 [ 2.  3.]
 [ 2.  3.]
 [ 2.  3.]
 [ 3.  4.]
 [ 3.  4.]
 [ 3.  4.]
 [ 3.  4.]]
 '''
print(sess.run(tile_a))
'''
[[ 4.  4.]
 [ 5.  5.]
 [ 6.  6.]
 [ 7.  7.]
 [ 4.  4.]
 [ 5.  5.]
 [ 6.  6.]
 [ 7.  7.]
 [ 4.  4.]
 [ 5.  5.]
 [ 6.  6.]
 [ 7.  7.]]
'''
print(sess.run(tile_b))
#[3 3 3]
print(sess.run(result_max))
```








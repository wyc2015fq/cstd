# TensorFlow学习笔记之四——源码分析之基本操作 - SHINING的博客 - CSDN博客
2016年05月11日 11:13:36[snsn1984](https://me.csdn.net/snsn1984)阅读数：10713
所属专栏：[Tensorflow学习](https://blog.csdn.net/column/details/tensorflow.html)
例子源码地址：
[https://github.com/aymericdamien/TensorFlow-Examples/blob/master/examples/1%20-%20Introduction/basic_operations.py](https://github.com/aymericdamien/TensorFlow-Examples/blob/master/examples/1%20-%20Introduction/basic_operations.py)
根据网上的入门例子，一点点的熟悉代码和TensorFlow。对这个基本的例子，做一个注释，备忘之余分享给同样入门的初学者。
```python
import tensorflow as tf
a = tf.constant(2)
b = tf.constant(3)
```
```python
#把a,b定义为tensorflow的常量，并且赋值。
```
```python
with tf.Session() as sess:
    print "a=2, b=3"
    print "Addition with constants: %i" % sess.run(a+b)
    print "Multiplication with constants: %i" % sess.run(a*b)
```
```python
#使用Session的run()输出a+b和a*b的结果。使用with tf.Session() as sess这种用法，在sess.run()结束之后，不用调用sess.close()释放资源。
```
```python
a = tf.placeholder(tf.int16)
b = tf.placeholder(tf.int16)
```
```python
#把a,b定义成为tf.int16类型的占位符，并没有放具体的数值进去。
```
```python
add = tf.add(a, b)
mul = tf.mul(a, b)
```
```python
#在a,b两个占位符之上，定义了两个操作，add和mul。
```
```python
with tf.Session() as sess:
    print "Addition with variables: %i" % sess.run(add, feed_dict={a: 2, b: 3})
    print "Multiplication with variables: %i" % sess.run(mul, feed_dict={a: 2, b: 3})
```
```python
#给a,b赋值，并且对他们进行前文定义的add和mul操作。
```
```python
matrix1 = tf.constant([[3., 3.]])
matrix2 = tf.constant([[2.],[2.]])
product = tf.matmul(matrix1, matrix2)
```
```python
#定义两个常量矩阵，并且为这两个矩阵定义了一个matmul操作product。
```
```python
with tf.Session() as sess:
    result = sess.run(product)
    print result
```
```python
#运行product操作，并且将结果result输出。
```

# 【Tensorflow】tf.nn.embedding_lookup函数的用法 - zkq_1986的博客 - CSDN博客





2018年10月16日 19:12:22[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：37
个人分类：[tensorflow](https://blog.csdn.net/zkq_1986/article/category/7167521)









tf.nn.embedding_lookup函数的用法主要是选取一个张量里面索引对应的元素。tf.nn.embedding_lookup（tensor, id）:tensor就是输入张量，id就是张量对应的索引，其他的参数不介绍。

例如：

```python
import tensorflow as tf;
import numpy as np;
 
c = np.random.random([10,1])
b = tf.nn.embedding_lookup(c, [1, 3])
 
with tf.Session() as sess:
    sess.run(tf.initialize_all_variables())
    print sess.run(b)
    print c
```



输出：

[[ 0.77505197]

 [ 0.20635818]]

[[ 0.23976515]

 [ 0.77505197]

 [ 0.08798201]

 [ 0.20635818]

 [ 0.37183035]

 [ 0.24753178]

 [ 0.17718483]

 [ 0.38533808]

 [ 0.93345168]

 [ 0.02634772]]

分析：输出为张量的第一和第三个元素。



举例2：

```python
import tensorflow as tf

input_x = tf.placeholder(tf.int32, [None, 3], name='input_x')
keep_prob = tf.placeholder(tf.float32, name='keep_prob')
embedding = tf.get_variable('embedding', [5, 6])
embedding_inputs = tf.nn.embedding_lookup(embedding, input_x)

x=[0,1,4]
with tf.Session() as session:
    session.run(tf.global_variables_initializer())

    y1,y2=session.run([embedding,embedding_inputs], feed_dict={input_x:[x]})
    print(y1)
    print(y2)
```

输出：

[[-3.7497959e-01 -1.3574934e-01  6.3621998e-04 -3.2882205e-01

  -2.6098430e-02 -6.4995855e-01]
 [ 4.9875981e-01  7.5718999e-02  1.6283578e-01  1.7291355e-01

   6.0358101e-01 -2.5407076e-01]

 [ 7.0739985e-03  5.7720226e-01 -7.6602280e-02 -5.9611917e-02

   1.0062820e-01 -3.7122670e-01]

 [ 1.3654292e-01 -4.5706773e-01 -2.6346862e-02  1.5305221e-02

   1.6879094e-01 -1.1148870e-01]
[ 3.3369654e-01 -5.9345675e-01 -1.8395966e-01 -6.0551977e-01

   5.6261355e-01  1.8064469e-01]]

[[[-3.7497959e-01 -1.3574934e-01  6.3621998e-04 -3.2882205e-01

   -2.6098430e-02 -6.4995855e-01]

  [ 4.9875981e-01  7.5718999e-02  1.6283578e-01  1.7291355e-01

    6.0358101e-01 -2.5407076e-01]

  [ 3.3369654e-01 -5.9345675e-01 -1.8395966e-01 -6.0551977e-01

    5.6261355e-01  1.8064469e-01]]]

表示embedding的第0行，第1行，第4行，分别查找出来。




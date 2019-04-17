# 【tensorflow】tf.name_scope和tf.variable_scope以及tf.variable和tf.get_variable区别 - zkq_1986的博客 - CSDN博客





2018年10月10日 18:33:32[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：44
个人分类：[tensorflow](https://blog.csdn.net/zkq_1986/article/category/7167521)









tf.name_scope()和tf.variable_scope()是两个作用域，一般与两个创建、调用变量的函数tf.variable() 和tf.get_variable()搭配使用。它们搭配在一起的两个常见用途：1)变量共享，2)tensorboard画流程图时为了可视化封装变量。

tf.name_scope（名字范围）下可以有相同的变量名，但仅限tf.Variable的变量

tf.variable_scope（变量范围）下可以有相同的变量名，tf.get_variable（变量共享）和tf.Variable的变量都支持。



样例1:

```python
import tensorflow as tf

with tf.variable_scope('v_scope') as scope1:
    Weights1 = tf.get_variable('Weights', shape=[2,3])
    bias1 = tf.get_variable('bias', shape=[3])

# 下面来共享上面已经定义好的变量
# note: 在下面的 scope 中的变量必须已经定义过了，才能设置 reuse=True，否则会报错
with tf.variable_scope('v_scope', reuse=True) as scope2:
    Weights2 = tf.get_variable('Weights')

# 下面来共享上面已经定义好的变量
# note: 在下面的 scope 中的变量必须已经定义过了，才能设置 reuse=True，否则会报错
with tf.variable_scope('v_scope', reuse=True) as scope2:
    Weights3 = tf.get_variable('Weights')

print (Weights1.name)
print (Weights2.name)
print (Weights3.name)
```

输出：

v_scope/Weights:0

v_scope/Weights:0

v_scope/Weights:0



样例2

```python
import tensorflow as tf;  
import numpy as np;  
import matplotlib.pyplot as plt;  
 
with tf.variable_scope('V1'):
	a1 = tf.get_variable(name='a1', shape=[1], initializer=tf.constant_initializer(1))
	a2 = tf.Variable(tf.random_normal(shape=[2,3], mean=0, stddev=1), name='a2')
with tf.variable_scope('V2'):
	a3 = tf.get_variable(name='a1', shape=[1], initializer=tf.constant_initializer(1))
	a4 = tf.Variable(tf.random_normal(shape=[2,3], mean=0, stddev=1), name='a2')
  
with tf.Session() as sess:
	sess.run(tf.initialize_all_variables())
	print a1.name
	print a2.name
	print a3.name
	print a4.name
```

输出：

V1/a1:0

V1/a2:0

V2/a1:0

V2/a2:0



样例2

```python
import tensorflow as tf;  
import numpy as np;  
import matplotlib.pyplot as plt;  
 
with tf.name_scope('V1'):
	a1 = tf.get_variable(name='a1', shape=[1], initializer=tf.constant_initializer(1))
	a2 = tf.Variable(tf.random_normal(shape=[2,3], mean=0, stddev=1), name='a2')
with tf.name_scope('V2'):
	a3 = tf.get_variable(name='a1', shape=[1], initializer=tf.constant_initializer(1))
	a4 = tf.Variable(tf.random_normal(shape=[2,3], mean=0, stddev=1), name='a2')
  
with tf.Session() as sess:
	sess.run(tf.initialize_all_variables())
	print a1.name
	print a2.name
	print a3.name
	print a4.name
```

报错：Variable a1 already exists, disallowed. Did you mean to set reuse=True in VarScope? Originally defined at:

换成下面的代码就可以执行：

```python
import tensorflow as tf;  
import numpy as np;  
import matplotlib.pyplot as plt;  
 
with tf.name_scope('V1'):
	# a1 = tf.get_variable(name='a1', shape=[1], initializer=tf.constant_initializer(1))
	a2 = tf.Variable(tf.random_normal(shape=[2,3], mean=0, stddev=1), name='a2')
with tf.name_scope('V2'):
	# a3 = tf.get_variable(name='a1', shape=[1], initializer=tf.constant_initializer(1))
	a4 = tf.Variable(tf.random_normal(shape=[2,3], mean=0, stddev=1), name='a2')
  
with tf.Session() as sess:
	sess.run(tf.initialize_all_variables())
	# print a1.name
	print a2.name
	# print a3.name
	print a4.name
```

输出：

V1/a2:0

V2/a2:0

转载自：[https://blog.csdn.net/uestc_c2_403/article/details/72328815](https://blog.csdn.net/uestc_c2_403/article/details/72328815)




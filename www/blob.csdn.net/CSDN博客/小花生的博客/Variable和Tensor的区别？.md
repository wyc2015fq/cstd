
# Variable和Tensor的区别？ - 小花生的博客 - CSDN博客


2018年10月07日 20:06:06[Peanut_范](https://me.csdn.net/u013841196)阅读数：536


Tensor和Variable的区别
**代码：**c=a+b
`import tensorflow as tf
a = tf.Variable(1.0,name='a')
b = tf.Variable(2.0,name='b')
c = tf.add(a,b)
sess = tf.Session()
sess.run(tf.global_variables_initializer())
print(sess.run(c))
sess.close()`其中，a,b是**Variable**，而c是**Tensor**。
**注：**在TensorFlow中，变量的定义和初始化是分开的，所有关于图变量的赋值和计算都要通过**tf.Session的run来进行**。想要将所有图变量进行集体初始化时应该使用**tf.global_variables_initializer**。
**Variable和Tensor之间的区别：**
**1.**Variable是可更改的，而Tensor是不可更改的。
**2.**Variable用于存储网络中的权重矩阵等变量，而Tensor更多的是中间结果等。
**3.**Variable是会显示分配内存空间的，需要初始化操作（assign一个tensor），由Session管理，可以进行存储、读取、更改等操作。相反地，诸如Const, Zeros等操作创造的Tensor，是记录在Graph中，所以没有单独的内存空间；而其他未知的由其他Tensor操作得来的Tensor则是只会在程序运行中间出现。
**4.**Tensor可以使用的地方，几乎都可以使用Variable。
---

###### 注：博众家之所长，集群英之荟萃。


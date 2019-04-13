
# tf.nn.softmax_cross_entropy_with_logits - 郭云飞的专栏 - CSDN博客


2018年07月06日 11:12:39[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：123


```python
#coding=utf-8
"""
tf.nn.softmax_cross_entropy_with_logits(logits, labels, name=None)
第一个参数logits：就是神经网络最后一层的输出，如果有batch的话，
它的大小就是[batchsize，num_classes]
第二个参数labels：实际的标签，大小同上
注意:
这个函数的返回值并不是一个数，而是一个向量，
如果要求交叉熵，我们要再做一步tf.reduce_sum操作,
就是对向量里面所有元素求和，最后才得到.
如果求loss，则要做一步tf.reduce_mean操作，对向量求均值
"""
import tensorflow as tf
 
#our NN's output [batch_size = 3, num_classes = 3]
logits=tf.constant([[1.0,2.0,3.0],[1.0,2.0,3.0],[1.0,2.0,3.0]])
#step1:do softmax
y=tf.nn.softmax(logits)
#true label
y_=tf.constant([[0.0,0.0,1.0],[0.0,0.0,1.0],[0.0,0.0,1.0]])
#step2:do cross_entropy
#下式完全按照交叉熵的公式来计算
cross_entropy = -tf.reduce_sum(y_ * tf.log(y))
#do cross_entropy just one step
cross_entropy2=tf.reduce_sum(tf.nn.softmax_cross_entropy_with_logits(logits = logits, labels = y_))
init = tf.global_variables_initializer()
with tf.Session() as sess:
    sess.run(init)
    softmax=sess.run(y)
    c_e = sess.run(cross_entropy)
    c_e2 = sess.run(cross_entropy2)
    print("step1:softmax result=")
    print(softmax)
    print("step2:cross_entropy result=")
    print(c_e)
    print("Function(softmax_cross_entropy_with_logits) result=")
    print(c_e2)

"""
tf.nn.sparse_softmax_cross'_entropy_with_logits(logits, labels) 
tf.nn.softmax_cross_entropy_with_logits(logits, labels)
两函数之区别：
sparse_softmax_cross_entropy_with_logits中lables接受直接的数字标签 
如[1], [2], [3], [4]（类型只能为int32，int64） 
而softmax_cross_entropy_with_logits中 labels只能接受one-hot标签 
如[1,0,0,0], [0,1,0,0],[0,0,1,0], [0,0,0,1]（类型为int32， int64）
相当于sparse_softmax_cross_entropy_with_logits 对标签多做一个
one-hot动作
"""
```



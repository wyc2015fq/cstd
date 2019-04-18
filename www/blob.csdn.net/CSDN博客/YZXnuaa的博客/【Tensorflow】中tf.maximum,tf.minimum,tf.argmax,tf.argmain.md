# 【Tensorflow】中tf.maximum,tf.minimum,tf.argmax,tf.argmain - YZXnuaa的博客 - CSDN博客
2018年03月28日 20:44:26[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：340
tf.maximum：用法tf.maximum(a,b),返回的是a,b之间的最大值，
tf.miniimum：用法tf.miiinimum(a,b),返回的是a,b之间的最小值，
tf.argmax：用法tf.argmax(a,dimension),返回的是a中的某个维度最大值的索引，
tf.argmain：用法tf.argmin(a,dimension),返回的是a中的某个维度最小值的索引，
例如：
**[python]**[view plain](https://blog.csdn.net/uestc_c2_403/article/details/74370409#)[copy](https://blog.csdn.net/uestc_c2_403/article/details/74370409#)
- import tensorflow as tf;    
- 
- a = [1,5,3]  
- 
- f1 = tf.maximum(a, 3)  
- f2 = tf.minimum(a, 3)  
- f3 = tf.argmax(a, 0)  
- f4 = tf.argmin(a, 0)  
- 
- with tf.Session() as sess:  
- print sess.run(f1)#print f1.eval()
- print sess.run(f2)  
- print sess.run(f3)  
- print sess.run(f4)  
结果：[3 5 3]
[1 3 3]
1
0            

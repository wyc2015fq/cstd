# 【Tensorflow】tf.stack() tf.unstack() - YZXnuaa的博客 - CSDN博客
2018年02月01日 17:06:14[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：533
一。
- import tensorflow as tf  
- 
- a = tf.constant([1,2,3])  
- b = tf.constant([4,5,6])  
- 
- c = tf.stack([a,b],axis=0)  
- d = tf.stack([a,b],axis=1)  
- 
- e = tf.unstack(d,axis=0)  
- f = tf.unstack(d,axis=1)  
- 
- 
- with tf.Session() as sess:  
- print('原始向量数据：')  
- print(sess.run(a))  
- print(sess.run(b))       
- print('矩阵拼接的函数示例,得到一个矩阵：')  #返回值是多维矩阵
- print('以"0维"的方式进行拼接')  
- print(sess.run(c))  
- print('以"1维"的方式进行拼接')  
- print(sess.run(d))  
- 
- print('矩阵分解的函数示例，得到一个list：') #返回值是一个list   
- print('以"0维"的方式进行分解')  
- print(sess.run(e))  
- print('以"1维"的方式进行分解')  
- print(sess.run(f))  
二。
![](https://img-blog.csdn.net/20180201170552924?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvWVpYbnVhYQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

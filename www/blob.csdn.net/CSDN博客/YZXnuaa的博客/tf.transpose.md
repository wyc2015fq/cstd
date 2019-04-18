# tf.transpose - YZXnuaa的博客 - CSDN博客
2018年03月29日 20:32:49[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：60
个人分类：[TFboy养成记](https://blog.csdn.net/YZXnuaa/article/category/7388667)
tf.transpose(input, [dimension_1, dimenaion_2,..,dimension_n]):这个函数主要适用于交换输入张量的不同维度用的，如果输入张量是二维，就相当是转置。dimension_n是整数，如果张量是三维，就是用0,1,2来表示。这个列表里的每个数对应相应的维度。如果是[2,1,0]，就把输入张量的第三维度和第一维度交换。
例如：
**[python]**[view plain](https://blog.csdn.net/uestc_c2_403/article/details/73350498#)[copy](https://blog.csdn.net/uestc_c2_403/article/details/73350498#)
- import tensorflow as tf;  
- import numpy as np;  
- 
- A = np.array([[1,2,3],[4,5,6]])  
- x = tf.transpose(A, [1,0])  
- 
- B = np.array([[[1,2,3],[4,5,6]]])  
- y = tf.transpose(B, [2,1,0])  
- with tf.Session() as sess:  
- print A[1,0]  
- print sess.run(x[0,1])  
- print B[0,1,2]  
- print sess.run(y[2,1,0])  
输出：
4
4
6
6

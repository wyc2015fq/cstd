
# tf.newaxis和np.newaxis - 小花生的博客 - CSDN博客


2018年11月19日 21:32:25[Peanut_范](https://me.csdn.net/u013841196)阅读数：309标签：[tf.newaxis																](https://so.csdn.net/so/search/s.do?q=tf.newaxis&t=blog)[np.newaxis																](https://so.csdn.net/so/search/s.do?q=np.newaxis&t=blog)[newaxis																](https://so.csdn.net/so/search/s.do?q=newaxis&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=np.newaxis&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=tf.newaxis&t=blog)个人分类：[Tensorflow的学习																](https://blog.csdn.net/u013841196/article/category/7063671)
[
																								](https://so.csdn.net/so/search/s.do?q=tf.newaxis&t=blog)


`# -*- coding: utf-8 -*-
"""
tf.newaxis 和 numpy newaxis
"""
import numpy as np
import tensorflow as tf

if __name__ == '__main__':
    feature = np.array([[1,2,3],
                        [2,4,6]])
    center = np.array([[1,1,1],
                       [0,0,0]])
    
    print("原始数组大小：")
    print(feature.shape)
    print(center.shape)
    
    
    np_feature = feature[:, np.newaxis]  
    np_center = center[np.newaxis, :]
    
    print("添加 np.newaxis 后数组大小：")
    print(np_feature.shape)
    print(np_center.shape)
    
    np_diff = np_feature - np_center
    
    print("矩阵相减，np_diff:")
    print(np_diff)
      
    print('\n*********************\n')
    
    tf_feature = tf.constant(feature)[:,tf.newaxis]
    tf_center = tf.constant(center)[tf.newaxis,:]
    
    print("添加 tf.newaxis 后数组大小：")
    print(tf_feature.shape)
    print(tf_center.shape)
   
    tf_diff = tf_feature - tf_center       
    mask = 1 - tf.eye(2, 2, dtype=tf_diff.dtype)
    diffs = tf_diff * mask[:, :, tf.newaxis]
    
    sess = tf.Session()
    print("矩阵相减，tf_diff:")
    print(sess.run(tf_diff))
    
    print("对角线元素置为0：")
    print(sess.run(diffs))`结果：
`原始数组大小：
(2, 3)
(2, 3)
添加 np.newaxis 后数组大小：
(2, 1, 3)
(1, 2, 3)
矩阵相减，np_diff:
[[[0 1 2]
  [1 2 3]]
 [[1 3 5]
  [2 4 6]]]
*********************
添加 tf.newaxis 后数组大小：
(2, 1, 3)
(1, 2, 3)
矩阵相减，tf_diff:
[[[0 1 2]
  [1 2 3]]
 [[1 3 5]
  [2 4 6]]]
对角线元素置为0：
[[[0 0 0]
  [1 2 3]]
 [[1 3 5]
  [0 0 0]]]`



# numpy：矩阵或者数组相减 - 小花生的博客 - CSDN博客


2018年11月19日 21:45:46[Peanut_范](https://me.csdn.net/u013841196)阅读数：1235


`# -*- coding: utf-8 -*-
"""
numpy:矩阵或者数组相减
"""
import numpy as np

if __name__ == '__main__':
    feature = np.array([2,3,5])
    center = np.array([1,2,3])
    
    print("原始数据维度：")
    print(feature.shape)
    print(center.shape)
    
    result = feature - center
    print("维度相同的减法：")
    print(result)
    
    print('\n******************\n')
    
    feature2 = np.array([[2,3,5]])
    center2 = np.array([[1,2,3],[2,3,4]])
    
    print("扩充维度：")
    print(feature2.shape)
    print(center2.shape)
    
    result2 = feature2 - center2
    print("维度不同的减法：")
    print(result2)   
    print(result2.shape)
    
    #产生这种结果的原因：是因为由于维度不同，在计算的时候将feature2变为了与center2同样的维度，等同于如下的计算：   
    feature3 = np.array([[2,3,5],[2,3,5]])
    result3 = feature3 - center2
    print("python的广播机制：")
    print(result3)   
    print(result3.shape)`结果：
`原始数据维度：
(3,)
(3,)
维度相同的减法：
[1 1 2]
******************
扩充维度：
(1, 3)
(2, 3)
维度不同的减法：
[[1 1 2]
 [0 0 1]]
(2, 3)
python的广播机制：
[[1 1 2]
 [0 0 1]]
(2, 3)`


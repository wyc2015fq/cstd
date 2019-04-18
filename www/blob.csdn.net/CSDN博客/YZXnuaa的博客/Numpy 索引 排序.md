# Numpy 索引 排序 - YZXnuaa的博客 - CSDN博客
2019年04月17日 02:57:31[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：3
### numpy.argmax() 和 numpy.argmin()
numpy.argmax() 和 numpy.argmin()函数分别沿给定轴返回最大和最小元素的索引。
## 实例
### numpy.sort()
numpy.sort() 函数返回输入数组的排序副本。函数格式如下：
numpy.sort(a, axis, kind, order)
参数说明：
- a: 要排序的数组
- axis: 沿着它排序数组的轴，如果没有数组会被展开，沿着最后的轴排序， axis=0 按列排序，axis=1 按行排序
- kind: 默认为'quicksort'（快速排序）
- order: 如果数组包含字段，则是要排序的字段
## 实例
import numpy as np a = np.array([[3,7],[9,1]]) print ('我们的数组是：') print (a) print ('\n') print ('调用 sort() 函数：') print (np.sort(a)) print ('\n') print ('按列排序：') print (np.sort(a, axis = 0)) print ('\n') # 在 sort 函数中排序字段 dt = np.dtype([('name', 'S10'),('age', int)]) a = np.array([("raju",21),("anil",25),("ravi", 17), ("amar",27)], dtype = dt) print ('我们的数组是：') print (a) print ('\n') print ('按 name 排序：') print (np.sort(a, order = 'name'))
输出结果为：
我们的数组是：
[[3 7]
 [9 1]]
调用 sort() 函数：
[[3 7]
 [1 9]]
按列排序：
[[3 1]
 [9 7]]
我们的数组是：
[(b'raju', 21) (b'anil', 25) (b'ravi', 17) (b'amar', 27)]
按 name 排序：
[(b'amar', 27) (b'anil', 25) (b'raju', 21) (b'ravi', 17)]
### numpy.argsort()
numpy.argsort() 函数返回的是数组值从小到大的索引值。
## 实例
import numpy as np x = np.array([3, 1, 2]) print ('我们的数组是：') print (x) print ('\n') print ('对 x 调用 argsort() 函数：') y = np.argsort(x) print (y) print ('\n') print ('以排序后的顺序重构原数组：') print (x[y]) print ('\n') print ('使用循环重构原数组：') for i in y: print (x[i], end=" ")
输出结果为：
我们的数组是：
[3 1 2]
对 x 调用 argsort() 函数：
[1 2 0]
以排序后的顺序重构原数组：
[1 2 3]
使用循环重构原数组
1 2 3

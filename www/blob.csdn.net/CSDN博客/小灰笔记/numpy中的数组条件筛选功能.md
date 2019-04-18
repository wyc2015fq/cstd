# numpy中的数组条件筛选功能 - 小灰笔记 - CSDN博客





2017年04月09日 20:09:02[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：11135
个人分类：[Python](https://blog.csdn.net/grey_csdn/article/category/6639459)








在程序设计中，时常会遇到数据的唯一化、相同、相异信息的提取等工作，在格式化的向量存储矩阵中南，numpy能够提供比较不错的快速处理功能。

1，唯一化的实现：

In [63]: data = np.array(['int','float','int','boolean','double','boolean'])



In [64]: data

Out[64]:

array(['int', 'float', 'int', 'boolean', 'double', 'boolean'],

      dtype='|S7')



In [65]: np.unique(data)

Out[65]:

array(['boolean', 'double', 'float', 'int'],

      dtype='|S7')



In [66]: data = np.array([1,5,3,6,2,4,1,3,5,7,9])



In [67]: data

Out[67]: array([1, 5, 3, 6, 2, 4, 1, 3, 5, 7, 9])



In [68]: np.unique(data)

Out[68]: array([1, 2, 3, 4, 5, 6, 7, 9])
通过unique可以实现数组的唯一化，并且，唯一化后的返回值会进行排序。

2，交集的实现

In [69]: data1 = np.arange(10)



In [70]: data1

Out[70]: array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9])



In [71]: data2 = np.array([2,8,6,4])



In [72]: np.intersect1d(data1,data2)

Out[72]: array([2, 4, 6, 8])
使用intersect1d可以实现求取两个数组集合的交集。

2，并集计算

In [73]: np.union1d(data1,data2)

Out[73]: array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9])

union1d可以实现对两个数组集合的并集计算。

3，子集判断

In [74]: np.in1d(data1,data2)

Out[74]: array([False, False,  True, False,  True, False,  True, False,  True, False], dtype=bool)



In [75]: np.in1d(data2,data1)

Out[75]: array([ True,  True,  True,  True], dtype=bool)
通过in1d可以实现对第一个参数数组中的每个元素是否是第二个参数数组子集的判断，而最终通过判断返回的布尔数组即可判断两个参数数组的子集关系。

4，差异判断

4.1，集合差判断

In [76]: np.setdiff1d(data1,data2)

Out[76]: array([0, 1, 3, 5, 7, 9])



In [77]: np.setdiff1d(data2,data1)

Out[77]: array([], dtype=int32)

setdiff1d可以求解出存在于第一个集合但是并不存在于第二个集合中的元素。返回值是一个数组集合。

4.1 数组“异或”求解

In [78]: np.setxor1d(data1,data2)

Out[78]: array([0, 1, 3, 5, 7, 9])



In [79]: np.setxor1d(data2,data1)

Out[79]: array([0, 1, 3, 5, 7, 9])
setxor1d用于求解不同时存在于两个数组中的元素，并返回一个数组集合。两个参数的顺序变化不会改变求解的结果。返回的结果是是一个有序的数组序列。


上面的几个基本的逻辑判断功能如果能够使用得当，能够实现多种筛选判断的操作。
            



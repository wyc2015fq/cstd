# Python numpy包 np.random.shuffle(x) 数据集 batch 预处理 - YZXnuaa的博客 - CSDN博客
2018年01月16日 11:24:33[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：564

- numpy.random.shuffle(x)
 的重要因素就是仅仅打乱第一层的数据，一个元组中首先默认行为第一纬度，列是第二个纬度，所以打乱机器学习和深度学习常用标签时，一般使用 M*(N+1) 结构（M代表数据总量，N代表每个数据的N个特征，1代表数据标注）将M个数据的排列顺序打乱，方便形成一个batch进行学习。
import numpy as np
arr = np.arange(10)
print(arr)
np.random.shuffle(arr)
print(arr)
arr = np.arange(18).reshape((9, 2))
print(arr)
np.random.shuffle(arr)
print(arr)
结果：
[0 1 2 3 4 5 6 7 8 9]
[1 5 3 4 8 2 6 7 0 9]
[[ 0  1]
 [ 2  3]
 [ 4  5]
 [ 6  7]
 [ 8  9]
 [10 11]
 [12 13]
 [14 15]
 [16 17]]
[[ 0  1]
 [ 8  9]
 [12 13]
 [ 2  3]
 [ 4  5]
 [ 6  7]
 [16 17]
 [14 15]
 [10 11]]


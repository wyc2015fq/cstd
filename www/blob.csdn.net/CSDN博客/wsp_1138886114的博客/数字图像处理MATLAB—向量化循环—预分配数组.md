# 数字图像处理MATLAB—向量化循环—预分配数组 - wsp_1138886114的博客 - CSDN博客





2018年07月31日 13:30:54[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：268








### 优化MATLAB代码方法

数字图像处理中： 

MATLAB是一种专门为数组运算而设计的编程语言。 

这里讨论两种优化MATLAB代码方法。
##### 1. 向量化循环：

MATLAB会自动处理索引h。当坐标中涉及0时，会有混乱之源，因为本书和手册中反复强调MATLAB数组不能有0索引。

```
import time
import numpy as np

a = np.random.rand(1000000)
b = np.random.rand(1000000)

tic = time.time()      #开始计时
c = np.dot(a,b)        #对a,b 做点积
toc = time.time()      #计时结束
print(c)
print("Vectorized version（向量化循环）: " , str(1000*(toc-tic)) + "ms")

c = 0
tic1 = time.time()
for i in range(1000000):
    c += a[i]*b[i]
toc1 = time.time()
print(c)
print("For loop version（一般for循环）: " , str(1000*(toc1-tic1)) + "ms")
```

```
当时处理小数据量时，速度快倍
处理百万数据，耗时相差400多倍。
```

##### 2.预分配数组

加快代码执行时间的另一种方法是在程序中预分配数组的大小。在处理数值或逻辑数组时，预分配只是简单地创建有着适当维数的数组，数组的元素均为0。例如，若我们正在处理两幅大小均为〖024x1024像素的图像f和g，则预分配由如下语句构成： 

大处理大数组时，预分配也可帮助我们减少存储器碎片。动态存储器的分配和去分配会使得存储器出现碎片。实际的结果是在计算过程中可能会有足够空间的可用物理存储器，但可能没有足够的连续空间来容纳一个较大的变董。预分配通过在计算开始时就允许MATLAB为大数据构造保留足够的存储空间，来阻止无连续空间的情形出现。





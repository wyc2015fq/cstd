# numpy中数组元素的切片复制 - 小灰笔记 - CSDN博客





2017年04月03日 22:33:34[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1528








                
代码1：

#!/usr/bin/python



import numpy as np



arr1 = np.arange(10)

print(arr1)



slice_data = arr1[3:5]

print(slice_data)



slice_data[0] = 123



print(slice_data)

print(arr1)
类似的代码之前应用过，简单看了一下numpy中的数组切片。程序的执行结果如下：

In [2]: %run exp01.py

[0 1 2 3 4 5 6 7 8 9]

[3 4]

[123   4]

[  0   1   2 123   4   5   6   7   8   9]
由此得出来的结论是：切片后的变量与原始的数据共享同一数据存储。而这种特点是专门针对大数据的处理而定制的。然而，在日常的使用中总有需要对数据进行拷贝的时候，此时需要的便是显式的数据复制操作。程序修改如下：

#!/usr/bin/python



import numpy as np



arr1 = np.arange(10)

print(arr1)



slice_data = arr1[3:5].copy()

print(slice_data)



slice_data[0] = 123



print(slice_data)

print(arr1)
程序的执行结果如下：

In [3]: %run exp01.py

[0 1 2 3 4 5 6 7 8 9]

[3 4]

[123   4]

[0 1 2 3 4 5 6 7 8 9]
由上面的结果可以看出，通过切片后复制操作生成的新的数据对象的操作与原始的数据是独立的。新的数据对象的变化并不会影响到原始数据的信息。
            



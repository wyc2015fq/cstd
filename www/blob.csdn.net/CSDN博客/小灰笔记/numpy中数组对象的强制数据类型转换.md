# numpy中数组对象的强制数据类型转换 - 小灰笔记 - CSDN博客





2017年03月29日 23:41:29[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：38994
个人分类：[Python](https://blog.csdn.net/grey_csdn/article/category/6639459)









       在我们的普通编程过程中，强制数据类型转换时很寻常的，尤其是在类似于C语言这样的编程语言。由于自己是一个C程序员，很多时候在这方面会多想一点。其实，纯粹的Python中，数据类型转换至少在语法上跟C是差不多的。但是，在numpy中，处理数组元素的时候这一切都不一样了。在numpy中，面向对象编程的味道更浓一些，而相应的转换是通过对象的方法调用来实现。

       例如，有如下代码：

import numpy as np



data1 =[1,5,3,6,2,4]

data2 =[1,5.6,3,6.4,2,4.1]

array1 =np.array(data1)

array2 =np.array(data2)

print('array1:')

print(array1)

print('array2:')

print(array2)

       运行结果如下：

In [1]: %runexp_1.py

array1:

[1 5 3 6 2 4]

array2:

[ 1.   5.6 3.   6.4  2.  4.1]

In [2]: array2

Out[2]: array([ 1.,  5.6, 3. ,  6.4,  2. , 4.1])



In [3]: array1

Out[3]: array([1,5, 3, 6, 2, 4])

       如果把array2进行强制数据类型转换转换成整型，那么得到的结果应该跟array1一致。通过以下命令查看array1的类型：

In [4]:array1.dtype

Out[4]:dtype('int32')

       由上面的结果确认array1是int32类型，对array2进行强制数据类型转换如下：

In [5]:array2.dtype

Out[5]:dtype('float64')



In [6]:array2.astype(np.int32)

Out[6]: array([1,5, 3, 6, 2, 4])



In [7]:array2.dtype

Out[7]:dtype('float64')

       从上面的结果可以看出，输出类型进行了强制数据类型转换，但是原始数据并没有变化。如果想要彻底改变array2的类型，还需要一步赋值：

In [8]: array2 =array2.astype(np.int32)



In [9]: array2

Out[9]: array([1,5, 3, 6, 2, 4])



In [10]:array2.dtype

Out[10]:dtype('int32')

       上面这种方式在一定程度上来说比较繁琐，因为在对array2进行数据类型转换之前先手动对array1的属性进行了查看。如果做成让计算机自己查看，则需要再做一点变动，重新运行原始代码如下：

In [11]: %runexp_1.py

array1:

[1 5 3 6 2 4]

array2:

[ 1.   5.6 3.   6.4  2.  4.1]



In [12]:array1.dtype

Out[12]:dtype('int32')



In [13]:array2.dtype

Out[13]:dtype('float64')

       从上面的结果中可以看出，之前测试的数据恢复了初始的状态。此时如果想对array2进行进行强制数据类型转换可以采用如下方式：

In [14]: array2 =array2.astype(array1.dtype)



In [15]: array2

Out[15]: array([1,5, 3, 6, 2, 4])



In [16]:array2.dtype

Out[16]:dtype('int32')

       通过上面的结果可以看出，强制数据类型转换使得两个对象的类型实现了统一。




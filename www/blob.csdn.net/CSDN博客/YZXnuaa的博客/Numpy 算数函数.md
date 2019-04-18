# Numpy 算数函数 - YZXnuaa的博客 - CSDN博客
2019年04月17日 03:03:21[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：3
### numpy.mod()
numpy.mod() 计算输入数组中相应元素的相除后的余数。 函数 numpy.remainder() 也产生相同的结果。
## 实例
import numpy as np a = np.array([10,20,30]) b = np.array([3,5,7]) print ('第一个数组：') print (a) print ('\n') print ('第二个数组：') print (b) print ('\n') print ('调用 mod() 函数：') print (np.mod(a,b)) print ('\n') print ('调用 remainder() 函数：') print (np.remainder(a,b))
输出结果为：
第一个数组：
[10 20 30]
第二个数组：
[3 5 7]
调用 mod() 函数：
[1 0 2]

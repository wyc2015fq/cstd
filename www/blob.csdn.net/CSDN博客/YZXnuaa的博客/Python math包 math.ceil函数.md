# Python math包 math.ceil函数 - YZXnuaa的博客 - CSDN博客
2018年01月16日 11:38:57[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：2418
import.math  !!!!!         不能直接访问，需要导入`math`模块，然后使用`math`静态对象调用此函数。
Python数字`ceil()`方法返回`x`的最大值，即大于等于`x`的最小整数。
**参数**
- *x* - 这是一个数字表达式。
**返回值**
- 此方法返回不小于`x`的最小整数。
示例：
#!/usr/bin/python3
import math   # This will import math module
print ("math.ceil(-45.17) : ", math.ceil(-45.17))
print ("math.ceil(100.12) : ", math.ceil(100.12))
print ("math.ceil(100.72) : ", math.ceil(100.72))
print ("math.ceil(math.pi) : ", math.ceil(math.pi))
math.ceil(-45.17) :  -45
math.ceil(100.12) :  101
math.ceil(100.72) :  101
math.ceil(math.pi) :  4

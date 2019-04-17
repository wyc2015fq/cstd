# Python内置函数(1)——abs - lincappu - 博客园







# [Python内置函数(1)——abs](https://www.cnblogs.com/lincappu/p/8143888.html)





**英文文档：**

abs(*x*)

    Return the absolute value of a number. The argument may be an integer or a floating point number. If the argument is a complex number, its magnitude is returned

**说明：**

    1. 返回数字的绝对值，参数可以是整数、浮点数或者复数

    2. 如果参数是一个复数，此方法返回此复数的绝对值（此复数与它的共轭复数的乘积的平方根）

示例：

>>> abs(34)
34
>>> abs(-2)
2
>>> abs(0)
0
>>> abs(454L)  #python3已经没有了 long 类型，定义的时候自动识别
  File "<stdin>", line 1
    abs(454L)
           ^
SyntaxError: invalid syntax
>>> c1=comples(1,1)
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
NameError: name 'comples' is not defined

>>> c1=complex(1,1)
>>> c1
(1+1j)
>>> abs(c1)
1.4142135623730951
>>>
>>> c2 = complex(-1,-1)
>>> c2   #复数
(-1-1j)
>>> abs(c2)  #复数绝对值
1.4142135623730951
>>>


















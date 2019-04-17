# Python内置函数(2)——divmod - lincappu - 博客园







# [Python内置函数(2)——divmod](https://www.cnblogs.com/lincappu/p/8143922.html)





**英文文档：**

`divmod`(*a*, *b*)

Take two (non complex) numbers as arguments and return a pair of numbers consisting of their quotient and remainder when using integer division. With mixed operand types, the rules for binary arithmetic operators apply. For integers, the result is the same as `(a // b, a % b)`. For floating point numbers the result is `(q, a % b)`, where *q* is usually `math.floor(a / b)` but may be 1 less than that. In any case `q * b + a % b` is very close to *a*, if `a % b` is non-zero it has the same sign as *b*, and `0 <= abs(a % b) < abs(b)`.



**说明：**

　　1. 接受两个数值(非复数)，返回两个数值的相除得到的商，和余数组成的元组。

　　2. 如果参数都是整数，执行的是地板除，相当于 `(a//b,a%b)。`

　　3. 如果参数时浮点数，相当于( `math.floor(a/b),a%b`)。

整数：

>>> divmod(5,3)
(1, 2)
>>>
>>> 5//3
1
>>> 5%3
2


浮点数：
>>> divmod(5.5,3)
(1.0, 2.5)
>>> 5.5/3
1.8333333333333333
>>>
>>> import math
>>> math.floor(5.5/3.0)
1
>>>
>>> 5.5%3
2.5
















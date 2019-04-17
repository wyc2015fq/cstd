# Python内置函数(5)——pow - lincappu - 博客园







# [Python内置函数(5)——pow](https://www.cnblogs.com/lincappu/p/8144320.html)





**英文文档：**
- `pow`(*x*, *y*[, *z*])
- 
- Return *x* to the power *y*; if *z* is present, return *x* to the power *y*, modulo *z* (computed more efficiently than `pow(x, y)% z`). The two-argument form `pow(x, y)` is equivalent to using the power operator: `x**y`.
- 
- The arguments must have numeric types. With mixed operand types, the coercion rules for binary arithmetic operators apply. For `int` operands, the result has the same type as the operands (after coercion) unless the second argument is negative; in that case, all arguments are converted to float and a float result is delivered. For example, `10**2` returns `100`, but `10**-2` returns `0.01`. If the second argument is negative, the third argument must be omitted. If *z* is present, *x* and *y* must be of integer types, and *y* must be non-negative.
- 
- 


- **说明：**
- 
- 　　1. 函数有两个必需参数x,y和一个可选参数z，结果返回x的y次幂乘(相当于x**y)，如果可选参数z有传入值，则返回幂乘之后再对z取模(相当于pow(x,y)%z)。
- 

>>> pow(2,5)
32
>>> pow(2,3)
8
>>> pow(2,3,5)
3
>>>


　　2.所有的参数必须是数值类型，不能是其他类型。

>>> pow(2,'2')
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: unsupported operand type(s) for ** or pow(): 'int' and 'str'
>>>


　　3.若果x，y 有一个浮点数，则结果将转换为浮点数。

>>> pow(2,3.2)
9.18958683997628
>>> pow(2,3.0)
8.0

　　4.如果x，y 都是整数，则结果也是整数，如果 y 是负数，浮点数不能取摸，所以传入的参数 z 不能取摸。**如果可选参数 z 传入了值，则x，y 必须为整数，且 y 不能 负数，**

>>> pow(2,-1)
0.5
>>> pow(2,-2)
0.25
>>> pow(2,-10)
0.0009765625
>>>
>>> pow(2,-10,2)
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
ValueError: pow() 2nd argument cannot be negative when 3rd argument specified













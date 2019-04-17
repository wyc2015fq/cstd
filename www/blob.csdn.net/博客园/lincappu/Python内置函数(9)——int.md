# Python内置函数(9)——int - lincappu - 博客园







# [Python内置函数(9)——int](https://www.cnblogs.com/lincappu/p/8144664.html)





**英文文档：**

*class *`int`(*x=0*) *class *`int`(*x*, *base=10*)

Return an integer object constructed from a number or string *x*, or return `0` if no arguments are given. If *x* is a number, return `x.__int__()`. For floating point numbers, this truncates towards zero.

If *x* is not a number or if *base* is given, then *x* must be a string, `bytes`, or `bytearray` instance representing an integer literal in radix *base*. Optionally, the literal can be preceded by `+` or `-` (with no space in between) and surrounded by whitespace. A base-n literal consists of the digits 0 to n-1, with `a` to `z` (or `A` to `Z`) having values 10 to 35. The default *base* is 10. The allowed values are 0 and 2-36. Base-2, -8, and -16 literals can be optionally prefixed with `0b`/`0B`, `0o`/`0O`, or `0x`/`0X`, as with integer literals in code. Base 0 means to interpret exactly as a code literal, so that the actual base is 2, 8, 10, or 16, and so that `int('010', 0)` is not legal, while `int('010')` is, as well as `int('010', 8)`.



**说明：**

　　1. 不传入参数时，得到结果0。

```
>>> int()
0
```

　　2. 传入数值时，调用其__int__()方法，浮点数将向下取整。

```
>>> int(3)
3
>>> int(3.6)
3
```

　　3. 传入字符串时，默认以10进制进行转换。

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

>>> int('36')
36

>>> int('3.6')
Traceback (most recent call last):
  File "<pyshell#2>", line 1, in <module>
    int('3.6')
ValueError: invalid literal for int() with base 10: '3.6'

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

　　4. 字符串中允许包含"+"、"-"号，但是加减号与数值间不能有空格，数值后、符号前可出现空格。

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

>>> int('+36')
36
>>> int('-36')
-36
>>> int('   -36        ')
-36
>>> int(' - 36        ')
Traceback (most recent call last):
  File "<pyshell#7>", line 1, in <module>
    int(' - 36        ')
ValueError: invalid literal for int() with base 10: ' - 36   

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

　　5. 传入字符串，并指定了进制，则按对应进制将字符串转换成10进制整数。

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

>>> int('01',2)
1
>>> int('02',3)
2
>>> int('07',8)
7
>>> int('0f',16)
15

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```













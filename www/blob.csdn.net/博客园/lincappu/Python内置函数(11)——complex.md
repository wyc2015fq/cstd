# Python内置函数(11)——complex - lincappu - 博客园







# [Python内置函数(11)——complex](https://www.cnblogs.com/lincappu/p/8144689.html)





**英文文档：**

*class *`complex`([*real*[, *imag*]])

Return a complex number with the value *real* + *imag**1j or convert a string or number to a complex number. If the first parameter is a string, it will be interpreted as a complex number and the function must be called without a second parameter. The second parameter can never be a string. Each argument may be any numeric type (including complex). If *imag* is omitted, it defaults to zero and the constructor serves as a numeric conversion like `int` and `float`. If both arguments are omitted, returns `0j`.


Note

When converting from a string, the string must not contain whitespace around the central `+` or `-` operator. For example, `complex('1+2j')` is fine, but `complex('1 + 2j')` raises `ValueError`.

 　　根据传入的参数创建一个复数[](http://www.cnblogs.com/sesshoumaru/p/5989192.html)

**说明：**

　　1. 函数功能，返回一个复数。有两个可选参数。

　　2. 当两个参数都不提供时，返回复数 0j。

```
>>> complex()
0j
```



　　3. 当第一个参数为字符串时，调用时不能提供第二个参数。此时字符串参数，需是一个能表示复数的字符串，而且加号或者减号左右不能出现空格。

```
>>> complex('1+2j',2) #第一个参数为字符串，不能接受第二个参数
Traceback (most recent call last):
  File "<pyshell#2>", line 1, in <module>
    complex('1+2j',2)
TypeError: complex() can't take second arg if first is a string

>>> complex('1 + 2j') #不能有空格
Traceback (most recent call last):
  File "<pyshell#3>", line 1, in <module>
    complex('1 + 2j')
ValueError: complex() arg is a malformed string
```



　　4. 当第一个参数为int或者float时，第二个参数可为空，表示虚部为0；如果提供第二个参数，第二个参数也需为int或者float。

```
>>> complex(2)
(2+0j)
>>> complex(2.1,-3.4)
(2.1-3.4j)
```














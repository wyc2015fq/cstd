# Python内置函数(10)——float - lincappu - 博客园







# [Python内置函数(10)——float](https://www.cnblogs.com/lincappu/p/8144674.html)





**英文文档：**

*class *`float`([*x*])

Return a floating point number constructed from a number or string *x*.

If the argument is a string, it should contain a decimal number, optionally preceded by a sign, and optionally embedded in whitespace. The optional sign may be `'+'` or `'-'`; a `'+'` sign has no effect on the value produced. The argument may also be a string representing a NaN (not-a-number), or a positive or negative infinity. More precisely, the input must conform to the following grammar after leading and trailing whitespace characters are removed:
**sign          ** ::=  "+" | "-"
**infinity      ** ::=  "Infinity" | "inf"
**nan           ** ::=  "nan"
**numeric_value ** ::=  `floatnumber` | `infinity` | `nan`**numeric_string** ::=  [`sign`] `numeric_value`
Here `floatnumber` is the form of a Python floating-point literal, described in Floating point literals. Case is not significant, so, for example, “inf”, “Inf”, “INFINITY” and “iNfINity” are all acceptable spellings for positive infinity.

Otherwise, if the argument is an integer or a floating point number, a floating point number with the same value (within Python’s floating point precision) is returned. If the argument is outside the range of a Python float, an `OverflowError`will be raised.

For a general Python object `x`, `float(x)` delegates to `x.__float__()`.

If no argument is given, `0.0` is returned.



**说明：**

　　1. 函数功能将一个数值或者字符转换成浮点型数值。

```
>>> float(3)
3.0
>>> float('3')
3.0
```

　　2. 不提供参数的时候，返回0.0。

```
>>> float()
0.0
```

　　3. 字符串必须能正确转换成浮点型数值的，否则报错。

```
>>> float('3.14.15926')
Traceback (most recent call last):
  File "<pyshell#3>", line 1, in <module>
    float('3.14.15926')
ValueError: could not convert string to float: '3.14.15926'
```

　　4. 字符串中允许出现“+”、“-”两个符号，两个符号和数字之间不能出现空格，但是符号前面和数字后面允许出现空格。

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

>>> float('+3.14') #带正号
3.14
>>> float('-3.14') #带负号
-3.14
>>> float('  -3.14  ') #正负号前、数字后可以有空格
-3.14
>>> float('- 3.14') #正负号与数字间不可以有空格
Traceback (most recent call last):
  File "<pyshell#8>", line 1, in <module>
    float('- 3.14')
ValueError: could not convert string to float: '- 3.14'

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

　　5. 有几个特殊的字符串能正确转换，"Infinity"或者“inf”(不区分大小写)，能正确转换，表示无穷大，可以和“+”、“-”一起使用；“nan”也能正确转换，表示没有值。

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

>>> float('Infinity')
inf
>>> float('inf')
inf

>>> float('inFinIty') #不区分大小写
inf

>>> float('+inFinIty') #正无穷
inf
>>> float('-inFinIty') #负无穷
-inf

>>> float('nan') #没有值
nan

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

　　6. 定义的对象如果要被float函数正确转换成浮点数，需要定义__float__函数。

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

>>> class X:
    def __init__(self,score):
        self.score = score

>>> x = X(9.7)
>>> float(x) #不能转换
Traceback (most recent call last):
  File "<pyshell#20>", line 1, in <module>
    float(x)
TypeError: float() argument must be a string or a number, not 'X'


>>> class X: #重新定义类，加入__float__方法
    def __init__(self,score):
        self.score = score
    def __float__(self):
        return self.score

>>> x = X(9.7)
>>> float(x) #可以转换
9.7

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```













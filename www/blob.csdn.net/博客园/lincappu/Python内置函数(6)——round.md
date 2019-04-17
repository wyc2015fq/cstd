# Python内置函数(6)——round - lincappu - 博客园







# [Python内置函数(6)——round](https://www.cnblogs.com/lincappu/p/8144467.html)





**英文文档：**
- `round`(*number*[, *ndigits*])
- 
- 
- Return the floating point value *number* rounded to *ndigits* digits after the decimal point. If *ndigits* is omitted, it returns the nearest integer to its input. Delegates to `number.__round__(ndigits)`.
- 
- 
- For the built-in types supporting `round()`, values are rounded to the closest multiple of 10 to the power minus *ndigits*; if two multiples are equally close, rounding is done toward the even choice (so, for example, both `round(0.5)` and `round(-0.5)` are `0`, and `round(1.5)` is `2`). The return value is an integer if called with one argument, otherwise of the same type as *number*.
- 
- 
- **Note**：

The behavior of `round()` for floats can be surprising: for example, `round(2.675, 2)` gives `2.67` instead of the expected `2.68`. This is not a bug: it’s a result of the fact that most decimal fractions can’t be represented exactly as a float. See Floating Point Arithmetic: Issues and Limitations for more information.

**说明：**
**1.round 函数用于对浮点数进行四舍五入的求值，具体保留几位小数，以出阿如的 ndigits 参数来控制**
>>> round(1.423432432)
1
>>>
>>> round(1.423432432,2)
1.42
>>> round(1.423432432,5)
1.42343
>>>
>>>
>>> round(1.423432432)
1
>>>


　　2. ndigits参数为可选参数，当不传入时，即以默认保留0位小数进行取整，返回的是整数。

```
>>> round(1.1314926)
1**　　**
```
3.当传入的值是0时与不传值一样保留0位小数，但是是浮点类型的数
>>> round(1.423432432,0)
1.0
>>>


　　4.也可以传入负数，则标示是对整数部分进行四舍五入，小数部分全部清0，若果传入的 ndigits 值大于浮点数的整数部分，则返回0.0

>>> round(1314.423432432,-2)
1300.0
>>> round(1314.423432432,-4)
0.0
>>>


5.round 四舍五入是靠近0原则，所以-0.5和0.5进行0位的四舍五入结果都是0，

>>> round(0.5)
0
>>> round(-0.5)
0
>>>


　　6.round 有时会有 bug，因为浮点数在存储的时候因为数位有限，存储的不精却，存储的值和实际显示的值有误差，导致四舍五入的时候也不精确。

>>> round(2.2458,3)
2.246
>>> round(2.245,3)
2.245
>>> round(2.245,2)
2.25
>>> round(2.675,2)
2.67
>>> round(2.674545,4)
2.6745
>>>
>>>


　　7.round 对整数也能进行操作，但会也是整形。

>>>
>>> round(123)
123
>>> round(123,2)
123
>>> round(123,-2)
100
>>> round(123,-3)
0
>>>
















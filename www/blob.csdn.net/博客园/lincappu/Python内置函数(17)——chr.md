# Python内置函数(17)——chr - lincappu - 博客园







# [Python内置函数(17)——chr](https://www.cnblogs.com/lincappu/p/8144762.html)





**英文文档：**
- `chr`(*i*)
- 　　Return the string representing a character whose Unicode code point is the integer *i*. For example, `chr(97)` returns the string `'a'`, while `chr(8364)` returns the string `'€'`. This is the inverse of `ord()`.
- 　　The valid range for the argument is from 0 through 1,114,111 (0x10FFFF in base 16). `ValueError` will be raised if *i*is outside that range
- 
- 
- **　　返回整数所对应的 unicode 字符**
- 
- **说明：**
- 　　1. 函数返回整形参数值所对应的Unicode字符的字符串表示

```
>>> chr(97) #参数类型为整数
'a'

>>> chr('97') #参数传入字符串时报错
Traceback (most recent call last):
  File "<pyshell#6>", line 1, in <module>
    chr('97')
TypeError: an integer is required (got type str)

>>> type(chr(97)) #返回类型为字符串
<class 'str'>
```

　　2. 它的功能与ord函数刚好相反

```
>>> chr(97)
'a'
>>> ord('a')
97
```

　　3. 传入的参数值范围必须在0-1114111(十六进制为0x10FFFF)之间，否则将报`ValueError错误`

```
>>> chr(-1) #小于0报错
Traceback (most recent call last):
  File "<pyshell#10>", line 1, in <module>
    chr(-1)
ValueError: chr() arg not in range(0x110000)

>>> chr(1114111)
'\U0010ffff'

>>> chr(1114112) #超过1114111报错
Traceback (most recent call last):
  File "<pyshell#13>", line 1, in <module>
    chr(1114112)
ValueError: chr() arg not in range(0x110000)
```













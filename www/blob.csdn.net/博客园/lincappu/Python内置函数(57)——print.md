# Python内置函数(57)——print - lincappu - 博客园







# [Python内置函数(57)——print](https://www.cnblogs.com/lincappu/p/8145209.html)





**英文文档：**

`print`(**objects*, *sep=' '*, *end='\n'*, *file=sys.stdout*, *flush=False*)

Print *objects* to the text stream *file*, separated by *sep* and followed by *end*. *sep*, *end* and *file*, if present, must be given as keyword arguments.

All non-keyword arguments are converted to strings like `str()` does and written to the stream, separated by *sep* and followed by *end*. Both *sep* and *end* must be strings; they can also be `None`, which means to use the default values. If no *objects* are given, `print()` will just write *end*.

The *file* argument must be an object with a `write(string)` method; if it is not present or `None`, `sys.stdout` will be used. Since printed arguments are converted to text strings, `print()` cannot be used with binary mode file objects. For these, use `file.write(...)` instead.

Whether output is buffered is usually determined by *file*, but if the *flush* keyword argument is true, the stream is forcibly flushed.

 　　想标准输出对象打印输出

**说明：**

　　1. 用于对象打印输出。通过命名参数sep来确定多个输出对象的分隔符(默认' ')，通过命名参数end确定输出结果的结尾(默认'\n')，通过命名参数file确定往哪里输出(默认sys.stdout)，通过命名参数fiush确定输出是否使用缓存(默认False)。

```
>>> print(1,2,3)
1 2 3
>>> print(1,2,3,sep = '+')
1+2+3
>>> print(1,2,3,sep = '+',end = '=?')
1+2+3=?
```

　　2. sep、end、file、flush都必须以命名参数方式传参，否则将被当做需要输出的对象了。

```
>>> print(1,2,3,'+','=?')
1 2 3 + =?
```

　　3. sep和end参数必须是字符串；或者为None，为None时意味着将使用其默认值。

```
>>> print(1,2,3,sep = 97,end = 100)
Traceback (most recent call last):
  File "<pyshell#34>", line 1, in <module>
    print(1,2,3,sep = 97,end = 100)
TypeError: sep must be None or a string, not int
>>> print(1,2,3,sep = None,end = None)
1 2 3
```

　　4. 不给print传递任何参数，将只输出end参数的默认值。

```
>>> print()

>>> print(end = 'by 2016')
by 2016
```

　　5. file参数必须是一个含有`write(string)` 方法的对象。

```
>>> class A:
    @staticmethod    
    def write(s):
        print(s)

        
>>> a = A()
>>> print(1,2,3,sep = '+',end = '=?',file = a)
1
+
2
+
3
=?
```

　　6.format 参数使用

```
# 常见形式
print('{0},{1}'.format('zhangk', 32))
print('{},{},{}'.format('zhangk', 'boy', 32))
print('{name},{sex},{age}'.format(age=32, sex='male', name='zhangk'))
# 格式限定符：^ < >  居中  左对齐  右对齐 后面带宽度
l1=['a','bb','ccccc','dddd']
l2=[1,2,333333333333333333,4]
print("{:1<8} {:a>8} {:p^8} {:<8}".format(*l1))
print("{:<8} {:<8} {:<8} {:<8}".format(*l2))

# 精度通常跟类型f 一起使用
print('{:.2f}'.format(21.42423423))

# 其他类型
# 主要就是进制了，b、d、o、x分别是二进制、十进制、八进制、十六进制
print('{:b}'.format(15))
print('{:d}'.format(15))
print('{:o}'.format(15))
print('{:x}'.format(15))

# 用逗号还能用来做金额的千位分隔符
print('{:,}'.format(123456789)
```















# python cookbook 2.13 字符串对齐 - 别说话写代码的博客 - CSDN博客





2019年04月14日 11:41:40[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：11
所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)









```
#你想通过某种字符串对齐操作格式化字符串。基本的可使用ljust(),rjust()，center()
text='hello world'
print(text.ljust(20))
print(text.rjust(20))    #20个字符中右对齐
print(text.center(20))    #居中
```

![](https://img-blog.csdnimg.cn/20190414113856512.png)

```
#这些方法可接受一个可选填充字符。
print(text.rjust(20,'='))
print(text.center(20,'*'))
```

![](https://img-blog.csdnimg.cn/20190414113925304.png)

```
#函数format()同样可以用来实现字符串对齐。使用<,>或者^字符后面紧跟一个指定宽度
print(format(text,'>20'))
print(format(text,'<20'))
print(format(text,'^20'))
```

![](https://img-blog.csdnimg.cn/20190414113946235.png)

```
#format()也可以指定一个费空格填充字符
print(format(text,'=>20s'))
print(format(text,'*^20s'))
```

![](https://img-blog.csdnimg.cn/20190414114012360.png)

```
#当格式化多个值时，这些格式代码也可以被用在format()方法中
print('{:*>10s} {:->10s}'.format('hello','world'))
```

![](https://img-blog.csdnimg.cn/20190414114035616.png)

```
#format()函数不仅适用于字符串。它可以用来格式化任何值，使得它非常的通用。可以用它来格式化数字：
x=1.2345
print(format(x,'>10'))
print(format(x,'^10.2f'))
```

![](https://img-blog.csdnimg.cn/20190414114055722.png)

```
#老代码中，经常会看到被用来格式化文本的%操作符。比如：
print('%-20s'%text)
print('%20s'%text)
#建议使用format方法，更加强大也更加通用
```

![](https://img-blog.csdnimg.cn/20190414114117412.png)




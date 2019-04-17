# 05  讲： 闲聊之 Python  的数据类型 - 博客堂 - CSDN博客





2016年12月20日 22:42:56[最小森林](https://me.csdn.net/u012052268)阅读数：761标签：[Python](https://so.csdn.net/so/search/s.do?q=Python&t=blog)
个人分类：[Python](https://blog.csdn.net/u012052268/article/category/6625160)









本期内容介绍：



1. 基本数据类型：

1）整型，在 Python3 中长整形和整形归为一类，所有的整数都属于整型，例如 1，0,1000,1203 等等

2）浮点型，数字中有小数点的数，如 12.1 1.85 10.0 等等，另外 1e10 也表示浮点数

3）字符串，所有以单引号或双引号括起来的值都叫做字符串，例如“小甲鱼” '小甲虫' '醉酒青牛'

4）布尔类型， 布尔类型只有两个值，True 或 False ，记得在 Python 里面这两个值首字母均大写。

加减乘除：

5.2 / 2 = 2.6

5.2 // 2 = 2

5.2 % 2 = 0.6




2. e 记法；

如果数位很多的话，一般将其表示为 e 记法形式，例如 0.000000000000135 用 e 记法表示为 1.35e-13,它是一个浮点数类型，

例如用 e 记法表示为 1e3，表示的是 1000.0 是个浮点数，这里 e 也可以改写成 E。



3. 数据类型转换

整型强制转换函数 int(),可以将纯数字的字符串或浮点型强制转换为整形，

如 int('0012')为 12； int(12.5)为 12

浮点型强制转换函数 float(),可以将包含一个小数点的数字字符串或者整形转换为浮点型，

如 float('0012')为 12.0，float('.123')为 0.123； float(12)为 12.0

字符串强制类型转换函数 str(),可以将任何类型转换为字符串类型，所以str是一个BIF 不要把他定义为变量。

例如 str(1e28)为„1e+28' str(1e2)为'100.0'等等



4. 数据类型信息获取；

函数 type()，直接给出数的数据类型，例如 type(12.0)则返回 float，type(1000)返回 int， type(False)返回 bool 等

函数 isinstance()需要给出需要判断的数和一个数据类型，是则返回 True，否则返回 False，

如 isinstace(1,int)返回True ，isinstance(12.5,float)返回 True，isinstance('I love u',float)返回 False，

而 isinstance('I love u',str)返回 True。


补充：

s 为字符串
s.isalnum() 所有字符都是数字或者字母，为真返回 Ture，否则返回 False。
s.isalpha() 所有字符都是字母，为真返回 Ture，否则返回 False。
s.isdigit() 所有字符都是数字，为真返回 Ture，否则返回 False。
s.islower() 所有字符都是小写，为真返回 Ture，否则返回 False。
s.isupper() 所有字符都是大写，为真返回 Ture，否则返回 False。
s.istitle() 所有单词都是首字母大写，为真返回 Ture，否则返回 False。
s.isspace() 所有字符都是空白字符，为真返回 Ture，否则返回 False。

例如：

>>> s = 'I LOVE FISHC'

>>> s.isupper()

>>> True





例子：

1. 写一个程序，判断给定年份是否为闰年。（注意：请使用已学过的 BIF 进行灵活运用）

这样定义闰年的:能被 4 整除但不能被 100 整除,或者能被 400 整除都是闰年。







```python
temp = input('请输入一个年份：')
while not temp.isdigit(): #用来判断输入是否为数字，记好
   temp = input("抱歉，您的输入有误，请输入一个整数：")
year = int(temp)
if year/400 == int(year/400):
   print(temp + ' 是闰年！')
else:
   if (year/4 == int(year/4)) and (year/100 != int(year/100)):
      print(temp + ' 是闰年！')
   else:
      print(temp + ' 不是闰年！')
```






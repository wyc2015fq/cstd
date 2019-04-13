
# 2.5 Python变量与赋值 -  芝麻软件工作室的专栏 - CSDN博客


2016年06月30日 10:03:25[seven-soft](https://me.csdn.net/softn)阅读数：181个人分类：[Python																](https://blog.csdn.net/softn/article/category/6290759)



变量用来存储数据，是标识符的一种。Python 变量的命名规则与标识符的命名规则一样。
Python 是动态类型语言， 也就是说不需要预先声明变量的类型，变量的类型和值在赋值那一刻被初始化。
>>> counter = 0
>>> miles = 1000.0
>>> name = 'Bob'
>>> counter = counter + 1
>>> kilometers = 1.609 * miles
>>> print '%f miles is the same as %f km' % (miles, kilometers)
1000.000000 miles is the same as 1609.000000 km
Python 也支持增量赋值，也就是运算符和等号合并在一起， 看下面的例子：n=n*10

将上面的例子改成增量赋值方式就是：n*=10

两点注意：Python的变量名是大小写敏感的。也就是说变量 "cAsE" 与 "CaSe" 是两个不同的变量。
Python 不支持自增和自减运算符。这是因为 + 和 － 也是单目运算符，Python 会将 --n 解释为-(-n) 从而得到 n，同样 ++n 的结果也是 n。


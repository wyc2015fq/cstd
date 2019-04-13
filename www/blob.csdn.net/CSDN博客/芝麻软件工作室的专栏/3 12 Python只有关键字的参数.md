
# 3.12 Python只有关键字的参数 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月01日 07:37:20[seven-soft](https://me.csdn.net/softn)阅读数：572


如果我们想要指定特定的关键字参数作为只有关键字的参数，而不是位置参数，它们可以被声明在星号参数后。例如：deftotal(initial=5,*numbers,extra_number):
count=initial
fornumberinnumbers:
count+=number
count+=extra_number
print(count)
total(10,1,2,3,extra_number=50)
total(10,1,2,3)
\# Raises error because we have not supplied a default argument value for 'extra_number'

输出：
66
Traceback (most recent call last):
File "keyword_only.py", line 12, in <module>
total(10, 1, 2, 3)
TypeError: total() needs keyword-only argument extra_number
它是如何工作的：
在星号参数后面声明参数，结果是只有关键字参数，如果这些参数没有提供一个默认值，那么如果关键字参数没有提供值，函数调用会产生错误，如上所示。
注意，+=的用法是操作符的缩写，因此，x = x + y，你可以写成x += y。
如果你不需要星号参数，但还想使用只有关键字参数，那么只需简单使用一个星号而不使用任何名字，例如，def total(initial=5, *, extra_number)。


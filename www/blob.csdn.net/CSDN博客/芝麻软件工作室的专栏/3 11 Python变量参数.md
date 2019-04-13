
# 3.11 Python变量参数 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月01日 07:36:42[seven-soft](https://me.csdn.net/softn)阅读数：415


有时你可能想定义一个函数，它可以获取参数的任何值，这可以通过使用星号（*）实现：deftotal(initial=5,*numbers,**keywords):
count=initial
fornumberinnumbers:
count+=number
forkeyinkeywords:
count+=keywords[key]
returncount
print(total(10,1,2,3,vegetables=50,fruits=100))

输出：
166
它是如何工作的：
当我们声明一个星号的参数，如*param，那么从这一点开始到结束的所有位置的参数都被收集到一个叫param的元组中。
同样,当我们声明一个双星参数，如**param，那么人那一点开始到结束的所有关键字参数都被收集到一个叫param的字典中。
我们将在后续章节中探讨元组和字典。


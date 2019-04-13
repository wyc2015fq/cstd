
# 3.13 Python return语句 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月01日 07:38:25[seven-soft](https://me.csdn.net/softn)阅读数：248个人分类：[Python																](https://blog.csdn.net/softn/article/category/6290759)



return 语句用来从函数中return(返回)，也就是说跳出函数。同样，我们也可以从函数中选择性地返回一个值。
return语句举例：defmaximum(x,y):
ifx>y:
returnx
elifx==y:
return'两个数相等'
else:
returny
print(maximum(2,3))

输出：
3
它是如何工作的：
函数maximum返回参数中的最大值，在这个例子中是提供给函数的数值。它使用了简单的if..else语句找到比较大的值，然后return(返回)那个值。
注意，没有一个值的return语句相当于return None(什么也不返回)。None是Python中的一个特殊类型，它代表什么也没有。例如，如果一个变量的值是None，它说明这个变量没有值。
除非你已经写了自己的return语句，否则，每个函数都默认包含一个return None语句。通过运行print(someFunction())你可以看到这一点，这里someFunction 没有使用return语句，比如：defsomeFunction():
pass

在Python中pass语句用来说明一个空的语句块。
注意：已经有一个叫max的内建函数能够完成'find maximum'函数的功能 ，因此，只要可能使用这个内建函数。


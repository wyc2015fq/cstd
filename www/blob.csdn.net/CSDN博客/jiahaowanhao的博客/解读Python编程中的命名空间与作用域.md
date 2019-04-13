
# 解读Python编程中的命名空间与作用域 - jiahaowanhao的博客 - CSDN博客


2018年05月09日 09:36:11[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：22标签：[Python																](https://so.csdn.net/so/search/s.do?q=Python&t=blog)


[解读Python编程中的命名空间与作用域](http://cda.pinggu.org/view/25496.html)
变量是拥有匹配对象的名字（标识符）。命名空间是一个包含了变量名称们（键）和它们各自相应的对象们（值）的字典。
一个Python表达式可以访问局部命名空间和全局命名空间里的变量。如果一个局部变量和一个全局变量重名，则局部变量会覆盖全局变量。
每个函数都有自己的命名空间。类的方法的作用域规则和通常函数的一样。
Python会智能地猜测一个变量是局部的还是全局的，它假设任何在函数内赋值的变量都是局部的。
因此，如果要给全局变量在一个函数里赋值，必须使用global语句。
global VarName的表达式会告诉Python， VarName是一个全局变量，这样Python就不会在局部命名空间里寻找这个变量了。
命名空间的定义
Python命名空间是名称到对象的映射，这就像是字典，键名是变量名，值是变量的值。比如：
>>> x = 3
>>> globals()
{'__builtins__': <module '__builtin__' (built-in)>, '__name__': '__main__', '__doc__': None, 'x': 3, '__package__': None}
可以看到变量x，3以字典的形式存放在globals空间内。以之对应的名字空间还有：locals()。
>>> locals()
{'__builtins__': <module '__builtin__' (built-in)>, '__name__': '__main__', '__doc__': None, 'x': 3, '__package__': None}
实际上，你可以通过向名字添加键名和值：
>>> globals()['y'] = 5
>>> y
5
上图左侧是内置命名空间，右侧是不同的模块，有各自的全局命名空间，全局命名空间内定义函数就会有局部命名空间。

命名空间的种类
Python中有三种命名空间：
a) 局部，函数内的命名空间就是局部的；
b) 全局，模块内的命名空间就是全局的；
c) 内置，包括异常类型、内建函数和特殊方法，可以代码中任意地方调用；
下面讨论关于名字空间的搜索顺序，先来看张图：
![](http://files.jb51.net/file_images/article/201510/20151016154142575.png?201591615423)
命名空间的可见性（作用域）
a) 内置命名空间在代码所有位置都是可见的，所以可以随时被调用；
b) 全局命名空间和局部命名空间中， 如果有同名变量，在全局命名空间处，局部命名空间内的同名变量是不可见的；
c) 在局部命名空间处，全局命名空间的同名变量是不可见的（只有变量不同名的情况下，可使用 global关键字让其可见）。
知道了可见性，下面说变量的查找顺序就要清楚多了。
命名空间的查找顺序
a) 如果在函数内调用一个变量，先在函数内（局部命名空间）查找，如果找到则停止查找。否则在函数外部（全局命名空间）查找，如果还是没找到，则查找内置命名空间。如果以上三个命名都未找到，则抛出NameError 的异常错误。
b) 如果在函数外调用一个变量，则在函数外查找（全局命名空间，局部命名空间此时不可见），如果找到则停止查找，否则到内置命名空间中查找。如果两者都找不到，则抛出异常。只有当局部命名空间内，使用global 关键字声明了一个变量时，查找顺序则是 a) 的查找顺序。
为了帮助理解，来举个例子，我们在全局命名空间里定义一个变量money。我们再在函数内给变量money赋值，然后Python会假定money是一个局部变量。然而，我们并没有在访问前声明一个局部变量money，结果就是会出现一个UnboundLocalError的错误。取消global语句的注释就能解决这个问题。

\#!/usr/bin/python
\# -*- coding: UTF-8 -*-
Money = 2000
def AddMoney():
\# 想改正代码就取消以下注释:
\# global Money
Money = Money + 1
print Money
AddMoney()
print Money
dir()函数
dir()函数一个排好序的字符串列表，内容是一个模块里定义过的名字。
返回的列表容纳了在一个模块里定义的所有模块，变量和函数。如下一个简单的实例：
\#!/usr/bin/python
\# -*- coding: UTF-8 -*-
\# 导入内置math模块
import math
content = dir(math)
print content;
以上实例输出结果：
['__doc__', '__file__', '__name__', 'acos', 'asin', 'atan',
'atan2', 'ceil', 'cos', 'cosh', 'degrees', 'e', 'exp',
'fabs', 'floor', 'fmod', 'frexp', 'hypot', 'ldexp', 'log',
'log10', 'modf', 'pi', 'pow', 'radians', 'sin', 'sinh',
'sqrt', 'tan', 'tanh']
在这里，特殊字符串变量__name__指向模块的名字，__file__指向该模块的导入文件名。
globals()和locals()函数
根据调用地方的不同，globals()和locals()函数可被用来返回全局和局部命名空间里的名字。
如果在函数内部调用locals()，返回的是所有能在该函数里访问的命名。
如果在函数内部调用globals()，返回的是所有在该函数里能访问的全局名字。
两个函数的返回类型都是字典。所以名字们能用keys()函数摘取。
reload()函数
当一个模块被导入到一个脚本，模块顶层部分的代码只会被执行一次。
因此，如果你想重新执行模块里顶层部分的代码，可以用reload()函数。该函数会重新导入之前导入过的模块。语法如下：
reload(module_name)
在这里，module_name要直接放模块的名字，而不是一个字符串形式。比如想重载hello模块，如下：
reload(hello)



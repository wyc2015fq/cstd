# Python命名空间和作用域窥探 - 宇宙浪子的专栏 - CSDN博客
2018年01月08日 18:10:33[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：117标签：[python																[作用域](https://so.csdn.net/so/search/s.do?q=作用域&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[Python](https://blog.csdn.net/bluehawksky/article/category/2542577)
转自：http://python.jobbole.com/81367/
# Namespace and Scope（命名空间和作用域）[](http://blog.cipherc.com/2015/04/25/python_namespace_and_scope/#namespace-and-scope)
## namespace
Namespace（只）是 从名字到对象的一个映射(a mapping from name to objects) 。大部分namespace都是按Python中的字典来实现的。有一些常见的namespace：built-in中的集合（ abs() 函数等）、一个模块中的全局变量等。
从某种意义上来说，一个对象(object)的所有属性(attribute)也构成了一个namespace。在程序执行期间，可能（其实是肯定）会有多个名空间同时存在。不同namespace的创建/销毁时间也不同。
此外，两个不同namespace中的两个相同名字的变量之间没有任何联系。
## scope
有了namespace基础之后，让我们再来看看scope。Scope是Python程序的一块文本区域(textual region)。
在该文本区域中，对namespace是可以直接访问，而不需要通过属性来访问。
Scope是定义程序该如何搜索确切地“名字-对象”的名空间的层级关系。
(The “scope” in Python defines the “hirerchy level” in which we search namespaces for
certain “name-to-object” mappings.)
Tip
直接访问：对一个变量名的引用会在所有namespace中查找该变量，而不是通过属性访问。
属性访问：所有名字后加 . 的都认为是属性访问。
如 module_name.func_name ，需要指定 func_name 的名空间，属于属性访问。
而 abs(-1) ， abs 属于直接访问。
## 两者之间有什么联系呢？
Important
在Python中，scope是由namespace按特定的层级结构组合起来的。
scope一定是namespace，但namespace不一定是scope.
# LEGB-rule
在一个Python程序运行中，至少有4个scopes是存在的。
直接访问一个变量可能在这四个namespace中逐一搜索。
- - 
Local(innermost)
包含局部变量。
比如一个函数/方法内部。
- - 
Enclosing
包含了非局部(non-local)也非全局(non-global)的变量。
比如两个嵌套函数，内层函数可能搜索外层函数的namespace，但该namespace对内层函数而言既非局部也非全局。 
- - 
Global(next-to-last)
当前脚本的最外层。
比如当前模块的全局变量。 
- - 
Built-in(outtermost)
Python __builtin__ 模块。
包含了内建的变量/关键字等。 
那么，这么多的作用域，Python是按什么顺序搜索对应作用域的呢？
著名的”LEGB-rule”，即scope的搜索顺序：
Important
Local -> Enclosing -> Global -> Built-in
怎么个意思呢？
当有一个变量在 local 域中找不到时，Python会找上一层的作用域，即 enclosing 域（该域不一定存在）。enclosing 域还找不到的时候，再往上一层，搜索模块内的 global 域。最后，会在 built-in 域中搜索。对于最终没有搜索到时，Python会抛出一个 NameError 异常。
作用域可以嵌套。比如模块导入时。
这也是为什么不推荐使用 from a_module import * 的原因，导入的变量可能被当前模块覆盖。
# Assignment rule
看似python作用域到此为止已经很清晰了，让我们再看一段代码：

Python
```
```
|1234567891011|defouter():a=0b=1definner():printaprintbinner()outer()|
你觉得结果是什么呢？So easy是不是？

``
|123|cipher<ahref="http://www.jobbole.com/members/rachel">@Rachel</a>~/Development/Workspace/test_Python$python2a.py01|
如果多加一句呢？

Python
```
```
|123456789101112131415|defouter():a=0b=1definner():printaprintb# b += 1        # Ab=4# Binner()outer()|
结果又会是什么呢？

``
|12345678910|cipher<ahref="http://www.jobbole.com/members/rachel">@Rachel</a>~/Development/Workspace/test_Python$python2a.py0Traceback(mostrecentcalllast):File"a.py",line34,in<module>outer()File"a.py",line32,inouterinner()File"a.py",line29,ininnerprintbUnboundLocalError:localvariable'b'referencedbeforeassignment|
是不是很奇怪？
原因是这样的：
Python解释器执行到 inner() 中的 print b 时，发现有个变量 b 在当前作用域(local)中
无法找到该变量。它继续尝试把整块代码解释完。
Bingo! 找到了。那么 b 是属于 inner() 作用域的。
既然对变量 b 的赋值（声明）发生在 print 语句之后， print 语句执行时
变量 b 是还未被声明的，于是抛出错误：变量在赋值前就被引用。
在这个例子中，只有A语句没有B语句也会导致同样的结果。
因为 b += 1 等同于 b = b + 1。
对于变量的作用域查找有了了解之后，还有两条很重要的规则：
Important
- 赋值语句通常隐式地会创建一个局部(local)变量，即便该变量名已存在于赋值语句发生的上一层作用域中；
- 如果没有 global 关键字声明变量，对一个变量的赋值总是认为该变量存在于最内层(innermost)的作用域中；
也就是说在作用域内有没有发生赋值是不一样的。
但是，在这点上，Python 2和Python 3又有不同， [Python
 access non-local variable](http://stackoverflow.com/questions/13282910/python-cant-access-nonlocal-variable-before-local-variable-is-defined-with-same):
Python’s scoping rules indicate that a function defines a new scope level,
and a name is bound to a value in only one scope level – it is statically scoped.
…
In Python 2.x, it is not possible to modify a non-local variable;
1) you have either read-only access to a global or non-local variable,
2) or read-write access to a global variable by using the global statement,
3) or read-write access to a local variable (by default).
In Python 3.x, the nonlocal statement has been introduced with a similar effect
to global, but for an intermediate scope.
# for 循环
为什么讲到作用域要说到 for 循环呢？难道!@#$%^&*()？？？
对于大部分语言（比如 C 语言）而言， for-loop 会引入一个新的作用域。
但Python有点一样却又不太一样。
让我们先来看个例子：


```
```
|12345678910111213141516171819|CipherChen@CIPHERC~/Development/Workspace/test_python$python2Python2.7.9(default,Jan252015,13:42:57)[GCC4.2.1Compatible Apple LLVM6.0(clang-600.0.56)]on darwinType"help","copyright","credits"or"license"formore information.>>>foriinrange(10):printi...0123456789>>>printi9>>>|
有点不可思议是不是？
在 [Python 2.x for语句](https://docs.python.org/2.7/reference/compound_stmts.html#the-for-statement) 中是这么说的：
The for-loop makes assignments to the variable(s) in the target list.
This overwrites all previous assignments to those variablees including those made in the suite of the for-loop.
…
The target list is not deleted when the loop is finished.
But if the sequence is empty, they will not have been assigned to at all the loop.
for 后面跟着的变量(target list)在循环结束后是不会被删除的，
但如果 for 循环的序列为空，这些变量是完全不会被赋值的。
这在Python中是个大坑啊。
避免这个坑的解决办法就是规范命名规范。
比如用于循环的变量尽量使用单字符。在任何有疑议的情况可以直接将索引值初始化。
很不幸，Python 3中这点没有改变。
# List Comprehension vs. Generator Expression
关于Python作用域这堂课已经上了很久了，我们先休息一下，说个题外话吧。
- 列表推导式(List Comprehension)
> 

``
|12|# List comprehension[expressionforvariniterable]|
简单的理解列表推导式：

Python
```
```
|1234567|def_lc(arg):result=[]foriinarg:result.append(i*i)returnresult<expr_value>=_lc(x)|
- 列表推导式会把所有数据都加载到内存。适合 “结果需要多次被使用” 或者 “需要使用list相关的方法（分片等）” 等的情况。
- 生成器表达式(Generator Expression)
> 

``
|12|# Generator expression(expressionforvariniterable)|
简单的理解生成器表达式：

Python
```
```
|12345|def_ge(arg):foriinarg:yieldi*i&lt;expr_value&gt;=_ge(x)|
- 使用生成器实现。适合“数据量非常大或者无限”的情况。
它们的表现效果分别是这样的：

``
|12345|>>>[iforiinrange(10)][0,1,2,3,4,5,6,7,8,9]>>>(iforiinrange(10))<generatorobject<genexpr>at0x7fd5ab625b88>|
# Python 作用域，我已经完全掌握了！
稍作小憩之后，看来大家对Python作用域很有信心了。
好的。那我们来测试一下。
这是类(class)定义中的一个小问题：

Python
```
```
|123|classA(object):a=3b=list(a+iforiinrange(10))|
这段代码执行起来是不是跟你想的有点一样但又不那么一样呢？

``
|123456789|cipher<ahref="http://www.jobbole.com/members/rachel">@Rachel</a>~/Development/Workspace/test_Python$pythona.pyTraceback(mostrecentcalllast):File"a.py",line3,in<module>classA(object):File"a.py",line5,inAb=list(a+iforiinrange(10))File"a.py",line5,in<genexpr>b=list(a+iforiinrange(10))NameError:globalname'a'isnotdefined|
刚刚总结的规则完全用不上啊！！！
“元芳，你怎么看？”
真相只有一个：
class没有作用域(scope)，但有一个局部的名空间(namespace)，它并不构成一个作用域。
这意味着在类定义中的表达式可以访问该名空间。
但在类体(class body)中， 对 b 的赋值表达式中，该表达式引入了一个新的作用域，该作用域并不能访问类的名空间。
就像刚刚说的，函数会引入一个新的作用域。
比如说：

Python
```
```
|1234|classC(object):a=2deffoo(self):returna# NameError: name 'a' is not defined, use return self.__class__.a|
在Python 2中，列表推导式没有引入一个新的作用域。所以：

``
|1234567|Python2.7.9(default,Jan252015,13:42:57)[GCC4.2.1CompatibleAppleLLVM6.0(clang-600.0.56)]ondarwinType"help","copyright","credits"or"license"formoreinformation.>>>[aforainrange(3)][0,1,2]>>>printa2|
而对于Python 2和Python 3，生成器表达式都有引入新的作用域。
为了让列表推导式和生成器表达式的表现一致，
在Python 3中，列表推导式也有引入一个新的作用域。所以：


```
```
|123456789|Python3.4.2(default,Apr252015,15:59:50)[GCC4.2.1Compatible Apple LLVM6.0(clang-600.0.57)]on darwinType"help","copyright","credits"or"license"formore information.>>>[aforainrange(3)][0,1,2]>>>print(a)Traceback(most recent call last):File"<stdin>",line1,in<module>NameError:name'a'isnotdefined|
## 解决方案
所以，要解决这个问题，有几种解决办法：
- 用生成器表达式

Python
``
|1|b=[a+iforiinrange(10)]|
- 用函数/lambda引入新的作用域

Python
```
```
|1|b=(lambdaa:((a+iforiinrange(10))))(a)|
有没有开始怀疑人生怀疑理想？
附一份：访问权限汇总表
|Can access class attributes|Python 2|Python 3|
|----|----|----|
|list comp. iterable|Y|Y|
|list comp. expression|Y|N|
|gen expr. iterable|Y|Y|
|gen expr. expression|N|N|
|dict comp. iterable|Y|Y|
|dict comp. expression|N|N|
# 总结
本文介绍了Python中 namespace 和 scope 的区别，
以及复杂作用域的搜索规则（ LEGB ）。
此外，还介绍了一些常见的会创建scope的情况（函数定义，生成器表达式等），当然包括
了Python 2和Python 3中的不同实现。
Python中对于作用域的定义确实是个大问题，我并没有找到像 C 语言那样，
“代码块 {} 中定义的即是一个局部作用域”这样简洁的规则来清晰地表明
Python中作用域的 创建/销毁 的条件。
这篇文章的内容积压了很久，终于抽了点时间出来整理了下。
写的也有点没章法了，各位看官看得懂就看吧；看不懂多看几遍吧。
看望之后也提点啥建议意见之类的，好让后来人也能更快速简单的理解这个问题。
万一我理解错了呢？
欢迎探讨。
但有一点可以肯定，“这事儿还没完”。
# 参考文献
[1] [A Beginner
 Guide to Python’s namespaces and scope resolution](http://nbviewer.ipython.org/github/rasbt/python_reference/blob/master/tutorials/scope_resolution_legb_rule.ipynb)
[2] [Python Scopes and Namespaces](https://docs.python.org/2/tutorial/classes.html#python-scopes-and-namespaces)
[3] [Generator Expressions vs. List Comprehension](http://stackoverflow.com/questions/47789/generator-expressions-vs-list-comprehension)
[4] [Reference
 class variable in a comprehension of another class variable](http://stackoverflow.com/questions/11749629/reference-class-variable-in-a-comprehension-of-another-class-variable)
[5] [Undefined
 global in list generator expression](http://stackoverflow.com/questions/11669379/undefined-global-in-list-generator-expression-using-python3-works-with-python2/11670273)
[6] [Python
 access non-local variable](http://stackoverflow.com/questions/13282910/python-cant-access-nonlocal-variable-before-local-variable-is-defined-with-same)
[7] [Seeming unintended difference between list comprehensions
 and generator expressions](https://mail.python.org/pipermail/python-dev/2009-February/086287.html)
[8] [Python 2.x for语句](https://docs.python.org/2.7/reference/compound_stmts.html#the-for-statement)
本文来自 CipherChen 的投稿自荐


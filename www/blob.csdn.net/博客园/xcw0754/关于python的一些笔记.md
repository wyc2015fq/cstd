# 关于python的一些笔记 - xcw0754 - 博客园
# [关于python的一些笔记](https://www.cnblogs.com/xcw0754/p/5060433.html)
　　Python源文件默认以UTF-8编码。在这种编码下，世界上大多数语言的字符可以在字符串，标识符和注释中同时使用 — 尽管标准库中的标识符只使用ASCII字符，它是可移植代码应该遵循的一个惯例。为了能够正确显示所有的这些字符，你的编辑器必须能够识别文件是UTF-8编码，且必须使用支持文件中所有字符的字体。
　　当然，也可以给源文件指定一个不同的编码。方法是在 #! 行的后面再增加一行特殊的注释来定义源文件的编码：
```
# -*- coding: encoding -*-
```
　　如果你只打印range，会出现奇怪的结果：
```
>>> print(range(10))
range(0, 10)
```
　　range()返回的对象的行为在很多方面很像一个列表，但实际上它并不是列表。当你迭代它的时候它会依次返回期望序列的元素，但是它不会真正产生一个列表，因此可以节省空间。
　　我们把这样的对象称为可迭代的，也就是说，它们适合作为期望连续获得元素直到穷尽的函数和构造器的目标。我们已经看到[for](http://python.usyiyi.cn/python_343/reference/compound_stmts.html#for)语句是这样的一个迭代器。[list()](http://python.usyiyi.cn/python_343/library/stdtypes.html#list)函数是另外一个；它从可迭代对象创建列表。可以想象，list()函数迫使可迭代对象将所有元素一次性生成为list型。
```
>>> list(range(5))
[0, 1, 2, 3, 4]
```
　　循环语句while/for可以有一个 else 子句；当循环是因为迭代完整个列表( [for](http://python.usyiyi.cn/python_343/reference/compound_stmts.html#for) 语句)或者循环条件不成立（[while](http://python.usyiyi.cn/python_343/reference/compound_stmts.html#while) 语句）终止，即正常终止，而非由[break](http://python.usyiyi.cn/python_343/reference/simple_stmts.html#break) 语句终止时，else子句将被执行。下面循环搜索质数的代码例示了这一点：
```
1 >>> for n in range(2, 10):
 2 ...     for x in range(2, n):
 3 ...         if n % x == 0:
 4 ...             print(n, 'equals', x, '*', n//x)
 5 ...             break
 6 ...     else:
 7 ...         # loop fell through without finding a factor
 8 ...         print(n, 'is a prime number')
 9 ...
10 2 is a prime number
11 3 is a prime number
12 4 equals 2 * 2
13 5 is a prime number
14 6 equals 2 * 3
15 7 is a prime number
16 8 equals 2 * 4
17 9 equals 3 * 3
```
　　是的，这是正确的代码。看仔细：else子句属于[for](http://python.usyiyi.cn/python_343/reference/compound_stmts.html#for) 循环，**不**属于 [if](http://python.usyiyi.cn/python_343/reference/compound_stmts.html#if) 语句。
　　函数体的第一行可以是一个可选的字符串文本，此字符串是该函数的文档字符串，或称为docstring。有工具能使用 docstrings 自动生成文档（如在线文档等），以方便查阅。以下是两种常见的写法：
```
1 >>> def fib(n):    # write Fibonacci series up to n
2 ...     """Print a Fibonacci series up to n."""
3 ...     a, b = 0, 1
4 ...     while a < n:
5 ...         print(a, end=' ')
6 ...         a, b = b, a+b
7 ...     print()
```
　　如果在文档字符串中有更多的行，第二行应该是空白，余下的行应该是一段或多段描述对象的调用约定、 其副作用等。若docstring是多行的，则该串不会被自动去除行缩进，所以必要时文档字符串处理工具应自己作清除缩进工作。以下是一种“约定”，用函数体首行之后的第一个非空行字符串确定整个串的缩进的量（注：首行的缩进并不适合使用，因为它通常紧靠着字符串起始的引号，缩进格式不清晰）。剩下其他行的行首等于缩进量的空格都将被过滤掉。当然，这仅是一种约定。
```
1 >>> def my_function():
 2 ...     """Do nothing, but document it.
 3 ...
 4 ...     No, really, it doesn't do anything.
 5 ...     """
 6 ...     pass
 7 ...
 8 >>> print(my_function.__doc__)
 9 Do nothing, but document it.
10 
11     No, really, it doesn't do anything.
```
函数的执行会引入一个新的符号表，用于函数的局部变量。更确切地说，函数中的所有的赋值都是将值存储在局部符号表；而变量引用首先查找局部符号表，然后是上层函数的局部符号表，然后是全局符号表，最后是内置名字表。因此，在函数内部全局变量不能直接赋值 （除非用 [global](http://python.usyiyi.cn/python_343/reference/simple_stmts.html#global) 语句命名），虽然可以引用它们。换句话说，在函数内所做的赋值操作仅在该函数内有效，而函数外的变量仍然不变。
```
1 a=10086
2 def change():
3     a=10010
4     return a
5 print(change())    --打印10010
6 print(a)        --打印10086
```
注释以字典的形式存储在函数的__annotations__属性中，对函数的其它任何部分都没有影响。参数注释用一个冒号在参数名后面定义 , 冒号后面紧跟着一个用于计算注释的表达式。返回值的注释使用 "->"来定义的，是紧跟着参数列表和 [def](http://python.usyiyi.cn/python_343/reference/compound_stmts.html#def) 语句的末尾的冒号之间的一个表达式。下面的示例包含有位置参数，关键字参数和返回值的注释各1个:
```
1 >>> def f(bird: str, egg:str ='pain')->str:
 2 ...     print("注释是", f.__annotations__)
 3 ...     print("参数有", bird, egg)
 4 ...     return bird+' and '+egg
 5 ...
 6 >>> f('viki')
 7 
 8 注释是 {'bird': <class 'str'>, 'egg': <class 'str'>, 'return': <class 'str'>}
 9 参数有 viki pain
10 viki and pain
```
　　仔细看这句 egg:str ='pain' ，其中的'pain'是egg的一个默认值，与str无关。没有注释时应该是这样的，def f(bird, egg='pain')。
　　代码风格 --Style推荐：[PEP 0008 -- Style Guide for Python Code](https://www.python.org/dev/peps/pep-0008/)
　　如果想同时获取dict中的某个key-value对怎么办？通常在循环迭代字典的时候，键和对应的值通过使用items()方法可以同时得到。 具体操作如下：
```
1 >>> knights = {'gallahad': 'the pure', 'robin': 'the brave'}
2 >>> for k, v in knights.items():
3 ...     print(k, v)
4 ...
5 gallahad the pure
6 robin the brave
```
　　此外，在序列（list，tuple）中遍历时，使用 [enumerate()](http://python.usyiyi.cn/python_343/library/functions.html#enumerate) 函数也可以同时得到索引和对应的值。
```
1 >>> for i, v in enumerate(['tic', 'tac', 'toe']):
2 ...     print(i, v)
3 ...
4 0 tic
5 1 tac
6 2 toe
```
 　　如果想同时遍历两个或多个序列，使用 [zip()](http://python.usyiyi.cn/python_343/library/functions.html#zip) 函数可以成对读取元素，这样子可能会引发一些序列的问题，比如多个序列的长度不一，有的就会被裁剪。简单操作例子：
```
1 >>> questions = ['name', 'quest', 'favorite color']
2 >>> answers = ['lancelot', 'the holy grail', 'blue']
3 >>> for q, a in zip(questions, answers):
4 ...     print('What is your {0}?  It is {1}.'.format(q, a))
5 ...
6 What is your name?  It is lancelot.
7 What is your quest?  It is the holy grail.
8 What is your favorite color?  It is blue.
```
　　其实zip()函数就是返回一个可迭代对象，每次将传进去的所有参数的同一列的元素给返回了，以至于能同时获取多个序列的同一列的元素。
　　反向遍历，首先可以正向生成这个序列，然后调用 [reversed()](http://python.usyiyi.cn/python_343/library/functions.html#reversed) 函数来将序列反置，注意返回的是一个新的对象：
```
1 >>> x=[1,2,3,4,6,8,10]
 2 >>> for i in reversed(x):
 3 ...     print(i)
 4 ...
 5 10
 6 8
 7 6
 8 4
 9 3
10 2
11 1
12 [1, 2, 3, 4, 6, 8, 10]
```
　　有一点需要特别注意，若要在循环内部修改正在遍历的序列（例如删除某些元素），建议您首先制作副本，因为在序列上循环每次都会读取对象，并不会隐式地创建副本。切片表示法使这尤其方便：
```
1 >>> words = ['cat', 'window', 'defenestrate']
2 >>> for w in words[:]:  # 关键在这使用切片
3 ...     if len(w) > 6:
4 ...         words.insert(0, w)
5 ...
6 >>> words
7 ['defenestrate', 'cat', 'window', 'defenestrate']
```
## 序列和其它类型的比较
　　序列对象可以与同序列类型的其他对象相比较。比较按照‘字典序’进行。如果一个序列是另一个序列的初始子序列，较短的序列就小于另一个。字符串的排序按照Unicode编码点的数值排序单个字符。下面是同类型序列之间比较的一些例子：
```
(1, 2, 3)              < (1, 2, 4)
[1, 2, 3]              < [1, 2, 4]
'ABC' < 'C' < 'Pascal' < 'Python'
(1, 2, 3, 4)           < (1, 2, 4)
(1, 2)                 < (1, 2, -1)
(1, 2, 3)             == (1.0, 2.0, 3.0)
(1, 2, ('aa', 'ab'))   < (1, 2, ('abc', 'a'), 4)
```
　　注意，使用< 或者 >比较不同类型的对象是合法的，只要这些对象具有合适的比较方法。例如，不同的数字类型按照它们的数值比较，所以 0 等于 0.0，等等。但也不是任何情况下都是合法的，比如list和tuple比较时，解释器将引发一个[TypeError](http://python.usyiyi.cn/python_343/library/exceptions.html#TypeError)异常。


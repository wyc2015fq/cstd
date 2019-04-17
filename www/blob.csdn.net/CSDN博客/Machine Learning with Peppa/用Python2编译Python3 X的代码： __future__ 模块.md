# 用Python2编译Python3.X的代码： __future__ 模块 - Machine Learning with Peppa - CSDN博客





2018年01月09日 22:05:17[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：328标签：[Python																[Python3																[编程](https://so.csdn.net/so/search/s.do?q=编程&t=blog)
个人分类：[***Data Science***																[编程之美：Python](https://blog.csdn.net/qq_39521554/article/category/7392111)](https://blog.csdn.net/qq_39521554/article/category/7293623)

所属专栏：[用Python玩转数据](https://blog.csdn.net/column/details/18811.html)





## 简介


从单词含义上猜应该是“未来”的模块。它有下面几个[目的](https://docs.python.org/2.7/library/__future__.html)：
- 避免和现有分析import工具混淆，并得到你期望的模块
- 确保2.1之前的版本导入__future__产生运行时异常，因为2.1之前没有这个模块
- 文档化不兼容的改变，通常这些改变会在新版中强制执行。这类文档以可执行的形式组织，通过导入__future__进行可编程式的检查。


以上是对官方解释的粗略翻译，翻译起来感觉有些拗口。我是这么理解的，某个版本中出现了某个新的功能特性，而且这个特性和当前版本中使用的不兼容，也就是它在该版本中不是语言标准，那么我如果想要使用的话就需要从__future__模块导入。在2.1版本之前并没有__future__，所以使用它会引发异常。当然，在以后的某个版本中，比如说3中，某个特性已经成为标准的一部分，那么使用该特性就不用从__future__导入了。


下面说一下__future__是如何实现新特性的。

## _Feature类


__future__.py中有形如下面的语句：

```python
```python
FeatureName = _Feature(OptionalRelease, MandatoryRelease, CompilerFlag)

class _Feature:
    def __init__(self, optionalRelease, mandatoryRelease, compiler_flag):
        self.optional = optionalRelease    # 某个特性被认可的初始版本
        self.mandatory = mandatoryRelease  # 某个特性成为标准的版本
        self.compiler_flag = compiler_flag

    def getOptionalRelease(self):
        """Return first release in which this feature was recognized.

        This is a 5-tuple, of the same form as sys.version_info.
        """

        return self.optional

    def getMandatoryRelease(self):
        """Return release in which this feature will become mandatory.

        This is a 5-tuple, of the same form as sys.version_info, or, if
        the feature was dropped, is None.
        """

        return self.mandatory

    def __repr__(self):
        return "_Feature" + repr((self.optional,
                                  self.mandatory,
                                  self.compiler_flag))
```
```

## OptionalRelease参数


通常OptionalRelease版本小于MandatoryRelease，每个都是5个元素的元组，类似sys.version_info。

```python
```python
(PY_MAJOR_VERSION, # the 2 in 2.1.0a3; an int
PY_MINOR_VERSION, # the 1; an int
PY_MICRO_VERSION, # the 0; an int
PY_RELEASE_LEVEL, # "alpha", "beta", "candidate" or "final"; string
PY_RELEASE_SERIAL # the 3; an int
)

# 例如: (2, 1, 0, "alpha", 3)表示2.1.0a3版
```
```


OptionalRelease版本中开始通过下列方式使用某个特性：

```python
```python
from __future__ import FeatureName
```
```

## MandatoryRelease参数


在MandatoryRelease版本中该特性变成Python标准的一部分。此外MandatoryRelease版本后不需要上面的导入语句就能使用该特性。MandatoryRelease可能是None，表示一个计划中的特性被放弃了。

## CompilerFlag参数


CompilerFlag编译器标志，它是一个位域标志，传给内建函数compile()做第四个参数，用来在动态编译代码的时候允许新的特性。


CompilerFlag值等价于Include/compile.h的预定义的CO_xxx标志。

## Python2 __future__模块的features


一共是以下7种，其对应的CompilerFlag:

```python
```python
all_feature_names = [
    "nested_scopes",
    "generators",
    "division",
    "absolute_import",
    "with_statement",
    "print_function",
    "unicode_literals",
]

CO_NESTED            = 0x0010   # nested_scopes
CO_GENERATOR_ALLOWED = 0        # generators (obsolete, was 0x1000)
CO_FUTURE_DIVISION   = 0x2000   # division
CO_FUTURE_ABSOLUTE_IMPORT = 0x4000 # perform absolute imports by default
CO_FUTURE_WITH_STATEMENT  = 0x8000   # with statement
CO_FUTURE_PRINT_FUNCTION  = 0x10000   # print function
CO_FUTURE_UNICODE_LITERALS = 0x20000 # unicode string literals
```
```

### nested_scopes

```python
```python
nested_scopes = _Feature((2, 1, 0, "beta",  1),
                         (2, 2, 0, "alpha", 0),
                         CO_NESTED)
```
```


nested_scopes指的是嵌套作用域，2.1.0b1中出现，2.2.0a0中成为标准。


提到作用域，那么就不得不说命名空间。

#### 命名空间的定义


Python命名空间是名称到对象的映射，目前是用字典实现，键名是变量名，值是变量的值。比如：

```python
```python
>>> x = 3
>>> globals()
{'__builtins__': <module '__builtin__' (built-in)>, '__name__': '__main__', '__doc__': None, 'x': 3, '__package__': None}
```
```


可以看到变量x，3以字典的形式存放在globals空间内。以之对应的名称空间还有：locals()。

```python
```python
>>> locals()
{'__builtins__': <module '__builtin__' (built-in)>, '__name__': '__main__', '__doc__': None, 'x': 3, '__package__'
```
```


实际上，你可以通过向名字添加键名和值，然后就可以直接使用名称了：

```python
```python
>>> globals()['y'] = 5
>>> y
5
```
```


通常，我们用属性来称呼'.'点号之后的名称为属性。比如，在z.real中，real是z的一个属性。严格来说，模块中的名称引用就是属性引用。modname.funcname中，modname是一个模块对象，而funcname是它的一个属性。模块属性和全局名称有映射关系，它们共享全局命名空间。上面代码中的x和y都是模块**main**的属性。


属性可能是只读的，也可能是可写的。模块属性是可写的，你可以这么做，modname.the_answer = 42。可写的属性能够用del语句来删除。比如，del modname.the_answer将会从模块modname中删除the_answer属性。

```python
```python
>>> x = 3
>>> globals()
{'__builtins__': <module '__builtin__' (built-in)>, '__package__': None, 'func': <function func at 0x029FA270>, 'x': 3, '__name__': '__main__', '__doc__': None}
>>> del x
>>> globals()
{'__builtins__': <module '__builtin__' (built-in)>, '__package__': None, 'func': <function func at 0x029FA270>, '__name__': '__main__', '__doc__': None}
```
```


del做的事实际上是删除了全局名称字典中的x键值。

#### 命名空间的种类


Python中有三种命名空间：


a) 局部，函数内的命名空间就是局部的，它记录了函数的变量，包括函数的参数和局部定义的变量。


b) 全局，模块内的命名空间就是全局的，它记录了模块的变量，包括函数、类、其它导入的模块、模块级的变量和常量。


c) 内置，包括异常类型、内建函数和特殊方法，可以代码中任意地方调用；


上面提到的globals是全局命名空间，locals是局部命名空间。


命名空间会在不同的时间创建，并有不同的生命周期。包含内置名称的命名空间是在python解释器启动的时候创建的，并且不会被删除。一个模块的全局命名空间是在模块定义代码被读入的时候创建的，一般情况下，模块命名空间会持续到解释器结束。在解释器最上层调用的代码，不管是从脚本中读入的还是在交互式界面中，都会被认为是属于一个叫做**main**模块的,所以它们有自己的全局命名空间。（内置名称实际上也放置在一个模块中，称为builtins）。


一个函数的局部命名空间在函数被调用的时候创建，在函数返回或者引发一个不在函数内部处理的异常时被删除。（实际上用遗忘来描述这个删除比较好。）当然了，递归调用的函数每个都有它们自己的命名空间。

#### 命名空间的可见性（作用域）


作用域是一个Python程序中命名空间直接可见的代码区域，也就是说这个区域内可以直接使用命名空间内的名称。


a) 内置命名空间在代码所有位置都是可见的，所以可以随时被调用；


b) 全局命名空间和局部命名空间中， 如果有同名变量，在全局命名空间处，局部命名空间内的同名变量是不可见的；


c) 在局部命名空间处，全局命名空间的同名变量是不可见的（只有变量不同名的情况下，可使用 global关键字让其可见）。

#### 命名空间的查找顺序


a) 如果在函数内调用一个变量，先在函数内（局部命名空间）查找，如果找到则停止查找。否则在函数外部（全局命名空间）查找，如果还是没找到，则查找内置命名空间。如果以上三个命名都未找到，则抛出NameError 的异常错误。


b) 如果在函数外调用一个变量，则在函数外查找（全局命名空间，局部命名空间此时不可见），如果找到则停止查找，否则到内置命名空间中查找。如果两者都找不到，则抛出异常。只有当局部命名空间内，使用global 关键字声明了一个变量时，查找顺序则是 a) 的查找顺序。

#### nested_scopes说明


Python2.2引入了一种略有不同但重要的改变，它会影响命名空间的搜索顺序：嵌套的作用域。在2.0中，当你在一个嵌套函数或 lambda 函数中引用一个变量时，Python会在当前（嵌套的或 lambda）函数的名称空间中搜索，然后在模块的名称空间。2.2将支在当前（嵌套的或 lambda）函数的名称空间中搜索，然后是在父函数的名称空间，接着是模块的名称空间。2.1可以两种方式工作，缺省地，按n2.0的方式工作，如果想像2.2中那样工作，使用下面的导入语句：

```python
```python
from __future__ import nested_scopes
```
```


当然现在一般都用2.7或者3了，所以已经是嵌套作用域了。


来看下面一段代码：

```python
```python
>>> x = 3
>>> globals()
{'__builtins__': <module '__builtin__' (built-in)>, '__package__': None, 'func': <function func at 0x028BA2B0>, 'x': 3, '__name__': '__main__', '__doc__': None}
>>> def func():
        x = 2
          print locals()

>>> func()
{'x': 2}
>>> globals()
{'__builtins__': <module '__builtin__' (built-in)>, '__package__': None, 'func': <function func at 0x028BA2B0>, 'x': 3, '__name__': '__main__', '__doc__': None}
```
```


全局命名空间中x的值前后并没有改变，反而在func函数的局部命名空间中产生了一个新的名称x。由此可以看出，外层作用域的命名空间对于内层来说是只读的，当写一个同名的名称时，只会在内层生成一个新的名称。但是如果一个名称被声明为global，对其引用和复制都会直接作用域全局名称。

```python
```python
>>> x = 2
>>> def func():
        global x
          x = 3
          print locals()

>>> globals()
{'__builtins__': <module '__builtin__' (built-in)>, '__package__': None, 'func': <function func at 0x029FA270>, 'x': 2, '__name__': '__main__', '__doc__': None
>>> func()
{}
>>> globals()
{'__builtins__': <module '__builtin__' (built-in)>, '__package__': None, 'func': <function func at 0x029FA270>, 'x': 3, '__name__': '__main__', '__doc__': None}
```
```


x的值前后改变了，而且func函数中也没用增加x。

#### import module和from module import func


import module将模块自身导入到当前命名空间，所以如果要使用module的某个函数或属性，只能module.func这么用。


而使用from module import func，则是将函数func导入当前的名称空间，这时候使用这个函数就不需要模块名称而是直接使用func。


我们通过一段代码来描述：

```python
```python
>>> globals()
{'__builtins__': <module '__builtin__' (built-in)>, '__package__': None, '__name__': '__main__', '__doc__': None}
>>> import os
>>> globals()
{'__builtins__': <module '__builtin__' (built-in)>, '__package__': None, '__name__': '__main__', 'os': <module 'os' from 'C:\Python27\lib\os.pyc'>, '__doc__': None}
>>> del os
>>> from os import sys
>>> globals()
{'__builtins__': <module '__builtin__' (built-in)>, '__package__': None, 'sys': <module 'sys' (built-in)>, '__name__': '__main__', '__doc__': None}
```
```


是不是很清晰，担任sys也是一个模块，如果要使用sys模块的属性，也必须要使用sys模块名了。这也是嵌套作用域的一个例子。


额。。。貌似本文的正题是__future__，哈哈，扯远了，我们继续来看下面一个feauture。

### generators

```python
```python
generators = _Feature((2, 2, 0, "alpha", 1),
                      (2, 3, 0, "final", 0),
                      CO_GENERATOR_ALLOWED)
```
```


generators生成器起于2.2.0a1版，在2.3.0f0中成为标准。

#### 简介


生成器是这样一个函数，它记住上一次返回时在函数体中的位置。对生成器函数的第二次（或第 n 次）调用跳转至该函数中间，而上次调用的所有局部变量都保持不变。Python中yeild就是一个生成器。

#### yield 生成器的运行机制


当你问生成器要一个数时，生成器会执行，直至出现 yield 语句，生成器把 yield 的参数给你，之后生成器就不会往下继续运行。 当你问他要下一个数时，他会从上次的状态。开始运行，直至出现yield语句，把参数给你，之后停下。如此反复直至退出函数。

#### 示例

```python
```python
>>> def my_generator():
          yield 1
          yield 2
          yield 3

>>> gen = my_generator()
>>> gen.next()
1
>>> gen.next()
2
>>> gen.next()
3
>>> gen.next()

Traceback (most recent call last):
  File "<pyshell#92>", line 1, in <module>
    gen.next()
StopIteration
>>> for n in my_generator:
        print n
1
2
3
```
```


yield在被next调用之前并没有执行（for循环内部也是使用next），在执行完最后一个yield之后再继续调用next，那么就好遇到StopIteration异常了。这里涉及到迭代器了，不再进行详细的描述了，后面会单开一章来讲Python的三大器：迭代器、生成器、装饰器。

### division

```python
```python
division = _Feature((2, 2, 0, "alpha", 2),
                    (3, 0, 0, "alpha", 0),
                    CO_FUTURE_DIVISION)
```
```


这个很简单，举例说明一下大家就懂了。

```python
```python
# python2.7中，不导入__future__
>>> 10/3
3

# 导入__future__
>>> from __future__ import division
>>> 10/3
3.3333333333333335
```
```


很容易看出来，2.7中默认的整数除法是结果向下取整，而导入了__future__之后除法就是真正的除法了。这也是python2和python3的一个重要区别。

### absolute_import

```python
```python
absolute_import = _Feature((2, 5, 0, "alpha", 1),
                           (3, 0, 0, "alpha", 0),
                           CO_FUTURE_ABSOLUTE_IMPORT)
```
```


python2.7中，在默认情况下，导入模块是相对导入的（relative import），比如说

```python
```python
from . import json
from .json import json_dump
```
```


这些以'.'点导入的是相对导入，而绝对导入（absolute import）则是指从系统路径sys.path最底层的模块导入。比如:

```python
```python
import os
from os import sys
```
```

### with_statement

```python
```python
with_statement = _Feature((2, 5, 0, "alpha", 1),
                          (2, 6, 0, "alpha", 0),
                          CO_FUTURE_WITH_STATEMENT)
```
```


with语句也不详细讲了，看这篇[浅谈Python的with语句](http://blog.syfun.net/2015/07/07/python-with-statement)

### print_function

```python
```python
print_function = _Feature((2, 6, 0, "alpha", 2),
                          (3, 0, 0, "alpha", 0),
                          CO_FUTURE_PRINT_FUNCTION)
```
```


这个就是最经典的python2和python3的区别了，python2中print不需要括号，而在python3中则需要。

```python
```python
# python2.7
print "Hello world"

# python3
print("Hello world")
```
```

### unicode_literals

```python
```python
unicode_literals = _Feature((2, 6, 0, "alpha", 2),
                            (3, 0, 0, "alpha", 0),
                            CO_FUTURE_UNICODE_LITERALS)
```
```

这是unicode的问题，讲起来又是长篇大论了，容我偷个懒，后面再讲吧。

至此，__future__模块的几个特性，算是说完了。好多内容都是参照官方文档，所以大家还是多看文档吧。

最后来点小插曲：python的with语句



## 引言




with 语句是从 Python 2.5 开始引入的一种与异常处理相关的功能（2.5 版本中要通过 from __future__ import with_statement 导入后才可以使用），从 2.6 版本开始缺省可用（参考 [What’s
 new in Python 2.6?](https://docs.python.org/release/2.6/whatsnew/2.6.html#pep-343-the-with-statement) 中 with 语句相关部分介绍）。with 语句适用于对资源进行访问的场合，确保不管使用过程中是否发生异常都会执行必要的“清理”操作，释放资源，比如文件使用后自动关闭、线程中锁的自动获取和释放等。

## 术语


要使用 with 语句，首先要明白上下文管理器这一概念。有了上下文管理器，with 语句才能工作。

下面是一组与上下文管理器和with 语句有关的概念。


上下文管理协议（Context Management Protocol）：包含方法 __enter__() 和 __exit__()，支持

该协议的对象要实现这两个方法。


上下文管理器（Context Manager）：支持上下文管理协议的对象，这种对象实现了

__enter__() 和 __exit__() 方法。上下文管理器定义执行 with 语句时要建立的运行时上下文，

负责执行 with 语句块上下文中的进入与退出操作。通常使用 with 语句调用上下文管理器，

也可以通过直接调用其方法来使用。


运行时上下文（runtime context）：由上下文管理器创建，通过上下文管理器的 __enter__() 和

__exit__() 方法实现，__enter__() 方法在语句体执行之前进入运行时上下文，__exit__() 在

语句体执行完后从运行时上下文退出。with 语句支持运行时上下文这一概念。


上下文表达式（Context Expression）：with 语句中跟在关键字 with 之后的表达式，该表达式

要返回一个上下文管理器对象。


语句体（with-body）：with 语句包裹起来的代码块，在执行语句体之前会调用上下文管

理器的 __enter__() 方法，执行完语句体之后会执行 __exit__() 方法。

## 基本语法和工作原理


with 语句的语法格式如下：

```
# 清单 1. with 语句的语法格式
with context_expression [as target(s)]:
    with-body
```


这里 context_expression 要返回一个上下文管理器对象，该对象并不赋值给 as 子句中的 target(s) ，如果指定了 as 子句的话，会将上下文管理器的__enter__() 方法的返回值赋值给 target(s)。target(s) 可以是单个变量，或者由“()”括起来的元组（不能是仅仅由“,”分隔的变量列表，必须加“()”）。


Python 对一些内建对象进行改进，加入了对上下文管理器的支持，可以用于 with 语句中，比如可以自动关闭文件、线程锁的自动获取和释放等。假设要对一个文件进行操作，使用 with 语句可以有如下代码：

```
# 清单 2. 使用 with 语句操作文件对象
with open(r'somefileName') as somefile:
    for line in somefile:
        print line
        # ...more code
```


这里使用了 with 语句，不管在处理文件过程中是否发生异常，都能保证 with 语句执行完毕后已经关闭了打开的文件句柄。如果使用传统的 try/finally 范式，则要使用类似如下代码：

```
# 清单 3. try/finally 方式操作文件对象
somefile = open(r'somefileName')
try:
    for line in somefile:
        print line
        # ...more code
finally:
    somefile.close()
```


比较起来，使用 with 语句可以减少编码量。已经加入对上下文管理协议支持的还有模块 threading、decimal 等。


PEP 0343 对 with 语句的实现进行了描述。with 语句的执行过程类似如下代码块：

```
# 清单 4. with 语句执行过程
context_manager = context_expression
exit = type(context_manager).__exit__
value = type(context_manager).__enter__(context_manager)
exc = True   # True 表示正常执行，即便有异常也忽略；False 表示重新抛出异常，需要对异常进行处理
try:
    try:
        target = value  # 如果使用了 as 子句
        with-body     # 执行 with-body
    except:
        # 执行过程中有异常发生
        exc = False
        # 如果 __exit__ 返回 True，则异常被忽略；如果返回 False，则重新抛出异常
        # 由外层代码对异常进行处理
        if not exit(context_manager, *sys.exc_info()):
            raise
finally:
    # 正常退出，或者通过 statement-body 中的 break/continue/return 语句退出
    # 或者忽略异常退出
    if exc:
        exit(context_manager, None, None, None)
    # 缺省返回 None，None 在布尔上下文中看做是 False
```
- 

执行 context_expression，生成上下文管理器 context_manager

- 

调用上下文管理器的 __enter__() 方法；如果使用了 as 子句，则将 __enter__() 方法的返回值赋值给 as 子句中的 target(s)

- 

执行语句体 with-body

- 

不管是否执行过程中是否发生了异常，执行上下文管理器的 __exit__() 方法，__exit__() 方法负责执行“清理”工作，如释放资源等。如果执行过程中没有出现异常，或者语句体中执行了语句 break/continue/return，则以 None 作为参数调用 __exit__(None, None, None) ；如果执行过程中出现异常，则使用 sys.exc_info 得到的异常信息为参数调用__exit__(exc_type, exc_value, exc_traceback)

- 

出现异常时，如果 __exit__(type, value, traceback) 返回 False，则会重新抛出异常，让with 之外的语句逻辑来处理异常，这也是通用做法；如果返回 True，则忽略异常，不再对异常进行处理


## 自定义上下文管理器


开发人员可以自定义支持上下文管理协议的类。自定义的上下文管理器要实现上下文管理协议所需要的 __enter__() 和 __exit__() 两个方法：


context_manager.__enter__()：进入上下文管理器的运行时上下文，在语句体执行前调用。with 语句将该方法的返回值赋值给 as 子句中的 target，如果指定了 as 子句的话


context_manager.__exit__(exc_type, exc_value, exc_traceback)： 退出与上下文管理器相关的运行时上下文，返回一个布尔值表示是否对发生的异常进行处理。参数表示引起退出操作的异常，如果退出时没有发生异常，则3个参数都为None。如果发生异常，返回True 表示不处理异常，否则会在退出该方法后重新抛出异常以由 with 语句之外的代码逻辑进行处理。如果该方法内部产生异常，则会取代由 statement-body 中语句产生的异常。要处理异常时，不要显示重新抛出异常，即不能重新抛出通过参数传递进来的异常，只需要将返回值设置为
 False 就可以了。之后，上下文管理代码会检测是否__exit__() 失败来处理异常


下面通过一个简单的示例来演示如何构建自定义的上下文管理器。注意，上下文管理器必须同时提供 __enter__() 和 __exit__() 方法的定义，缺少任何一个都会导致 AttributeError；with 语句会先检查是否提供了 __exit__() 方法，然后检查是否定义了 __enter__() 方法。


假设有一个资源 DummyResource，这种资源需要在访问前先分配，使用完后再释放掉；分配操作可以放到 __enter__() 方法中，释放操作可以放到 __exit__() 方法中。简单起见，这里只通过打印语句来表明当前的操作，并没有实际的资源分配与释放。

```
# 清单 5. 自定义支持 with 语句的对象
class DummyResource:
    def __init__(self, tag):
            self.tag = tag
            print 'Resource [%s]' % tag
        def __enter__(self):
            print '[Enter %s]: Allocate resource.' % self.tag
            return self	  # 可以返回不同的对象
        def __exit__(self, exc_type, exc_value, exc_tb):
            print '[Exit %s]: Free resource.' % self.tag
            if exc_tb is None:
                print '[Exit %s]: Exited without exception.' % self.tag
            else:
                print '[Exit %s]: Exited with exception raised.' % self.tag
                return False   # 可以省略，缺省的None也是被看做是False
```


DummyResource 中的 __enter__() 返回的是自身的引用，这个引用可以赋值给 as 子句中的 target 变量；返回值的类型可以根据实际需要设置为不同的类型，不必是上下文管理器对象本身。


__exit__() 方法中对变量 exc_tb 进行检测，如果不为 None，表示发生了异常，返回 False 表示需要由外部代码逻辑对异常进行处理；注意到如果没有发生异常，缺省的返回值为 None，在布尔环境中也是被看做 False，但是由于没有异常发生，__exit__() 的三个参数都为 None，上下文管理代码可以检测这种情况，做正常处理。


下面在 with 语句中访问 DummyResource ：

```
# 清单 6. 使用自定义的支持 with 语句的对象
with DummyResource('Normal'):
    print '[with-body] Run without exceptions.'

with DummyResource('With-Exception'):
    print '[with-body] Run with exception.'
    raise Exception
    print '[with-body] Run with exception. Failed to finish statement-body!'
```


第1个 with 语句的执行结果如下：

```
# 清单 7. with 语句1执行结果
Resource [Normal]
[Enter Normal]: Allocate resource.
[with-body] Run without exceptions.
[Exit Normal]: Free resource.
[Exit Normal]: Exited without exception.
```


可以看到，正常执行时会先执行完语句体 with-body，然后执行 __exit__() 方法释放资源。


第2个 with 语句的执行结果如下：

```
# 清单 8. with 语句2执行结果
Resource [With-Exception]
[Enter With-Exception]: Allocate resource.
[with-body] Run with exception.
[Exit With-Exception]: Free resource.
[Exit With-Exception]: Exited with exception raised.

Traceback (most recent call last):
  File "G:/demo", line 20, in <module>
    raise Exception
Exception
```


可以看到，with-body 中发生异常时with-body 并没有执行完，但资源会保证被释放掉，同时产生的异常由 with 语句之外的代码逻辑来捕获处理。


可以自定义上下文管理器来对软件系统中的资源进行管理，比如数据库连接、共享资源的访问控制等。Python 在线文档 Writing Context Managers 提供了一个针对数据库连接进行管理的上下文管理器的简单范例。

## contextlib 模块


contextlib 模块提供了3个对象：装饰器 contextmanager、函数 nested 和上下文管理器 closing。使用这些对象，可以对已有的生成器函数或者对象进行包装，加入对上下文管理协议的支持，避免了专门编写上下文管理器来支持 with 语句。

### 装饰器 contextmanager


contextmanager 用于对生成器函数进行装饰，生成器函数被装饰以后，返回的是一个上下文管理器，其 __enter__() 和 __exit__() 方法由 contextmanager 负责提供，而不再是之前的迭代子。被装饰的生成器函数只能产生一个值，否则会导致异常 RuntimeError；产生的值会赋值给 as 子句中的 target，如果使用了 as 子句的话。下面看一个简单的例子。

```
# 清单 9. 装饰器 contextmanager 使用示例
from contextlib import contextmanager

@contextmanager
def demo():
    print '[Allocate resources]'
    print 'Code before yield-statement executes in \_\_enter\_\_'
    yield '*** contextmanager demo ***'
    print 'Code after yield-statement executes in \_\_exit\_\_'
    print '[Free resources]'

with demo() as value:
    print 'Assigned Value: %s' % value
```


结果输出如下：

```
# 清单 10. contextmanager 使用示例执行结果
[Allocate resources]
Code before yield-statement executes in __enter__
Assigned Value: *** contextmanager demo ***
Code after yield-statement executes in __exit__
[Free resources]
``
可以看到，生成器函数中 yield 之前的语句在 \_\_enter\_\_() 方法中执行，yield 之后的语句在 \_\_exit\_\_() 中执行，而 yield 产生的值赋给了 as 子句中的 value 变量。

需要注意的是，contextmanager 只是省略了 \_\_enter\_\_() / \_\_exit\_\_() 的编写，但并不负责实现资源的“获取”和“清理”工作；“获取”操作需要定义在 yield 语句之前，“清理”操作需要定义 yield 语句之后，这样 with 语句在执行 \_\_enter\_\_() / \_\_exit\_\_() 方法时会执行这些语句以获取/释放资源，即生成器函数中需要实现必要的逻辑控制，包括资源访问出现错误时抛出适当的异常。

### 函数 nested

nested 可以将多个上下文管理器组织在一起，避免使用嵌套 with 语句。

```python
# 清单 11. nested 语法
with nested(A(), B(), C()) as (X, Y, Z):
      # with-body code here
```


类似于：

```
# 清单 12. nested 执行过程
with A() as X:
    with B() as Y:
        with C() as Z:
              # with-body code here
```


需要注意的是，发生异常后，如果某个上下文管理器的 __exit__() 方法对异常处理返回 False，则更外层的上下文管理器不会监测到异常。

### 上下文管理器 closing


closing 的实现如下：

```
# 清单 13. 上下文管理 closing 实现
class closing(object):
    # help doc here
    def __init__(self, thing):
        self.thing = thing
    def __enter__(self):
        return self.thing
    def __exit__(self, *exc_info):
        self.thing.close()
```


上下文管理器会将包装的对象赋值给 as 子句的 target 变量，同时保证打开的对象在 with-body 执行完后会关闭掉。closing 上下文管理器包装起来的对象必须提供 close() 方法的定义，否则执行时会报 AttributeError 错误。

```
# 清单 14. 自定义支持 closing 的对象
class ClosingDemo(object):
    def __init__(self):
        self.acquire()
    def acquire(self):
        print 'Acquire resources.'
    def free(self):
        print 'Clean up any resources acquired.'
    def close(self):
        self.free()

with closing(ClosingDemo()):
    print 'Using resources'
```


结果输出如下：

```
# 清单 15. 自定义 closing 对象的输出结果
Acquire resources.
Using resources
Clean up any resources acquired.
```


closing 适用于提供了 close() 实现的对象，比如网络连接、数据库连接等，也可以在自定义类时通过接口 close() 来执行所需要的资源“清理”工作。

## 小结


本文对 with 语句的语法和工作机理进行了介绍，并通过示例介绍了如何实现自定义的上下文管理器，最后介绍了如何使用 contextlib 模块来简化上下文管理器的编写。](https://so.csdn.net/so/search/s.do?q=Python3&t=blog)](https://so.csdn.net/so/search/s.do?q=Python&t=blog)





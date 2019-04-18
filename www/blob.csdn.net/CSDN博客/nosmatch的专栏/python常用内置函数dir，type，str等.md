# python常用内置函数dir，type，str等 - nosmatch的专栏 - CSDN博客
2011年08月15日 11:12:07[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：3823
Python有小部分相当有用的内置函数。除这些函数之外，其它所有的函数都被分到了各个模块中。其实这是个非常明智的设计策略，避免了核心语言变得像其它脚本语言一样臃肿。
1 type函数
type函数返回任意对象的数据类型。在types模块中列出了可能的数据类型，这对于处理多种数据类型的帮助者函数非常有用。
>>> type(1)          
<type 'int'>
>>> li = []
>>> type(li)          
<type 'list'>
>>> import odbchelper
>>> type(odbchelper) 
<type 'module'>
>>> import types      
>>> type(odbchelper) == types.ModuleType
True
type可以接收任何东西作为参数，并返回它的数据类型。整型、字符串、列表、字典、元组、函数、类、模块，甚至类型对象都可以作为参数被type函数接受。type可以接收变量作为参数，并返回它的数据类型。type还可以作用于模块。你可以使用types模块中的变量来进行对象类型的比较。这就是info函数所做的。
2 str函数
str将数据强制转换为字符串。每种数据类型都可以强制转换为字符串。
>>> str(1)          
'1'
>>> horsemen = ['war', 'pestilence', 'famine']
>>> horsemen
['war', 'pestilence', 'famine']
>>> horsemen.append('Powerbuilder')
>>> str(horsemen)   
"['war', 'pestilence', 'famine', 'Powerbuilder']"
>>> str(odbchelper) 
"<module 'odbchelper' from 'c:\\docbook\\dip\\py\\odbchelper.py'>"
>>> str(None)       
'None'
对于简单的数据类型比如整型，你可以预料到str的正常工作，因为几乎每种语言都有一个将整型转化为
字符串的函数，然而str可以作用于任何数据类型的任何对象，这里作用于列表。str还允许作用于模块。注意模块的字符串形式包含了模块在磁盘上的路径名。str的一个细小但重要的行为是它可以作用于None，None是Python的Null值。这个调用返回字符串None。可以使用这一点来改进info函数。
3 dir函数
info函数的核心是强大的dir函数。dir函数返回任意对象的属性和方法列表，包括模块对象、函数对象、字符串对象、列表对象、字典对象。。。等。
>>> li = []
>>> dir(li)           
['append', 'count', 'extend', 'index', 'insert',
'pop', 'remove', 'reverse', 'sort']
>>> d = {}
>>> dir(d)            
['clear', 'copy', 'get', 'has_key', 'items', 'keys', 'setdefault', 'update', 'values']
>>> import odbchelper
>>> dir(odbchelper)   
['__builtins__', '__doc__', '__file__', '__name__', 'buildConnectionString']
li是一个列表，所以dir(li)返回一个包含所有列表方法的列表。注意返回的列表只包含了字符串形式的方法名称，而不是方法对象本身。d是一个字典，所以dir(d)返回字典方法的名称列表。最后是 callable 函数，它接收任何对象作为参数，如果参数对象是可调用的，返回 True；否则返回 False。可调用对象包括函数、类方法，甚至类自身 (下一章将更多的关注类)。
callable介绍
>>> import string
>>> string.punctuation           
'!"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~'
>>> string.join                  <function join at 00C55A7C>
>>> callable(string.punctuation) False
>>> callable(string.join)        True
>>> print string.join.__doc__    join(list [,sep]) -> string
    Return a string composed of the words in list, with
    intervening occurrences of sep. The default separator is a
    single space.
    (joinfields and join are synonymous)
string 模块中的函数现在已经不赞成使用了 (尽管很多人现在仍然还在使用 join 函数)，但是在这个模块中包含了许多有用的变量，例如 string.punctuation，这个字符串包含了所有标准的标点符号字符。string.join 是一个用于连接字符串列表的函数。string.punctuation 是不可调用的对象；它是一个字符串。(字符串确有可调用的方法，但是字符串本身不是可调用的。) string.join 是可调用的；这个函数可以接受两个参数。任何可调用的对象都有 doc string。通过将
 callable 函数作用于一个对象的每个属性，可以确定哪些属性 (方法、函数、类) 是你要关注的，哪些属性 (常量等等) 是你可以忽略、之前不需要知道的。 
4 内置函数 
type、str、dir 和其它的 Python 内置函数都归组到了 __builtin__ (前后分别是双下划线) 这个特殊的模块中。如果有帮助的话，你可以认为 Python 在启动时自动执行了 from __builtin__ import *，此语句将所有的 “内置” 函数导入该命名空间，所以在这个命名空间中可以直接使用这些内置函数。
像这样考虑的好处是，你是可以获取 __builtin__ 模块信息的，并以组的形式访问所有的内置函数和属性。猜到什么了吗，现在我们的 Python 有一个称为 info 的函数。自己尝试一下，略看一下结果列表。后面我们将深入到一些更重要的函数。(一些内置的错误类，比如 AttributeError，应该看上去已经很熟悉了。)
内置属性和内置函数
>>> from apihelper import info
>>> import __builtin__
>>> info(__builtin__, 20)
ArithmeticError      Base class for arithmetic errors.
AssertionError       Assertion failed.
AttributeError       Attribute not found.
EOFError             Read beyond end of file.
EnvironmentError     Base class for I/O related errors.
Exception            Common base class for all exceptions.
FloatingPointError   Floating point operation failed.
IOError              I/O operation failed.
[...snip...]
Python 提供了很多出色的参考手册，你应该好好地精读一下所有 Python 提供的必备模块。对于其它大部分语言，你会发现自己要常常回头参考手册或者 man 页来提醒自己如何使用这些模块，但是 Python 不同于此，它很大程度上是自文档化的。

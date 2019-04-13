
# Python 学习入门（33）—— 内置函数 - 阳光岛主 - CSDN博客

2013年12月21日 23:09:40[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：5444所属专栏：[Python 学习入门](https://blog.csdn.net/column/details/python-learning.html)



python 的内置函数可以直接调用，无需import，常用到的有：
int（）将字符串或者小数转换成整数
str（）将其他类型转换是字符型
len（X）返回X的长度。The argument may be a sequence (string, tuple or list) or a mapping (dictionary).
print（）输出
type（X）返回X的数据类型
open（f）打开一个文件f并返回文件类型的对象，和file（）相似。
在python2.7.2 doc中可以查到每个函数的详细用法：[function](http://docs.python.org/library/functions.html)
|Built-in Functions|
|---|---|---|---|---|
|abs()|divmod()|input()|open()|staticmethod()|
|all()|enumerate()|int()|ord()|str()|
|any()|eval()|isinstance()|pow()|sum()|
|basestring()|execfile()|issubclass()|print()|super()|
|bin()|file()|iter()|property()|tuple()|
|bool()|filter()|len()|range()|type()|
|bytearray()|float()|list()|raw_input()|unichr()|
|callable()|format()|locals()|reduce()|unicode()|
|chr()|frozenset()|long()|reload()|vars()|
|classmethod()|getattr()|map()|repr()|xrange()|
|cmp()|globals()|max()|reversed()|zip()|
|compile()|hasattr()|memoryview()|round()|__import__()|
|complex()|hash()|min()|set()|apply()|
|delattr()|help()|next()|setattr()|buffer()|
|dict()|hex()|object()|slice()|coerce()|
|dir()|id()|oct()|sorted()|[intern()](http://docs.python.org/2/library/functions.html#intern)
|
想要查所有内置函数名可以在python命令行方式中如下输入
>>> dir(__builtins__)
['ArithmeticError', 'AssertionError', 'AttributeError', 'BaseException', 'BufferError', 'BytesWarning', 'DeprecationWarning', 'EOFError', 'Ellipsis', 'EnvironmentError', 'Exception', 'False', 'FloatingPointError', 'FutureWarning', 'GeneratorExit', 'IOError', 'ImportError', 'ImportWarning', 'IndentationError', 'IndexError', 'KeyError', 'KeyboardInterrupt', 'LookupError', 'MemoryError', 'NameError', 'None', 'NotImplemented', 'NotImplementedError', 'OSError', 'OverflowError', 'PendingDeprecationWarning', 'ReferenceError', 'RuntimeError', 'RuntimeWarning', 'StandardError', 'StopIteration', 'SyntaxError', 'SyntaxWarning', 'SystemError', 'SystemExit', 'TabError', 'True', 'TypeError', 'UnboundLocalError', 'UnicodeDecodeError', 'UnicodeEncodeError', 'UnicodeError', 'UnicodeTranslateError', 'UnicodeWarning', 'UserWarning', 'ValueError', 'Warning', 'ZeroDivisionError', '_', '__debug__', '__doc__', '__import__', '__name__', '__package__', 'abs', 'all', 'any', 'apply', 'basestring', 'bin', 'bool', 'buffer', 'bytearray', 'bytes', 'callable', 'chr', 'classmethod', 'cmp', 'coerce', 'compile', 'complex', 'copyright', 'credits', 'delattr', 'dict', 'dir', 'divmod', 'enumerate', 'eval', 'execfile', 'exit', 'file', 'filter', 'float', 'format', 'frozenset', 'getattr', 'globals', 'hasattr', 'hash', 'help', 'hex', 'id', 'input', 'int', 'intern', 'isinstance', 'issubclass', 'iter', 'len', 'license', 'list', 'locals', 'long', 'map', 'max', 'memoryview', 'min', 'next', 'object', 'oct', 'open', 'ord', 'pow', 'print', 'property', 'quit', 'range', 'raw_input', 'reduce', 'reload', 'repr', 'reversed', 'round', 'set', 'setattr', 'slice', 'sorted', 'staticmethod', 'str', 'sum', 'super', 'tuple', 'type', 'unichr', 'unicode', 'vars', 'xrange', 'zip']


**参考推荐：**
[Built-in Functions](http://docs.python.org/2/library/functions.html)（官方）
[python的内置函数（built-in functions）](http://blog.sina.com.cn/s/blog_4d14fb2b0100wnoz.html)
[Python内置函数清单](http://www.cnblogs.com/vamei/archive/2012/11/09/2762224.html)




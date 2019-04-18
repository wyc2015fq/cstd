# Python入门--18--异常与try，except语句 - 默槑 - 博客园







# [Python入门--18--异常与try，except语句](https://www.cnblogs.com/modaidai/p/7290780.html)





Python标准异常总结




|AssertionError|断言语句（assert）失败|
|----|----|
|AttributeError|尝试访问未知的对象属性|
|EOFError|用户输入文件末尾标志EOF（Ctrl+d）|
|FloatingPointError|浮点计算错误|
|GeneratorExit|generator.close()方法被调用的时候|
|ImportError|导入模块失败的时候|
|IndexError|索引超出序列的范围|
|KeyError|字典中查找一个不存在的关键字|
|KeyboardInterrupt|用户输入中断键（Ctrl+c）|
|MemoryError|内存溢出（可通过删除对象释放内存）|
|NameError|尝试访问一个不存在的变量|
|NotImplementedError|尚未实现的方法|
|OSError|操作系统产生的异常（例如打开一个不存在的文件）|
|OverflowError|数值运算超出最大限制|
|ReferenceError|弱引用（weak reference）试图访问一个已经被垃圾回收机制回收了的对象|
|RuntimeError|一般的运行时错误|
|StopIteration|迭代器没有更多的值|
|SyntaxError|Python的语法错误|
|IndentationError|缩进错误|
|TabError|Tab和空格混合使用|
|SystemError|Python编译器系统错误|
|SystemExit|Python编译器进程被关闭|
|TypeError|不同类型间的无效操作|
|UnboundLocalError|访问一个未初始化的本地变量（NameError的子类）|
|UnicodeError|Unicode相关的错误（ValueError的子类）|
|UnicodeEncodeError|Unicode编码时的错误（UnicodeError的子类）|
|UnicodeDecodeError|Unicode解码时的错误（UnicodeError的子类）|
|UnicodeTranslateError|Unicode转换时的错误（UnicodeError的子类）|
|ValueError|传入无效的参数|
|ZeroDivisionError|除数为零|



以下是 Python 内置异常类的层次结构：

BaseException
+-- SystemExit
+-- KeyboardInterrupt
+-- GeneratorExit
+-- Exception
      +-- StopIteration
      +-- ArithmeticError
      |    +-- FloatingPointError
      |    +-- OverflowError
      |    +-- ZeroDivisionError
      +-- AssertionError
      +-- AttributeError
      +-- BufferError
      +-- EOFError
      +-- ImportError
      +-- LookupError
      |    +-- IndexError
      |    +-- KeyError
      +-- MemoryError
      +-- NameError
      |    +-- UnboundLocalError
      +-- OSError
      |    +-- BlockingIOError
      |    +-- ChildProcessError
      |    +-- ConnectionError
      |    |    +-- BrokenPipeError
      |    |    +-- ConnectionAbortedError
      |    |    +-- ConnectionRefusedError
      |    |    +-- ConnectionResetError
      |    +-- FileExistsError
      |    +-- FileNotFoundError
      |    +-- InterruptedError
      |    +-- IsADirectoryError
      |    +-- NotADirectoryError
      |    +-- PermissionError
      |    +-- ProcessLookupError
      |    +-- TimeoutError
      +-- ReferenceError
      +-- RuntimeError
      |    +-- NotImplementedError
      +-- SyntaxError
      |    +-- IndentationError
      |         +-- TabError
      +-- SystemError
      +-- TypeError
      +-- ValueError
      |    +-- UnicodeError
      |         +-- UnicodeDecodeError
      |         +-- UnicodeEncodeError
      |         +-- UnicodeTranslateError
      +-- Warning
           +-- DeprecationWarning
           +-- PendingDeprecationWarning
           +-- RuntimeWarning
           +-- SyntaxWarning
           +-- UserWarning
           +-- FutureWarning
           +-- ImportWarning
           +-- UnicodeWarning
           +-- BytesWarning
           +-- ResourceWarning



配合try：

　　　　operation

　　except exception[as result]:

　　　　print('出错了')

举一些栗子和用法：



try:

　　f = open('我为什么是一个文件.txt')

　　print(f.read())

　　f.close()
except OSError:

　　print('文件出错啦！')　　　　　　　　　　　　#运行一下，如果前面出现的是OSError的错误，这里就会输出这段话



try:

　　f = open('我为什么是一个文件.txt')

　　print(f.read())

　　f.close()
except OSError as reult:

　　print('文件出错啦！原因是：'+str(result))　　　#运行一下，如果前面出现的是OSError的错误，这里就会输出这段话加上报错的信息



try语句也可以连接多个except异常：

try:

　　sum =1+'1'

　　f = open('我为什么是一个文件.txt')

　　print(f.read())

　　f.close()
except OSError as reult:

　　print('文件出错啦！原因是：'+str(result))　　　#运行一下，如果前面出现的是OSError的错误，这里就会输出这段话加上报错的信息

except TypeError as result:

　　print('类型出错了') 



try语句加上finally：

try:

　　sum =1+'1'

　　f = open('我为什么是一个文件.txt')

　　print(f.read())

except OSError as reult:

　　print('文件出错啦！原因是：'+str(result))　　　#运行一下，如果前面出现的是OSError的错误，这里就会输出这段话加上报错的信息

finally：

　　f.close()                                                             #finally语句是无论报不报错都要执行的语句













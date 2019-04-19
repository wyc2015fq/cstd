# Python3 简介 - Simple 专栏 - CSDN博客
2018年10月04日 18:39:03[Simple_Zz](https://me.csdn.net/love284969214)阅读数：69标签：[Python 3.x](https://so.csdn.net/so/search/s.do?q=Python 3.x&t=blog)
个人分类：[Python 3.x](https://blog.csdn.net/love284969214/article/category/8102121)
### Python简介
Python是一种高层次，解释，互动性和面向对象的脚本语言。[Python](http://www.yiibai.com/python/) 被设计成具有很强的可读性语言。它采用英语关键字，而其他语言一般使用标点符号，并且具有比其他语言有较少的句法结构。
- **Python是解释型的语言：**Python 是在运行时被解释处理。你不需要在执行前编译程序。这类似于Perl和PHP。
- **Python是交互式的：** 实际上，可以在 Python 提示和解释直接交互来编写程序。
- **Python是面向对象：**Python支持面向对象式或编程的对象中封装代码的技术。
- **Python是一种初学者的语言：**Python是初学者级别程序员伟大的语言，并支持广泛从简单的文字处理，网页浏览器应用程序到游戏的开发。
### Python历史
- Python是由Guido van Rossum 于八十年代末和90年代初在美国国家研究所数学与计算机科学在荷兰开发的。
- Python是从许多其他语言，包括ABC，Modula-3语言，C，C++，Algol-68，Smalltalk和Unix Shell和其它脚本语言参考借荐开发的。
- Python是有版权的。类似于Perl，Python源代码现在是在GNU通用公共许可证（GPL）下提供。
- Python现在是由核心开发团队在研究所维护，虽然Guido van Rossum 仍持有指挥它的进展至关重要的作用。
- Python 1.0发布于1994年11月。在2000年，Python 2.0发布。Python 2.7.11是Python2 中的最新版本。
- 同时，Python 3.0发布于2008年。 Python3 不与落后的 Python2 兼容。Python3 的重点是一直在删除重复编程结构和模块，使“应该有一个 - 最好只有一个 - 明显的方法来做到这一点。”Python 3.5.1是 Python3 的最新版本。
### Python的特点
**Python的功能包括：**
- **易于学习：**Python有几个关键字，结构简单，明确定义的语法。这让初学者可以快速上手语言。
- **易于阅读：** Python代码更加明确和看得见。
- **易于维护：**Python源代码是相当容易维护的。
- **广泛的标准库：**Python的库是非常便携的，在UNIX，Windows和Macintosh跨平台兼容。
- **交互式模式：**Python有一个交互动的模式，可支持互动测试和代码段的调试。
- **便携：**Python可以在多种硬件平台上运行，并且对所有平台相同的接口。
- **可扩展****:** 可以添加低级别的模块到Python解释器。这些模块使程序员能够更有效地添加或定制自己的工具。
- **数据库:** Python提供了接口，支持所有主要的商业数据库。
- **GUI编程:** Python支持，可以创建并移植到许多系统调用GUI应用程序， 库和Windows系统，如：Windows MFC，Macintosh和Unix X窗口系统。
- **可扩展****:** 对于大型程序，Python提供比shell脚本更加合理的结构和支持。
**除了上面提到的功能，Python还有很多的功能列表，这里列举一些如下：**
- 它支持函数和结构化以及OOP程序设计方法。
- 它可以被用来作为一种脚本语言或可以被编译成字节码用于构建大的应用程序。
- 它提供了非常高的水平动态数据类型和支持动态类型检查。
- 它支持自动垃圾收集。
- 它可与C，C ++，COM和ActiveX，CORBA和Java轻松集成。
# Python3 引入新特性
### __future__ 模块
Python 3.x 引入一些 Python2 不兼容的关键字和函数，可以通过在 Python2 内置的模块 __future__ 导入。建议如果你想在代码中支持 Python3.x，使用__future__导入它。
例如，如果想在 Python2 中拥有 Python 3.x 整数的除法行为，添加下面的 import 语句
```python
from __future__ import division
```
### print函数
在 Python3 最值得注意和最广为人知的变化是print函数的使用。print 函数使用的括号()在Python3中是强制性的。它在 Python2 中是可选的。
```python
print "Hello World" #is acceptable in Python 2
print ("Hello World") # in Python 3, print must be followed by ()
```
print()函数默认情况下在结束时会插入一个换行。在 Python2，它可以通过 '，' 在末行抑制输出换行。 在 Python3 则使用"end=' '" 附加空格，而不是换行。
```python
print x,           # Trailing comma suppresses newline in Python 2
print(x, end=" ")  # Appends a space instead of a newline in Python 3
```
### 从键盘读取输入
Python2 中有输入函数两个版本。 input() 和 raw_input()。如果它被包含在引号 '' 或 ""，input() 对待接收到的数据作为字符串，否则数据将被视为数字类型。
在 Python3 中 raw_input()函数已被弃用。此外，接收到的输入数据总是作为字符串处理。
```python
In Python 2 >>> x=input('something:') 
something:10 #entered data is treated as number
>>> x
10
>>> x=input('something:')
something:'10' #eentered data is treated as string
>>> x
'10'
>>> x=raw_input("something:")
something:10 #entered data is treated as string even without ''
>>> x
'10'
>>> x=raw_input("something:")
something:'10' #entered data treated as string including ''
>>> x
"'10'" In Python 3 >>> x=input("something:")
something:10
>>> x
'10'
>>> x=input("something:")
something:'10' #entered data treated as string with or without ''
>>> x
"'10'"
>>> x=raw_input("something:") # will result NameError
Traceback (most recent call last):
  File "", line 1, in  x=raw_input("something:")
NameError: name 'raw_input' is not defined
```
### 整数除法
在Python2，两个整数的除法的结果会四舍五入到最接近的整数。如：3/2 其结果将显示 1。 为了获得一个浮点除法，分子或分母必须明确为浮点数。因此无论是 3.0/2 或 3/2.0 或 3.0/2.0 将产生1.5 。
Python3 计算 3/2 默认结果值为 1.5，这对新手程序员更加直观。
### Unicode表示
Python2 里如果你想将它保存为 Unicode，需要标记为 U 的字符串。
Python3 中的字符串默认存储为 Unicode。在Python3，我们有个Unicode(UTF-8)字符串和 2 字节类：字节和字节数组。
### xrange() 函数已被删除
在 Python2 的 range() 函数返回一个列表，还有 xrange()返回一个对象只会在需要时在范围内产生所需项目以节省内存。
在Python3，range()函数去除了，而 xrange()已更名为 range()。 另外在 Python3.2 以及更高的版本中， range()对象支持切片。
### 引发异常
Python2 中同时接受符号的'大胆'和'新'的语法；如果我们不在括号中括入异常参数，Python3 中会引发一个 SyntaxError：
```python
raise IOError, "file error" #This is accepted in Python 2
raise IOError("file error") #This is also accepted in Python 2
raise IOError, "file error" #syntax error is raised in Python 3
raise IOError("file error") #this is the recommended syntax in Python 3
```
### 异常的参数
在 Python3，异常参数应以 'as' 关键字来声明。
```python
except Myerror, err: # In Python2
except Myerror as err: #In Python 3
```
## next() 函数和.next()方法
在Python 2，next() 作为生成器对象的一个方法是允许的。在 Python2，next()函数过度产生器对象遍历也是可以接受的。在Python3，但是，next()函数作为生成器方法来中止并引发AttributeError。
```python
gen = (letter for letter in 'Hello World') # creates generator object
next(my_generator) #allowed in Python 2 and Python 3
my_generator.next() #allowed in Python 2. raises AttributeError in Python 3
```
## 2to3实用工具
随着 Python3 解释器，2t03.py 脚本将被通常安装在  tools/scripts 文件夹。 它读取 Python2.x 源代码，并应用了一系列的修复将它转变成有效的 Python3.x 代码。
```python
Here is a sample Python 2 code (area.py):
def area(x,y=3.14): 
    a=y*x*x
    print a
    return a
a=area(10)
print "area",a
To convert into Python 3 version:
$2to3 -w area.py
Converted code :
def area(x,y=3.14): # formal parameters
    a=y*x*x
    print (a)
    return a
a=area(10)
print("area",a)
```

# Python内置函数(39)——help - lincappu - 博客园







# [Python内置函数(39)——help](https://www.cnblogs.com/lincappu/p/8144984.html)





**英文文档：**

`help`([*object*])

Invoke the built-in help system. (This function is intended for interactive use.) If no argument is given, the interactive help system starts on the interpreter console. If the argument is a string, then the string is looked up as the name of a module, function, class, method, keyword, or documentation topic, and a help page is printed on the console. If the argument is any other kind of object, a help page on the object is generated.

This function is added to the built-in namespace by the `site` module.

　　返回对象的帮助信息 

**说明：**

　　1. 在解释器交互界面，不传参数调用函数时，将激活内置的帮助系统，并进入帮助系统。在帮助系统内部输入模块、类、函数等名称时，将显示其使用说明，输入quit退出内置帮助系统，并返回交互界面。

```
>>> help() #不带参数

Welcome to Python 3.5's help utility!

If this is your first time using Python, you should definitely check out
the tutorial on the Internet at http://docs.python.org/3.5/tutorial/.

Enter the name of any module, keyword, or topic to get help on writing
Python programs and using Python modules.  To quit this help utility and
return to the interpreter, just type "quit".

To get a list of available modules, keywords, symbols, or topics, type
"modules", "keywords", "symbols", or "topics".  Each module also comes
with a one-line summary of what it does; to list the modules whose name
or summary contain a given string such as "spam", type "modules spam".

#进入内置帮助系统  >>> 变成了 help>
help> str #str的帮助信息
Help on class str in module builtins:

class str(object)
 |  str(object='') -> str
 |  str(bytes_or_buffer[, encoding[, errors]]) -> str
 |  
 |  Create a new string object from the given object. If encoding or
 |  errors is specified, then the object must expose a data buffer
 |  that will be decoded using the given encoding and error handler.
 |  Otherwise, returns the result of object.__str__() (if defined)
 |  or repr(object).
 |  encoding defaults to sys.getdefaultencoding().
 |  errors defaults to 'strict'.
 |  
 |  Methods defined here:
 |  
 |  __add__(self, value, /)
 |      Return self+value.
 ................................


help> 1 #不存在的模块名、类名、函数名
No Python documentation found for '1'.
Use help() to get the interactive help utility.
Use help(str) for help on the str class.

help> quit #退出内置帮助系统

You are now leaving help and returning to the Python interpreter.
If you want to ask for help on a particular object directly from the
interpreter, you can type "help(object)".  Executing "help('string')"
has the same effect as typing a particular string at the help> prompt.

# 已退出内置帮助系统，返回交互界面 help> 变成 >>>
>>>
```

　　2. 在解释器交互界面，传入参数调用函数时，将查找参数是否是模块名、类名、函数名，如果是将显示其使用说明。

```
>>> help(str) 
Help on class str in module builtins:

class str(object)
 |  str(object='') -> str
 |  str(bytes_or_buffer[, encoding[, errors]]) -> str
 |  
 |  Create a new string object from the given object. If encoding or
 |  errors is specified, then the object must expose a data buffer
 |  that will be decoded using the given encoding and error handler.
 |  Otherwise, returns the result of object.__str__() (if defined)
 |  or repr(object).
 |  encoding defaults to sys.getdefaultencoding().
 |  errors defaults to 'strict'.
 |  
 |  Methods defined here:
 |  
 |  __add__(self, value, /)
 |      Return self+value.
 |  
  ***************************
```













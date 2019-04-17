# Python内置函数(47)——vars - lincappu - 博客园







# [Python内置函数(47)——vars](https://www.cnblogs.com/lincappu/p/8145088.html)





**英文文档：**
- `vars`([*object*])
- 
- 
- Return the `__dict__` attribute for a module, class, instance, or any other object with a `__dict__` attribute.
- Objects such as modules and instances have an updateable `__dict__` attribute; however, other objects may have write restrictions on their `__dict__` attributes (for example, classes use a dictproxy to prevent direct dictionary updates).
- Without an argument, `vars()` acts like `locals()`. Note, the locals dictionary is only useful for reads since updates to the locals dictionary are ignored.
- 

**　　返回当前作用域内的局部变量和其值组成的字典，或者返回对象的属性列表**

**说明**

　　1. 当函数不接收参数时，其功能和locals函数一样，返回当前作用域内的局部变量。

```
#不带参数功能和locals函数一样
>>> v1 = vars()
>>> l1 = locals()
>>> v1
{'__name__': '__main__', '__builtins__': <module 'builtins' (built-in)>, 'v1': {...}, 'l1': {...}, '__spec__': None, '__doc__': None, '__package__': None, '__loader__': <class '_frozen_importlib.BuiltinImporter'>}
>>> l1
{'__name__': '__main__', '__builtins__': <module 'builtins' (built-in)>, 'v1': {...}, 'l1': {...}, '__spec__': None, '__doc__': None, '__package__': None, '__loader__': <class '_frozen_importlib.BuiltinImporter'>}
```

　　2. 当函数接收一个参数时，参数可以是模块、类、类实例，或者定义了__dict__属性的对象。

```
#作用于模块
>>> import time
>>> vars(time)
{'gmtime': <built-in function gmtime>, 'tzname': ('ÖÐ¹ú±ê×¼Ê±¼ä', 'ÖÐ¹úÏÄÁîÊ±'), 'timezone': -28800, 'struct_time': <class 'time.struct_time'>, 'ctime': <built-in function ctime>, 'perf_counter': <built-in function perf_counter>, 'mktime': <built-in function mktime>, 'localtime': <built-in function localtime>, 'time': <built-in function time>, '__package__': '', 'altzone': -32400, 'clock': <built-in function clock>, 'strptime': <built-in function strptime>, 'monotonic': <built-in function monotonic>, '__loader__': <class '_frozen_importlib.BuiltinImporter'>, 'get_clock_info': <built-in function get_clock_info>, 'sleep': <built-in function sleep>, 'process_time': <built-in function process_time>, '__name__': 'time', '_STRUCT_TM_ITEMS': 9, '__spec__': ModuleSpec(name='time', loader=<class '_frozen_importlib.BuiltinImporter'>, origin='built-in'), '__doc__': 'This module provides various functions to manipulate time values.\n\nThere are two standard representations of time.  One is the number\nof seconds since the Epoch, in UTC (a.k.a. GMT).  It may be an integer\nor a floating point number (to represent fractions of seconds).\nThe Epoch is system-defined; on Unix, it is generally January 1st, 1970.\nThe actual value can be retrieved by calling gmtime(0).\n\nThe other representation is a tuple of 9 integers giving local time.\nThe tuple items are:\n  year (including century, e.g. 1998)\n  month (1-12)\n  day (1-31)\n  hours (0-23)\n  minutes (0-59)\n  seconds (0-59)\n  weekday (0-6, Monday is 0)\n  Julian day (day in the year, 1-366)\n  DST (Daylight Savings Time) flag (-1, 0 or 1)\nIf the DST flag is 0, the time is given in the regular time zone;\nif it is 1, the time is given in the DST time zone;\nif it is -1, mktime() should guess based on the date and time.\n\nVariables:\n\ntimezone -- difference in seconds between UTC and local standard time\naltzone -- difference in  seconds between UTC and local DST time\ndaylight -- whether local time should reflect DST\ntzname -- tuple of (standard time zone name, DST time zone name)\n\nFunctions:\n\ntime() -- return current time in seconds since the Epoch as a float\nclock() -- return CPU time since process start as a float\nsleep() -- delay for a number of seconds given as a float\ngmtime() -- convert seconds since Epoch to UTC tuple\nlocaltime() -- convert seconds since Epoch to local time tuple\nasctime() -- convert time tuple to string\nctime() -- convert time in seconds to string\nmktime() -- convert local time tuple to seconds since Epoch\nstrftime() -- convert time tuple to string according to format specification\nstrptime() -- parse string to time tuple according to format specification\ntzset() -- change the local timezone', 'strftime': <built-in function strftime>, 'asctime': <built-in function asctime>, 'daylight': 0}

#作用于类
>>> vars(slice)
mappingproxy({'__ne__': <slot wrapper '__ne__' of 'slice' objects>, '__getattribute__': <slot wrapper '__getattribute__' of 'slice' objects>, '__reduce__': <method '__reduce__' of 'slice' objects>, 'start': <member 'start' of 'slice' objects>, 'indices': <method 'indices' of 'slice' objects>, '__ge__': <slot wrapper '__ge__' of 'slice' objects>, 'stop': <member 'stop' of 'slice' objects>, '__eq__': <slot wrapper '__eq__' of 'slice' objects>, 'step': <member 'step' of 'slice' objects>, '__hash__': None, '__doc__': 'slice(stop)\nslice(start, stop[, step])\n\nCreate a slice object.  This is used for extended slicing (e.g. a[0:10:2]).', '__repr__': <slot wrapper '__repr__' of 'slice' objects>, '__le__': <slot wrapper '__le__' of 'slice' objects>, '__gt__': <slot wrapper '__gt__' of 'slice' objects>, '__new__': <built-in method __new__ of type object at 0x6A91B420>, '__lt__': <slot wrapper '__lt__' of 'slice' objects>})

#作用于类实例
>>> class A(object):
    pass

>>> a.__dict__
{}
>>> vars(a)
{}
>>> a.name = 'Kim'
>>> a.__dict__
{'name': 'Kim'}
>>> vars(a)
{'name': 'Kim'}
```













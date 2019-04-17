# Python内置函数(55)——globals - lincappu - 博客园







# [Python内置函数(55)——globals](https://www.cnblogs.com/lincappu/p/8145196.html)





**英文文档：**
- `globals`()
- 
- Return a dictionary representing the current global symbol table. This is always the dictionary of the current module (inside a function or method, this is the module where it is defined, not the module from which it is called).
- 
- 
- 
- 
- **　　返回当前作用域内全局变量和其组成的字典**
- **说明：**
- 
- 
- 
- 　　1. 返回当前作用域内全局变量的字典。
- 

```
>>> globals()
{'__spec__': None, '__package__': None, '__builtins__': <module 'builtins' (built-in)>, '__name__': '__main__', '__doc__': None, '__loader__': <class '_frozen_importlib.BuiltinImporter'>}
>>> a = 1
>>> globals() #多了一个a
{'__spec__': None, '__package__': None, '__builtins__': <module 'builtins' (built-in)>, 'a': 1, '__name__': '__main__', '__doc__': None, '__loader__': <class '_frozen_importlib.BuiltinImporter'>}
```













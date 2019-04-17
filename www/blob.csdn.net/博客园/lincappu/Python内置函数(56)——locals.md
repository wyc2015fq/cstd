# Python内置函数(56)——locals - lincappu - 博客园







# [Python内置函数(56)——locals](https://www.cnblogs.com/lincappu/p/8145204.html)





** 英文文档：**

`locals`()

Update and return a dictionary representing the current local symbol table. Free variables are returned by `locals()`when it is called in function blocks, but not in class blocks.

 　　返回当前作用域内的局部变量和其值组成的字典

**说明：**

　　1. 函数功能返回当前作用域内的局部变量和其值组成的字典，与globals函数类似(返回全局变量)

```
>>> locals()
{'__package__': None, '__loader__': <class '_frozen_importlib.BuiltinImporter'>, '__doc__': None, '__name__': '__main__', '__builtins__': <module 'builtins' (built-in)>, '__spec__': None}

>>> a = 1

>>> locals() # 多了一个key为a值为1的项
{'__package__': None, '__loader__': <class '_frozen_importlib.BuiltinImporter'>, 'a': 1, '__doc__': None, '__name__': '__main__', '__builtins__': <module 'builtins' (built-in)>, '__spec__': None}
```

　　2. 可用于函数内。

```
>>> def f():
    print('before define a ')
    print(locals()) #作用域内无变量
    a = 1
    print('after define a')
    print(locals()) #作用域内有一个a变量，值为1

    
>>> f
<function f at 0x03D40588>
>>> f()
before define a 
{} 
after define a
{'a': 1}
```

　　3. 返回的字典集合不能修改。

```
>>> def f():
    print('before define a ')
    print(locals()) # 作用域内无变量
    a = 1
    print('after define a')
    print(locals()) # 作用域内有一个a变量，值为1
    b = locals()
    print('b["a"]: ',b['a']) 
    b['a'] = 2 # 修改b['a']值
    print('change locals value')
    print('b["a"]: ',b['a'])
    print('a is ',a) # a的值未变

    
>>> f()
before define a 
{}
after define a
{'a': 1}
b["a"]:  1
change locals value
b["a"]:  2
a is  1
>>>
```













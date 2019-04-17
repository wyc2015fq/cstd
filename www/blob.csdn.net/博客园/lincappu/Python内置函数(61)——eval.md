# Python内置函数(61)——eval - lincappu - 博客园







# [Python内置函数(61)——eval](https://www.cnblogs.com/lincappu/p/8145344.html)





**英文文档：**
- `eval`(*expression*, *globals=None*, *locals=None*)
- 
- The arguments are a string and optional globals and locals. If provided, *globals* must be a dictionary. If provided, *locals*can be any mapping object.
- 
- The *expression* argument is parsed and evaluated as a Python expression (technically speaking, a condition list) using the *globals* and *locals* dictionaries as global and local namespace. If the *globals* dictionary is present and lacks ‘__builtins__’, the current globals are copied into *globals* before *expression* is parsed. This means that *expression*normally has full access to the standard `builtins` module and restricted environments are propagated. If the *locals*dictionary is omitted it defaults to the *globals* dictionary. If both dictionaries are omitted, the expression is executed in the environment where `eval()` is called. The return value is the result of the evaluated expression. Syntax errors are reported as exceptions. Example:
- 

```
>>> x = 1
>>> eval('x+1')
2
```

This function can also be used to execute arbitrary code objects (such as those created by `compile()`). In this case pass a code object instead of a string. If the code object has been compiled with `'exec'` as the *mode* argument, `eval()`‘s return value will be `None`.
- 
- Hints: dynamic execution of statements is supported by the `exec()` function. The `globals()` and `locals()` functions returns the current global and local dictionary, respectively, which may be useful to pass around for use by `eval()` or `exec()`.
- 
- See `ast.literal_eval()` for a function that can safely evaluate strings with expressions containing only literals.
- 
- 
- 
- 
- **　　执行动态标表达式求值**
- **说明：**
- 
- 　　1. 执行动态语句，返回语句执行的值。
- 

```
>>> eval('1+2+3+4')
10
```

　　2. 第一个参数为语句字符串，globals参数和locals参数为可选参数，如果提供，globals参数必需是字典，locals参数为mapping对象。

　　3. globals参数用来指定代码执行时可以使用的全局变量以及收集代码执行后的全局变量。

```
>>> g = {'num':2}

>>> eval('num + 2') #num未定义
Traceback (most recent call last):
  File "<pyshell#3>", line 1, in <module>
    eval('num + 2')
  File "<string>", line 1, in <module>
NameError: name 'num' is not defined

>>> eval('num + 2',g) #g中有定义num，可执行
4
```

 　　4. locals参数用来指定代码执行时可以使用的局部变量以及收集代码执行后的局部变量
- 

```
>>> g = {'num1':2}
>>> l = {'num2':4}
>>> eval('num1+num2',g,l)
6
```

 　　5. 为了保证代码成功运行，globals参数字典不包含 __builtins__ 这个 key 时，Python会自动添加一个key为 __builtins__ ，value为builtins模块的引用。如果确实要限制代码不使用builtins模块，需要在global添加一个key为__builtins__，value为{}的项即可（很少有人这么干吧）。

```
>>> g = {}
>>> eval('abs(-1)',g)
1

>>> g = {'__builtins__':{}}
>>> eval('abs(-1)',g) #不能使用内置函数了
Traceback (most recent call last):
  File "<pyshell#9>", line 1, in <module>
    eval('abs(-1)',g)
  File "<string>", line 1, in <module>
NameError: name 'abs' is not defined
```

 　　6. 当globals参数不提供是，Python默认使用globals()函数返回的字典去调用。当locals参数不提供时，默认使用globals参数去调用。

```
>>> num = 1
>>> eval('num+2')
3

>>> globals() #返回字典中含有num的key
{'__doc__': None, 'num': 1, '__package__': None, '__name__': '__main__', '__loader__': <class '_frozen_importlib.BuiltinImporter'>, '__spec__': None, '__builtins__': <module 'builtins' (built-in)>}

>>> eval('num+2',{}) #locals参数未提供，locals参数=globals参数
Traceback (most recent call last):
  File "<pyshell#3>", line 1, in <module>
    eval('num+2',{})
  File "<string>", line 1, in <module>
NameError: name 'num' is not defined

>>> l = locals() 
>>> eval('num+2',{},l) #locals参数含有num的key，能求值
3

>>> locals()
{'__doc__': None, 'l': {...}, 'num': 1, '__package__': None, '__name__': '__main__', '__loader__': <class '_frozen_importlib.BuiltinImporter'>, '__spec__': None, '__builtins__': <module 'builtins' (built-in)>}
>>>
```













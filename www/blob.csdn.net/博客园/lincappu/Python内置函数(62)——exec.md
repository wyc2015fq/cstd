# Python内置函数(62)——exec - lincappu - 博客园







# [Python内置函数(62)——exec](https://www.cnblogs.com/lincappu/p/8145355.html)





**英文文档：**
- `exec`(*object*[, *globals*[, *locals*]])
- 
- This function supports dynamic execution of Python code. *object* must be either a string or a code object. If it is a string, the string is parsed as a suite of Python statements which is then executed (unless a syntax error occurs). [1] If it is a code object, it is simply executed. In all cases, the code that’s executed is expected to be valid as file input (see the section “File input” in the Reference Manual). Be aware that the `return` and `yield` statements may not be used outside of function definitions even within the context of code passed to the `exec()` function. The return value is `None`.
- 
- In all cases, if the optional parts are omitted, the code is executed in the current scope. If only *globals* is provided, it must be a dictionary, which will be used for both the global and the local variables. If *globals* and *locals* are given, they are used for the global and local variables, respectively. If provided, *locals* can be any mapping object. Remember that at module level, globals and locals are the same dictionary. If exec gets two separate objects as *globals* and *locals*, the code will be executed as if it were embedded in a class definition.
- 
- If the *globals* dictionary does not contain a value for the key `__builtins__`, a reference to the dictionary of the built-in module `builtins` is inserted under that key. That way you can control what builtins are available to the executed code by inserting your own `__builtins__` dictionary into *globals* before passing it to `exec()`.
- 
- Note
- 
- The built-in functions `globals()` and `locals()` return the current global and local dictionary, respectively, which may be useful to pass around for use as the second and third argument to `exec()`.
- 
- Note
- 
- The default *locals* act as described for function `locals()` below: modifications to the default *locals* dictionary should not be attempted. Pass an explicit *locals* dictionary if you need to see effects of the code on *locals* after function `exec()`returns.
- 
- 
- 
- **　　动态执行语句块**
- **说明：**
- 
- 
- 　　1.  exec函数和eval函数类似，也是执行动态语句，只不过eval函数只用于执行表达式求值，而exec函数主要用于执行语句块。

第一个参数为语句字符串，globals参数和locals参数为可选参数，如果提供，globals参数必需是字典，locals参数为mapping对象。

```
>>> exec('print("你好")')
你好
>>> exec("print ('runoob.com')")
runoob.com
>>> exec ('for i in range(5):print("iter time: %d" % i)')
iter time: 0
iter time: 1
iter time: 2
iter time: 3
iter time: 4
>>>
>>> eval('a=1+2') #执行语句报错
Traceback (most recent call last):
  File "<pyshell#0>", line 1, in <module>
    eval('a=1+2')
  File "<string>", line 1
    a=1+2
     ^
SyntaxError: invalid syntax

>>> exec('a=1+2') #执行语句
>>> a
3
```



　　2.能够读取局部和全部变量，并且能修改全局变量，这个要注意的是，他修改完全局变量的值后，只能在内部使用因为没有 return 的功能，（eval 同理。）返回值永远是 None,

```
x=10
expr="z=30;sum=x+y+z;print(sum);print(x)"
y=20
exec(expr)
print(x,y,z)
exec(expr,{'x':1,'y':2,'z':1})
print(x,y)
exec(expr,{'x':0,'y':1,'z':0},{'x':0,'y':0,'z':1})
x=0
y=1
z=0
def expr():
    x=0
    y=0
    z=1
    z=30
    sum=x+y+z
    print(sum)


expr()
```



　　3. globals参数用来指定代码执行时可以使用的全局变量以及收集代码执行后的全局变量

```
>>> g = {'num':2}
>>> type(g)
<class 'dict'>

>>> exec('num2 = num + 2',g)

>>> g['num']
2
>>> g['num2'] #收集了exec中定义的num2全局变量
4
```



　　4. locals参数用来指定代码执行时可以使用的局部变量以及收集代码执行后的局部变量,如果该参数被忽略，那么它将会取与globals相同的值。

```
>>> g = {'num':2}
>>> type(g)
<class 'dict'>
>>> l = {'num2':3}
>>> type(l)
<class 'dict'>

>>> exec('''
num2 = 13
num3 = num + num2
''',g,l)

>>> l['num2'] #l中num2值已经改变
13
```



　　5. 为了保证代码成功运行，globals参数字典不包含 __builtins__ 这个 key 时，Python会自动添加一个key为 __builtins__ ，value为builtins模块的引用。如果确实要限制代码不使用builtins模块，需要在global添加一个key为__builtins__，value为{}的项即可（很少有人这么干吧）。

```
>>> g = {}
>>> exec('a = abs(-1)',g)
>>> 

>>> g = {'__builtins__':{}}
>>> exec('a = abs(-1)',g) #不能使用内置函数了
Traceback (most recent call last):
  File "<pyshell#30>", line 1, in <module>
    exec('a = abs(-1)',g)
  File "<string>", line 1, in <module>
NameError: name 'abs' is not defined
```



　　6. 当globals参数不提供是，Python默认使用globals()函数返回的字典去调用。

```
>>> num = 1
>>> exec('num2 = num + 1')
>>> globals()
{'__package__': None, '__loader__': <class '_frozen_importlib.BuiltinImporter'>, '__name__': '__main__', '__spec__': None, '__builtins__': <module 'builtins' (built-in)>, '__doc__': None, 'num2': 2, 'num': 1}
>>> 
>>> 
>>> exec('num2 = num + 1',{}) #指定了globals参数，globals中无num变量 执行失败
Traceback (most recent call last):
  File "<pyshell#5>", line 1, in <module>
    exec('num2 = num + 1',{})
  File "<string>", line 1, in <module>
NameError: name 'num' is not defined


>>> l = locals()
>>> l
{'__package__': None, '__loader__': <class '_frozen_importlib.BuiltinImporter'>, '__name__': '__main__', '__spec__': None, '__builtins__': <module 'builtins' (built-in)>, '__doc__': None, 'l': {...}, 'num2': 2, 'num': 1}
>>> 
>>> exec('num3 = num + 1',{},l)#指定了globals参数，globals中无num变量，指定了locals变量，locals变量含有num变量 执行成功
>>> l
{'__package__': None, '__loader__': <class '_frozen_importlib.BuiltinImporter'>, '__name__': '__main__', '__spec__': None, 'num3': 2, '__builtins__': <module 'builtins' (built-in)>, '__doc__': None, 'l': {...}, 'num2': 2, 'num': 1}
>>>
```



　　8.命名空间，为了不影响全局的命名空间，可以指定命名空间。如果不适用builtins模块，需要在global添加一个key为__builtins__，value为{}的项。

```
a=2
scope={}
exec('a=4',scope)
dic=globals()
print(a)
print(scope['a'])
print(dic.keys())
print(dic.items())
print(scope.keys())
print(scope.items()  # scope包含在 dic 里面



g={}    # 指定空的__builtins__内置函数
print(g)
g= globals()
print(g)
exec('print(abs(-1))',g)
print(g.keys())
#
g={'__builtins__':{}}
exec('print(abs(-1))',{})
```

　　9. exec 里不能出现 break，continue等关键字，因为 exec 语句块是独立执行，不会和外部的代码进行交互。

```
i = 5
j = 7
n = 0
while n < i
    print("looping")
    exec('if j > 5;break')
    n += 1
#报错：
looping
Traceback (most recent call last):
  File "<stdin>", line 4, in <module>
  File "<string>", line 3
SyntaxError: "break" outside loop
```















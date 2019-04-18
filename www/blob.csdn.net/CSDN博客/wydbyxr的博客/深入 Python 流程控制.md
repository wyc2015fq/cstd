# 深入 Python 流程控制 - wydbyxr的博客 - CSDN博客
2017年03月09日 17:41:08[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：177
# if 语句
可能会有零到多个 elif 部分，else 是可选的。关键字 ‘elif‘ 是 ’else if’ 的缩写，这个可以有效地避免过深的缩进。if … elif … elif … 序列用于替代其它语言中的 switch 或 case 语句（没有switch）。
```
>>> x = int(input("Please enter an integer: "))
Please enter an integer: 42
>>> if x < 0:
...      x = 0
...      print('Negative changed to zero')
... elif x == 0:
...      print('Zero')
... elif x == 1:
...      print('Single')
... else:
...      print('More')
...
```
# for 语句
Python 中的 for 语句和 C 或 Pascal 中的略有不同。Python 的 for 语句依据任意序列（链表或字符串）中的子项，按它们在序列中的顺序来进行迭代。
在迭代过程中修改迭代序列不安全。
```
>>> # Measure some strings:
... words = ['cat', 'window', 'defenestrate']
...     if len(w) > 6:
...         words.insert(0, w)
...
>>> words
['defenestrate', 'cat', 'window', 'defenestrate']
```
# range() 函数
如果你需要一个数值序列，内置函数 range() 会很方便，它生成一个等差级数链表:
```
range(5, 10)
   5 through 9
range(0, 10, 3)
   0, 3, 6, 9
range(-10, -100, -30)
  -10, -40, -70
```
## 需要迭代链表索引的话，如下所示结合使 用 range() 和 len()
```
>>> a = ['Mary', 'had', 'a', 'little', 'lamb']
>>> for i in range(len(a)):
...     print(i, a[i])
...
0 Mary
1 had
2 a
3 little
4 lamb
```
不过，这种场合可以方便的使用 enumerate()
## 如果你只是打印一个序列的话会发生奇怪的事情:
```
>>> print(range(10))
range(0, 10)
```
在不同方面 range() 函数返回的对象表现为它是一个列表，但事实上它并不是。当你迭代它时，它是一个能够像期望的序列返回连续项的对象；但为了节省空间，它并不真正构造列表。所以你应该这样：
```
>>> list(range(5))
[0, 1, 2, 3, 4]
```
# break 和 continue 语句, 以及循环中的 else 子句
break 语句和 C 中的类似，用于跳出最近的一级 for 或 while 循环。 
continue 语句是从 C 中借鉴来的，它表示循环继续执行下一次迭代。 
循环可以有一个 else 子句；它在循环迭代完整个列表（对于 for ）或执行条件为 false （对于 while ）时执行，但循环被 break 中止的情况下不会执行。以下搜索素数的示例程序演示了这个子句:
```
>>> for n in range(2, 10):
...     for x in range(2, n):
...         if n % x == 0:
...             print(n, 'equals', x, '*', n//x)
...             break
...     else:
... # loop fell through without finding a factor
... # else 语句是属于 for 循环之中， 不是 if 语句。
...         print(n, 'is a prime number')
...
2 is a prime number
3 is a prime number
4 equals 2 * 2
5 is a prime number
6 equals 2 * 3
7 is a prime number
8 equals 2 * 4
9 equals 3 * 3
```
注意：循环的 else 子句在未出现 break 时运行！！ 
与循环一起使用时，else 子句与 try 语句的 else 子句比与 if 语句的具有更多的共同点： 
try 语句的 else 子句在未出现异常时运行， 
循环的 else 子句在未出现 break 时运行。
# pass 语句
pass 语句什么也不做。它用于那些语法上必须要有什么语句，但程序什么也不做的场合，例如:
```
>>> while True:
...     pass  # Busy-wait for keyboard interrupt (Ctrl+C)
...
```
这通常用于创建最小结构的类:
```
>>> class MyEmptyClass:
...     pass
```
# def
关键字 def 引入了一个函数 定义。在其后必须跟有函数名和包括形式参数的圆括号。函数体语句从下一行开始，必须是缩进的。
## 可以作为通用的重命名机制
函数名指代的值（即函数体）有一个被 Python 解释器认定为 用户自定义函数 的类型。 这个值可以赋予其他的名字（即变量名），然后它也可以被当做函数使用。这可以作为通用的重命名机制:
```
>>> fib
<function fib at 10042ed0>
>>> f = fib
>>> f(100)
0 1 1 2 3 5 8 13 21 34 55 89
```
## 重要警告:
默认值只被赋值一次。这使得当默认值是可变对象时会有所不同，比如列表、字典或者大多数类的实例。例如，下面的函数在后续调用过程中会累积（前面）传给它的
```python
def f(a, L=[]):
    L.append(a)
    return L
print(f(1))
print(f(2))
print(f(3))
这将输出:
[1]
[1, 2]
[1, 2, 3]
```
不想让默认值在后续调用中累积，你可以像下面一样定义函数:
```python
def f(a, L=None):
    if L is None:
        L = []
    L.append(a)
    return L
```
# 关键字参数
函数可以通过 关键字参数 的形式来调用，形如 keyword = value。例如，以下的函数:
```python
def parrot(voltage, state='a stiff', action='voom', type='Norwegian Blue'):
    print("-- This parrot wouldn't", action, end=' ')
    print("if you put", voltage, "volts through it.")
    print("-- Lovely plumage, the", type)
    print("-- It's", state, "!")
#接受一个必选参数 (voltage) 以及三个可选参数 (state, action, 和 type)。
```
不过以下几种调用是无效的:
```
parrot()            
# required argument missing
parrot(voltage=5.0, 'dead')  
# non-keyword argument after a keyword argument
parrot(110, voltage=220)     
# duplicate value for the same argument
parrot(actor='John Cleese')  
# unknown keyword argument
```
## 更深入
引入一个形如 **name 的参数时，它接收一个字典。 
使用一个形如 *name  的形式参数，它接收一个元组。 
（ *name 必须在 **name 之前出现）。  
例如，
```python
def cheeseshop(kind, *arguments, **keywords):
    print("-- Do you have any", kind, "?")
    print("-- I'm sorry, we're all out of", kind)
    for arg in arguments:
        print(arg)
    print("-" * 40)
    keys = sorted(keywords.keys())
    for kw in keys:
        print(kw, ":", keywords[kw])
它可以像这样调用:
cheeseshop("Limburger", "It's very runny, sir.",
           "It's really very, VERY runny, sir.",
           shopkeeper="Michael Palin",
           client="John Cleese",
           sketch="Cheese Shop Sketch")
```
# 可变参数列表
最后，一个最不常用的选择是可以让函数调用可变个数的参数。这些参数被包装进一个元组（参见 元组和序列 ）。在这些可变个数的参数之前，可以有零到多个普通的参数
```
>>> def concat(*args, sep="/"):
...    return sep.join(args)
...
>>> concat("earth", "mars", "venus")
'earth/mars/venus'
>>> concat("earth", "mars", "venus", sep=".")
'earth.mars.venus'
```
# Lambda 形式
通过 lambda 关键字，可以创建短小的匿名函数。 
 Lambda 形式可以用于任何需要的函数对象。 
 出于语法限制，它们只能有一个单独的表达式。
```python
例子1：
>>> def make_incrementor(n):
...     return lambda x: x + n
例子2：
>>> pairs = [(1, 'one'), (2, 'two'), (3, 'three'), (4, 'four')]
>>> pairs.sort(key=lambda pair: pair[1])
>>> pairs
[(4, 'four'), (1, 'one'), (3, 'three'), (2, 'two')]
```

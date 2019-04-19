# python：global与nonlocal关键字 - 数据之美的博客 - CSDN博客
2018年02月26日 01:24:50[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：102
在Python中，当引用一个变量的时候，对这个变量的搜索是按找本地作用域(Local)、嵌套作用域(Enclosing function locals)、全局作用域(Global)、内置作用域(builtins模块)的顺序来进行的，即所谓的LEGB规则。 
然而当在一个函数内部为一个变量赋值时，并不是按照上面所说LEGB规则来首先找到变量，之后为该变量赋值。在Python中，在函数中为一个变量赋值时，有下面这样一条规则:
> “当在函数中给一个变量名赋值是(而不是在一个表达式中对其进行引用)，Python总是创建或改变本地作用域的变量名，除非它已经在那个函数中被声明为全局变量. ”
下面来看一个例子:
```python
```python
x = 99
def func():
    x = 88
func()
print(x)     #输出99
```
```
   上面的程序在func函数中为x赋值为88,按照上述规则，因为func函数中并没有变量x，因此Python会在func函数的本地作用域创建一个变量x，也就是说此x并非`x = 99`中的x，这样也就能够理解为什么最后程序输出依然为99了。 
若想要在函数中修改全局变量x，而不是在函数中新建一个变量，此时便要用到关键字`global`了，示例如下:
```python
```python
x = 99
def func()
    global x
    x = 88
func()
print(x)    #输出88
```
```
   上述程序中的`global x`语句告诉Python在func的本地作用域内，要使用全局作用域中的变量x，因此在下面的`x = 88`语句中，Python不会再在本地作用域中新建一个变量，而是直接引用全局作用域中的变量x，这样程序最后输出88也就不难理解了。 
关键子`nonlocal`的作用与关键字`global`类似，使用`nonlocal`关键字可以在一个嵌套的函数中修改嵌套作用域中的变量，示例如下:
```python
```python
def func():
    count = 1
    def foo():
        count = 12
    foo()
    print(count)
func()    #输出1
```
```
   上面的程序中，在嵌套的foo函数中，对变量count赋值，同样会创建一个新的变量，而非使用`count = 1`语句中的count，如果要修改嵌套作用域中的count，就要使用`nonlocal`关键字了:
```python
```python
def func():
    count = 1
    def foo():
        nonlocal count
        count = 12
    foo()
    print(count)
func()     #输出12
```
```
   上面的程序中，在foo函数中使用了`nonlocal`关键字，就会告诉Python在foo函数中使用嵌套作用域中的变量count，因此对变量count进行修改时，会直接影响到嵌套作用域中的count变量，程序最后也就输出12了. 
注意:使用`global`关键字修饰的变量之前可以并不存在，而使用`nonlocal`关键字修饰的变量在嵌套作用域中必须已经存在。

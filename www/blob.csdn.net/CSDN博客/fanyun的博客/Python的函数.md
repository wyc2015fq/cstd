# Python的函数 - fanyun的博客 - CSDN博客
2017年04月04日 19:34:30[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：381标签：[python](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[脚本语言](https://blog.csdn.net/fanyun_01/article/category/6838309)
所属专栏：[巧用脚本](https://blog.csdn.net/column/details/15130.html)
         Python函数通过def定义。def关键字后跟函数的标识符名称，然后跟一对圆括号，括号之内可以包含一些变量名，该行以冒号结尾；接下来是一块语句，即函数体。
```python
def sumOf(a, b):
   return a + b
```
1. 函数形参
　　函数中的参数名称为‘形参’，调用函数时传递的值为‘实参’
2. 局部变量
　　在函数内定义的变量与函数外具有相同名称的其他变量没有任何关系，即变量名称对于函数来说是局部的。这称为变量的作用域。global语句，为定义在函数外的变量赋值时使用global语句。
```python
def func():
   global x
   print "x is ", x
    x= 1
x = 3
func()
print x
 
#3
#1
```
3. 默认参数
　   通过使用默认参数可以使函数的一些参数是‘可选的’。
```python
def say(msg, times =  1):
   print msg * times
say("peter")
say("peter", 3)
```
　　注意：只有在形参表末尾的那些参数可以有默认参数值，即不能在声明函数形参的时候，先声明有默认值的形参而后声明没有默认值的形参，只是因为赋给形参的值是根据位置而赋值的。
4. 关键参数
　　如果某个函数有很多参数，而现在只想指定其中的部分，那么可以通过命名为这些参数赋值（称为‘关键参数’）。
　　优点：不必担心参数的顺序，使函数变的更加简单；假设其他参数都有默认值，可以只给我们想要的那些参数赋值。
```python
def func(a, b=2, c=3):
print "a is %s, b is %s, c is %s" % (a, b, c)
func(1) #a is 1, b is 2, c is 3
func(1, 5) #a is 1, b is 5, c is 3
func(1, c = 10) #a is 1, b is 2, c is 10
func(c = 20, a = 30) #a is 30, b is 2, c is20
```
5. return 语句
　　return语句用来从一个函数返回，即跳出函数。可从函数返回一个值。没有返回值的return语句等价于return None。None表示没有任何东西的特殊类型。
6. DocStrings (文档字符串)
```python
def func():
   '''This is self-defined function
 
Do nothing'''
   pass
 
print func.__doc__
 
#This is self-defined function
#
#Do nothing
```

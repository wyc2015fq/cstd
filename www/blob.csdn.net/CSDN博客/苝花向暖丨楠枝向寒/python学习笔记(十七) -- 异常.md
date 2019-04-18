# python学习笔记(十七) -- 异常 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年08月24日 00:43:46[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：72
所属专栏：[python学习笔记](https://blog.csdn.net/column/details/26223.html)



**Table of Contents**

[异常](#%C2%A0%E5%BC%82%E5%B8%B8)

[一：为什么会出现异常](#%E4%B8%80%EF%BC%9A%E4%B8%BA%E4%BB%80%E4%B9%88%E4%BC%9A%E5%87%BA%E7%8E%B0%E5%BC%82%E5%B8%B8)

[1：造成代码冗余](#1%EF%BC%9A%E9%80%A0%E6%88%90%E4%BB%A3%E7%A0%81%E5%86%97%E4%BD%99)

[2：无法知道详细的错误信息](#2%EF%BC%9A%E6%97%A0%E6%B3%95%E7%9F%A5%E9%81%93%E8%AF%A6%E7%BB%86%E7%9A%84%E9%94%99%E8%AF%AF%E4%BF%A1%E6%81%AF)

[二：抛出异常](#%E4%BA%8C%EF%BC%9A%E6%8A%9B%E5%87%BA%E5%BC%82%E5%B8%B8)

[1：异常的本质](#1%EF%BC%9A%E5%BC%82%E5%B8%B8%E7%9A%84%E6%9C%AC%E8%B4%A8)

[2：自定义异常](#2%EF%BC%9A%E8%87%AA%E5%AE%9A%E4%B9%89%E5%BC%82%E5%B8%B8)

[三：捕获异常](#%E4%B8%89%EF%BC%9A%E6%8D%95%E8%8E%B7%E5%BC%82%E5%B8%B8)

[1：作用](#1%EF%BC%9A%E4%BD%9C%E7%94%A8)

[2：使用](#2%EF%BC%9A%E4%BD%BF%E7%94%A8)

[3：向上抛出异常](#3%EF%BC%9A%E5%90%91%E4%B8%8A%E6%8A%9B%E5%87%BA%E5%BC%82%E5%B8%B8)

[四：python中内置异常的使用](#%E5%9B%9B%EF%BC%9Apython%E4%B8%AD%E5%86%85%E7%BD%AE%E5%BC%82%E5%B8%B8%E7%9A%84%E4%BD%BF%E7%94%A8)

[五：使用except注意事项](#%E4%BA%94%EF%BC%9A%E4%BD%BF%E7%94%A8except%E6%B3%A8%E6%84%8F%E4%BA%8B%E9%A1%B9)

#  异常

## 一：为什么会出现异常

### 1：造成代码冗余

我们下看下面的代码

```python
>>> def inner(value):  #假设inner是python的一个底层函数
	if value == 1:
		return value
	print('do something')
	return r

>>> def center(value): #center是python的一个内置函数，可以被我们调用
	return inner(value)

>>> def outter(value): #我们在调用的时候阅读了api，知道了，如果返回值为1，就代表报错
	if center(value) == 1:  #所以我们判断返回值，从而做了不同的处理
		print('error')
	else:
		print('no error')

		
>>> outter(1)
error
```

从上面的代码中，可以看出，由于最内部的函数，为了提示给调用者错误信息，导致调用者也需做相应的处理。从而给调用者造成了麻烦，这里的center函数还得将inner函数的结果返回给outter。

### 2：无法知道详细的错误信息

我们可以看到，上面代码只能知道我们调用outter函数时出现错误了，但是我们并无法定位到导致错误的根源。

## 二：抛出异常

为了解决上面所存在的问题，我们可以在某种情况下选择去抛出一个异常，然后在最外层调用的地方直接对异常做处理就好了，而中间涉及的那些函数也无需将内部函数的处理结果返回。

### 1：异常的本质

异常实际上就是一个 类，异常类 有很多，所有异常的父类是 BaseException

[这里](https://docs.python.org/3/library/exceptions.html#exception-hierarchy)可以看到所有的异常类以及继承关系。

### 2：自定义异常

我们也可以自己定义一个异常类，然后通过raise关键字抛出。

如下：

```python
class DatabaseException(Exception): #自定义的异常类一般都继承Exception
	def __init__(self,err='数据库错误'): #err是自定义的异常报错的信息
		Exception.__init__(self,err) #调用Exception的初始化对象的方法，把err传进去
```

测试：

```python
>>> def testRaise():
	raise DatabaseException()
    print('1111')
>>> testRaise()
Traceback (most recent call last): #告诉我们下面是错误信息的描述
  File "<pyshell#154>", line 1, in <module> #代码的第一行 调用testRaise函数时出错
    testRaise()
  File "<pyshell#150>", line 2, in testRaise #testRaise函数内部第二行出错
    raise DatabaseException()  #在执行这行时抛出了异常
DatabaseException: 数据库错误
```

我们可以看到 1111 并没有输出，由于系统发现了异常程序就会退出，不会再运行下面的代码。

但有时候我们仅仅是想将错误信息提示出来，但代码还可以继续执行，这时候我们需要将异常捕获。

## 三：捕获异常

### 1：作用

当系统发现了异常，我们将其捕获，然后可以做相应的处理，且程序不会中断

### 2：使用

```python
>>> class DatabaseException(Exception):
	def __init__(self,err='数据库错误'):
		Exception.__init__(self,err)
>>> def t():
	try:   #我们将可能会报错的代码放入到try代码块中
		testRaise()
		print('try')  #报错以后，try代码块中后面的代码不会再继续执行
	except DatabaseException as e:
		print(e)  #输出错误信息
	finally:
		print('finally')  # try代码报错进入到except模块，except执行完会执行finally模块中的代码
	print('out of try') #try、except、finally 外部的代码部分也会被执行

	
>>> t()
数据库错误
finally
out of try
```

我们会发现，这里出现的错误信息，仅仅是简单的 五个字 "数据库错误" ，这导致我们无法定位到具体出现问题的地方，

我们可以使用python内置的logging模块来显示出我们需要的信息

```python
>>> import logging    #导入模块
>>> def t(): 
	try:
		testRaise()
		print('try')
	except DatabaseException as e: 
		logging.exception(e)   #这里做了改动
	finally:
		print('finally')
	print('out of try')

	
>>> t()
ERROR:root:数据库错误
Traceback (most recent call last):
  File "<pyshell#177>", line 3, in t
  File "<pyshell#156>", line 2, in testRaise
DatabaseException: 数据库错误
finally
out of try
```

### 3：向上抛出异常

先说一下情景以便理解：

首先一个程序员发现数据库被删除了于是他向上级raise情况。

他的上级了解到了情况以后，发现这事大了，他做不了主，得向Ceo去汇报了，即将员工汇报的情况再叙述给Ceo

Ceo了解到了情况做出了决定

```python
>>> class DatabaseException(Exception):
	def __init__(self,err='数据库被删了！'):
		Exception.__init__(self,err)
>>> def Employee():
	raise DatabaseException()
>>> def Boss():
	try:
		Employee()
	except DatabaseException as e:
		print(e)
		raise   #向上抛出捕获到的异常
>>> def Ceo():
	try:
		Boss()
	except DatabaseException as e:
		print('收到Boss汇报的信息:',e)
	finally:
		print('找到谁删的数据库',"开除并将其告到法院！")
>>> Ceo()
数据库被删了！
收到Boss汇报的信息: 数据库被删了！
找到谁删的数据库 开除并将其告到法院！
```

## 四：python中内置异常的使用

```python
>>> def T():
	try:
		print('try...')
		r = 10 / 0
		print('result:',r)
	except Exception as e:  #其实我们这里可以明确出现的错误是被除数为0，而这个错误有一个特定的异常类：ZeroDivisionError
		logging.exception(e) #所以可以将Exception替换为ZeroDivisionError，使代码更加清晰
	finally:
		print('finally...')
	print('END')

	
>>> T()
try...
ERROR:root:division by zero
Traceback (most recent call last):
  File "<pyshell#220>", line 4, in T
ZeroDivisionError: division by zero
finally...
END
```

我们还可以使用多个 except模块来分情况捕获不同的错误从而有不同的对应策略，也可以添加一个else模块代表如果没有捕获到异常执行某些代码。

```python
try:
    print('try...')
    r = 10 / int('2')
    print('result:', r)
except ValueError as e:
    print('ValueError:', e)
except ZeroDivisionError as e:
    print('ZeroDivisionError:', e)
else:
    print('no error!')
finally:
    print('finally...')
print('END')
```

## 五：使用except注意事项

如果有多个except模块，且捕获的异常存在父子关系，那么子异常一定要在父异常前面捕获

```python
try:
    foo()
except ValueError as e:
    print('ValueError')
except UnicodeError as e:
    print('UnicodeError')
```

第二个except模块永远也捕获不到异常，因为 ValueError是UnicodeError的父类，父类可以捕获到子类的异常。如果出现了异常会全部被第一个except捕获。


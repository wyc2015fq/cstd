# python学习笔记(二十)  -- with的使用 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年08月28日 22:20:15[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：37
所属专栏：[python学习笔记](https://blog.csdn.net/column/details/26223.html)



**Table of Contents**

[with的使用](#with%E7%9A%84%E4%BD%BF%E7%94%A8)

[with的使用场景](#with%E7%9A%84%E4%BD%BF%E7%94%A8%E5%9C%BA%E6%99%AF)

网上看了很多说一堆理论的，我直接告诉你怎么使用。

## with的使用

```python
>>> class Sample:
	def __enter__(self):
		return self
	def __exit__(self,type,value,trace):
		print("type:",type)
		print("value:",value)
		print("trace:",trace)
	def do_something(self):
		bar = 1/0
		return bar+10

	
>>> with Sample() as sample:
	sample.do_something

	
type: <class 'ZeroDivisionError'>
value: division by zero
trace: <traceback object at 0x000001F067DD8FC8>
Traceback (most recent call last):
  File "<pyshell#75>", line 2, in <module>
    sample.do_something()
  File "<pyshell#70>", line 9, in do_something
    bar = 1/0
ZeroDivisionError: division by zero
```

首先定义了一个Sample类，然后使用with关键字，执行流程如下。

1: 执行with关键字后面的 Sample() ，创建了一个Sample对象，同时执行Sample类的 __enter__函数，将__enter__函数的返回值赋值给 as 后面的 sample 变量。

2: 此时sample变量指向的是一个 Sample类的对象，再执行 冒号后面的代码即第二行 sample.do_something，即通过Sample类的对象去调用Sample中定义的 do_something 函数。

3: 执行完 do_something函数，回去执行__exit__函数。

注意 with 代码块中执行的内容可以被捕获异常，即 执行 sample.do_something语句时，如果do_something函数中抛出了异常，可以被捕获到，并将异常信息作为参数传入到__exit__函数中，也就是说无论是否有异常，__exit__函数肯定会被执行，所以__exit__类似于 finally代码块。

__exit__函数有四个参数：

参数1  对象本身

参数2  type(抛出的异常对象)  的返回值

参数3  抛出的异常对象

参数4  Traceback 对象

如果 我们想当抛出某一种异常时不要打印出错误信息，可以让 __exit__函数返回true，如下所示：

```python
>>> class Sample:
	def __enter__(self):
		return self
	def __exit__(self,type,value,trace):
		print("type:",type)
		print("value:",value)
		print("trace:",trace)
		return isinstance(value,ZeroDivisionError)
	def do_something(self):
		bar = 1/0
		return bar+10

	
>>> with Sample() as sample:
	sample.do_something()

	
type: <class 'ZeroDivisionError'>
value: division by zero
trace: <traceback object at 0x000001F067DE6348>
```

注意 with 语句 只能捕获到  with代码块中执行函数的异常，即 with  xxx  as  x :   后面的内容，就是冒号后面执行的函数。

如下这种情况并不能捕获异常：

```python
>>> class Sample:
	def __enter__(self):
		bar = 1/0
		return self
	def __exit__(self,type,value,trace):
		print("type:",type)
		print("value:",value)
		print("trace:",trace)
	def do_something(self):
		print("In do_something function")

		
>>> 
>>> with Sample() as sample:
	sample.do_something()

	
Traceback (most recent call last):
  File "<pyshell#79>", line 1, in <module>
    with Sample() as sample:
  File "<pyshell#76>", line 3, in __enter__
    bar = 1/0
ZeroDivisionError: division by zero
```

我将 bar = 1/0  这句会抛出异常的代码写在 __enter__函数中，但是通过结果可以发现，__exit__函数以及do__somthing函数并没有被执行，所以说明执行__enter__函数时抛出异常后代码就中断了，从而可以得知异常并没有被捕获。

当然，__exit__函数中出现异常也是无法捕获的

```python
class Sample:
	def __enter__(self):
		print("In __enter__ function")
		return self
	def __exit__(self,type,value,trace):
		print("In __exit__ function")
		bar = 1/0
		print("type:",type)
		print("value:",value)
		print("trace:",trace)
	def do_something(self):
		print("In do_something function")

		
>>> with Sample() as sample:
	sample.do_something()

	
In __enter__ function
In do_something function
In __exit__ function
Traceback (most recent call last):
  File "<pyshell#103>", line 2, in <module>
    sample.do_something()
  File "<pyshell#101>", line 7, in __exit__
    bar = 1/0
ZeroDivisionError: division by zero
```

如果没有异常的话，__exit__函数中的后三个参数会被传入None

```python
>>> class Sample:
	def __enter__(self):
		return self
	def __exit__(self,type,value,trace):
		print("type:",type)
		print("value:",value)
		print("trace:",trace)
	def do_something(self):
		print("In do_something function")

		
>>> with Sample() as sample:
	sample.do_something()

	
In do_something function
type: None
value: None
trace: None
```

## with的使用场景

有一些任务，可能事先需要设置，事后做清理工作。对于这种场景，Python的with语句提供了一种非常方便的处理方式。一个很好的例子是文件处理，你需要获取一个文件句柄，从文件中读取数据，然后关闭文件句柄。

下一篇文章我将会展示读取文件操作时使用 with语句 ，点击[这里](https://blog.csdn.net/weixin_40247263/article/details/82120720)查看。


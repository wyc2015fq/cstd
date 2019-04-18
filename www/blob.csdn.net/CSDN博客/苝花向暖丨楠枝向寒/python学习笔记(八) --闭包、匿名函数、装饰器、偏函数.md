# python学习笔记(八) --闭包、匿名函数、装饰器、偏函数 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年08月14日 23:31:05[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：62
所属专栏：[python学习笔记](https://blog.csdn.net/column/details/26223.html)



## 闭包

函数里面包含函数，外层函数返回内层函数，调用外层函数的时候并不会执行内层函数，而是返回内层函数，当我们需要的时候再去调用。

如下所示：

```python
>>> def outer(*arg):
	def inner():
		for n in arg:
			print (n)
	return inner
>>> f = outer(1,2,3,4)
>>> f()
1
2
3
4
>>> outer(1,2,3,4)()
1
2
3
4
```

闭包函数切记以如下方式使用

```python
>>> def outer(*arg):
	def inner():
		n = i*i        #变量i会改变
		return n
	L = []
	for i in arg:
		L.append(inner)
	return L

>>> f1,f2,f3,f4,f5 = outer(1,2,3,4,5)
>>> f1()            #正常观念下，结果应该是 1,4,9,16,25
25                  #但结果都是25
>>> f2()            #原因：在我们调用内部方法之前，变量i因为for循环的原因一直在改变
25                  #当我们调用的时候，变量i 已经变成了 5
>>> f3()
25
>>> f4()
25
>>> f5()
25
```

如业务需要，必须要结合for循环，则需要以如下方式进行处理

```python
>>> def outer(*arg):
	def center(i): #多一层函数，将变量i的值绑定，这样每一次调用inner，里面的i都被调用的时候固定住
		def inner():
			n = i*i
			return n
		return inner
	L = []
	for i in arg:
		L.append(center(i)) #注意这里，方法名不跟括号代表获得该方法这个对象，加括号代表执行这个方法，但是获得带参数的方法对象，必须加括号进行参数传递
	return L

>>> f1,f2,f3,f4,f5 = outer(1,2,3,4,5)
>>> f1()
1
>>> f2()
4
>>> f3()
9
>>> f4()
16
>>> f5()
25
```

## 匿名函数

匿名函数实际上就是对一个简单的函数的简化，使用lambda关键字

以map函数来举例

如下是一个普通的写法

```python
>>> a = map(func,[1,2,3,4,5])
>>> list(a)
[1, 4, 9, 16, 25]
```

我们通过lambda函数来优化一下

```python
>>> a = map(lambda x:x*x,[1,2,3,4,5])  # 冒号前的 x 代表原来方法中的参数，冒号后代表方法返回值
>>> list(a)
[1, 4, 9, 16, 25]
```

匿名函数也可以当作返回值

```python
>>> def build(x, y):
	return lambda: x * x + y * y   #其实lambda出现在函数的返回值中也就相当于这是一个内部函数了

>>> a = map(build,[1,2,3,4,5],[1,2,3,4,5])
>>> list(a)
[<function build.<locals>.<lambda> at 0x00000000031A8C80>, <function build.<locals>.<lambda> at 0x00000000031A8D08>, <function build.<locals>.<lambda> at 0x00000000031A8D90>, <function build.<locals>.<lambda> at 0x00000000031A8E18>, <function build.<locals>.<lambda> at 0x00000000031A8EA0>]
>>> a1,a2,a3,a4,a5 = map(build,[1,2,3,4,5],[1,2,3,4,5])
>>> a1()
2
>>> a2()
8
>>> a3()
18
>>> a4()
32
>>> a5()
50
```

我们再来优化一下上面内部函数中的一个案例

```python
>>> def outer():
	def center(i):
		return lambda :i*i   # 此处之前的inner方法被lambda表达式所代替
	L = []
	for i in range(1,6):
		L.append(center(i))
	return L

>>> f1,f2,f3,f4,f5 = outer()
>>> f1()
1
>>> f2()
4
>>> f3()
9
>>> f4()
16
>>> f5()
25
>>>
```

## 装饰器

装饰器类似于java中的aop，即我们可以在调用函数前或后做一些事情，却不用更改我们的函数。

装饰器的使用类似java中的注解，我们还可以给装饰器传参数。

```python
#定义一个装饰器
>>> def aop(func):       #参数为被装饰的方法对象
	def wrapper(*arg,**kw):   #参数为被修饰的方法需要的参数，*arg，**kw可以接收所有参数
		print ('正在执行sql语句')  #执行被装饰方法前需要做的事
		return func(*arg,**kw) #执行被装饰的方法
	return wrapper

#在定义一个方法的时候，通过@装饰器名，来给一个方法加上装饰
>>> @aop
def excute_sql(table):
	print('select * from %s' % table)

	
>>> excute_sql('user')
正在执行sql语句
select * from user
```

装饰器执行过程：如果我们的一个方法上有装饰器的标识，那么在执行该方法时，会将被装饰的方法，以及被装饰的方法所需要的参数同时传入我们定义的装饰器当中，最外层的函数即上面的aop接收被装饰的函数对象，然后return 内层函数，注意，这里return内层函数以后会自动调用内层函数，与我们之前说的闭包是不一样的。看下图

```python
>>> def outter():
	def inner():
		print('1')
	return inner

>>> aop()     #调用外层函数返回来的是 inner函数的对象，inner函数并没有被执行
<function aop.<locals>.inner at 0x00000000031C7048>
>>> aop()()
1
```

但是这里的wrapper实际上是被自动执行了，内层的函数即wrapper用来接收被修饰函数所需的参数，*arg和**kw可以接收一切的参数。然后打印了输出语句，然后又返回了被装饰的方法，此处也自动被调用。

如果我们的装饰器除了要接收被装饰的方法外，还需要接收一些其他的参数。

如下所示

```python
>>> def accept_param(text):  #最外层加了一个函数，用来接收自己需要传入的参数
	def aop(func):  #从这曾开始 还和以前一样
		def wrapper(*arg,**kw):
			print('%s %s():' % (text,func.__name__ ))  #这里可以接收到外层函数的参数，之前在内包中提到过
			return func(*arg,**kw)
		return wrapper
	return aop

>>> @accept_param('正在执行')   #这里可以给装饰器传入参数
def excute_sql(table):
	print('select * from %s' % table)

>>> 
>>> excute_sql('user')
正在执行 excute_sql():
select * from user
```

但是要注意一点，一旦用了装饰器，我们函数的名字就会发生改变

下面先做一下铺垫

先看一眼最普通的函数的名字

```python
>>> def t(i):
	print(i)

	
>>> t.__name__   #查看方法t 名字 这个肯定没问题
't'
>>> t(5)     #执行方法t 
5
>>> t.__name__  #再查看还是没问题
't'
```

再看一眼return后面跟函数名的函数

```
>>> def t(i):
	def a():
		return i*i
	return a

>>> t.__name__
't'
>>> t(5).__name__  #执行方法t 返回方法a对象
'a'
>>> t.__name__  #再看t的名字是否正确，结果没问题
't'
>>> t(5)()  #拿到方法a对象后继续执行
25
>>> t.__name__ #再看 还是没问题
't'
```

重点在下面

```python
#定义一个装饰器
>>> def newaop(text):
	def decreation(func):
		def wrapper(*arg,**pw):
			print("%s" % text)
			return func(*arg,**pw)
		return wrapper
	return decreation

>>> t(5)

#定义一个被装饰的方法
>>> @newaop('abc')
def t(i):
	print(i)

#查看方法t的name
>>> t.__name__   #方法t的名字变成了wrapper！！！
'wrapper'

>>> t(5)   #执行方法t 以后
abc
5

>>> t.__name__  #查看t名字 依旧是 wrapper！！！
'wrapper'
```

通过上述的案例，可以看出，一旦一个方法被装饰器所装饰，那么这个方法名字将会变成装饰器中最内层方法的名字，因为该方法 t 在执行的时候实际真正执行的是 wrapper方法里面的内容。

所以我们需要将其名字改为原有的方法名，否则下面一旦有涉及方法名的代码，就会出现问题，并不是我们预期的结果，不过我们并不需要自己去手动修改，Python内置的`functools.wraps`就是干这个事的。将代码更改为如下所示

```python
import functools #导入工具类

>>> def newaop(text):
	def decreation(func):
		@functools.wraps(func)  #加上这一行就ok了，参数就是我们被装饰的方法对象
		def wrapper(*arg,**pw):
			print("%s" % text)
			return func(*arg,**pw)
		return wrapper
	return decreation

>>> @newaop('abc')
def t(i):
	print(i)

	
>>> t.__name__
't'
>>> t(5)
abc
5
>>> t.__name__
't'
```

## 偏函数

偏函数可以更改自定义函数或内置函数中的默认参数的值

如下所示 将内置函数int的默认参数base的值更改为2

```python
>>> import functools
>>> int2 = functools.partial(int, base=2)  #使用偏函数将int函数的默认参数base默认值改为2
>>> int2('1000000')                        #int函数base的默认值更改之前是10
64
>>> int2('1010101')
85
```

除了可以更改内置函数的默认参数也可以更改自定义函数如下

```python
>>> def test(a=1,*,b=2):
	       print(a)
	       print(b)

>>> a  = functools.partial(test,b=3) #我这里的b实际上是一个关键字参数与默认参数的结合
	                                 
>>> a(3)
	       
3
3
>>> a(4)
	       
4
3
```

这里补充一下我的文章 《python学习笔记(三)》中遗漏的知识点，参数a和b的区别，参数a是默认参数，就是不赋值就等于1，赋值的话传参的时候直接传入数字就行，但是b是一个关键字参数同时我给它一个默认值，不赋值的话默认为2，但是赋值的话，必须  b = 3 这样传参，也就是说必须带上b这个字母

以下代码演示我所说的内容

```python
>>> def test(a=1,*,b=2):
	       print(a)
	       print(b)
>>> test() #都不赋值
	       
1
2
>>> test(2) #给a赋值 
	       
2
2 
>>> test(2,3)  #这样赋值会报错
	       
Traceback (most recent call last):
  File "<pyshell#209>", line 1, in <module>
    test(2,3)
TypeError: test() takes from 0 to 1 positional arguments but 2 were given
>>> test(2,b=3)  #必须带上 b=
	       
2
3
>>> test(b=3)  #只给b赋值
	       
1
3
```


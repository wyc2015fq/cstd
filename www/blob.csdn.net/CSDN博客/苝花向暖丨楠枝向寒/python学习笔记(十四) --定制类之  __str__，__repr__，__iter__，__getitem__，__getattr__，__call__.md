# python学习笔记(十四) --定制类之   __str__，__repr__，__iter__，__getitem__，__getattr__，__call__ - 苝花向暖丨楠枝向寒 - CSDN博客

2018年08月19日 17:34:45[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：308
所属专栏：[python学习笔记](https://blog.csdn.net/column/details/26223.html)



## __str__   --类似于java中的toString()

我们print(对象名)  会输出对象属于哪个类，以及该对象在内存中的地址。那如果我们想print(对象名)输出的是该对象的属性名以及属性值该怎么做？

我们可以重写__str__方法，就像java中一样，如果我们自定义一个类，我们需要重写toString()方法，才能在system.out.println该类对象的时候，打印出我们想要的结果，否则输出的是地址。

```python
#如下，会输出对象的类和内存中的地址
>>> class Student(object):
	pass

>>> s = Student()
>>> print(s)
<__main__.Student object at 0x000002C847E807F0>

#重写__str__函数，return 的内容即输出的内容
>>> class Student(object):
	def __init__(self,value):
		self.name = value
	def __str__(self):
		return 'name = %s' % self.name

	
>>> student = Student('zzh')
>>> 
>>> print(student)
name = zzh
```

## __repr__

命令行模式支持 直接输入对象名 就可以输出内容

```python
>>> student
<__main__.Student object at 0x000002C847F74FD0>
```

显然上面发现输出的内容又变成 对象信息了。

这里说一下 print(对象名)  和直接输入 变量名 的区别：

print(对象名) 是用来给程序员看的，它会调用类中的__str__函数。

直接输入变量名 是给机器看的，它会调用类中的__repr__函数。

所以我们下面重写__repr__函数

```python
>>> class Student1(object):
	def __init__(self,value):
		self.name = value
	def __str__(self):     #重写__str__
		return 'name = %s' % self.name
	def __repr__(self):    #重写__repr__
		return 'name = %s' % self.name

	
>>> student = Student1('zzh')
>>> 
>>> student
name = zzh
```

可以看出一般来说__str__和__repr__ 中的代码基本上都是一样的。所以有一个简写的方法

```python
>>> class Student(object):
	def __init__(self,value):
		self.name = value
	def __str__(self):
		return 'name = %s' % self.name
	__repr__ = __str__  #简写 

	
>>> student = Student('zzh')
>>> student
name = zzh
```

解释：上面的简写可以理解为 使__repr__的引用指向__str__函数，这样输入对象名的时候会自动调用__repr__，__repr__又指向着__str___，最终执行__str__函数的内容。

## __iter__ 和 __next__

之前我们知道list、tuple、dict等可以被for循环迭代，那如果我们想自己写的类也可以被for循环迭代，我们需要实现__iter__ 和 __next__这两个方法。注意，python中的for相当于java中的foreach，都是内部调用next方法。

```python
>>> class Fib2(object):
	def __init__(self):
		self.a, self.b = 0, 1
	def __iter__(self): #iter函数返回一个被迭代的对象
		return self
	def __next__(self):
		self.a, self.b = self.b, self.a + self.b
		if self.a > 1000:
			raise StopIteration()
		return self.a   #迭代的返回值

	
>>> for n in Fib2():
	print(n)

	
1
1
2
3
5
8
13
21
34
55
89
144
233
377
610
987
```

我们再来看一下java中是如何使自定义类可以被foreach迭代的

一：实现Iterable接口

```java
import java.util.Iterator;

import com.neuedu.pojo.Brand;

public class T implements Iterable<String>{ //实现Iterable接口

	String[] str = {"1","2","3","4","5"};

	@Override
	public Iterator<String> iterator() {   //重写iterator方法
		// TODO Auto-generated method stub
		return new Iterator<String>() {  //要求返回Iterator对象，我们就new一个
			
			int cursor = 0;
			@Override
			public boolean hasNext() {  //new Iterator 需要重写里面的两个方法
				// TODO Auto-generated method stub
				return cursor < str.length; //当计数器的值大于数组长度了，代表后面没有值了
			}

			@Override
			public String next() {
				// TODO Auto-generated method stub
				return str[cursor++]; //获取当前索引位对应的值，然后再更改索引位
			}
			
		};
	}
	
	
	
	public static void main(String[] args) {
		
		T t = new T();
		for (String string : t) {
			System.out.println(string);
		}
		
	}

}
```

二：实现Iterator接口

```java
import java.util.Iterator;

public class T2 implements Iterator<String>{ //这个就更直接了

	String[] str = {"1","2","3","4","5"};

	int cursor = 0;
	
	@Override
	public boolean hasNext() {
		// TODO Auto-generated method stub
		return cursor < str.length;
	}

	@Override
	public String next() {
		// TODO Auto-generated method stub
		return str[cursor++];	
	}
	
	
	public static void main(String[] args) {
		
		T t = new T();
		for (String string : t) {
			System.out.println(string);
		}
	}

}
```

## __getitem__

我们知道，一个tuple，或者一个list，我们都可以通过 中括号加索引位 的方式获取到某个值

```python
>>> (1,2,3,4,5)[3]
4
```

其实  像[3] 这种写法它默认调用的是 该类中的  __getitem__函数，对应java中可以理解为集合中的 get(index) 方法。只不过在java中我们获取集合中某个索引位上的元素需要使用get方法，而python更加简单，我们只需要使用中括号加索引位的方式，它就会自动的调用__getitem__，无需我们手动的去调用。

其实通过__getitem__ 还可以引申出__setitem__、__delitem__、__len__ 等方法，往往都是配套使用的

下面演示这几个函数如何使用

```python
>>> class Test():
	def __init__(self):
		self.value = {}
	def __setitem__(self,key,value):
		print('调用setitem函数')
		self.value[key] = value
	def __getitem__(self,key):
		print('调用getitem函数')
		return self.value[key]
	def __delitem__(self,key):
		print('调用delitem函数')
		del self.value[key]
	def __len__(self):
		print('调用len函数')
		return len(self.value)

	
>>> t = Test()
>>> t['1'] = '1'
调用setitem函数
>>> t['2'] = '2'
调用setitem函数
>>> t['1']
调用getitem函数
'1'
>>> t['2']
调用getitem函数
'2'
>>> len(t)
调用len函数
2
>>> del t['1']
调用delitem函数
>>> len(t)
调用len函数
1
>>> t['1']
调用getitem函数
Traceback (most recent call last):
  File "<pyshell#27>", line 1, in <module>
    t['1']
  File "<pyshell#18>", line 9, in __getitem__
    return self.value[key]
KeyError: '1'
>>> t['2']
调用getitem函数
'2'
```

注意：python中还有一个特点就是切片，形如：

```python
>>> (1,2,3,4,5,6)[1:4]
(2, 3, 4)
```

所以我们在 编写__getitem__函数的时候也应将这种情况考虑进去

下面演示如何使自定义类可以通过中括号加上索引位来获取值

```python
>>> class Fib(object):
	def __getitem__(self,n):
		if isinstance(n,int): #如果输入的形如[3]
			a, b = 1, 1
			for x in range(n):
				a, b = b, a + b
			return a
		if isinstance(n,slice): #如果输入的形如[1:4]
			start = n.start  #取出冒号前的数
			stop = n.stop #取出冒号后的数
			if start is None: #如果冒号前没写数
				start = 0 #将其置为0
			a, b = 1, 1
			L = []
			for x in range(stop): #遍历 冒号后面的数字 那么多次
				if x >= start: #小于起始位之前，不要
					L.append(a)  #大于的放入列表
				a, b = b, a + b  #生成费波纳茨序列的下一个值
			return L

		
>>> Fib()[3:6]
[3, 5, 8]
```

## __getattr__

我们通过对象名.属性名的方式获取值，如果我们所调用的属性不存在，就会执行 __getattr__ 函数，这个函数我内置的，如果我们想实现一些其他功能，可以重写这个函数，不重写默认会抛出异常。

```python
>>> class Student(object):
	def __init__(self):
		self.name = 'Michael'
	def __getattr__(self,attr): #重写__getattr__函数
		if attr == 'score':  #如果获取的是score属性，返回99
			return 99

		
>>> s = Student()
>>> s.name
'Michael'
>>> s.a   #由于我们重写了__getattr__，在函数中我们并没有 raise 异常，所以属性不存在时也不会报错
>>> s.b  
>>> s.c
>>> s.score
99
>>> Student.a    #但是类属性 还是会报错的
Traceback (most recent call last):
  File "<pyshell#44>", line 1, in <module>
    Student.a
AttributeError: type object 'Student' has no attribute 'a'
```

 现在我想让上面的 s.a、s.b、s.c 调用的时候都会报异常，即除了 调用name和score属性，其他的均会抛出异常

```python
>>> class Student(object):
	def __init__(self):
		self.name = 'Michael'
	def __getattr__(self,attr): #重写__getattr__函数
		if attr == 'score':  #如果获取的是score属性，返回99
			return 99
		raise AttributeError('您所输入的属性 \' %s \' 不存在' % attr)

	
>>> student = Student()
>>> student.name
'Michael'
>>> student.score
99
>>> student.a
Traceback (most recent call last):
  File "<pyshell#54>", line 1, in <module>
    student.a
  File "<pyshell#50>", line 7, in __getattr__
    raise AttributeError('您所输入的属性 \' %s \' 不存在' % attr)
AttributeError: 您所输入的属性 ' a ' 不存在
>>> student.b
Traceback (most recent call last):
  File "<pyshell#55>", line 1, in <module>
    student.b
  File "<pyshell#50>", line 7, in __getattr__
    raise AttributeError('您所输入的属性 \' %s \' 不存在' % attr)
AttributeError: 您所输入的属性 ' b ' 不存在
```

## __call__

我之前提过，python中一切皆对象。在python中，其实函数和通过类创建出来的对象没有什么区别，我们的函数 可以通过

函数名() 的方式去调用，其实我们通过类创建出来的对象也是如此。也可以通过   对象名()  等方式去调用，但是我们需要在类中实现 __call__ 函数，如下所示：

```python
>>> class Student(object):
    def __init__(self, name):
        self.name = name

    def __call__(self):
        print('My name is %s.' % self.name)

        
>>> student = Student('zzh')
>>> student() #注意在调用的时候是无需传入参数的
My name is zzh.
```

所以，可以看出我们既可以把函数看成对象，也可以把对象看成一个函数，其实没什么区别。

我们暂且先将 函数称为 对象，那么就会出现 有的对象能够调用，有的不能被调用(通过类创建出得对象且没有实现__call__函数)。

我们可以通过 callable() 函数，来判断一个对象是否能被调用

```python
>>> class Student(object):
    def __init__(self, name):
        self.name = name

    def __call__(self):
        print('My name is %s.' % self.name)

        
>>> student = Student('zzh')
>>> student()
My name is zzh.
>>> callable(student)
True
```

如果类中没有实现__call__函数，返回结果为false

如下：

```python
>>> class Student1(object):
    def __init__(self, name):
        self.name = name

        
>>> student1 = Student1()
>>> student1 = Student1('zzh')
>>> callable(student1)
False
```

我们再来看看一些其他的常用的内置对象

list列表：

```python
>>> callable([1,2,3,4,5])
False
>>> [1,2,3,4,5]()
Traceback (most recent call last):
  File "<pyshell#11>", line 1, in <module>
    [1,2,3,4,5]()
TypeError: 'list' object is not callable
```

dict：

```python
>>> {'1':'a','2':'b'}()
Traceback (most recent call last):
  File "<pyshell#13>", line 1, in <module>
    {'1':'a','2':'b'}()
TypeError: 'dict' object is not callable
```

还有一些常用的函数

```python
>>> callable(max)
True
>>> callable(len)
True
>>> callable(isinstance)
True
>>> callable(callable)
True
```


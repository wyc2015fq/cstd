# python学习笔记(十三) -- 多重继承和MixIn，以及@property的使用 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年08月17日 20:02:07[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：82
所属专栏：[python学习笔记](https://blog.csdn.net/column/details/26223.html)



## 多重继承

一个类继承好几个类

下面 类C 继承A又继承B 

```python
>>> class A(object):
	name = 'zzh'
>>> class B():
	age = 22
	
>>> class C(A,B):
	pass
	
>>> C.age
22
>>> C.name
'zzh'
```

但是写代码不仅仅是写代码，而是把现实的东西映射到代码上。

像这种多重继承会导致逻辑非常混乱，就比如说每个人都只有一个亲爹，你要非任个干爹，想从人那得到点好处，那关系再怎么好他也不是亲爹啊。

体现到代码上，上面的类A是类C的爹，这类C又看上类B拥有的age属性了，自己也想要，然后就任人当干爹，既然是干爹那你用继承就不合适，尤其是这种东西业务复杂以后会非常混乱，所以python中 出现了MaxIn。

## MixIn   -- 类似于java中的Interface

当我们想给一个类附加一些别的类拥有的功能，且这两个类在现实逻辑中并没有包含关系，则我们往往在被继承的类的名字后面加上MixIn，用来标明继承该类可以获取一些功能，但这两个类又没有父子关系。

如下

张三是张四的爹，张三只会吃，所以他儿子张四也不咋地，就会吃

```python
>>> class zhangsan(object):
	def eat(self):
		print('吃')

>>> 
		      
>>> class zhangsi(zhangsan):
		      pass

		      
>>> zhangsi = zhangsi()
		      

>>> zhangsi.eat()
		      
吃
```

后来这张四上学了，也意识到自己这样不行了，就开始努力学习了。所以它要掌握学习这项技能

```python
>>> class BookMixIn(object):
	def learn(self):
		print('学习')
>>> class zhangsi(zhangsan,BookMixIn):
		      pass
>>> zhangsi = zhangsi()
		      
>>> zhangsi.eat()
		      
吃
>>> zhangsi.learn()
		      
学习
```

由于学习这是一项技能，张三和学习 并没有父子关系，所以显然说是继承不符合逻辑，于是我们在Book这个类名后面加上MixIn作为一个功能，且同时与继承的逻辑区分开来。

## @property

回顾一下，通过一个set方法给属性设值相对于对象名.属性名给属性赋值的好处：

我们可以避免垃圾数据在产生，我们可以在set函数体内，通过if条件判断，输入的值是否符合我们的需求。

但是从另一方面来说，通过set函数赋值 以及 通过get方式取值显然没有直接通过对象名.属性名 方便。

好在python 提供了 @property 

@property的作用在于可以使类中获取和设置属性值的get、set方法，通过类名.属性名的形式去调用。

它实际上就是一个装饰器，如果在java中我们可以称其为 动态代理。spring中的 aop。

下面写个小案例体会一下：

```python
>>> class Student(object):   
	def get_score(self):
		return self.score
	def set_score(self,value):
		if not isinstance(value,int):
			raise ValueError("请输入一个整数！")
		elif value < 0 or value > 100:
			raise ValueError("分数必须在1~100之间！")
		self.score = value

		
>>> s = Student()
>>> s.set_score(99)
>>> s.get_score()
99
```

```python
>>> class Student(object):
	@property    #在原有的get方法上面加上 @property
	def score(self):
		return self._score  #注意由于方法名为score，所以属性名一定不要与方法名重名，否则会进入无限递归
	@score.setter  #在原有的set方法上面加上  方法名.setter ，注意原来的get和set方法现在名字一样都为score
	def score(self,value):  
		if not isinstance(value,int):
			raise ValueError("请输入一个整数！")
		elif value < 0 or value > 100:
			raise ValueError("分数必须在1~100之间！")
		self._score = value

		
>>> s = Student()
>>> s.score = 100  #通过 对象名 . 属性名 直接赋值
>>> s.score   #通过 对象名 . 属性名 获取属性值
100
```


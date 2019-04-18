# python学习笔记(十)  -- 继承、方法的重写、多态 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年08月15日 22:07:12[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：106
所属专栏：[python学习笔记](https://blog.csdn.net/column/details/26223.html)



## 继承

假设有一个类People里面有 eat()、sleep()、work() 这三个函数

现在又定义一个类Bob 我们让Bob去继承People这个类，我们在Bob这个类中不去定义任何函数，然我们任然可以通过Bob的实例去调用eat()、sleep()、work() 这三个函数。其中Bob称为子类，People称为父类。

如果我们这时通过 isinstance函数来判断会发现，Bob既是Bob类型的对象，也是People类型的对象，也就是子类的实例算是父类的对象，但是如果反过来，并不成立。

```python
>>> isinstance(bob,People)
True
>>> isinstance(bob,object)  #这里看出来，孙子的实例也可以是爷爷的对象
True
>>> people = People()
>>> isinstance(people,People)
True
>>> isinstance(people,object)
True
>>> isinstance(people,Bob) # 反过来不成立
False
```

## 方法的重写 --建立在继承的基础之上

还是拿People和Bob这两个类来说，People的eat()方法中 打印了输出语句 print("吃2个包子")，Bob继承了People，但是Bob认为这People中的 eat()函数 吃的太少，他想吃五个，于是在自己类即Bob这个类内部又定义了一个叫 eat()的函数，然后在函数内部打印了print("吃5个包子")，这时我们再通过Bob的实例去调用eat函数的时候，就会输出 "吃5个包子" 了 ，这就叫方法的重写。

```python
>>> class People(object):
	def eat(self):    # 这里要注意，类中的函数必须有self这个参数，固定的，否则调用的时候会报错
		print('吃2个包子')

		
>>> class Bob(People):
	pass

>>> bob = Bob()
>>> bob.eat()
吃2个包子
>>> class Bob(People):
	def eat(self):
		print('吃5个包子')

		
>>> bob = Bob()
>>> bob.eat()
吃5个包子
```

## 多态

其实在python中多态并没有太严格的约束，在java中多态往往指的是用父类的变量去指向子类的实例，但是python的变量并没有绑定一个明确的数据类型。

至于python多态的体现举出个例子来说或许更加明确

继续上面的代码

我们再写一个吃两顿的方法

```python
>>> def double(People):
	People.eat()
	People.eat()
```

我们调用这个方法传一个People实例

```python
>>> double(People())
吃2个包子
吃2个包子
```

传一个Bob实例

```python
>>> double(Bob())
吃5个包子
吃5个包子
```

我们创建一个Kiki类，然后调用double方法将Kiki实例传进去

```python
class Kiki(People):
	def eat(self):
		print('吃1个包子')
>>> double(Kiki())
吃1个包子
吃1个包子
```

这就是多态，一个方法多个不同类型的实例均可调用，提高代码的可扩展性。

但是，我上面也说了，这python并不像java对多态有明确的限制，如果在java中，double函数的参数是要明确数据类型的，比如我参数类型为People，那我只能在调用的时候传People或People子类的实例进来，但是python中并没有这种约束。

那上面的double函数来说，只要我传的这个实例，它的类在定义的时候有run这个方法就行

```python
>>> class Zzh(object):    #这里并没有继承People
	def eat(self):
		print('吃3个包子')

		
>>> double(Zzh())
吃3个包子
吃3个包子
```

所以说python中的可扩展性比java中的更强


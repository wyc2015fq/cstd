# python学习笔记(九) -- 类、实例、私有属性 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年08月15日 21:10:01[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：51标签：[类																[实例																[私有属性](https://so.csdn.net/so/search/s.do?q=私有属性&t=blog)](https://so.csdn.net/so/search/s.do?q=实例&t=blog)](https://so.csdn.net/so/search/s.do?q=类&t=blog)
个人分类：[python基础](https://blog.csdn.net/weixin_40247263/article/category/7905597)

所属专栏：[python学习笔记](https://blog.csdn.net/column/details/26223.html)


## 如果接触过java这块非常容易理解

## 类  -- 对事物或生物的抽象是一个模板

类里面有属性，有方法，打个比方。类代表人类，类里面的属性就好比人的五官，四肢，就是每个人都会有，方法就好比人的动作，比如吃饭，喝水。但是为什么说类是一个模板的，因为人有一些共有的东西，就像上面说的，但是每个人又不一样，比如人的模样，身材，吃的多少等等。

## 实例  -- 通过类可以创建实例

通过一个类可以创建出一个实例，实例就代表具体的每一个人，创建实例的时候我们可以给里面的属性赋值。

## 私有属性

属性是在类的内部定义的，我们创建一个类的实例，就可以调用到类的属性，有时候我们不想让别人通过类的实例获取或修改类的属性，我们则需要将属性私有化，方法就是在属性名前面加上两个下划线。

也有一种情况，就是我们怕别人通过  实例.属性名  = 某个值  这种方式给实例的属性传递一些垃圾数据，我们通过将属性私有化，然后定义一个函数，调用者通过调用这个函数可以给属性赋值，但是在函数内部会写一些业务逻辑，来防止垃圾数据的注入。

下面用代码来说话

```python
#类的定义
>>> class Student(object):  #定义一个类Student 小括号代表Student继承的类，一般没有要继承的类都写Object，所有的都会继承Object
	def __init__(self,name,score): #这个函数名必须固定，用来给类里面的属性赋值，类似于java的有参构造方法，其中self也是固定的，代表当前创建实的引用
		self.name = name  #python类中的变量不是像java中那样单独定义出来的，而是通过这种形式进行绑定（self.属性名）
		self.score = score

#对象的创建		
>>> bart = Student('Bart Simpson',59)#这样就可以创建出一个实例，bart为实例的引用
>>> bart.name  #实例.属性名 就可以获得属性值
'Bart Simpson'
>>> bart.score
59
>>> class Student(object): #在类中也可以定义一个普通函数
	def __init__(self,name,score): #这是一个绑定属性的函数
		self.name = name
		self.score = score
	def toString(self):  #普通函数，我们让它打印出类的属性值
		print("name : %s , score: %d" % (self.name,self.score))

		
>>> 
>>> a = Student('zzh',100) 
>>> a.toString()   #调用方法
name : zzh , score: 100
>>> a.name
'zzh'
>>> a.score
100

#私有属性
>>> class Student(object):  #如果我们不想让类的调用者直接拿到类的属性，我们只需要在属性名前加上两个下划线
	def __init__(self,name,score):
		self._name = name  #name前面我加了一个下划线为了测试
		self.__score = score  #score前面加了两个下划线 
	def toString(self):
		print("name : %s , score: %d" % (self._name,self.__score))

>>> a = Student('zzh',100)
>>> a._name  #加了一个下划线的属性还是可以直接被调用到
'zzh'
>>> a.__score #加了两个下划线的调用会报错，提示没有这个属性
Traceback (most recent call last):
  File "<pyshell#20>", line 1, in <module>
    a.__score
AttributeError: 'Student' object has no attribute '__score'
>>> a._Student__score #但其实也可以通过这种方式调用到，实际上python将变量名前面加上了_类名，但是每个版本的python更改名字的方式并不是都一样
100
>>> a.__score = 10 #这里看似可以给私有属性赋值
>>> a.toString()  #打印出来的时候发现属性值并未改变，还是上面说的，这个score实际上名字已经变为_Student__score 了，我们上面只是在类外部又给一个实例绑定了一个属性
name : zzh , score: 100
>>> a.__score #通过打印可以验证上面所说的
10


#下面演示通过 设置私有变量 然后提供一个方法来修改变量 达到防止脏数据注入的目的
class Student(object):
	def __init__(self,name,score):
		self.__name = name
		self.__score = score
	def toString(self):
		print("name : %s , score: %d" % (self.__name,self.__score))
	def set_score(self, score):
		if 0 <= score <= 100:
		    self.__score = score
		else:
		    raise ValueError('bad score')

		
>>> a = Student('zzh',100)
>>> a.toString()
name : zzh , score: 100
>>> a.set_score(1111)
Traceback (most recent call last):
  File "<pyshell#34>", line 1, in <module>
    a.set_score(1111)
  File "<pyshell#31>", line 11, in set_score
    raise ValueError('bad score')
ValueError: bad score
```



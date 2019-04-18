# python学习笔记(十一) -- 实例属性和类属性、__init__（构造方法）、MethodType函数   与 java对比分析 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年08月16日 14:52:31[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：178
所属专栏：[python学习笔记](https://blog.csdn.net/column/details/26223.html)



## 实例属性   -- java中的全局变量

实例属性是给一个类的实例绑定一个属性

其中__init__方法对应的就是java中的有参构造函数，其中的第一个参数是固定的，必须为self，只当前实例本身，对应java中的this，在__init__方法中我们可以给实例绑定属性。即java中的有参构造函数对全局变量赋初始值。

我们通过 People(fit) 就可以创建一个实例，通过实例.fit 就可以取到 该实例属性的值。

```python
>>> class People(object):   #定义一个类
	def __init__(self,fit):  #当我们创建对象时会自动调用这个方法，在java中被称为有参构造方法
		self.fit = fit
```

## 类属性  -- java中被static 修饰的属性

类属性可以通过类名 . 属性名 直接调用。也可以通过实例 . 属性名 进行调用，但是 java中和 python还是有区别的

首先我们先看一下java

```java
public class B {  //定义一个类

	static String age = "15";   //静态属性，即类属性
	
	String name = "zzh";  //全局属性
}
```

测试

```java
public class T {

	public static void main(String[] args) {
		
		B b = new B();   //创建一个B类的 实例
		System.out.println(B.age); //类名．属性名　调用
		System.out.println(b.age); //实例名.属性名 调用
		
		//通过实例更改静态属性
		b.age = "99";  //通过实例修改 类属性
		System.out.println(B.age);  //输出结果为 99
		System.out.println(b.age);  //输出结果为 99

		//通过类更改静态属性
		B.age = "66";
		System.out.println(B.age); //输出结果为 66
		System.out.println(b.age); //输出结果为 66		
	}
}
```

事实证明 java中 可以通过 实例去修改 类属性的值，且 实例 . 属性名 和 类 . 属性名 无论是获取值还是 修改值均操作的是同一个对象。

我们再来看看python

```python
>>> class B(object):
	static_name = "zzh"  #这是一个类属性

	 
>>> b = B()  # 创建一个实例
>>> B.static_name  #通过类名 调用 类属性
'zzh'
>>> b.static_name  #通过实例 调用 类属性
'zzh'
>>> b.static_name = 'kiki'  #通过实例 修改类属性(实际上并没有修改类属性)
>>> b.static_name   #我们发现确实是我们修改后的值
'kiki'
>>> B.static_name   #但是注意这里，我们通过 类名调用 类属性时 发现并没有改变
'zzh'
>>> B.static_name = 'haha'  # 我们这次通过 类名 来修改类属性(这次是真的修改的类属性)
>>> B.static_name  #发现是我们修改后的值
'haha'
>>> b.static_name  #再通过实例调用类属性，发现没有变化
'kiki'
```

事实证明，在python中，通过 实例 . 类属性名  =  "xxx" 的方式，并不能够去修改类属性的值，而是给这个实例 新 绑定了一个 实例属性，只不过这个实例 属性 和 类属性的名字一样，并且实例属性的优先级要比类属性的优先级要高，所以我们通过  实例 . 类属性名 获得的实际上是 一个 实例属性。

通过 下面的代码，可以得到论证

```python
>>> del b.static_name  #我删除 实例b 的static_name 这个属性
>>> b.static_name  # 再次调用
'haha'   #这个值 就是我们上面的类属性的值了
```

## __init__   --  java中的构造方法

java中 构造方法分为有构造方法和无参构造方法，甚至java中可以由于参数列表不同，有多个有参构造方法同时存在。但是在python中的，只能存在一个__init__方法，但是注意，python中的变量是不用声明类型的，并且python的参数有默认参数，所以说即使python只能存在一个__init__方法，但实际上它比java更加灵活，且简洁。

有一个需要注意的点：

在java 创建一个子类对象，是要调用父类的构造方法的，默认调用无参构造方法，无参构造方法也不用显示的写出来，它默认就存在，只不过默认的无擦构造方法在方法体内没有做任何的操作，但是一旦 父类 存在有参构造方法，如果没有显式的写出无参构造方法，那么这个无参构造方法就不存在了。这时子类 必须要在自己的有参构造方法中去调用父类的有参构造。要不然java并不知道如何去创建一个子类的实例。但是在python中，这种情况下在子类中并不需要手动去调用父类的有参构造（__init__  函数），但是不用手动调用并不代表它创建子类实例的时候不会去调用父类的__init__方法。 

先看java代码

```java
public class B {
	
	String name;
	int age;
	
	public B(String name){  //有两个有参构造函数
		this.name = name;
	}
	
	public B(int age){
		this.age = age;

	}

}
```

```java
public class A extends B{

	public A(int age) {  
		super(age);  //必须手动调用 否则会报错
	}

}

public class A extends B{

	public A(String name) {
		super(name);   //调用B中的哪个有参构造都可以，主要目的就是告诉java如何去创建一个子类实例
	}

}
```

再看 python

```python
>>> class A(object):   #定一个类 A
	def __init__(self,name):  #通过 __init__函数给实例绑定一个name属性
		self.name = name

		
>>> 
>>> class B(A):   # B 类 集成 A 类
	pass

>>> b = B()  # 创建一个实例
Traceback (most recent call last):   #报错，告诉我们少传了一个参数 name
  File "<pyshell#177>", line 1, in <module>
    b = B()
TypeError: __init__() missing 1 required positional argument: 'name'  
>>> b = B('zzh')   #传 一个name
>>> b.name   #调用
'zzh'  #正常 输出
```

上面 说明的 问题就是，python中子类无需手动 调用父类的 __init__方法，但是在创建子类实例的时候，会默认调用父类的__init__方法。

还有一点要说的，如果子类 拥有自己的 __init__方法，那么它将不会去调用父类的__init__方法，但同时，子类无法调到父类的 实例属性了，因为父类实例属性是通过父类的__init__方法来绑定的 。但是却可以调用到父类的普通函数，以及类属性

```python
>>> class A(object):
	def __init__(self,name):
		self.name = name
	age = 20
	def toString():
		print('这是一个toString函数')

		
>>> class B(A):
	def __init__(self,age):  #子类有自己的 __init__函数
		self.age = age

		
>>> b = B()
Traceback (most recent call last):
  File "<pyshell#195>", line 1, in <module>
    b = B()
TypeError: __init__() missing 1

>>> b = B(30)  
>>> b.name   #调用不到父类的实例属性 name
Traceback (most recent call last):
  File "<pyshell#202>", line 1, in <module>
    b.name
AttributeError: 'B' object has no attribute 'name' 
>>> b.age
30
>>> B.age  #但是可以调用到父类的类属性
20
>>> B.toString()  #也可以调用到父类的方法
这是一个toString函数
```

其实python最具备特点的还是在于它是一个动态语言

体现就在于 我们定义好了一个类，但是却可以 在外部 动态的给 类或对象 添加属性，甚至方法，这个在java中是肯定不允许的，java中我们一旦定义好了一个类，在代码运行期间是无法给这个类或者说某个对象，动态的去添加属性的和方法的。

下面给出 动态语言特点的代码体现

```python
>>> class Student(object):  #定义个类，什么都不做
	pass

>>> student = Student()  # 创建一个实例
>>> 
>>> student.name = "zzh" # 动态的给student对象 绑定一个name属性并赋值 
>>> 
>>> student.name # 打印输出
'zzh'

>>> student1 = Student() # 再创建一个实例
>>> student1.name # 可以发现 对象绑定的属性作用于就是一个对象，不同对象之间并不通用
Traceback (most recent call last):
  File "<pyshell#242>", line 1, in <module>
    student1.name
AttributeError: 'Student' object has no attribute 'name'

# 动态添加一个 类属性，类属性是所有对象共享的
>>> Student.score = 100
>>> student.score
100
>>> student1.score
100

#下面我们给对象和类动态添加函数
>>> def set_score(self,score):  #定义一个修改分数的函数
	self.score = score
>>> from types import MethodType  #注意给对象绑定方法需要用到MethodType 函数
>>> student.set_score = MethodType(set_score,student) #给对象动态绑定一个方法
>>> student.set_score(99) # 测试
>>> student.score 
99
>>> student1.set_score(99) #同样，对象绑定的方法，不同对象之间并不共享
Traceback (most recent call last):
  File "<pyshell#253>", line 1, in <module>
    student1.set_score(99)
AttributeError: 'Student' object has no attribute 'set_score'

#下面我们给类动态绑定一个方法， 这个所有对象之间是可以共享的
>>> Student.set_score = set_score  #给类动态绑定方法，这样直接赋值就可以
>>> student.set_score(23)
>>> student.score
23
>>> student1.set_score(22)
>>> student1.score
22
```

总结：

        在java中，定义好一个类，通过这个类创建出来的对象，拥有的变量是一样的，拥有的方法也是一样的，且不允许在代码运行过程中，动态的添加属性以及方法。

        在python中，我们可以给任何一个对象，动态的去添加属性或方法，且它们之间是不共享的，也可以在运行过程当中动态的给类去添加属性和方法，类的属性和方法所有的对象均共享。

## MethodType函数

再解释一下 为什么给对象动态添加属性的时候需要使用 MethodType函数，而给类动态添加方法时却不用

看下面的代码

```python
>>> student.set_score = set_score  #我们不使用MethodType函数，来给对象动态添加方法
>>> student.set_score(50) #调用的时候报错 
Traceback (most recent call last):
  File "<pyshell#261>", line 1, in <module>
    student.set_score(50)
TypeError: set_score() missing 1 required positional argument: 'score' #说是我们没有传score这个参数
```

调用的时候报错，说是我们没有传score这个参数，但是实际上我们传了，所以只能理解为，python把 我传递的参数 50 当成了 set_score 方法中的 形参 的 self，因为只有在声明一个类的时候，在类里面生成的方法在调用时才会自动的将 当前实例对象本身传递进去，我们以这种动态的形式给对象添加方法，在调用的时候它找不到的，MethodType函数就为我们解决了这个问题。


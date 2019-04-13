
# Python(私有变量)类中的特殊方法 - .NET博文收藏 - CSDN博客


2008年06月10日 12:46:00[hejishan](https://me.csdn.net/hejishan)阅读数：1095标签：[python																](https://so.csdn.net/so/search/s.do?q=python&t=blog)[正则表达式																](https://so.csdn.net/so/search/s.do?q=正则表达式&t=blog)[destructor																](https://so.csdn.net/so/search/s.do?q=destructor&t=blog)[constructor																](https://so.csdn.net/so/search/s.do?q=constructor&t=blog)[object																](https://so.csdn.net/so/search/s.do?q=object&t=blog)[文档																](https://so.csdn.net/so/search/s.do?q=文档&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=object&t=blog)[
																															](https://so.csdn.net/so/search/s.do?q=constructor&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=destructor&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=destructor&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=正则表达式&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=python&t=blog)
原帖地址：[http://www.devfront.com:8080/?q=node/199](http://www.devfront.com:8080/?q=node/199)
## Python学习备忘录
Wed, 2006-10-11 08:14 — Marchday**类中的特殊方法**
一般说来，特殊的方法都被用来模仿某个行为。例如，如果你想要为你的类使用x[key]这样的索引操作（就像列表和元组一样），那么你只需要实现__getitem__()方法就可以了。想一下，Python就是对list类这样做的！
下面这个表中列出了一些有用的特殊方法。如果你想要知道所有的特殊方法，你可以在《Python参考手册》中找到一个庞大的列表。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)名称                     说明
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)---------------------------------------------------------
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)__init__(self,...)      这个方法在新建对象恰好要被返回使用之前被调用。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)__del__(self)           恰好在对象要被删除之前调用。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)__str__(self)           在我们对对象使用print语句或是使用str()的时候调用。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)__lt__(self,other)      当使用 小于 运算符（<）的时候调用。类似地，对于所有的运算符（+，>等等）都有特殊的方法。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)__getitem__(self,key)   使用x[key]索引操作符的时候调用。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)__len__(self)           对序列对象使用内建的len()函数的时候调用。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)__repr__(s)             repr()and`...` conversions
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)__cmp__(s, o)           Compares s to oandreturns<0, 0,or>0.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)Implements>,<,==etc...
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)__hash__(s)             Compute a32bit hash code; hash()anddictionary ops
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)__nonzero__(s)          Returns 0or1fortruth value testing
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)__getattr__(s, name)    called when attr lookup doesn't find
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)__setattr__(s, name, val) called when setting an attr
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)(inside, don't use "self.name = value"
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)use"self.__dict__[name] = val")
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)__delattr__(s, name)    called to delete attr<name>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)__call__(self,*args)   called when an instanceiscalled as function.
**exec和eval语句**
exec语句用来执行储存在字符串或文件中的Python语句。例如，我们可以在运行时生成一个包含Python代码的字符串，然后使用exec语句执行这些语句。
下面是一个简单的例子。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)>>>exec'print "Hello World"'
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)Hello World
eval语句用来计算存储在字符串中的有效Python表达式。下面是一个简单的例子。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)>>>eval('2*3')
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)6
**assert语句**
assert语句用来声明某个条件是真的。例如，如果你非常确信某个你使用的列表中至少有一个元素，而你想要检验这一点，并且在它非真的时候引发一个错误，那么assert语句是应用在这种情形下的理想语句。当assert语句失败的时候，会引发一个AssertionError。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)>>>mylist=['item']
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)>>>assertlen(mylist)>=1
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)>>>mylist.pop()
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)'item'
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)>>>assertlen(mylist)>=1
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)Traceback (most recent call last):
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)File"", line1,in?
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)AssertionError repr函数
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
repr函数用来取得对象的规范字符串表示。反引号（也称转换符）可以完成相同的功能。注意，在大多数时候有eval(repr(object)) == object。![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)>>>i=[]
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)>>>i.append('item')
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)>>>`i`
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)"['item']"
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)>>>repr(i)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)"['item']"
基本上，repr函数和反引号用来获取对象的可打印的表示形式。你可以通过定义类的__repr__方法来控制你的对象在被repr函数调用的时候返回的内容。
**类和实例变量**
有两种类型的域 —— 类的变量和对象的变量，它们根据是类还是对象拥有这个变量而区分。
类的变量 由一个类的所有对象（实例）共享使用。只有一个类变量的拷贝，所以当某个对象对类的变量做了改动的时候，这个改动会反映到所有其他的实例上。
对象的变量 由类的每个对象/实例拥有。因此每个对象有自己对这个域的一份拷贝，即它们不是共享的，在同一个类的不同实例中，虽然对象的变量有相同的名称，但是是互不相关的。通过一个例子会使这个易于理解。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#!/usr/bin/python
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#Filename: objvar.py
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)classPerson:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)'''Represents a person.'''
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)population=0
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)def__init__(self, name):
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)'''Initializes the person's data.'''
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)self.name=name
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)print'(Initializing %s)'%self.name
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#When this person is created, he/she
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#adds to the population
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)Person.population+=1
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)def__del__(self):
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)'''I am dying.'''
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)print'%s says bye.'%self.name
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)Person.population-=1
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)ifPerson.population==0:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)print'I am the last one.'
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)else:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)print'There are still %d people left.'%Person.population
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)defsayHi(self):
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)'''Greeting by the person.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)Really, that's all it does.'''
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)print'Hi, my name is %s.'%self.name
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)defhowMany(self):
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)'''Prints the current population.'''
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)ifPerson.population==1:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)print'I am the only person here.'
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)else:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)print'We have %d persons here.'%Person.population
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)swaroop=Person('Swaroop')
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)swaroop.sayHi()
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)swaroop.howMany()
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)kalam=Person('Abdul Kalam')
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)kalam.sayHi()
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)kalam.howMany()
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)swaroop.sayHi()
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)swaroop.howMany()输出
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)___FCKpd___5nbsp;python objvar.py
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)(Initializing Swaroop)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)Hi, my nameisSwaroop.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)I am the only person here.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)(Initializing Abdul Kalam)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)Hi, my nameisAbdul Kalam.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)We have2persons here.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)Hi, my nameisSwaroop.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)We have2persons here.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)Abdul Kalam says bye.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)There are still1people left.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)Swaroop says bye.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)I am the last one.
这是一个很长的例子，但是它有助于说明类与对象的变量的本质。这里，population属于Person类，因此是一个类的变量。name变量属于对象（它使用self赋值）因此是对象的变量。
观察可以发现__init__方法用一个名字来初始化Person实例。在这个方法中，我们让population增加1，这是因为我们增加了一个人。同样可以发现，self.name的值根据每个对象指定，这表明了它作为对象的变量的本质。
记住，你只能使用self变量来参考同一个对象的变量和方法。这被称为 属性参考 。
在这个程序中，我们还看到docstring对于类和方法同样有用。我们可以在运行时使用Person.__doc__和Person.sayHi.__doc__来分别访问类与方法的文档字符串。
就如同__init__方法一样，还有一个特殊的方法__del__，它在对象消逝的时候被调用。对象消逝即对象不再被使用，它所占用的内存将返回给系统作它用。在这个方法里面，我们只是简单地把Person.population减1。
当对象不再被使用时，__del__方法运行，但是很难保证这个方法究竟在什么时候运行。如果你想要指明它的运行，你就得使用del语句，就如同我们在以前的例子中使用的那样。
给C++/Java/C\#程序员的注释
Python中所有的类成员（包括数据成员）都是公共的 ，所有的方法都是有效的。
只有一个例外：如果你使用的数据成员名称以双下划线前缀 比如__privatevar，Python的名称管理体系会有效地把它作为私有变量。
这样就有一个惯例，如果某个变量只想在类或对象中使用，就应该以单下划线前缀。而其他的名称都将作为公共的，可以被其他类/对象使用。记住这只是一个惯例，并不是Python所要求的（与双下划线前缀不同）。
同样，注意__del__方法与destructor的概念类似。
**继承**
面向对象的编程带来的主要好处之一是代码的重用，实现这种重用的方法之一是通过继承机制。继承完全可以理解成类之间的类型和子类型关系。
假设你想要写一个程序来记录学校之中的教师和学生情况。他们有一些共同属性，比如姓名、年龄和地址。他们也有专有的属性，比如教师的薪水、课程和假期，学生的成绩和学费。
你可以为教师和学生建立两个独立的类来处理它们，但是这样做的话，如果要增加一个新的共有属性，就意味着要在这两个独立的类中都增加这个属性。这很快就会显得不实用。
一个比较好的方法是创建一个共同的类称为SchoolMember然后让教师和学生的类继承 这个共同的类。即它们都是这个类型（类）的子类型，然后我们再为这些子类型添加专有的属性。
使用这种方法有很多优点。如果我们增加/改变了SchoolMember中的任何功能，它会自动地反映到子类型之中。例如，你要为教师和学生都增加一个新的身份证域，那么你只需简单地把它加到SchoolMember类中。然而，在一个子类型之中做的改动不会影响到别的子类型。另外一个优点是你可以把教师和学生对象都作为SchoolMember对象来使用，这在某些场合特别有用，比如统计学校成员的人数。一个子类型在任何需要父类型的场合可以被替换成父类型，即对象可以被视作是父类的实例，这种现象被称为多态现象。
另外，我们会发现在重用父类的代码的时候，我们无需在不同的类中重复它。而如果我们使用独立的类的话，我们就不得不这么做了。
在上述的场合中，SchoolMember类被称为基本类或超类 。而Teacher和Student类被称为导出类或子类 。
现在，我们将学习一个例子程序。
使用继承
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#!/usr/bin/python
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)\#Filename: inherit.py
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)classSchoolMember:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)'''Represents any school member.'''
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)def__init__(self, name, age):
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)self.name=name
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)self.age=age
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)print'(Initialized SchoolMember: %s)'%self.name
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)deftell(self):
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)'''Tell my details.'''
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)print'Name:"%s" Age:"%s"'%(self.name, self.age),
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)classTeacher(SchoolMember):
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)'''Represents a teacher.'''
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)def__init__(self, name, age, salary):
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)SchoolMember.__init__(self, name, age)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)self.salary=salary
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)print'(Initialized Teacher: %s)'%self.name
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)deftell(self):
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)SchoolMember.tell(self)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)print'Salary: "%d"'%self.salary
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)classStudent(SchoolMember):
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)'''Represents a student.'''
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)def__init__(self, name, age, marks):
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)SchoolMember.__init__(self, name, age)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)self.marks=marks
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)print'(Initialized Student: %s)'%self.name
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)deftell(self):
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)SchoolMember.tell(self)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)print'Marks: "%d"'%self.marks
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)t=Teacher('Mrs. Shrividya',40,30000)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)s=Student('Swaroop',22,75)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)print\#prints a blank line
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)members=[t, s]
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)formemberinmembers:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)member.tell()\#works for both Teachers and Students输出
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)$ python inherit.py
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)(Initialized SchoolMember: Mrs. Shrividya)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)(Initialized Teacher: Mrs. Shrividya)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)(Initialized SchoolMember: Swaroop)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)(Initialized Student: Swaroop)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)Name:"Mrs. Shrividya"Age:"40"Salary:"30000"
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)Name:"Swaroop"Age:"22"Marks:"75"
为了使用继承，我们把基本类的名称作为一个元组跟在定义类时的类名称之后。然后，我们注意到基本类的__init__方法专门使用self变量调用，这样我们就可以初始化对象的基本类部分。这一点十分重要——Python不会自动调用基本类的constructor，你得亲自专门调用它。
我们还观察到我们在方法调用之前加上类名称前缀，然后把self变量及其他参数传递给它。
注意，在我们使用SchoolMember类的tell方法的时候，我们把Teacher和Student的实例仅仅作为SchoolMember的实例。
另外，在这个例子中，我们调用了子类型的tell方法，而不是SchoolMember类的tell方法。可以这样来理解，Python总是首先查找对应类型的方法，在这个例子中就是如此。如果它不能在导出类中找到对应的方法，它才开始到基本类中逐个查找。基本类是在类定义的时候，在元组之中指明的。
一个术语的注释——如果在继承元组中列了一个以上的类，那么它就被称作多重继承 。
**字符串**
字符串是字符的序列。字符串基本上就是一组单词。
我几乎可以保证你在每个Python程序中都要用到字符串，所以请特别留心下面这部分的内容。下面告诉你如何在Python中使用字符串。
* 使用单引号（'）
你可以用单引号指示字符串，就如同'Quote me on this'这样。所有的空白，即空格和制表符都照原样保留。
* 使用双引号（"）
在双引号中的字符串与单引号中的字符串的使用完全相同，例如"What's your name?"。
* 使用三引号（'''或"""）
利用三引号，你可以指示一个多行的字符串。你可以在三引号中自由的使用单引号和双引号。例如：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)'''This is a multi-line string. This is the first line.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)This is the second line.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)"What's your name?," I asked.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)He said "Bond, James Bond."
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)'''

* 转义符
假设你想要在一个字符串中包含一个单引号（'），那么你该怎么指示这个字符串？例如，这个字符串是What's your name?。你肯定不会用'What's your name?'来指示它，因为Python会弄不明白这个字符串从何处开始，何处结束。所以，你需要指明单引号而不是字符串的结尾。可以通过 转义符 来完成这个任务。你用/'来指示单引号——注意这个反斜杠。现在你可以把字符串表示为'What/'s your name?'。
另一个表示这个特别的字符串的方法是"What's your name?"，即用双引号。类似地，要在双引号字符串中使用双引号本身的时候，也可以借助于转义符。另外，你可以用转义符//来指示反斜杠本身。
值得注意的一件事是，在一个字符串中，行末的单独一个反斜杠表示字符串在下一行继续，而不是开始一个新的行。例如：
"This is the first sentence./
This is the second sentence."
等价于"This is the first sentence. This is the second sentence."
* 自然字符串
如果你想要指示某些不需要如转义符那样的特别处理的字符串，那么你需要指定一个自然字符串。自然字符串通过给字符串加上前缀r或R来指定。例如r"Newlines are indicated by /n"。
* Unicode字符串
Unicode是书写国际文本的标准方法。如果你想要用你的母语如北印度语或阿拉伯语写文本，那么你需要有一个支持Unicode的编辑器。类似地，Python允许你处理Unicode文本——你只需要在字符串前加上前缀u或U。例如，u"This is a Unicode string."。
记住，在你处理文本文件的时候使用Unicode字符串，特别是当你知道这个文件含有用非英语的语言写的文本。
* 字符串是不可变的
这意味着一旦你创造了一个字符串，你就不能再改变它了。虽然这看起来像是一件坏事，但实际上它不是。我们将会在后面的程序中看到为什么我们说它不是一个缺点。
* 按字面意义级连字符串
如果你把两个字符串按字面意义相邻放着，他们会被Python自动级连。例如，'What/'s' 'your name?'会被自动转为"What's your name?"。
给C/C++程序员的注释
在Python中没有专门的char数据类型。确实没有需要有这个类型，我相信你不会为此而烦恼。
给Perl/PHP程序员的注释
记住，单引号和双引号字符串是完全相同的——它们没有在任何方面有不同。
给正则表达式用户的注释
一定要用自然字符串处理正则表达式。否则会需要使用很多的反斜杠。例如，后向引用符可以写成'//1'或r'/1'。
**私有变量**
Python对私有类成员有部分支持。任何象__spam这样形式的标识符（至少有两个前导下划线，至多有一个结尾下划线）目前被替换成_classname__spam，其中classname是所属类名去掉前导下划线的结果。这种搅乱不管标识符的语法位置，所以可以用来定义类私有的实例、变量、方法，以及全局变量，甚至于保存对于此类是私有的其它类的实例。如果搅乱的名字超过255个字符可能会发生截断。在类外面或类名只有下划线时不进行搅乱。
名字搅乱的目的是给类一种定义“私有”实例变量和方法的简单方法，不需担心它的其它类会定义同名变量，也不怕类外的代码弄乱实例的变量。注意搅乱规则主要是为了避免偶然的错误，如果你一定想做的话仍然可以访问或修改私有变量。这甚至是有用的，比如调试程序要用到私有变量，这也是为什么这个漏洞没有堵上的一个原因。（小错误：导出类和基类取相同的名字就可以使用基类的私有变量）。
注意传递给exec，eval()或evalfile()的代码不会认为调用它们的类的类名是当前类，这与global语句的情况类似，global的作用局限于一起字节编译的代码。同样的限制也适用于getattr() ，setattr()和delattr()，以及直接访问__dict__的时候。
下面例子中的类实现了自己的__getattr__和__setattr__方法，把所有属性保存在一个私有变量中，这在Python的新旧版本中都是可行的：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)classVirtualAttributes:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)__vdict=None
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)__vdict_name=locals().keys()[0]
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)def__init__(self):
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)self.__dict__[self.__vdict_name]={}
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)def__getattr__(self, name):
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)returnself.__vdict[name]
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)def__setattr__(self, name, value):
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)self.__vdict[name]=value
**参考资料：**
http://www.e7blog.com/blog/user1/python/
http://www.czug.org/docs/python/TutorialCN/X_e7_ac_ac_e4_b9_9d_e7_ab_a0_e7_b1_bb/





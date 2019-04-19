# Python学习之路二---（基础知识） - BlueSky - CSDN博客
2015年10月26日 19:35:38[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：433标签：[python																[面向对象编程																[面向对象](https://so.csdn.net/so/search/s.do?q=面向对象&t=blog)](https://so.csdn.net/so/search/s.do?q=面向对象编程&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[Python学习](https://blog.csdn.net/ASIA_kobe/article/category/5896995)
## 面向对象编程
根据操作数据或者语句来设计程序，称之为**面向过程**的编程。把数据和功能结合起来的，用称之为对象的东西包裹起来组织程序的方法称之为**面向对象**的编程概念。 
类和对象是面向对象编程的两个主要的方面。类创建一个新类型，而对象则为类的实例。 
类使用class关键字创建。类的域和方法被列在一个缩进块中。 
属于一个对象或类的变量被称为域。对象也可以使用 属于 类的函数来具有功能。这样的函数被称为类的方法。这些术语帮助我们把它们与孤立的函数和变量区分开来。域和方法可以合称为类的属性。域有两种类型——属于每个实例/类的对象或属于类本身。它们分别被称为实例变量和类变量。
## 继承
面向对象的编程带来的主要好处之一是代码的重用，实现这种重用的方法之一是通过 继承 机制。继承完全可以理解成类之间的 类型和子类型 关系。
假设你想要写一个程序来记录学校之中的教师和学生情况。他们有一些共同属性，比如姓名、年龄和地址。他们也有专有的属性，比如教师的薪水、课程和假期，学生的成绩和学费。
你可以为教师和学生建立两个独立的类来处理它们，但是这样做的话，如果要增加一个新的共有属性，就意味着要在这两个独立的类中都增加这个属性。这很快就会显得不实用。
一个比较好的方法是创建一个共同的类称为SchoolMember然后让教师和学生的类 继承 这个共同的类。即它们都是这个类型（类）的子类型，然后我们再为这些子类型添加专有的属性。
使用这种方法有很多优点。如果我们增加/改变了SchoolMember中的任何功能，它会自动地反映到子类型之中。例如，你要为教师和学生都增加一个新的身份证域，那么你只需简单地把它加到SchoolMember类中。然而，在一个子类型之中做的改动不会影响到别的子类型。另外一个优点是你可以把教师和学生对象都作为SchoolMember对象来使用，这在某些场合特别有用，比如统计学校成员的人数。一个子类型在任何需要父类型的场合可以被替换成父类型，即对象可以被视作是父类的实例，这种现象被称为**多态现象**。 
另外，我们会发现在 重用 父类的代码的时候，我们无需在不同的类中重复它。而如果我们使用独立的类的话，我们就不得不这么做了。 
在上述的场合中，SchoolMember类被称为 基本类 或 超类 。而Teacher和Student类被称为 导出类 或 子类 。
## 存储器
python提供一个标准的模块，称为pickle。使用它可以在一个文件中存储任何python的对象，之后也可以完整无缺的取出来。这称为持久地存储对象。 
还有一个模块称为cPickle，功能和pickle相同，不过用C写的，速度较快。
```python
import cPickle as p
shoplist = ['food','appple','banada']
shopfile = 'shoplist.data'
f = file(shopfile,'w')
p.dump(shoplist,f)
storedlist = p.load(f)
print storedlist
输出：
['food','appple','banada']
```
## 异常处理
try…except 和  try…finally   使用来对异常进行处理：except对于每一个错误类型处理，finally则是对于错误的异常或者结束的时候做语句的处理。
## Python标准库
**sys模块**包含系统对应的功能。 
**os模块**
这个模块包含普遍的操作系统功能。如果你希望你的程序能够与平台无关的话，这个模块是尤为重要的。即它允许一个程序在编写后不需要任何改动，也不会发生任何问题，就可以在Linux和Windows下运行。一个例子就是使用os.sep可以取代操作系统特定的路径分割符。
下面列出了一些在os模块中比较有用的部分。它们中的大多数都简单明了。
os.name字符串指示你正在使用的平台。比如对于Windows，它是’nt’，而对于Linux/Unix用户，它是’posix’。
os.getcwd()函数得到当前工作目录，即当前Python脚本工作的目录路径。
os.getenv()和os.putenv()函数分别用来读取和设置环境变量。
os.listdir()返回指定目录下的所有文件和目录名。
os.remove()函数用来删除一个文件。
os.system()函数用来运行shell命令。
os.linesep字符串给出当前平台使用的行终止符。例如，Windows使用’\r\n’，Linux使用’\n’而Mac使用’\r’。
os.path.split()函数返回一个路径的目录名和文件名。
```
os.path.split('/home/swaroop/byte/code/poem.txt')
('/home/swaroop/byte/code', 'poem.txt')
```
os.path.isfile()和os.path.isdir()函数分别检验给出的路径是一个文件还是目录。类似地，os.path.existe()函数用来检验给出的路径是否真地存在。
## 列表综合
通过列表综合，可以从一个已有的列表导出一个新的列表。例如，你有一个数的列表，而你想要得到一个对应的列表，使其中所有大于2的数都是原来的2倍。对于这种应用，列表综合是最理想的方法。
```
listone = [2, 3, 4]
listtwo = [2*i for i in listone if i > 2]
print listtwo
输出：
[6, 8]
```
## 在函数中接收元组和列表
当要使函数接收元组或字典形式的参数的时候，有一种特殊的方法，它分别使用*和**前缀。这种方法在函数需要获取可变数量的参数的时候特别有用。
```
>>> def powersum(power, *args):
...     '''Return the sum of each argument raised to specified power.'''
...     total = 0
...     for i in args:
...          total += pow(i, power)
...     return total
...
>>> powersum(2, 3, 4)
25
>>> powersum(2, 10)
100
```
由于在args变量前有*前缀，所有多余的函数参数都会作为一个元组存储在args中。如果使用的是**前缀，多余的参数则会被认为是一个字典的键/值对。
exec和eval语句
exec语句用来执行储存在字符串或文件中的Python语句。例如，我们可以在运行时生成一个包含Python代码的字符串，然后使用exec语句执行这些语句。下面是一个简单的例子。
> 
> 
> 
exec ‘print “Hello World”’ 
      Hello World 
      eval语句用来计算存储在字符串中的有效Python表达式。下面是一个简单的例子。
eval(‘2*3’) 
      6

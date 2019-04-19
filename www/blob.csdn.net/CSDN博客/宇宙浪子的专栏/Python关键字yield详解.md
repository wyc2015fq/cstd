# Python关键字yield详解 - 宇宙浪子的专栏 - CSDN博客
2014年09月05日 16:11:04[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：340标签：[Python](https://so.csdn.net/so/search/s.do?q=Python&t=blog)
个人分类：[Python](https://blog.csdn.net/bluehawksky/article/category/2542577)
**导读：**此文由[伯乐](http://www.jobbole.com/)在线 –[刘志军](http://blog.jobbole.com/author/%E5%88%98%E5%BF%97%E5%86%9B/)编译自stackoverflow
 Python标签中投票率最高的一个问题《[The Python yield keyword explained](http://stackoverflow.com/questions/231767/the-python-yield-keyword-explained)》，[e-satis](http://stackoverflow.com/users/9951/e-satis) 详细回答了关于yield
 以及 generator、iterable、iterator、iteration之间的关系。
**迭代器(Iterator)**
为了理解yield是什么，首先要明白生成器(generator)是什么，在讲生成器之前先说说迭代器(iterator)，当创建一个列表(list)时，你可以逐个的读取每一项，这就叫做迭代（iteration）。
```python
mylist = [1, 2, 3]
 for i in mylist :
 print(i)
1
2
3
```
Mylist就是一个迭代器，不管是使用复杂的表达式列表，还是直接创建一个列表，都是可迭代的对象。
```python
mylist = [x*x for x in range(3)]
for i in mylist :
print(i)
0
1
4
```
你可以使用“for··· in ···”来操作可迭代对象，如：list,string,files,这些迭代对象非常方便我们使用，因为你可以按照你的意愿进行重复的读取。但是你不得不预先存储所有的元素在内存中，那些对象里有很多元素时，并不是每一项都对你有用。
**生成器(Generators)**
生成器同样是可迭代对象，但是你只能读取一次，因为它并没有把所有值存放内存中，它动态的生成值：
```python
mygenerator = (x*x for x in range(3))
for i in mygenerator :
print(i)
0
1
4
```
使用()和[]结果是一样的，但是，第二次执行“ for in mygenerator”不会有任何结果返回，因为它只能使用一次。首先计算0，然后计算1，之后计算4，依次类推。
**Yield**
Yield是关键字， 用起来像return，yield在告诉程序，要求函数返回一个生成器。
```python
def createGenerator() :
mylist = range(3)
for i in mylist :
yield i*i
mygenerator = createGenerator() # create a generator
print(mygenerator) # mygenerator is an object!
<generator object createGenerator at 0xb7555c34>
for i in mygenerator:
print(i)
0
1
4
```
这个示例本身没什么意义，但是它很清晰地说明函数将返回一组仅能读一次的值，要想掌握yield，首先必须理解的是：当你调用生成器函数的时候，如上例中的createGenerator()，程序并不会执行函数体内的代码，它仅仅只是返回生成器对象，这种方式颇为微妙。函数体内的代码只有直到每次循环迭代(for)生成器的时候才会运行。
函数第一次运行时，它会从函数开始处直到碰到yield时，就返回循环的第一个值，然后，交互的运行、返回，直到没有值返回为止。如果函数在运行但是并没有遇到yield，就认为该生成器是空，原因可能是循环终止，或者没有满足任何”if/else”。
接下来读一小段代码来理解生成器的优点：
**控制**生成器穷举****
```python
>>> class Bank(): # 创建银行，构造ATM机
...    crisis = False
...    def create_atm(self) :
...        while not self.crisis :
...            yield "$100"
>>> hsbc = Bank() # 没有危机时，你想要多少，ATM就可以吐多少
>>> corner_street_atm = hsbc.create_atm()
>>> print(corner_street_atm.next())
$100
>>> print(corner_street_atm.next())
$100
>>> print([corner_street_atm.next() for cash in range(5)])
['$100', '$100', '$100', '$100', '$100']
>>> hsbc.crisis = True # 危机来临，银行没钱了
>>> print(corner_street_atm.next())
<type 'exceptions.StopIteration'>
>>> wall_street_atm = hsbc.ceate_atm() # 新建ATM，银行仍然没钱
>>> print(wall_street_atm.next())
<type 'exceptions.StopIteration'>
>>> hsbc.crisis = False # 麻烦就是，即使危机过后银行还是空的
>>> print(corner_street_atm.next())
<type 'exceptions.StopIteration'>
>>> brand_new_atm = hsbc.create_atm() # 构造新的ATM，恢复业务
>>> for cash in brand_new_atm :
...    print cash
$100
$100
$100
$100
$100
$100
$100
$100
$100
```
对于访问控制资源，生成器显得非常有用。
**迭代工具，你最好的朋友**
迭代工具模块包含了操做指定的函数用于操作迭代器。想复制一个迭代器出来？链接两个迭代器？以one liner（这里的one-liner只需一行代码能搞定的任务)用内嵌的列表组合一组值？不使用list创建Map/Zip？···，你要做的就是 import itertools，举个例子吧：
四匹马赛跑到达终点排名的所有可能性：
```python
>>> horses = [1, 2, 3, 4]
>>> races = itertools.permutations(horses)
>>> print(races)
<itertools.permutations object at 0xb754f1dc>
>>> print(list(itertools.permutations(horses)))
[(1, 2, 3, 4),
 (1, 2, 4, 3),
 (1, 3, 2, 4),
 (1, 3, 4, 2),
 (1, 4, 2, 3),
 (1, 4, 3, 2),
 (2, 1, 3, 4),
 (2, 1, 4, 3),
 (2, 3, 1, 4),
 (2, 3, 4, 1),
 (2, 4, 1, 3),
 (2, 4, 3, 1),
 (3, 1, 2, 4),
 (3, 1, 4, 2),
 (3, 2, 1, 4),
 (3, 2, 4, 1),
 (3, 4, 1, 2),
 (3, 4, 2, 1),
 (4, 1, 2, 3),
 (4, 1, 3, 2),
 (4, 2, 1, 3),
 (4, 2, 3, 1),
 (4, 3, 1, 2),
 (4, 3, 2, 1)]
```
**理解迭代的内部机制：**
迭代(iteration）就是对可迭代对象（iterables，实现了__iter__()方法）和迭代器（iterators，实现了__next__()方法）的一个操作过程。可迭代对象是任何可返回一个迭代器的对象，迭代器是应用在迭代对象中迭代的对象，换一种方式说的话就是：iterable对象的__iter__()方法可以返回iterator对象，iterator通过调用next()方法获取其中的每一个值(译者注)，读者可以结合Java API中的 Iterable接口和Iterator接口进行类比。

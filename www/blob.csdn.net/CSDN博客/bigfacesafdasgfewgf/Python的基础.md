# Python的基础 - bigfacesafdasgfewgf - CSDN博客





2014年09月27日 16:16:30[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：659
个人分类：[Python](https://blog.csdn.net/puqutogether/article/category/2595125)









1.     Python提供了ord()和chr()函数，可以把字母和对应的数字相互转换：


2.     输出内容包含变量内容的时候，用法和C语言一样，使用%。

你可能猜到了，%运算符就是用来格式化字符串的。在字符串内部，%s表示用字符串替换，%d表示用整数替换，有几个%?占位符，后面就跟几个变量或者值，顺序要对应好。如果只有一个%?，括号可以省略。
常见的占位符有：
%d     整数
%f     浮点数
%s     字符串
%x     十六进制整数

3.     在list中，如果要取最后一个元素，除了计算索引位置外，还可以用-1做索引，直接获取最后一个元素：
>>> classmates[-1]
'Tracy'
以此类推，可以获取倒数第2个、倒数第3个：
>>> classmates[-2]
'Bob'
>>> classmates[-3]
'Michael'

在list中，还可以使用append和insert函数来实现增加和插入元素的操作。
pop函数可以实现删除。
要删除list末尾的元素，用pop()方法：

>>> classmates.pop()
'Adam'
>>> classmates
['Michael', 'Jack', 'Bob', 'Tracy']
要删除指定位置的元素，用pop(i)方法，其中i是索引位置：
>>> classmates.pop(1)
'Jack'
>>> classmates
['Michael', 'Bob', 'Tracy']

list里面的元素的数据类型也可以不同，比如：

>>> L = ['Apple', 123, True]


4.     元组 tuple
另一种有序列表叫元组：tuple。tuple和list非常类似，但是tuple一旦初始化就不能修改，比如同样是列出同学的名字：
>>> classmates = ('Michael', 'Bob', 'Tracy')
现在，classmates这个tuple不能变了，它也没有append()，insert()这样的方法。其他获取元素的方法和list是一样的，你可以正常地使用classmates[0]，classmates[-1]，但不能赋值成另外的元素。
不可变的tuple有什么意义？因为tuple不可变，所以代码更安全。如果可能，能用tuple代替list就尽量用tuple。

5.     range函数（Python 3中的range函数用法有变）
Python提供一个range()函数，可以生成一个整数序列，比如range(5)生成的序列是从0开始小于5的整数。也就是range函数的参数是生成序列的元素个数，而且是从0开始的。
>>> range(5)
[0, 1, 2, 3, 4]
range(101)就可以生成0-100的整数序列，计算如下：
sum = 0
for x in range(101):
    sum = sum + x
print sum

6.     从raw_input()读取的内容永远以字符串的形式返回
把字符串和整数比较就不会得到期待的结果，必须先用int()把字符串转换为我们想要的整型：
birth = int(raw_input('birth: '))
再次运行，就可以得到正确地结果。但是，如果输入abc呢？又会得到一个错误信息：
Traceback (most recent call last):
ValueError: invalid literal for int() with base 10: 'abc'
原来int()发现一个字符串并不是合法的数字时就会报错，程序就退出了。

7.     字典
Python内置了字典：dict的支持，dict全称dictionary，在其他语言中也称为map，使用键-值（key-value）存储，具有极快的查找速度。
如果用dict实现，只需要一个“名字”-“成绩”的对照表，直接根据名字查找成绩，无论这个表有多大，查找速度都不会变慢。用Python写一个dict如下：

>>> d = {'Michael': 95, 'Bob': 75, 'Tracy': 85}
>>> d['Michael']
95
这种key-value存储方式，在放进去的时候，必须根据key算出value的存放位置，这样，取的时候才能根据key直接拿到value。

把数据放入dict的方法，除了初始化时指定外，还可以通过key放入：

>>> d['Adam'] = 67
>>> d['Adam']
67
由于一个key只能对应一个value，所以，多次对一个key放入value，后面的值会把前面的值冲掉：

>>> d['Jack'] = 90
>>> d['Jack']
90
>>> d['Jack'] = 88
>>> d['Jack']
88
如果key不存在，dict就会报错：

>>> d['Thomas']
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
KeyError: 'Thomas'
要避免key不存在的错误，有两种办法，一是通过in判断key是否存在：

>>> 'Thomas' in d
False
二是通过dict提供的get方法，如果key不存在，可以返回None，或者自己指定的value：

>>> d.get('Thomas')
>>> d.get('Thomas', -1)
-1
注意：返回None的时候Python的交互式命令行不显示结果。

要删除一个key，用pop(key)方法，对应的value也会从dict中删除：

>>> d.pop('Bob')
75
>>> d
{'Michael': 95, 'Tracy': 85}
请务必注意，dict内部存放的顺序和key放入的顺序是没有关系的。

和list比较，dict有以下几个特点：

查找和插入的速度极快，不会随着key的增加而增加；
需要占用大量的内存，内存浪费多。
而list相反：

查找和插入的时间随着元素的增加而增加；
占用空间小，浪费内存很少。
所以，dict是用空间来换取时间的一种方法。

dict可以用在需要高速查找的很多地方，在Python代码中几乎无处不在，正确使用dict非常重要，需要牢记的第一条就是dict的key必须是不可变对象。

这是因为dict根据key来计算value的存储位置，如果每次计算相同的key得出的结果不同，那dict内部就完全混乱了。这个通过key计算位置的算法称为哈希算法（Hash）。

要保证hash的正确性，作为key的对象就不能变。在Python中，字符串、整数等都是不可变的，因此，可以放心地作为key。而list是可变的，就不能作为key：

>>> key = [1, 2, 3]
>>> d[key] = 'a list'
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: unhashable type: 'list'

8.     文件输入输出
以读文件的模式打开一个文件对象，使用Python内置的open()函数，传入文件名和标示符：

>>> f = open('/Users/michael/test.txt', 'r')
标示符'r'表示读，这样，我们就成功地打开了一个文件。

如果文件不存在，open()函数就会抛出一个IOError的错误，并且给出错误码和详细的信息告诉你文件不存在：

>>> f=open('/Users/michael/notfound.txt', 'r')
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
IOError: [Errno 2] No such file or directory: '/Users/michael/notfound.txt'
如果文件打开成功，接下来，调用read()方法可以一次读取文件的全部内容，Python把内容读到内存，用一个str对象表示：

>>> f.read()
'Hello, world!'
最后一步是调用close()方法关闭文件。文件使用完毕后必须关闭，因为文件对象会占用操作系统的资源，并且操作系统同一时间能打开的文件数量也是有限的：

>>> f.close()

Python引入了with语句来自动帮我们调用close()方法：

with open('/path/to/file', 'r') as f:
print f.read()

调用read()会一次性读取文件的全部内容，如果文件有10G，内存就爆了，所以，要保险起见，可以反复调用read(size)方法，每次最多读取size个字节的内容。另外，调用readline()可以每次读取一行内容，调用readlines()一次读取所有内容并按行返回list。因此，要根据需要决定怎么调用。

如果文件很小，read()一次性读取最方便；如果不能确定文件大小，反复调用read(size)比较保险；如果是配置文件，调用readlines()最方便：
写文件和读文件是一样的，唯一区别是调用open()函数时，传入标识符'w'或者'wb'表示写文本文件或写二进制文件：

>>> f = open('/Users/michael/test.txt', 'w')
>>> f.write('Hello, world!')
>>> f.close()
你可以反复调用write()来写入文件，但是务必要调用f.close()来关闭文件。当我们写文件时，操作系统往往不会立刻把数据写入磁盘，而是放到内存缓存起来，空闲的时候再慢慢写入。只有调用close()方法时，操作系统才保证把没有写入的数据全部写入磁盘。忘记调用close()的后果是数据可能只写了一部分到磁盘，剩下的丢失了。所以，还是用with语句来得保险：

with open('/Users/michael/test.txt', 'w') as f:
    f.write('Hello, world!')

9.     要到两个for循环比较两个文件内容的时候，第一个for前面open the first file, and the second file must be opened after the first for but before the second for. DO NOT open two files simutaneously before
 the first loop. Refer to the details in the following linkpage:
http://bbs.chinaunix.net/thread-1500622-1-1.html



10.     在Python中只有6中数据类型：int, float, str, list, tuple, dict.





11.     list  排序

有两种方式：

1）使用list.sort函数。



``>>> ``list``=``[``2``,``5``,``8``,``9``,``3``]  

``>>> ``list

``[``2``,``5``,``8``,``9``,``3``]  

``>>> ``list``.sort()  

``>>> ``list

``[``2``, ``3``, ``5``, ``8``, ``9``]




2）使用sorted(list)函数



``>>> ``list``=``[``2``,``5``,``8``,``9``,``3``]  

``>>> ``list

``[``2``,``5``,``8``,``9``,``3``]  

``>>> ``sorted``(``list``)  

``[``2``, ``3``, ``5``, ``8``, ``9``]







sorted(list)返回一个对象，可以用作表达式。原来的list不变，生成一个新的排好序的list对象。


list.sort() 不会返回对象，改变原有的list。


其他sort的实例：
实例1:正向排序


1


2


3


4



``>>>L ``=``[``2``,``3``,``1``,``4``]

``>>>L.sort()

``>>>L

``>>>[``1``,``2``,``3``,``4``]
实例2:反向排序


1


2


3


4



``>>>L ``=``[``2``,``3``,``1``,``4``]

``>>>L.sort(reverse``=``True``)

``>>>L

``>>>[``4``,``3``,``2``,``1``]
实例3:对第二个关键字排序 



1


2


3


4



``>>>L ``=``[(``'b'``,``6``),(``'a'``,``1``),(``'c'``,``3``),(``'d'``,``4``)]

``>>>L.sort(``lambda``x,y:``cmp``(x[``1``],y[``1``])) 

``>>>L

``>>>[(``'a'``, ``1``), (``'c'``, ``3``), (``'d'``, ``4``), (``'b'``, ``6``)]
实例4: 对第二个关键字排序 


1


2


3


4



``>>>L ``=``[(``'b'``,``6``),(``'a'``,``1``),(``'c'``,``3``),(``'d'``,``4``)]

``>>>L.sort(key``=``lambda``x:x[``1``]) 

``>>>L

``>>>[(``'a'``, ``1``), (``'c'``, ``3``), (``'d'``, ``4``), (``'b'``, ``6``)]
实例5: 对第二个关键字排序 


1


2


3


4


5



``>>>L ``=``[(``'b'``,``2``),(``'a'``,``1``),(``'c'``,``3``),(``'d'``,``4``)]

``>>>``import``operator

``>>>L.sort(key``=``operator.itemgetter(``1``)) 

``>>>L

``>>>[(``'a'``, ``1``), (``'b'``, ``2``), (``'c'``, ``3``), (``'d'``, ``4``)]
实例6:(DSU方法:Decorate-Sort-Undercorate)


1


2


3


4


5


6



``>>>L ``=``[(``'b'``,``2``),(``'a'``,``1``),(``'c'``,``3``),(``'d'``,``4``)]

``>>>A ``=``[(x[``1``],i,x) ``for``i,x ``in``enumerate``(L)] ``#i can confirm the stable sort

``>>>A.sort()

``>>>L ``=``[s[``2``] ``for``s ``in``A]

``>>>L

``>>>[(``'a'``, ``1``), (``'b'``, ``2``), (``'c'``, ``3``), (``'d'``, ``4``)]




12.     list和tuple下标的引用



``>>>``print``s1[:``5``]             ``# 从开始到下标4 （下标5的元素 不包括在内）

``>>>``print``s1[``2``:]             ``# 从下标2到最后

``>>>``print``s1[``0``:``5``:``2``]          ``# 从下标0到下标4 (下标5不包括在内)，每隔2取一个元素 （下标为0，2，4的元素）

``>>>``print``s1[``2``:``0``:``-``1``]         ``# 从下标2到下标1





``>>>``print``s1[``-``1``]             ``# 序列最后一个元素

``>>>``print``s1[``-``3``]             ``# 序列倒数第三个元素


同样，如果s1[0:-1], 那么最后一个元素不会被引用 （再一次，不包括上限元素本身）


字符串是元组


字符串是一种特殊的元素，因此可以执行元组的相关操作。


1


2



``>>>``str``=``'abcdef'

``>>>``print``str``[``2``:``4``]
总结


tuple元素不可变，list元素可变


序列的引用 s[2], s[1:8:2]


字符串是一种tuple，故其不可变




13.     判断

if ... in...: 

还有 is /is not/in/ not in等运算符。




14.     def函数中的值传递和指针传递





``a ``=``1

``def``change_integer(a):

````a ``=``a ``+``1

````return``a

``print``change_integer(a)

``print``a

``#===(Python中 "#" 后面跟的内容是注释，不执行 )

``b ``=``[``1``,``2``,``3``]

``def``change_list(b):

````b[``0``] ``=``b[``0``] ``+``1

````return``b

``print``change_list(b)

``print``b


第一个例子，我们将一个整数变量传递给函数，函数对它进行操作，但原整数变量a不发生变化。


第二个例子，我们将一个表传递给函数，函数进行操作，原来的表b发生变化。


对于基本数据类型的变量，变量传递给函数后，函数会在内存中复制一个新的变量，从而不影响原来的变量。（我们称此为值传递）


但是对于表来说，表传递给函数的是一个指针，指针指向序列在内存中的位置，在函数中对表的操作将在原有内存中进行，从而影响原有变量。 （我们称此为指针传递）




15.     class and object



``class``Bird(``object``):

````have_feather ``=``True

````way_of_reproduction ``=``'egg'

````def``move(``self``, dx, dy):

````position ``=``[``0``,``0``]

````position[``0``] ``=``position[``0``] ``+``dx

````position[``1``] ``=``position[``1``] ``+``dy

````return``position

``summer ``=``Bird()

``print``'after move:'``,summer.move(``5``,``8``)

它的参数中有一个self，它是为了方便我们引用对象自身。方法的第一个参数必须是self，无论是否用到。




类别本身还可以进一步细分成子类


比如说，鸟类可以进一步分成鸡，大雁，黄鹂。


在OOP中，我们通过继承(inheritance)来表达上述概念。


1


2


3


4


5


6


7


8


9



``class``Chicken(Bird):

````way_of_move ``=``'walk'

````possible_in_KFC ``=``True

``class``Oriole(Bird):

````way_of_move ``=``'fly'

````possible_in_KFC ``=``False

``summer ``=``Chicken()

``print``summer.have_feather

``print``summer.move(``5``,``8``)


新定义的鸡（Chicken）类的，增加了两个属性：移动方式（way_of_move），可能在KFC找到（possible_in_KFC）


在类定义时，括号里为了Bird。这说明，Chicken是属于鸟类（Bird）的一个子类，即Chicken继承自Bird。自然而然，Bird就是Chicken的父类。Chicken将享有Bird的所有属性。尽管我只声明了summer是鸡类，它通过继承享有了父类的属性（无论是变量属性have_feather还是方法属性move）


新定义的黄鹂(Oriole)类，同样继承自鸟类。在创建一个黄鹂对象时，该对象自动拥有鸟类的属性。


通过继承制度，我们可以减少程序中的重复信息和重复语句。如果我们分别定义两个类，而不继承自鸟类，就必须把鸟类的属性分别输入到鸡类和黄鹂类的定义中。整个过程会变得繁琐，因此，面向对象提高了程序的可重复使用性。


（回到问题1, 括号中的object，当括号中为object时，说明这个类没有父类（到头了））

所以，在定义object的时候就可以发现: class 类名之后括号中的是object就是最大的类，没有父类；如果括号里面还有类名，那么该类就是一个子类，括号里的就是父类。




建立对一个对象： 对象名 = 类名()


引用对象的属性： object.attribute




当定义类的方法时，必须要传递一个self的参数。这个参数指代的就是类的一个对象。我们可以通过操纵self，来修改某个对象的性质。比如用类来新建一个对象，即下面例子中的li_lei, 那么li_lei就被self表示。我们通过赋值给self.attribute，给li_lei这一对象增加一些性质，比如说性别的男女。self会传递给各个方法。在方法内部，可以通过引用self.attribute，查询或修改对象的性质。


这样，在类属性的之外，又给每个对象增添了各自特色的性质，从而能描述多样的世界。


1


2


3


4


5


6


7


8



``class``Human(``object``):

````def``__init__(``self``, input_gender):

````self``.gender ``=``input_gender

````def``printGender(``self``):

````print``self``.gender

``li_lei ``=``Human(``'male'``) ``# 这里，'male'作为参数传递给__init__()方法的input_gender变量。

``print``li_lei.gender

``li_lei.printGender()


在初始化中，将参数input_gender，赋值给对象的性质，即self.gender。


li_lei拥有了对象性质gender。gender不是一个类属性。Python在建立了li_lei这一对象之后，使用li_lei.gender这一对象性质，专门储存属于对象li_lei的特有信息。


对象的性质也可以被其它方法调用，调用方法与类属性的调用相似，正如在printGender()方法中的调用。


总结


通过self调用类属性


__init__(): 在建立对象时自动执行


类属性和对象的性质的区别




16.     关于list的更多用法




实验一些list的方法：


1


2


3


4


5


6


7



``>>>``print``nl.count(``5``)       ``# 计数，看总共有多少个5

``>>>``print``nl.index(``3``)       ``# 查询 nl 的第一个3的下标

``>>>nl.append(``6``)            ``# 在 nl 的最后增添一个新元素6

``>>>nl.sort()               ``# 对nl的元素排序

``>>>``print``nl.pop()          ``# 从nl中去除最后一个元素，并将该元素返回。

``>>>nl.remove(``2``)            ``# 从nl中去除第一个2

``>>>nl.insert(``0``,``9``)          ``# 在下标为0的位置插入9


总之，list是一个类。每个列表都属于该类。




17.     读取excel文件




python中要想处理excel，必须用到第三方模块xlrd，所以windows 我安装方法是cmd中命令：E:\ANZHUANG\Python\Scripts>easy_install xlrd （路径必须在python的scripts下和必须安装easy_install）


1


2


3


4


5


6


7


8


9


10


11


12


13


14


15



``#coding=utf-8

``import``xlrd

``def``print_xls(path):

``data``=``xlrd.open_workbook(path)   ``#打开excel

``table``=``data.sheets()[``1``] ``#打开excel的第几个sheet

``nrows``=``table.nrows   ``#捕获到有效数据的行数

``books``=``[]

``for``i ``in``range``(nrows):

``ss``=``table.row_values(i)   ``#获取一行的所有值，每一列的值以列表项存在

``#print ss

``for``i ``in``range``(``len``(ss)):

``print``ss[i]            ``#输出一行中各个列的值

``print``'+++++++++++++++++++'

``if``__name__``=``=``'__main__'``:

``print_xls(``'D:\\文档\\431547909921.xls'``)


18、类似于+=的符号，一定要注意原来的值是否存在，如果没有的话要先初始化。这一点尤其在给字典key-value做+=运算的时候要注意。

否则就会出现KeyError的错误。




19、append和extend函数的区别

![](file:///C:/DOCUME~1/ADMINI~1/LOCALS~1/Temp/enhtmlclip/Image(16).png)

![](file:///C:/DOCUME~1/ADMINI~1/LOCALS~1/Temp/enhtmlclip/Image(17).png)




20、set集合和list的区别：集合数据类型与列表类型相似，不同之处在于集合类型中的每个值互不相同。从列表中创建集合是python语言得到列表中唯一元素值的最快方法；




21、list.count(a)作用是计数list中等于a的个数；




22、def中参数传递都是引用形式的，如果需要在函数定义中修改，需要先复制下来；



















































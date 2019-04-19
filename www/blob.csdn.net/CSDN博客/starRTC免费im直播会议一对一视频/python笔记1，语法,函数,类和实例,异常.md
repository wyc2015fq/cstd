# python笔记1，语法,函数,类和实例,异常 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年11月10日 15:48:15[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：203
个人分类：[python](https://blog.csdn.net/elesos/article/category/7109747)
[http://www.liaoxuefeng.com/wiki/001374738125095c955c1e6d8bb493182103fac9270762a000](http://www.liaoxuefeng.com/wiki/001374738125095c955c1e6d8bb493182103fac9270762a000)
如果你听说过TIOBE排行榜，你就能知道编程语言的大致流行程度。这是最近10年最常用的10种编程语言的变化图：
完成同一个任务，C语言要写1000行代码，Java只需要写100行，而Python可能只要20行。
很多著名的网站包括YouTube就是Python写的
可以做网络游戏的后台，很多在线游戏的后台都是Python开发的。

2.x还是3.x
目前，Python有两个版本，一个是2.x版，一个是3.x版，这两个版本是不兼容的，因为现在Python正在朝着3.x版本进化，在进化过程中，大量的针对2.x版本的代码要修改后才能运行，所以，目前有许多第三方库还暂时无法在3.x上使用。
为了保证你的程序能用到大量的第三方库，我们的教程仍以2.x版本为基础，确切地说，是2.7版本。

直接运行py文件
在.py文件的第一行加上：
*#!/usr/bin/env python*
然后，通过命令：
$
chmod a+x hello.py
./hello.py就可以支行了。
当语句以冒号“:”结尾时，缩进的语句视为代码块。始终坚持使用4个空格的缩进。
Python程序是大小写敏感的
数据类型转换
>>>
int('123')123>>>
int(12.34)12>>>
float('12.34')12.34>>>
str(1.23)'1.23'>>>
unicode(100)u'100'>>>
bool(1)True>>>
bool('')False

定义一个函数要使用def语句，依次写出函数名、括号、括号中的参数和冒号:，然后，在缩进块中编写函数体，函数的返回值用return语句返回。
**def****my_abs**(x):
**if** x >=
0:
**return** x
**else**:
**return** -x
help(abs)查看abs函数的帮助信息。
数据类型检查可以用内置函数isinstance实现
**def****my_abs**(x):
**if****not** isinstance(x,
 (int, float)): **raise**
 TypeError('bad operand type')
**if** x >=
0:
**return** x
**else**:
**return** -x
返回多个值：
**return**
 nx, ny
使用时
x,
 y = move（）
但其实这只是一种假象，Python函数返回的仍然是单一值：
>>>
r = move()>>>
**print** r(151.96152422706632,
70.0)
原来返回值是一个tuple！
在Python中，一个.py文件就称之为一个模块（Module）
使用模块还可以避免函数名和变量名冲突。相同名字的函数和变量完全可以分别存在不同的模块中，因此，我们自己在编写模块时，不必考虑名字会与其他模块冲突。但是也要注意，尽量不要与内置函数名字冲突。
一个abc.py的文件就是一个名字叫abc的模块
如果不同的人编写的模块名相同怎么办？为了避免模块名冲突，Python又引入了按目录来组织模块的方法，称为包（Package）。

所有文件开头：
*#!/usr/bin/env python**# -*- coding: utf-8 -*-*'
 a test module '__author__ =
'Michael Liao'
**if**
 __name__=='__main__':
 test()
当我们在命令行运行hello模块文件时，Python解释器把一个特殊变量__name__置为__main__，而如果在其他地方导入该hello模块时，if判断将失败，因此，这种if测试可以让一个模块通过命令行运行时执行一些额外的代码，最常见的就是运行测试。
安装第三方模块，是通过setuptools这个工具完成的。Python有两个封装了setuptools的包管理工具：easy_install和pip。目前官方推荐使用pip。
让我们来安装一个第三方库——Python Imaging Library，这是Python下非常强大的处理图像的工具库。一般来说，第三方库都会在Python官方的[pypi.python.org](https://pypi.python.org/)网站注册，要安装一个第三方库，必须先知道该库的名称，可以在官网或者pypi上搜索，比如Python
 Imaging Library的名称叫PIL，因此，安装Python Imaging Library的命令就是：
pip install PIL

有了PIL，处理图片易如反掌。随便找个图片生成缩略图：
>>>
**import** Image>>>
im = Image.open('test.png')>>>
**print** im.format,
 im.size, im.modePNG (400,
300) RGB>>>
im.thumbnail((200,
100))>>>
im.save('thumb.jpg',
'JPEG')
其他常用的第三方库还有MySQL的驱动：MySQL-python
类和实例
**class**** Student(object):****pass**
class后面紧接着是类名，即Student，类名通常是大写开头的单词，紧接着是(object)，表示该类是从哪个类继承下来的，继承的概念我们后面再讲，通常，如果没有合适的继承类，就使用object类，这是所有类最终都会继承的类。
**class**** Student(object):****def****__init__**(self,
 name, score): self.name = name self.score = score
注意到__init__方法的第一个参数永远是self，表示创建的实例本身，因此，在__init__方法内部，就可以把各种属性绑定到self，因为self就指向创建的实例本身。
有了__init__方法，在创建实例的时候，就不能传入空的参数了，必须传入与__init__方法匹配的参数，但self不需要传，Python解释器自己会把实例变量传进去：
和普通的函数相比，在类中定义的函数只有一点不同，就是第一个参数永远是实例变量self，并且，调用时，不用传递该参数。除此之外，类的方法和普通函数没有什么区别
**def****print_score**(self):
**print**'%s: %s' % (self.name, self.score)
Python的pdb可以让我们以单步方式执行代码。
用错误码来表示是否出错十分不便，调用者必须用大量的代码来判断是否出错，一旦出错，还要一级一级上报，所以高级语言通常都内置了一套try...except...finally...的错误处理机
**try**:
**print**'try...' r =
10 /
0**print**'result:', r**except**
 ZeroDivisionError, e: **print**'except:', e**finally**:
**print**'finally...'**print**'END'
当我们认为某些代码可能会出错时，就可以用try来运行这段代码，如果执行出错，则后续代码不会继续执行，而是直接跳转至错误处理代码，即except语句块，执行完except后，如果有finally语句块，则执行finally语句块，至此，执行完毕。
上面的代码在计算10 / 0时会产生一个除法运算错误：
**try**...**except**:
 integer division **or**
 modulo by zero**finally**...END
 //程序继续按照流程往下走。
finally如果有，则一定会被执行（可以没有finally语句）。
可以有多个except来捕获不同类型的错误：
**try**:
**print**'try...' r =
10 / int('a')
**print**'result:', r**except**
 ValueError, e: **print**'ValueError:', e**except**
 ZeroDivisionError, e: **print**'ZeroDivisionError:', e**finally**:
**print**'finally...'**print**'END'

此外，如果没有错误发生，可以在except语句块后面加一个else，当没有错误发生时，会自动执行else语句：
**try**:
**print**'try...' r =
10 / int('a')
**print**'result:', r**except**
 ValueError, e: **print**'ValueError:', e**except**
 ZeroDivisionError, e: **print**'ZeroDivisionError:', e**else**:
**print**'no error!'**finally**:
**print**'finally...'**print**'END'
使用try...except捕获错误还有一个巨大的好处，就是可以跨越多层调用，比如函数main()调用foo()，foo()调用bar()，结果bar()出错了，这时，只要main()捕获到了，就可以处理：
**def****foo**(s):
**return**10 / int(s)**def****bar**(s):
**return** foo(s)
 * 2**def****main**():
**try**: bar('0')
**except** StandardError,
 e: **print**'Error!'**finally**:
**print**'finally...'
也就是说，不需要在每个可能出错的地方去捕获错误，只要在合适的层次去捕获错误就可以了。这样一来，就大大减少了写try...except...finally的麻烦。
用raise语句抛出一个错误的实例
*# err.py***def****foo**(s): n =
 int(s) **return**10 / n**def****bar**(s):
**try**:
**return** foo(s)
 * 2**except** StandardError,
 e: **print**'Error!'**raise****def****main**(): bar('0')main()
在bar()函数中，我们明明已经捕获了错误，但是，打印一个Error!后，又把错误通过raise语句抛出去了，这不有病么？
其实这种错误处理方式不但没病，而且相当常见。捕获错误目的只是记录一下，便于后续追踪。但是，由于当前函数不知道应该怎么处理该错误，所以，最恰当的方式是继续往上抛，让顶层调用者去处理。
raise语句如果不带参数，就会把当前错误原样抛出。
循环有两种，一种是for...in循环，依次把list或tuple中的每个元素迭代出来，看例子：
names = ['Michael',
'Bob',
'Tracy']**for**
 name **in** names:
**print** name
for x in ...循环就是把每个元素代入变量x，然后执行缩进块的语句。
第二种循环是while循环
Python内置了字典：dict的支持，在其他语言中也称为map，使用键-值（key-value）存储，具有极快的查找速度。
为什么dict查找速度这么快？因为dict的实现原理和查字典是一样的。
dict内部存放的顺序和key放入的顺序是没有关系的。
list是一种有序的集合，用len()函数可以获得list元素的个数：用索引来访问list中每一个位置的元素，记得索引是从0开始的：
如果要取最后一个元素，除了计算索引位置外，还可以用-1做索引，直接获取最后一个元素：
>>>
classmates[-1]'Tracy'
以此类推，可以获取倒数第2个、倒数第3个：
>>> classmates[-2]'Bob'>>> classmates[-3]
可以往list中追加元素到末尾：
>>>
classmates.append('Adam')
也可以把元素插入到指定的位置，比如索引号为1的位置：
>>>
classmates.insert(1,
'Jack')
要删除list末尾的元素，用pop()方法：
>>>
classmates.pop()
要删除指定位置的元素，用pop(i)方法，其中i是索引位置：
list里面的元素的数据类型也可以不同，比如：
>>>
L = ['Apple',
123,
True]
list元素也可以是另一个list，比如：
>>>
s = ['python',
'java', ['asp',
'php'],
'scheme']>>>
len(s)4
要注意s只有4个元素，其中s[2]又是一个list，如果拆开写就更容易理解了：
>>>
p = ['asp',
'php']>>>
s = ['python',
'java', p,
'scheme']
要拿到'php'可以写p[1]或者s[2][1]，因此s可以看成是一个二维数组，
如果一个list中一个元素也没有，就是一个空的list，它的长度为0：
>>>
L = []>>>
len(L)0
>>>
classmates = ('Michael',
'Bob',
'Tracy')
另一种有序列表叫元组：tuple。tuple和list非常类似，但是tuple一旦初始化就不能修改
它也没有append()，insert()这样的方法。其他获取元素的方法和list是一样的
不可变的tuple有什么意义？因为tuple不可变，所以代码更安全。如果可能，能用tuple代替list就尽量用tuple。
有1个元素的tuple定义时必须加一个逗号,，来消除歧义：
>>>
t = (1,)>>>
t(1,)
Python在显示只有1个元素的tuple时，也会加一个逗号,，以免你误解成数学计算意义上的括号。
列表用[ ]标识。
元组用"()"标识。


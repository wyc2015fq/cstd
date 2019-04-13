
# Python基础学习笔记之（一） - zouxy09的专栏 - CSDN博客


2013年11月24日 15:58:17[zouxy09](https://me.csdn.net/zouxy09)阅读数：35148


**Python基础学习笔记之（一）**
zouxy09@qq.com
http://blog.csdn.net/zouxy09
前段时间参加微软的windows Azure云计算的一个小培训，其中Python被用的还是蛮多的。另外，一些大公司如Google（实现web爬虫和搜索引擎中的很多组件），Yahoo（管理讨论组），NASA，YouTube（视频分享服务大部分由Python编写）等等对Python都很青睐。而国内的豆瓣可以说是给Python予千万宠爱了，它的前台后台清一色的都是Python的身影。另外，我们计算机视觉这块用的很频繁的OpenCV也提供了Python的接口，网上还提供了不少Python的机器学习的库（例如[milk](http://pythonhosted.org/milk/)，[scikit-learn](http://scikit-learn.org/stable/)，[Pylearn2](https://github.com/lisa-lab/pylearn2)等），Deep learning的一个知名的Python的库[theano](http://deeplearning.net/software/theano/)，自然语言处理的库[NLTK](http://nltk.org/)。此外，Python为数学、科学、工程和绘图等提供了有趣的标准库（例如，NumPy ，SciPy和matplotlib等），这使得一部分Matlab的使用者慢慢的倒戈到Python阵营，没办法，谁叫Matlab贵呢！当然了，Python不仅是免费，它还具有Matlab较弱或者没有的一些其他的功能，例如文件管理、界面设计、网络通信等。这就使得Python占有的用户群更广。
Python是“蟒蛇”的意思，这个名字是有点故事的。当然了，这个得追溯到它的发明者Guido van Rossum。在1989年圣诞节期间，Guido身处阿姆斯特丹。这个都市的美丽和繁华没能填满Guido空虚的内心，周边的喧闹使他内心的寂寞无处安放（夜的黑已不再纯粹，哈哈）。为了打发这种无趣，Guido决心为他之前孕育的ABC语言开发一个插件，这个插件就是大名鼎鼎的Python（男人因为孤独而优秀啊！）。一个新的脚本解释程序就此诞生。在给这个孩子起名字的时候，Guido取其所爱的一个叫Monty Python的喜剧团体的“Python”。
![](https://img-blog.csdn.net/20131124155022312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
官方点说，Python是一种解释型、面向对象、动态数据类型的高级程序设计语言。Python语法简洁而清晰，具有丰富和强大的类库。它常被昵称为胶水语言，它能够很轻松的把用其他语言制作的各种模块（尤其是C/C++）轻松地联结在一起。自从20世纪90年代初Python语言诞生至今，它逐渐被广泛应用于处理系统管理任务和Web编程。Python已经成为最受欢迎的程序设计语言之一。2011年1月，它被TIOBE编程语言排行榜评为2010年度语言。自从2004年以后，python的使用率是呈线性增长。
这篇博文是我看了“中谷教育-Python视频教程”的一个笔记，记录的是一些Python的学习基础，整理到这里，一是总结，二是备查。网上也有比较好的教程，例如“Python 精要参考”。
**目录**
一、安装、编译与运行
二、变量、运算与表达式
三、数据类型
1、数字
2、字符串
3、元组
4、列表
5、字典
四、流程控制
1、if-else
2、for
3、while
4、switch
五、函数
1、自定义函数
2、Lambda函数
3、Python内置函数
六、包与模块
1、模块module
2、包package
七、正则表达式
1、元字符
2、常用函数
3、分组
4、一个小实例-爬虫
八、深拷贝与浅拷贝
九、文件与目录
1、文件读写
2、OS模块
3、目录遍历
十、异常处理
**一、安装、编译与运行**
Python的安装很容易，直接到官网：[http://www.python.org/](http://www.python.org/)下载安装就可以了。Ubuntu一般都预安装了。没有的话，就可以\#apt-get install python。Windows的话直接下载msi包安装即可。Python 程序是通过解释器执行的，所以安装后，可以看到Python提供了两个解析器，一个是IDLE (Python GUI)，一个是Python (command line)。前者是一个带GUI界面的版本，后者实际上和在命令提示符下运行python是一样的。运行解释器后，就会有一个命令提示符>>>，在提示符后键入你的程序语句，键入的语句将会立即执行。就像Matlab一样。
另外，Matlab有.m的脚步文件，python也有.py后缀的脚本文件，这个文件除了可以解释执行外，还可以编译运行，编译后运行速度要比解释运行要快。
例如，我要打印一个helloWorld。
方法1：直接在解释器中，>>> print ‘helloWorld’。
方法2：将这句代码写到一个文件中，例如hello.py。运行这个文件有三种方式：
1）在终端中：python hello.py
2）先编译成.pyc文件：
import py_compile
py_compile.compile("hello.py")
再在终端中：python hello.pyc
3）在终端中：
python -O -m py_compile hello.py
python hello.pyo
编译成.pyc和.pyo文件后，执行的速度会更快。所以一般一些重复性并多次调用的代码会被编译成这两种可执行的方式来待调用。
**二、变量、运算与表达式**
这里没什么好说的，有其他语言的编程基础的话都没什么问题。和Matlab的相似度比较大。这块差别不是很大。具体如下：
![](https://img-blog.csdn.net/20131124155204765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
需要注意的一个是：5/2 等于2，5.0/2才等于2.5。

```python
###################################
### compute #######
# raw_input() get input from keyboard to string type
# So we should transfer to int type
# Some new support computing type:
# and or not in is < <= != == | ^ & << + - / % ~ **
print 'Please input a number:'
number = int(raw_input()) 
number += 1
print number**2 # ** means ^
print number and 1
print number or 1
print not number
5/2 # is 2
5.0/2 # is 2.5, should be noted
```

**三、数据类型**
**1、数字**
通常的int, long,float,long等等都被支持。而且会看你的具体数字来定义变量的类型。如下：

```python
###################################
### type of value #######
# int, long, float
# do not need to define the type of value, python will
# do this according to your value
num = 1   # stored as int type
num = 1111111111111   # stored as long int type
num = 1.0   # stored as float type
num = 12L # L stands for long type
num = 1 + 12j # j stands for complex type
num = '1' # string type
```

**2、字符串**
单引号，双引号和三引号都可以用来定义字符串。三引号可以定义特别格式的字符串。字符串作为一种序列类型，支持像Matlab一样的索引访问和切片访问。

```python
###################################
### type of string #######
num = "1" # string type
num = "Let's go" # string type
num = "He's \"old\"" # string type
mail = "Xiaoyi: \n hello \n I am you!"
mail = """Xiaoyi:
	hello
	I am you!
	""" # special string format
string = 'xiaoyi' # get value by index
copy = string[0] + string[1] + string[2:6] # note: [2:6] means [2 5] or[2 6)
copy = string[:4] # start from 1
copy = string[2:] # to end
copy = string[::1] # step is 1, from start to end
copy = string[::2] # step is 2
copy = string[-1] # means 'i', the last one
copy = string[-4:-2:-1] # means 'yoa', -1 step controls direction
memAddr = id(num) # id(num) get the memory address of num
type(num) # get the type of num
```

**3、元组**
元组tuple用()来定义。相当于一个可以存储不同类型数据的一个数组。可以用索引来访问，但需要注意的一点是，里面的元素不能被修改。

```python
###################################
### sequence type #######
## can access the elements by index or slice
## include: string, tuple(or array? structure? cell?), list
# basis operation of sequence type
firstName = 'Zou'
lastName = 'Xiaoyi'
len(string) # the length
name = firstName + lastName # concatenate 2 string
firstName * 3 # repeat firstName 3 times
'Z' in firstName # check contain or not, return true
string = '123'
max(string)
min(string)
cmp(firstName, lastName) # return 1, -1 or 0
## tuple(or array? structure? cell?)
## define this type using ()
user = ("xiaoyi", 25, "male")
name = user[0]
age = user[1]
gender = user[2]
t1 = () # empty tuple
t2 = (2, ) # when tuple has only one element, we should add a extra comma
user[1] = 26 # error!! the elements can not be changed
name, age, gender = user # can get three element respectively
a, b, c = (1, 2, 3)
```

**4、列表**
列表list用[]来定义。它和元组的功能一样，不同的一点是，里面的元素可以修改。List是一个类，支持很多该类定义的方法，这些方法可以用来对list进行操作。

```python
## list type (the elements can be modified)
## define this type using []
userList = ["xiaoyi", 25, "male"]
name = userList[0]
age = userList[1]
gender = userList[2]
userList[3] = 88888 # error! access out of range, this is different with Matlab
userList.append(8888) # add new elements
"male" in userList # search
userList[2] = 'female' # can modify the element (the memory address not change)
userList.remove(8888) # remove element
userList.remove(userList[2]) # remove element
del(userList[1]) # use system operation api
## help(list.append)
################################
######## object and class ######
## object = property + method
## python treats anything as class, here the list type is a class,
## when we define a list "userList", so we got a object, and we use
## its method to operate the elements
```

**5、字典**
字典dictionary用{}来定义。它的优点是定义像key-value这种键值对的结构，就像struct结构体的功能一样。它也支持字典类支持的方法进行创建和操作。

```python
################################
######## dictionary type ######
## define this type using {}
item = ['name', 'age', 'gender']
value = ['xiaoyi', '25', 'male']
zip(item, value) # zip() will produce a new list: 
# [('name', 'xiaoyi'), ('age', '25'), ('gender', 'male')]
# but we can not define their corresponding relationship
# and we can define this relationship use dictionary type
# This can be defined as a key-value manner
# dic = {key1: value1, key2: value2, ...}, key and value can be any type
dic = {'name': 'xiaoyi', 'age': 25, 'gender': 'male'}
dic = {1: 'zou', 'age':25, 'gender': 'male'}
# and we access it like this: dic[key1], the key as a index
print dic['name']
print dic[1]
# another methods create dictionary
fdict = dict(['x', 1], ['y', 2]) # factory mode
ddict = {}.fromkeys(('x', 'y'), -1) # built-in mode, default value is the same which is none
# access by for circle
for key in dic
	print key
	print dic[key]
# add key or elements to dictionary, because dictionary is out of sequence,
# so we can directly and a key-value pair like this:
dic['tel'] = 88888 	
# update or delete the elements
del dic[1] # delete this key
dic.pop('tel') # show and delete this key
dic.clear() # clear the dictionary
del dic # delete the dictionary
dic.get(1) # get the value of key
dic.get(1, 'error') # return a user-define message if the dictionary do not contain the key
dic.keys()
dic.values()
dic.has_key(key)
# dictionary has many operations, please use help to check out
```

**四、流程控制**
在这块，Python与其它大多数语言有个非常不同的地方，Python语言使用缩进块来表示程序逻辑（其它大多数语言使用大括号等）。例如：
if age < 21:
print("你不能买酒。")
print("不过你能买口香糖。")
print("这句话处于if语句块的外面。")
这个代码相当于c语言的：
if (age < 21)
{
print("你不能买酒。")
print("不过你能买口香糖。")
}
print("这句话处于if语句块的外面。")
可以看到，Python语言利用缩进表示语句块的开始和退出（Off-side规则），而非使用花括号或者某种关键字。增加缩进表示语句块的开始（注意前面有个:号），而减少缩进则表示语句块的退出。根据PEP的规定，必须使用4个空格来表示每级缩进（不清楚4个空格的规定如何，在实际编写中可以自定义空格数，但是要满足每级缩进间空格数相等）。使用Tab字符和其它数目的空格虽然都可以编译通过，但不符合编码规范。
为了使我们自己编写的程序能很好的兼容别人的程序，我们最好还是按规范来，用四个空格来缩减（注意，要么都是空格，要是么都制表符，千万别混用）。
**1、if-else**
If-else用来判断一些条件，以执行满足某种条件的代码。

```python
################################
######## procedure control #####
## if else
if expression: # bool type and do not forget the colon
	statement(s) # use four space key 
if expression: 
statement(s) # error!!!! should use four space key 
	
if 1<2:
	print 'ok, ' # use four space key
	print 'yeah' # use the same number of space key
	
if True: # true should be big letter True
	print 'true'
def fun():
	return 1
if fun():
	print 'ok'
else:
	print 'no'
	
con = int(raw_input('please input a number:'))
if con < 2:
	print 'small'
elif con > 3:
	print 'big'
else:
	print 'middle'
	
if 1 < 2:
	if 2 < 3:
		print 'yeah'
	else:
		print 'no'	
	print 'out'
else:
	print 'bad'
if 1<2 and 2<3 or 2 < 4 not 0: # and, or, not
	print 'yeah'
```

**2、for**
for的作用是循环执行某段代码。还可以用来遍历我们上面所提到的序列类型的变量。

```python
################################
######## procedure control #####
## for
for iterating_val in sequence:
	statements(s)
# sequence type can be string, tuple or list
for i in "abcd":
	print i
for i in [1, 2, 3, 4]:
	print i
# range(start, end, step), if not set step, default is 1, 
# if not set start, default is 0, should be noted that it is [start, end), not [start, end]
range(5) # [0, 1, 2, 3, 4]
range(1, 5) # [1, 2, 3, 4]
range(1, 10, 2) # [1, 3, 5, 7, 9]
for i in range(1, 100, 1): 
	print i
# ergodic for basis sequence
fruits = ['apple', 'banana', 'mango']
for fruit in range(len(fruits)): 
	print 'current fruit: ', fruits[fruit]
# ergodic for dictionary
dic = {1: 111, 2: 222, 5: 555}
for x in dic:
	print x, ': ', dic[x]
	
dic.items() # return [(1, 111), (2, 222), (5, 555)]
for key,value in dic.items(): # because we can: a,b=[1,2]
	print key, ': ', value
else:
	print 'ending'
################################
import time
# we also can use: break, continue to control process
for x in range(1, 11):
	print x
	time.sleep(1) # sleep 1s
	if x == 3:
		pass # do nothing
	if x == 2:
		continue
	if x == 6:
		break
	if x == 7:	
		exit() # exit the whole program
	print '#'*50
```

**3、while**
while的用途也是循环。它首先检查在它后边的循环条件，若条件表达式为真，它就执行冒号后面的语句块，然后再次测试循环条件，直至为假。冒号后面的缩近语句块为循环体。

```python
################################
######## procedure control #####
## while
while expression:
	statement(s)
while True:
	print 'hello'
	x = raw_input('please input something, q for quit:')
	if x == 'q':
		break
else:
	print 'ending'
```

**4、switch**
其实Python并没有提供switch结构，但我们可以通过字典和函数轻松的进行构造。例如：

```python
#############################
## switch ####
## this structure do not support by python
## but we can implement it by using dictionary and function
## cal.py ##
#!/usr/local/python
from __future__ import division
# if used this, 5/2=2.5, 6/2=3.0
def add(x, y):
	return x + y
def sub(x, y):
	return x - y
def mul(x, y):
	return x * y
def div(x, y):
	return x / y
operator = {"+": add, "-": sub, "*": mul, "/": div}
operator["+"](1, 2) # the same as add(1, 2)
operator["%"](1, 2) # error, not have key "%", but the below will not
operator.get("+")(1, 2) # the same as add(1, 2)
def cal(x, o, y):
	print operator.get(o)(x, y)
cal(2, "+", 3)
# this method will effect than if-else
```

**五、函数**
**1、自定义函数**
在Python中，使用def语句来创建函数：

```python
################################
######## function #####	
def functionName(parameters): # no parameters is ok
	bodyOfFunction
def add(a, b):
	return a+b # if we do not use a return, any defined function will return default None 
	
a = 100
b = 200
sum = add(a, b)
##### function.py #####
#!/usr/bin/python
#coding:utf8  # support chinese
def add(a = 1, b = 2): # default parameters
	return a+b	# can return any type of data
# the followings are all ok
add()
add(2)
add(y = 1)
add(3, 4)
###### the global and local value #####
## global value: defined outside any function, and can be used
##				in anywhere, even in functions, this should be noted
## local value: defined inside a function, and can only be used
##				in its own function
## the local value will cover the global if they have the same name
val = 100 # global value
def fun():
	print val # here will access the val = 100
print val # here will access the val = 100, too
def fun():
	a = 100 # local value
	print a
print a # here can not access the a = 100
def fun():
	global a = 100 # declare as a global value
	print a
print a # here can not access the a = 100, because fun() not be called yet
fun()
print a # here can access the a = 100
############################
## other types of parameters
def fun(x):
	print x
# the follows are all ok
fun(10)	# int
fun('hello') # string
fun(('x', 2, 3))  # tuple
fun([1, 2, 3])	  # list
fun({1: 1, 2: 2}) # dictionary
## tuple
def fun(x, y):
	print "%s : %s" % (x,y) # %s stands for string
fun('Zou', 'xiaoyi')
tu = ('Zou', 'xiaoyi')
fun(*tu)	# can transfer tuple parameter like this
## dictionary
def fun(name = "name", age = 0):
	print "name: %s" % name
	print "age: " % age
dic = {name: "xiaoyi", age: 25} # the keys of dictionary should be same as fun()
fun(**dic) # can transfer dictionary parameter like this
fun(age = 25, name = 'xiaoyi') # the result is the same
## the advantage of dictionary is can specify value name
#############################
## redundancy parameters ####
## the tuple
def fun(x, *args): # the extra parameters will stored in args as tuple type 
	print x
	print args
# the follows are ok
fun(10)
fun(10, 12, 24) # x = 10, args = (12, 24)
## the dictionary
def fun(x, **args): # the extra parameters will stored in args as dictionary type 
	print x
	print args
# the follows are ok
fun(10)
fun(x = 10, y = 12, z = 15) # x = 10, args = {'y': 12, 'z': 15}
# mix of tuple and dictionary
def fun(x, *args, **kwargs):
	print x
	print args
	print kwargs
fun(1, 2, 3, 4, y = 10, z = 12) # x = 1, args = (2, 3, 4), kwargs = {'y': 10, 'z': 12}
```

**2、Lambda函数**
Lambda函数用来定义一个单行的函数，其便利在于：

```python
#############################
## lambda function ####
## define a fast single line function
fun = lambda x,y : x*y # fun is a object of function class
fun(2, 3)
# like
def fun(x, y):
	return x*y
## recursion
# 5=5*4*3*2*1, n!
def recursion(n):
	if n > 0:
		return n * recursion(n-1) ## wrong
def mul(x, y):
	return x * y
numList = range(1, 5)
reduce(mul, numList) # 5! = 120
reduce(lambda x,y : x*y, numList) # 5! = 120, the advantage of lambda function avoid defining a function
### list expression
numList = [1, 2, 6, 7]
filter(lambda x : x % 2 == 0, numList)
print [x for x in numList if x % 2 == 0] # the same as above
map(lambda x : x * 2 + 10, numList)
print [x * 2 + 10 for x in numList] # the same as above
```

**3、Python内置函数**
Python内置了很多函数，他们都是一个个的.py文件，在python的安装目录可以找到。弄清它有那些函数，对我们的高效编程非常有用。这样就可以避免重复的劳动了。下面也只是列出一些常用的：

```python
###################################
## built-in function of python ####
## if do not how to use, please use help()
abs, max, min, len, divmod, pow, round, callable,
isinstance, cmp, range, xrange, type, id, int()
list(), tuple(), hex(), oct(), chr(), ord(), long()
callable # test a function whether can be called or not, if can, return true
# or test a function is exit or not
isinstance # test type
numList = [1, 2]
if type(numList) == type([]):
	print "It is a list"
if isinstance(numList, list): # the same as above, return true
	print "It is a list"
	
for i in range(1, 10001) # will create a 10000 list, and cost memory
for i in xrange(1, 10001)# do not create such a list, no memory is cost
## some basic functions about string
str = 'hello world'
str.capitalize() # 'Hello World', first letter transfer to big
str.replace("hello", "good") # 'good world'
ip = "192.168.1.123"
ip.split('.') # return ['192', '168', '1', '123']
help(str.split)
import string
str = 'hello world'
string.replace(str, "hello", "good") # 'good world'
## some basic functions about sequence
len, max, min
# filter(function or none, sequence)
def fun(x):
	if x > 5:
		return True
numList = [1, 2, 6, 7]
filter(fun, numList) # get [6, 7], if fun return True, retain the element, otherwise delete it
filter(lambda x : x % 2 == 0, numList)
# zip()
name = ["me", "you"]
age = [25, 26]
tel = ["123", "234"]
zip(name, age, tel) # return a list: [('me', 25, '123'), ('you', 26, '234')]
# map()
map(None, name, age, tel) # also return a list: [('me', 25, '123'), ('you', 26, '234')]
test = ["hello1", "hello2", "hello3"]
zip(name, age, tel, test) # return [('me', 25, '123', 'hello1'), ('you', 26, '234', 'hello2')]
map(None, name, age, tel, test) # return [('me', 25, '123', 'hello1'), ('you', 26, '234', 'hello2'), (None, None, None, 'hello3')]
a = [1, 3, 5]
b = [2, 4, 6]
def mul(x, y):
	return x*y
map(mul, a, b) # return [2, 12, 30]
# reduce()
reduce(lambda x, y: x+y, [1, 2, 3, 4, 5]) # return ((((1+2)+3)+4)+5)
```




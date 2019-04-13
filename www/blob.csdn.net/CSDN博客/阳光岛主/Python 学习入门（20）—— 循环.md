
# Python 学习入门（20）—— 循环 - 阳光岛主 - CSDN博客

2013年12月10日 22:09:18[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：13060所属专栏：[Python 学习入门](https://blog.csdn.net/column/details/python-learning.html)



1. for 循环
for循环需要预先设定好循环的次数(n)，然后执行隶属于for的语句n次。
基本构造是
for元素in序列: 
    statement
举例来说，我们编辑一个叫forDemo.py的文件
forain[3,4.4,'life']:printa
这个循环就是每次从表[3,4.4,'life'] 中取出一个元素（回忆：表是一种序列），然后将这个元素赋值给a，之后执行隶属于for的操作(print)。
介绍一个新的python函数range()，来帮助你建立表。
idx = range(5)printidx
可以看到idx是[0,1,2,3,4]
这个函数的功能是新建一个表。这个表的元素都是整数，从0开始，下一个元素比前一个大1， 直到函数中所写的上限 （不包括该上限本身）
(关于range()，还有丰富用法，有兴趣可以查阅， python 3中， range()用法有变化，见评论区)
举例
forainrange(10):printa**2
2. while循环
while的用法是
while条件:
    statement
while会不停地循环执行隶属于它的语句，直到条件为假(False)
举例
whilei < 10:printi
    i= i + 1
3. 中断循环
(定义一个环的说法。循环是相同的一组操作重复多次，我们把其中的一组操作叫做一环)
continue\# 在同一循环的某一环，如果遇到continue, 那么跳过这一环，进行下一次环的操作
break\# 停止执行整个循环
foriinrange(10):ifi == 2:
continueprinti
当循环执行到i = 2的时候，if条件成立，触发continue, 跳过本环(不执行print)，继续进行下一环(i = 3)
foriinrange(10):ifi == 2:breakprinti
当循环执行到i = 2的时候，if条件成立，触发break, 循环停止执行。
**总结**
range()
for 元素 in 序列:
while 条件:
continue
break

**更加灵活的循环方式**
1. 利用range(), 得到下标
在Python中，for循环后的in跟随一个序列的话，循环每次使用的序列元素，而不是序列的下标。
之前我们已经使用过range来控制for循环。现在，我们继续开发range的功能，以实现下标对循环的控制：
S ='abcdefghijk'foriinrange(0,len(S),2):printS[i]
在该例子中，我们利用len()函数和range()函数，用i作为S序列的下标来控制循环。在range函数中，分别定义上限，下限和每次循环的步长。这就和C语言中的for循环相类似了。
2. 利用enumerate(), 同时得到下标和元素
利用enumerate()函数，可以在每次循环中同时得到下标和元素：
S ='abcdefghijk'for(index,char)inenumerate(S):printindexprintchar
实际上，enumerate()在每次循环中，返回的是一个包含两个元素的定值表(tuple)，两个元素分别赋予index和char
3. 利用zip(), 实现并行循环
如果你多个等长的序列，然后想要每次循环时从各个序列分别取出一个元素，可以利用zip()方便地实现：
ta = [1,2,3]
tb= [9,8,7]
tc= ['a','b','c']for(a,b,c)inzip(ta,tb,tc):print(a,b,c)
每次循环时，从各个序列分别从左到右取出一个元素，合并成一个tuple，然后tuple的元素赋予给a,b,c
zip()函数的功能，就是从多个列表中，依次各取出一个元素。每次取出的(来自不同列表的)元素合成一个元组，合并成的元组放入zip()返回的列表中。
zip()函数起到了聚合列表的功能。我们还可以分解该聚合后的列表，如下:
ta = [1,2,3]
tb= [9,8,7]
\# clusterzipped=zip(ta,tb)print(zipped)
\# decompose
na, nb = zip(*zipped)
print(na, nb)
**总结：**
range()
enumerate()
zip()


循环对象的并不是随着Python的诞生就存在的，但它的发展迅速，特别是Python 3x的时代，从zip()或者map()的改变来看，循环对象正在成为循环的标准形式。
1. 什么是循环对象
循环对象是这样一个对象，它包含有一个next()方法(__next__()方法，在python 3x中)， 这个方法的目的是进行到下一个结果，而在结束一系列结果之后，举出StopIteration错误。
当一个循环结构（比如for）调用循环对象时，它就会每次循环的时候调用next()方法，直到StopIteration出现，for循环接收到，就知道循环已经结束，停止调用next()。
假设我们有一个test.txt的文件:
1234abcd
efg
我们运行一下python命令行：
>>> f = open('test.txt')
>>> f.next()
>>> f.next()
...
不断地输入f.next()，直到最后出现StopIteration
open()返回的实际上是一个循环对象，包含有next()方法。而该next()方法每次返回的就是新的一行的内容，到达文件结尾时举出StopIteration。这样，我们相当于手工进行了循环。
自动进行的话，就是：
forlineinopen('test.txt'):printline
在这里，for结构自动调用next()方法，将该方法的返回值赋予给line。循环知道出现StopIteration的时候结束。
相对于序列，用循环对象来控制循环的好处在于：可以不用在循环还没有开始的时候，就生成每次要使用的元素。所使用的元素在循环过程中逐次生成。这样，就节省了空间，提高了效率，并提高编程的灵活性。
2. iter()函数和循环器(iterator)
从技术上来说，循环对象和for循环调用之间还有一个中间层，就是要将循环对象转换成循环器(iterator)。这一转换是通过使用iter()函数实现的。但从逻辑层面上，常常可以忽略这一层，所以循环对象和循环器常常相互指代对方。
3. 生成器(generator)
生成器的主要目的是构成一个用户自定义的循环对象。
生成器的编写方法和函数定义类似，只是在return的地方改为yield。生成器中可以有多个yield。当生成器遇到一个yield时，会暂停运行生成器，返回yield后面的值。当再次调用生成器的时候，会从刚才暂停的地方继续运行，直到下一个yield。生成器自身又构成一个循环器，每次循环使用一个yield返回的值。
下面是一个生成器:
defgen():
    a= 100yielda
    a= a*8yieldayield1000
该生成器共有三个yield， 如果用作循环器时，会进行三次循环。
foriingen():printi
再考虑如下一个生成器:
defgen():foriinrange(4):yieldi
它又可以写成生成器表达式(Generator Expression):
G = (xforxinrange(4))
生成器表达式是生成器的一种简便的编写方式。读者可进一步查阅。
4. 表推导(list comprehension)
表推导是快速生成表的方法。假设我们生成表L：
L =[]forxinrange(10):
    L.append(x**2)
以上产生了表L，但实际上有快捷的写法，也就是表推导的方式:
L = [x**2forxinrange(10)]
这与生成器表达式类似，只不过用的是中括号。
（表推导的机制实际上是利用循环对象，有兴趣可以查阅。）
考虑下面的表推导会生成什么？
xl = [1,3,5]
yl= [9,12,13]
L= [ x**2for(x,y)inzip(xl,yl)ify > 10]
**总结：**
循环对象
生成器
表推导

**参考推荐：**
[Python 系列教程](http://www.cnblogs.com/vamei/tag/Python/)



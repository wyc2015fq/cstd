
# [Python学习] 专题二.条件语句和循环语句的基础知识 - 杨秀璋的专栏 - CSDN博客

2014年09月22日 19:45:44[Eastmount](https://me.csdn.net/Eastmount)阅读数：3661所属专栏：[Python学习系列](https://blog.csdn.net/column/details/eastmount-python.html)



**前面讲述了"****专题一.函数的基础知识****",而这篇文章讲述的Python的条件语句和循环语句的基础知识.****主要内容包括:**
**1.条件语句:包括单分支、双分支和多分支语句,if-elif-else**
**2.循环语句:while的使用及简单网络刷博器爬虫**
**3.循环语句:for的使用及遍历列表、元组、文件和字符串**
## 前言： 语句块
**在讲诉条件语句、循环语句和其他语句之前,先来补充语句块知识.(前面讲函数时已经用到过)**
**语句块并非一种语句,它是在条件为真(条件语句)时执行或执行多次(循环语句)的一组语句.在代码前放置空格或tab字符来缩进语句即可创建语句块.很多语言特殊单词或字符(如begin或{)来表示一个语句块的开始,用另外的单词或字符(如end或})来表示语句块的结束.**
**而在Python中使用冒号(:)来标识语句块的开始,块中每一个语句都是缩进的(缩进量相同).当回退到和已经闭合的块一样的缩进量时,就表示当前块已经结束.**
## 一. 条件语句if
**if分支语句表达式基本类型常见的有一下三种:**
**1.单分支语句**
**它的基本格式是:**
**if condition:**
**statement**
**statement**
**需要注意的是Ptthon中if条件语句条件无需圆括号(),条件后面需要添加冒号,它没有花括号{}而是使用TAB实现区分.其中condition条件判断通常有布尔表达式(True|False 0-假|1-真 非0即真)、关系表达式(>= <= == !=)和逻辑运算表达式(and or not).**
**2.双分支语句**
**它的基本格式是:**
**if condition:**
**statement**
**statement**
**else:**
**statement**
**statement**
**3.多分支语句**
**if多分支由if-elif-else组成,其中elif相当于else if,同时它可以使用多个if的嵌套.具体代码如下所示:**
```python
#双分支if-else
count = input("please input:")
print 'count=',count
if count>80:
    print 'lager than 80'
else:
    print 'lower than 80'
print 'End if-else'
#多分支if-elif-else
number = input("please input:")
print 'number=',number
if number>=90:
    print 'A'
elif number>=80:
    print 'B'
elif number>=70:
    print 'C'
elif number>=60:
    print 'D'
else:
    print 'No pass'
print 'End if-elif-else'
#条件判断
sex = raw_input("plz input your sex:")
if sex=='male' or sex=='m' or sex=='man':
    print 'Man'
else:
    print 'Woman'
```
## 二. 循环语句while
**while循环语句的基本格式如下:**
**while condition:**
**statement**
**statement**
**else:**
**statement**
**statement**
**其中判断条件语句condition可以为布尔表达式、关系表达式和逻辑表达式,else可以省略(此处列出为与C语言等区别).举个例子:**
```python
#循环while计数1+2+..+100
i = 1
s = 0
while i <= 100:
    s = s+i
    i = i+1
else:
    print 'exit while'
print 'sum = ',s
'''
输出结果为：exit while
          sum = 5050
'''
```
**它的输出结果为5050,当时当i加到101时,由于i>100将执行else语句.**
**需要注意的是Python中使用井号(\#)表示行注释,使用三引号('''...''')表示多行注释.区别于C/C++的//行注释和/**/多行注释.**
**下面讲述一段代码刷博器爬虫,先给出代码再讲解:**
```python
import webbrowser as web
import time
import os
i=0
while i<5:
    web.open_new_tab('http://andy111.blog.sohu.com/46684846.html')
    i=i+1
    time.sleep(0.8)
else:
    os.system('taskkill /F /IM iexplore.exe')
print 'close IE'
```
**在搜狐博客或新浪博客中只要在新窗口打开就会增加浏览访问次数,所以上面的代码主要是通过调用webbrowser浏览器的open_new_tab打开新的窗口,而CSDN不行(估计绑定用户或ip有关).**
**上面代码中windoes命令taskkill的作用是杀掉应用程序IE浏览器,在DOS中输入"taskkill /F /IM iexplore.exe"可以强行关闭应用程序(chrome.exe或qq.exe),其中/F表示强行终止程序,/IM表示图像.在该程序中主要的作用是清除内存,防止内存消耗太大出现死机现象;但是需要调用import os的system()函数打开,而Linux下用kill命令(kill -pid或killall)终止进程.**
![](https://img-blog.csdn.net/20140922135451421)
**代码中time.sleep(seconds)表示"Delay execution for a given number of seconds.",从打开到加载有一定时间.**
**当你需要大量增加浏览量时可以使用两层循环嵌套,每次打开5个网页在关闭在执行100次,这样你的内存也不会因为消耗太大出现死机现象,也可以使用import random count=random.randint(20,40)产生20到40随机数来执行外层循环.代码比较简单,主要是想通过它介绍些Python的基础知识.但是初次打开IE浏览器会出现打开次数不一致的错误.why?**
## 三. 循环语句for
**该循环语句的基础格式为:**
**for target in sequences:**
**statements**
**target表示变量名,sequences表示序列,常见类型有list(列表)、tuple(元组)、strings(字符串)和files(文件).**
**Python的for没有体现出循环的次数,不像C语言的for(i=0;i<10;i++)中i循环计数,Python的for指每次从序列sequences里面的数据项取值放到target里,取完即结束,取多少次循环多少次.其中in为成员资格运算符,检查一个值是否在序列中.同样可以使用break和continue跳出循环.**
**1.字符串循环**
```python
s1 = 'Eastmount of CSDN'
for c in s1:
    print c,
```
**注意:如果在print结尾加上逗号,那么接下来语句会与前一条语句在同一行打印.故上面输出显示一行.**
**2.列表循环**
```python
list1 = [1,3,4,5,'x',12.5]
i = 0
for val in list1:
    print format(i,'2d'),val
    i = i+1
else:
    print 'out for'
```
**注意:列表List由一堆数据用逗号间隔,方括号括起,可以是同类型也可以是不同类型.format(i,'2d')相当于输出两位,不足的补空格.当输出0-9时显示"口0",而输出10-99时显示"10"实现对其功能.输出结果如下：**
```python
1 3
 2 4
 3 5
 4 x
 5 12.5
out for
```
**因为迭代(循环另一种说法)某范围的数字是很常用的,所以有个内建的范围函数range供使用.列表中for n in [1,2,3,4,5,6,7,8]相当于listNum=range(1,9).其格式"range(start, stop[, step]) -> list of integers",它的工作方式类似于分片,它包含下限(本例range(1,9)中为1),但不包含上限(本例中9),如果希望下限为0,可以只提供上限如range(4)=[0,1,2,3].**
**产生1到100的数字range(1,101),输出1到100的奇数range(1,101,2),输出1到100的偶数range(2,101,2).**
**3.元组循环**
```python
tup = (1,2,3,4,5)
for n in tup:
    print n
else:
    print 'End for'
```
**元组tuple每个数据项不可修改,只可读,而序列list[1,2,3,4]可以修改.**
**4.文件循环**
**help(file.read)返回一个字符串."read([size]) -> read at most size bytes, returned as a string."**
**help(file.readlines)返回一个列表."readlines([size]) -> list of strings, each a line from the file."相当于读n行,由n次readline组成,读出的字符串构成列表.**
**help(file.readline)从某个文件读一行."readline([size]) -> next line from the file, as a string."**
```python
#文件循环遍历三种对比
for n in open('for.py','r').read():
    print n,
print 'End'
for n in open('for.py','r').readlines():
    print n,
print 'End'
for n in open('for.py','r').readline():
    print n,
print 'End'
```
**输出显示：**
```python
#第一个read()输出:每个字符间有个空格
s 1   =   ' E a s t m o u n t   o f   C S D N ' 
f o r   c   i n   s 1 :
....
End
#第二个readlines()输出:读取的是一行
s1 = 'Eastmount of CSDN'
for c in s1:
....
End
#第三个readline()输出:读取for.py文件第一行并输出
s 1   =   ' E a s t m o u n t   o f   C S D N '
End
```
**如果需要文件输出也可以通过下面代码实现,使用w会覆盖而a+是追加功能,后面讲文件详细叙述.**
**for r in open('test.txt','r').readlines():**
**open('test.txt','a+').write(c)**
**PS:我主要是通过《Python基础教程》和"51CTO学院 智普教育的python视频"学习.所以文中引用了很多视频中的知识、书籍知识和自己的知识,感谢那些作者和老师,希望文章对大家有所帮助,才开始学习python知识,如果文章中有错误或不足之处,还请海涵,也希望大家提出意见与君共勉.勿喷~**
**(By:Eastmount 2014-9-22 夜7点 原创CSDN****http://blog.csdn.net/eastmount/****)**



# [Python学习] 专题三.字符串的基础知识 - 杨秀璋的专栏 - CSDN博客

2014年09月28日 11:10:23[Eastmount](https://me.csdn.net/Eastmount)阅读数：3678所属专栏：[Python学习系列](https://blog.csdn.net/column/details/eastmount-python.html)



**在Python中最重要的数据类型包括字符串、列表、元组和字典等.该篇主要讲述Python的字符串基础知识.**
## 一.字符串基础
**字符串指一有序的字符序列集合,用单引号、双引号、三重(单双均可)引号引起来.如:**
**s1='www.csdn.net'   s2="www.csdn.net"   s3='''aaabbb'''**
**其中字符串又包括:**
**1.转义字符串**
**像C语言中定义了一些字母前加"\"来表示常见的那些不能显示的ASCII字符,python也有转义字符.如下:**
**\\-反斜杠符号   \'-单引号   \"-双引号  \ a-响铃   \b-退格(Backspace)**
**\n-换行   \r-回车   \f-换页   \v-纵向制表符   \t-横向制表符   \e-转义**
**\000-空   \oyy-八进制数yy代表的字符   \xyy-十进制yy代表的字符**
**2.raw字符串**
**Python中原始字符串(raw strings),r关闭转义机制.告诉Python后面是连串,"\"不当转义字符处理.例:**
```python
#转义字符和raw字符
s1="aa\nbb"
print s1
s2=r"aa\nbb"
print s2
#输出
aa
bb
aa\nbb
#raw原始字符串处理磁盘路径
open(r'C:\temp\test.txt','a+')
open('C:\\temp\\test.txt','a+')
```
**3.unicode字符串**
**告诉Python是Unicode编码,Unicode(统一码、万国码)是一种在计算机上使用的字符编码.在Unicode之前用的都是ASCII码,Unicode通过使用一个或者多个字节来表示一个字符.Python里面默认所有字面上的字符串都用ASCII编码,可以通过在字符串前面加一个'ｕ'前缀的方式声明Unicode字符串,这个'ｕ'前缀告诉Python后面的字符串要编成Unicode字符串.例:s=u'aa\nbb'**
**中文处理一直很让人头疼,推荐:****Unicode和Python的中文处理**
**4.格式化字符串**
**字符串格式化功能使用字符串格式化操作符%(百分号)实现,在%的左侧放置一个字符串(格式化字符串),而右侧放置希望格式化的值,也可是元组和字典.如果需要在字符串里包括百分号,使用%%.如果右侧是元组的话,则其中每一个元素都会被单独格式化,每个值都对应一个转化说明符.例:**
**"your age %d,sex %s,record %f"%(28,"Male",78.5)**
**输出:'your age 28,sex Male,record 78.500000'**
**它有点类似于C语言的printf("%d",x),其中百分号%相当于C语言的逗号.其中字符串格式化转换类型如下:**
**d,i  带符号的十进制整数**
**o   不带符号的八进制**
**u   不带符号的十进制**
**x   不带符号的十六进制(小写)**
**X   不带符号的十六进制(大写)**
**e,E  科学计数法表示的浮点数(小写,大写)**
**f,F   十进制浮点数**
**c   单字符**
**r    字符串(使用repr转换的任意Python)**
**s   字符串(使用str转换的任意Python)**
**g,G  指数大于4或小于精度值和e相同,否则和f相同**
## 二.字符串操作
**字符串的基础操作包括分割,索引,乘法,判断成员资格,求长度等.**
**1.+连接操作**
**如:s1='csdn' s2='Eastmount' s3=s1+s2**
**print s1,s2 => 输出:csdn Eastmount**
**print s3 => 输出:csdnEastmount**
**2.*重复操作**
**如:s1='abc'*5**
**print s1 => 输出:abcabcabcabcabc**
**3.索引s[index]**
**Python的索引格式string_name[index],可以访问字符串里面的字符成员.**
**4.切片s[i:j]**
**Python中切片的基本格式是s[i:j:step],其中step表示切片的方向,起点不写从0开始,终点不写切到最后.如:**
**s='abcdefghijk'**
**sub=s[3:8]**
**print sub => 输出defgh_**
**3      78 (起点是3 终点8不取)**
**其中当step=-1时表示反方向切片.如:**
**s='abcdefghijk'**
**sub=s[-1:-4:-1]**
**print sub => 输出kji**
**因为最后一个"-1"表示从反方向切片,s[9]='j' s[-2]='j',正方向第一个'a'索引下标值为0,最后一个'k'索引下标值为-1.故'j'为-2,而sub[-1:-4:-1]表示从k(-1位置)切到h(-4位置,但不取该值).故结果为"kji".**
**如果想完成字符串逆序,s='www.baidu.com',则可s1=[-1::-1]即可.起点为m(-1),无终点表示切到最后.**
**5.字段宽度和精度**
**前面讲述的format()函数中涉及到该知识,如'%6.2f'%12.345678 输出"口12.35"其中6表示字段宽度,2表示精度,故补一个空格,同时采用四舍五入的方法结果输出12.35.**
**同时,零(0)可表示数字将会用0填充,减号(-)用来实现左对齐数值,空白(" ")意味着正数前加上空格,在正负数对其时非常有用,加号表示不管正数还是负数都标识出符号,对齐时也有用.例:**
```python
#字段宽度和精度
num = 12.345678
s1 = '%6.2f'%num
print s1
#补充0
s2 = '%08.2f'%num
print s2
#减号实现左对齐
s3 = '%-8.2f'%num
print s3
#空白
print ('% 5d'%10) + '\n' + ('% 5d'%-10)
#符号
print ('%+5d'%10) + '\n' + ('%+5d'%-10)
#输出
 12.35
00012.35
12.35   
   10
  -10
  +10
  -10
```
## 三.字符串方法
**字符串从string模块中"继承"了很多方法,下面讲解一些常用的方法:**
**find()**
**在一个较长的字符串中查找子字符串,它返回子串所在位置的最左端索引,如果没有找到则返回-1.其格式为"S.find(sub [,start [,end]]) -> int",其中该方法可接受可选的起始点和结束点参数.而rfind()从右往左方向查找.**
```python
title = 'Hello Python,Great Python'
length = len(title)
print length
print title.find('Python')
print title.find('Python',10,30)
#输出:
25
6
19
```
**join()**
**其格式为"S.join(iterable) -> string",含义为"Return a string which is the concatenation of the strings in the iterable. The separator between elements is S."即用来在队列中添加元素,但队列中元素必须是字符串.它是split方法的逆方法.**
```python
seq = ['1','2','3','4']
sep = '+'
print sep.join(seq)   #连接字符串列表 sep表示'+'连接
dirs = '','usr','bin','env'
print '/'.join(dirs)
print 'C:'+'\\'.join(dirs)
#输出
1+2+3+4
/usr/bin/env
C:\usr\bin\env
```
**split()**
**字符串分割函数,格式为"S.split([sep [,maxsplit]]) -> list of strings",将字符串分割成序列,如果不提供分割符,程序将会把所有空格作为分隔符.**
```python
#按空格拆分成4个单词,返回list
s = 'please use the Python!'
li = s.split()
print li
print '1+2+3+4+5'.split('+')
#输出
['please', 'use', 'the', 'Python!']
['1', '2', '3', '4', '5']
```
**strip()**
**去掉开头和结尾的空格键(两侧且不包含内部),S.strip([chars])可以去除指定字符.而函数lstrip()去除字符串最开始的所有空格,rstrip()去除字符串最尾部的所有空格.**
**replace()**
**该方法返回某字符串的所有匹配项均被替换后得到字符串,如文字处理程序中"查找并替换"功能.**
**translate()**
**该方法和replace一样,可以替换字符串中某部分,但与前者的区别是translate只处理单个字符,它的优势在于可以同时替换多个,有时候效率比replace高.**
**如:s='eastmount' s1=s.replace('e','E') => 替换后'Eastmount'**
**字符串判断方法**
**isalnum()判断是否都是有效字符(字母+数字),如判断密码帐号,输出Ture\False.**
**isalpha()判断是否是字母**
**isdigit()判断是否是数字**
**islower()判断是否全是小写**
**isupper()判断是否全是大写**
**isspace()判断是否是空格(' ')**
**lower()**
**该方法返回字符串的小写字母版,在判断用户名不区分大小写时使用.upper()转换为大写,title()函数将字符串转换为标题——所有单词的首字母大写,而其他字母小写,但是它使用的单词划分方法可能会得到不自然的结果.**
```python
s = 'this is a good idea'
s1 = s.upper()
print s1
s2 = s.title()
print s2
#输出
THIS IS A GOOD IDEA
This Is A Good Idea
```
**PS:我主要是通过《Python基础教程》和"51CTO学院 智普教育的python视频"学习.所以文中引用了很多视频中的知识、书籍知识和自己的知识,感谢那些作者和老师,希望文章对大家有所帮助,才开始学习python知识,如果文章中有错误或不足之处,还请海涵,也希望大家提出意见与君共勉.勿喷~**
**(By:Eastmount 2014-9-28 中午11点 原创CSDN****http://blog.csdn.net/eastmount/****)**


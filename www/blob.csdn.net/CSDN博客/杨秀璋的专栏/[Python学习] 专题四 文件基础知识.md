
# [Python学习] 专题四.文件基础知识 - 杨秀璋的专栏 - CSDN博客

2014年10月08日 12:11:30[Eastmount](https://me.csdn.net/Eastmount)阅读数：2730标签：[python																](https://so.csdn.net/so/search/s.do?q=python&t=blog)[文件																](https://so.csdn.net/so/search/s.do?q=文件&t=blog)[基础知识																](https://so.csdn.net/so/search/s.do?q=基础知识&t=blog)[迭代器																](https://so.csdn.net/so/search/s.do?q=迭代器&t=blog)[操作																](https://so.csdn.net/so/search/s.do?q=操作&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=迭代器&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=基础知识&t=blog)个人分类：[Python基础知识																](https://blog.csdn.net/Eastmount/article/category/2547623)
[
																					](https://so.csdn.net/so/search/s.do?q=基础知识&t=blog)所属专栏：[Python学习系列](https://blog.csdn.net/column/details/eastmount-python.html)[
							](https://so.csdn.net/so/search/s.do?q=基础知识&t=blog)
[
																	](https://so.csdn.net/so/search/s.do?q=文件&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=python&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=python&t=blog)

**前面讲述了函数、语句和字符串的基础知识,该篇文章主要讲述文件的基础知识(与其他语言非常类似).**
## 一. 文件的基本操作
**文件是指存储在外部介质(如磁盘)上数据的集合.文件的操作流程为:**
**打开文件(读方式\写方式)->读写文件(read\readline\readlines\write\writelines)->关闭文件**
**1.打开文件**
**调用函数open打开文件,其函数格式为:**
**file_obj=open(filename[, mode[, buffering]]) 返回一个文件对象(file object)**
**— filename文件名(唯一强制参数)**
**·原始字符串 r'c:\temp\test.txt'**
**·转移字符串 'c:\\temp\\test.txt'**
**— mode文件模式**
**·r 读模式**
**·w 写模式**
**·a 追加模式(写在上次后面)**
**·+ 读/写模式(没有文件即创建,可添加到其他模式中使用)**
**·b 二进制模式(可添加到其他模式中使用)**
**— buffering缓冲(可选参数)**
**·参数=0或False 输入输出I/O是无缓冲的,所有读写操作针对硬盘**
**·参数=1或True 输入输出I/O是有缓冲的,内存替代硬盘**
**·参数>1数字代表缓冲区的大小,单位字节.-1或负数代表使用默认缓冲区大小**
**注意:当处理二进制文件如声音剪辑或图像时使用'b'二进制模式,可以'rb'读取一个二进制文件.**
**2.关闭文件**
**应该牢记使用close方法关闭文件,因为Python可能会缓存(出于效率考虑把数据临时存储某处)写入数据,如果程序突然崩溃,数据根本不会被写入文件,为安全起见,在使用完文件后关闭.如果想确保文件被关闭,应该使用try/finally语句,并且在finally子句中调用close方法.如:**
**\#Open your file**
**try:**
**\#Write data to your file**
**finally:**
**file.close()**
**3.读写文件**
**调用函数write方法向文件中写入数据,其函数格式为:**
**file_obj.write(string) 参数string会被追加到文件中已存部分后面**
**file_obj.writelines(sequence_of_strings) 仅传递一个参数,列表[ ] 元组() 字典{}**
**注意:实用字典时字符串的顺序出现是随机的.**
```python
#使用write()写文件
file_obj=open('test.txt','w')
str1='hello\n'
str2='world\n'
str3='python'
file_obj.write(str1)
file_obj.write(str2)
file_obj.write(str3)
file_obj.close()
#使用writelines()写文件
file_obj=open('test.txt','w')
str1='hello\n'
str2='world\n'
str3='python'
file_obj.writelines([str1,str2,str3])
file_obj.close()
#输出 本地test.txt文件
hello
word
python
```
**调用函数read方法读取数据,其函数格式为:var=file_obj.read(),其中read全部读取,返回string;readline读取一行,返回string;readlines读取文件所有行,返回a list of string.例:**
```python
#使用read
print 'Use the read'
file_obj=open('test.txt','r')
s=file_obj.read()
print s
file_obj.close
#使用readline
print 'Use the readline'
file_obj=open('test.txt','r')
line1=file_obj.readline()
line1=line1.rstrip('\n')
print 'l1 ',line1
line2=file_obj.readline()
line2=line2.rstrip('\n')
print 'l2 ',line2
line3=file_obj.readline()
line3=line3.rstrip('\n')
print 'l3 ',line3
file_obj.close
#使用readlines
print 'Use the readlines'
file_obj=open('test.txt','r')
li=file_obj.readlines()
print li
file_obj.close
```
**输出内容如下:**
```python
Use the read
hello
world
python
Use the readline
l1  hello
l2  world
l3  python
Use the readlines
['hello\n', 'world\n', 'python']
```
**可以发现在使用readline()函数时它返回的结果是'hello\n'字符串,需要使用rstrip去除'\n',否则print输出时总空一行.同时写入文件时使用格式化写入比较方便,如s="xxx%dyyy%s\n"%(28,'csdn').**
```python
#格式化写入
fd=open('format.txt','w')
head="%-8s%-10s%-10s\n"%('Id','Name','Record')
fd.write(head)
item1="%-8d%-10s%-10.2f\n"%(10001,'Eastmount',78.9)
fd.write(item1)
item2="%-8d%-10s%-10.2f\n"%(10002,'CSDN',89.1234)
fd.write(item2)
fd.close()
#输出
Id      Name      Record    
10001   Eastmount 78.90     
10002   CSDN      89.12
```
## 二. 文件与循环
**前面介绍了文件的基本操作和使用方法,但是文件操作通常会与循环联系起来,下面介绍while循环和for循环实现文件操作.代码如下:**
```python
#使用while循环
fr=open('test.txt','r')
str=fr.readline()
str=str.rstrip('\n')
while str!="":
    print str
    str=fr.readline()
    str=str.rstrip('\n')
else:
    print 'End While'
fr.close
#使用for循环
rfile=open('test.txt','r')
for s in rfile:
    s=s.rstrip('\n')
    print s
print 'End for'
rfile.close()
```
**其中for调用迭代器iterator,迭代器提供一种方法顺序访问一个聚合对象中的各个元素,它相当于通过Iter函数获取对象的迭代器,再通过next函数(该方法调用时不需要任何参数)获取下一个值.for可以遍历iterator_obj包括List\String\Tuple\Dict\****File.如:**
**s='www.csdn.net'**
**si=iter(s)          \#生成迭代器**
**print si.next()  \#调用next依次获取元素,最后迭代器没有返回值时引发StopIteration异常**
## 三. 总结
**该篇文章主要讲述了Python文件基础知识,包括文件的打开、读写、关闭操作、使用循环读写文件及迭代器的知识.希望对大家有所帮助,如果有错误或不足之处,还请海涵!**
**(By:Eastmount 2014-10-8 中午11点 原创CSDN****http://blog.csdn.net/eastmount/****)**
**参考资料:**
**1.****51CTO学院 智普教育的python视频****http://edu.51cto.com/course/course_id-581.html**
**2.《Python基础教程（第2版）》Magnus Lie Hetland[挪]著**


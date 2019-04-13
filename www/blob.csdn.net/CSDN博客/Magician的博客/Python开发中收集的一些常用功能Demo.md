
# Python开发中收集的一些常用功能Demo - Magician的博客 - CSDN博客


2018年05月29日 10:20:22[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：258所属专栏：[Python总结](https://blog.csdn.net/column/details/26813.html)




### 文章目录
[目录：](#_2)
[前言：](#_5)
[1、Python判断文件是否存在的几种方法：](#1Python_9)
[2、Python中写入List到文本中并换行的方法](#2PythonList_79)
[3、Python按行读取文件的几种简单实现方法](#3Python_103)
[4、Python在一个文件中追加文件内容](#4Python_155)
[5、Python去掉字符串中空格的几种方法](#5Python_195)
[6、Python实现定时任务](#6Python_220)

# 目录：
## 前言：
虽然之前自学了些Python基础，但是一段时间没用再加上学的时间也比较短，搞的现在重拾Python写代码的时候，总要去查一些API（还是Coding不够，仍需多加实践），但整个过程又比较耗时。所以就准备把之前以及以后遇到的一些常用的功能Demo给记录下来，方便自己后来查看。此文章会不定时一直更新。
## 1、Python判断文件是否存在的几种方法：
通常在读写文件之前，需要判断文件或目录是否存在，不然某些处理方法可能会使程序出错。所以最好在做任何操作之前，先判断文件是否存在。
这里将介绍三种判断文件或文件夹是否存在的方法，**分别使用os模块、Try语句、pathlib模块**
**1.1、使用os模块**
os模块中的os.path.exists()方法用于检验文件是否存在。
判断文件或文件夹是否存在
`import os
os.path.exists(test_file.txt)
#True
os.path.exists(no_exist_file.txt)
#False`其实这种方法还是有个问题，假设你想检查文件“test_data”是否存在，但是当前路径下有个叫“test_data”的文件夹，这样就可能出现误判。为了避免这样的情况，可以这样:
只检查文件
`import os
os.path.isfile("test-data")`通过这个方法，如果文件”test-data”不存在将返回False，反之返回True。
即是文件存在，你可能还需要判断文件是否可进行读写操作。
**1.2、使用Try语句（比较严谨的写法）**
可以在程序中直接使用open()方法来检查文件是否存在和可读写。
语法：
open()
如果你open的文件不存在，程序会抛出错误，使用try语句来捕获这个错误。
程序无法访问文件，可能有很多原因：
如果你open的文件不存在，将抛出一个FileNotFoundError的异常;
文件存在，但是没有权限访问，会抛出一个PersmissionError的异常。
其实没有必要去这么细致的处理每个异常，上面的这两个异常都是IOError的子类。所以可以将程序简化一下:
`try:
    f =open()
    f.close()
except IOError:
    print "File is not accessible."`使用try语句进行判断，处理所有异常非常简单和优雅的。而且相比其他不需要引入其他外部模块。
**1.3、使用pathlib模块**
pathlib模块在Python3版本中是内建模块，但是在Python2中是需要单独安装三方模块。
使用pathlib需要先使用文件路径来创建path对象。此路径可以是文件名或目录路径。
检查路径是否存在
`path = pathlib.Path("path/file")
path.exist()`检查路径是否是文件
`path = pathlib.Path("path/file")
path.is_file()`
## 2、Python中写入List到文本中并换行的方法
f.writelines(lists) 是不换行的写入，可用以下方法在写入时换行。
**方法一：**
`for line in lists:
	f.write(line+'\n')`**方法二：**
`lists=[line+"\n" for line in lists]
f.writelines(lists)`**方法三：**
`f.write('\n'.join(lists))`
## 3、Python按行读取文件的几种简单实现方法
**3.1、readline()**
`file = open("sample.txt") 
while 1:
  line = file.readline()
  if not line:
    break
  pass # do something
file.close()`一行一行得从文件读数据，显然比较慢；不过很省内存；测试读10M的sample.txt文件，每秒大约读32000行；
**3.2、fileinput**
`import fileinput 
for line in fileinput.input("sample.txt"):
  pass`写法简单一些，不过测试以后发现每秒只能读13000行数据，效率比上一种方法慢了两倍多；
**3.3、readlines()**
`file = open("sample.txt") 
while 1:
  lines = file.readlines(100000)
  if not lines:
    break
  for line in lines:
    pass # do something
file.close()`用同样的数据测试，它每秒可以读96900行数据！效率是第一种方法的3倍，第二种方法的7倍！
**3.4、文件迭代器**
每次只读取和显示一行，读取大文件时应该这样：
`file = open("sample.txt") 
for line in file:
  pass # do something
file.close()`
## 4、Python在一个文件中追加文件内容
刚开始用的如下的write()方法，发下会先把原文件的内容清空再写入新的东西，文件里面每次都是最新生成的一个账号
`mobile = Method.createPhone()
file = r'D:\test.txt'
with open(file, 'w+') as f:
      f.write(mobile)`查了资料，关于open()的mode参数（**对于open方法来说本身就可以创建一个文件不需要重新new一个对象**）：
`'r'：读
'w'：写
'a'：追加
'r+' == r+w（可读可写，文件若不存在就报错(IOError)）
'w+' == w+r（可读可写，文件若不存在就创建）
'a+' ==a+r（可追加可写，文件若不存在就创建）
对应的，如果是二进制文件，就都加一个b就好啦：
'rb'　　'wb'　　'ab'　　'rb+'　　'wb+'　　'ab+'`发现方法用错了，像这种不断生成新账号 增加写入的，应该用追加‘a’
改为如下后，解决：
`mobile = Method.createPhone()
file = r'D:\test.txt'
with open(file, 'a+') as f:
     f.write(mobile+'\n')   #加\n换行显示`
## 5、Python去掉字符串中空格的几种方法
strip()：把头和尾的空格去掉
lstrip()：把左边的空格去掉
rstrip()：把右边的空格去掉
replace(‘c1’,‘c2’)：把字符串里的c1替换成c2。故可以用replace(’ ‘,’’)来去掉字符串里的所有空格
split()：通过指定分隔符对字符串进行切片，如果参数num 有指定值，则仅分隔 num 个子字符串
`In[2]: a='     ddd dfe dfd     efre    ddd   '
In[3]: a
Out[3]: '     ddd dfe dfd     efre    ddd   '
In[4]: a.strip()
Out[4]: 'ddd dfe dfd     efre    ddd'
In[5]: a.lstrip()
Out[5]: 'ddd dfe dfd     efre    ddd   '
In[6]: a.rstrip()
Out[6]: '     ddd dfe dfd     efre    ddd'
In[7]: a.replace(' ','')
Out[7]: 'ddddfedfdefreddd'
In[8]: a.split()
Out[8]: ['ddd', 'dfe', 'dfd', 'efre', 'ddd']`
# 6、Python实现定时任务
**threading的Timer：**
threading模块中的Timer能够帮助实现定时任务，而且是非阻塞的。
比如每3秒打印一次helloworld：
`def printHello(): 
  print "Hello World" 
  t = Timer(2, printHello) 
  t.start() 
 
 
if __name__ == "__main__": 
  printHello()`参考：
[http://www.spiderpy.cn/blog/detail/28](http://www.spiderpy.cn/blog/detail/28)
[https://www.cnblogs.com/SheilaSun/p/4380933.html](https://www.cnblogs.com/SheilaSun/p/4380933.html)
[http://www.jb51.net/article/87103.htm](http://www.jb51.net/article/87103.htm)
[https://www.cnblogs.com/feiyueNotes/p/7897064.html](https://www.cnblogs.com/feiyueNotes/p/7897064.html)
[https://www.cnblogs.com/zywscq/p/5325604.html](https://www.cnblogs.com/zywscq/p/5325604.html)
[https://www.jb51.net/article/104926.htm](https://www.jb51.net/article/104926.htm)


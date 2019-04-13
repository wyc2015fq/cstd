
# Python基础学习笔记之（二） - zouxy09的专栏 - CSDN博客


2013年11月24日 16:04:46[zouxy09](https://me.csdn.net/zouxy09)阅读数：16045


**Python基础学习笔记之（二）**
zouxy09@qq.com
http://blog.csdn.net/zouxy09
**六、包与模块**
**1、模块module**
Python中每一个.py脚本定义一个模块，所以我们可以在一个.py脚本中定义一个实现某个功能的函数或者脚本，这样其他的.py脚本就可以调用这个模块了。调用的方式有三种，如下：

```python
###################################
## package and module ####
## a .py file define a module which can be used in other script
## as a script, the name of module is the same as the name of the .py file
## and we use the name to import to a new script
## e.g., items.py, import items
## python contains many .py files, which we can import and use
# vi cal.py
def add(x, y):
	return x + y
def sub(x, y):
	return x - y
def mul(x, y):
	return x * y
def div(x, y):
	return x / y
print "Your answer is: ", add(3, 5)
if __name__ == "__main__"
	r = add(1, 3)
	print r
	
# vi test.py
import cal # will expand cal.py here
# so, this will execute the following code in cal.py
# print "Your answer is: ", add(3, 5)
# it will print "Your answer is: 8"
# but as we import cal.py, we just want to use those functions
# so the above code can do this for me, the r=add(1, 3) will not execute
result = cal.add(1, 2)
print result
# or
import cal as c
result = c.add(1, 2)
# or
from cal import add
result = add(1, 2)
```
**2、包package**
python 的每个.py文件执行某种功能，那有时候我们需要多个.py完成某个更大的功能，或者我们需要将同类功能的.py文件组织到一个地方，这样就可以很方便我们的使用。模块可以按目录组织为包，创建一个包的步骤：
\# 1、建立一个名字为包名字的文件夹
\# 2、在该文件夹下创建一个__init__.py空文件
\# 3、根据需要在该文件夹下存放.py脚本文件、已编译拓展及子包
\# 4、import pack.m1,pack.m2 pack.m3

```python
#### package 包
## python 的模块可以按目录组织为包，创建一个包的步骤：
# 1、建立一个名字为包名字的文件夹
# 2、在该文件夹下创建一个__init__.py 空文件
# 3、根据需要在该文件夹下存放.py脚本文件、已编译拓展及子包
# 4、import pack.m1, pack.m2 pack.m3
mkdir calSet
cd calSet
touch __init_.py
cp cal.py .
# vi test.py
import calSet.cal
result = calSet.cal.add(1, 2)
print result
```
**七、正则表达式**
正则表达式，（英语：RegularExpression，在代码中常简写为regex、regexp或RE），计算机科学的一个概念。正则表达式使用单个字符串来描述、匹配一系列符合某个句法规则的字符串。在很多文本编辑器里，正则表达式通常被用来检索、替换那些符合某个模式的文本。
Python提供了功能强大的正则表达式引擎re，我们可以利用这个模块来利用正则表达式进行字符串操作。我们用import re来导入这个模块。
正则表达式包含了很多规则，如果能灵活的使用，在匹配字符串方面是非常高效率的。更多的规则，我们需要查阅其他的资料。
**1、元字符**
很多，一些常用的元字符的使用方法如下：

```python
##############################
## 正则表达式 RE
## re module in python
import re
rule = r'abc' # r prefix, the rule you want to check in a given string
re.findall(rule, "aaaaabcaaaaaabcaa") # return ['abc', 'abc']
# [] 用来指定一个字符集 [abc] 表示 abc其中任意一个字符符合都可以
rule = r"t[io]p" 
re.findall(rule, "tip tep twp top") # return ['tip', 'top']
# ^ 表示 补集，例如[^io] 表示除i和o外的其他字符
rule = r"t[^io]p" 
re.findall(rule, "tip tep twp top") # return ['tep', 'twp']
# ^ 也可以 匹配行首，表示要在行首才匹配，其他地方不匹配
rule = r"^hello"
re.findall(rule, "hello tep twp hello") # return ['hello']
re.findall(rule, "tep twp hello") # return []
# $ 表示匹配行尾
rule = r"hello$"
re.findall(rule, "hello tep twp hello") # return ['hello']
re.findall(rule, "hello tep twp") # return []
# - 表示范围
rule = r"x[0123456789]x" # the same as
rule = r"x[0-9]x"
re.findall(rule, "x1x x4x xxx") # return ['x1x', 'x4x']
rule = r"x[a-zA-Z]x"
# \ 表示转义符
rule = r"\^hello"
re.findall(rule, "hello twp ^hello") # return ['^hello']
# \d 匹配一个数字字符。等价于[0-9]。
# \D 匹配一个非数字字符。等价于[^0-9]。
# \n 匹配一个换行符。等价于\x0a和\cJ。
# \r 匹配一个回车符。等价于\x0d和\cM。
# \s 匹配任何空白字符，包括空格、制表符、换页符等等。等价于[ \f\n\r\t\v]。
# \S 匹配任何非空白字符。等价于[^ \f\n\r\t\v]。
# \t 匹配一个制表符。等价于\x09和\cI。
# \w 匹配包括下划线的任何单词字符。等价于“[A-Za-z0-9_]”。
# \W 匹配任何非单词字符。等价于“[^A-Za-z0-9_]”。
# {} 表示重复规则
# 例如我们要查找匹配是否是 广州的号码，020-八位数据
# 以下三种方式都可以实现
rule = r"^020-\d\d\d\d\d\d\d\d$"
rule = r"^020-\d{8}$" # {8} 表示前面的规则重复8次
rule = r"^020-[0-9]{8}$"
re.findall(rule, "020-23546813") # return ['020-23546813']
# * 表示将其前面的字符重复0或者多次
rule = r"ab*"
re.findall(rule, "a") # return ['a']
re.findall(rule, "ab") # return ['ab']
# + 表示将其前面的字符重复1或者多次
rule = r"ab+"
re.findall(rule, "a") # return []
re.findall(rule, "ab") # return ['ab']
re.findall(rule, "abb") # return ['abb']
# ? 表示前面的字符可有可无
rule = r"^020-?\d{8}$"
re.findall(rule, "02023546813") # return ['020-23546813
re.findall(rule, "020-23546813") # return ['020-23546813']
re.findall(rule, "020--23546813") # return []
# ? 表示非贪婪匹配
rule = r"ab+?"
re.findall(rule, "abbbbbbb") # return ['ab']
# {} 可以表示范围
rule = r"a{1,3}"
re.findall(rule, "a") # return ['a']
re.findall(rule, "aa") # return ['aa']
re.findall(rule, "aaa") # return ['aaa']
re.findall(rule, "aaaa") # return ['aaa', 'a']
## compile re string
rule = r"\d{3,4}-?\d{8}"
re.findall(rule, "020-23546813")
# faster when you compile it
# return a object
p_tel = re.compile(rule)
p_tel.findall("020-23546813")
# the parameter re.I 不区分大小写
name_re = re.compile(r"xiaoyi", re.I)
name_re.findall("Xiaoyi")
name_re.findall("XiaoYi")
name_re.findall("xiAOyi")
```

**2、常用函数**
Re模块作为一个对象，它还支持很多的操作，例如：

```python
# the object contain some methods we can use
# match 去搜索字符串开头，如果匹配对，那就返回一个对象，否则返回空
obj = name_re.match('Xiaoyi, Zou')
# search 去搜索字符串（任何位置），如果匹配对，那就返回一个对象
obj = name_re.search('Zou, Xiaoyi')
# 然后可以用它来进行判断某字符串是否存在我们的正则表达式
if obj:
	pass
# findall 返回一个满足正则的列表
name_re.findall("Xiaoyi")
# finditer 返回一个满足正则的迭代器
name_re.finditer("Xiaoyi")
# 正则替换
rs = r"z..x"
re.sub(rs, 'python', 'zoux ni ziox me') # return 'python ni python me'
re.subn(rs, 'python', 'zoux ni ziox me') # return ('python ni python me', 2), contain a number
# 正则切片
str = "123+345-32*78"
re.split(r'[\+\-\*]', str) # return ['123', '345', '32', '78']
# 可以打印re模块支持的属性和方法，然后用help
dir(re)
##### 编译正则表达式式 可以加入一些属性，可以增加很多功能
# 多行匹配
str = """
	hello xiaoyi
	xiaoyi hello
	hello zou
	xiaoyi hello
	"""
re.findall(r'xiaoyi', str, re.M)
```
**3、分组**
分组有两个作用，它用()来定义一个组，组内的规则只对组内有效。

```python
# () 分组
email = r"\w{3}@\w+(\.com|\.cn|\.org)"	
re.match(email, "zzz@scut.com")
re.match(email, "zzz@scut.cn")
re.match(email, "zzz@scut.org")
```
另外，分组可以优先返回分组内匹配的字符串。

```python
# 另外，分组可以优先返回分组内匹配的字符串
str = """
	idk hello name=zou yes ok d
	hello name=xiaoyi yes no dksl
	dfi lkasf dfkdf hello name=zouxy yes d
	"""
r1 = r"hello name=.+ yes"
re.findall(r1, str) # return ['hello name=zou yes', 'hello name=xiaoyi yes', 'hello name=zouxy yes']
r2 = r"hello name=(.+) yes"
re.findall(r2, str) # return ['zou', 'xiaoyi', 'zouxy']
# 可以看到，它会匹配整个正则表达式，但只会返回()括号分组内的字符串，
# 用这个属性，我们就可以进行爬虫，抓取一些想要的数据
```
**4、一个小实例-爬虫**
这个实例利用上面的正则和分组的优先返回特性来实现一个小爬虫算法。它的功能是到一个给定的网址里面将.jpg后缀的图片全部下载下来。

```python
## 一个小爬虫
## 下载贴吧 或 空间中的所有图片
## getJpg.py
#!/usr/bin/python
import re
import urllib
# Get the source code of a website
def getHtml(url):
	print 'Getting html source code...'
	page = urllib.open(url)
	html = page.read()
	return html
# Open the website and check up the address of images,
# and find the common features to decide the re_rule
def getImageAddrList(html):
	print 'Getting all address of images...'
	rule = r"src=\"(.+\.jpg)\" pic_ext"
	imReg = re.compile(rule)
	imList = re.findall(imReg, html)
	return imList
def getImage(imList):
	print 'Downloading...'
	name = 1;
	for imgurl in imList:
		urllib.urlretrieve(imgurl, '%s.jpg' % name)
		name += 1
	print 'Got ', len(imList), ' images!'
## main
htmlAddr = "http://tieba.baidu.com/p/2510089409"
html = getHtml(htmlAddr)
imList = getImageAddrList(html)
getImage(imList)
```
**八、深拷贝与浅拷贝**
Python中对数据的复制有两个需要注意的差别：
浅拷贝：对引用对象的拷贝（只拷贝父对象），深拷贝：对对象资源的拷贝。具体的差别如下：

```python
##############################
### memory operation
## 浅拷贝：对引用对象的拷贝（只拷贝父对象）
## 深拷贝：对对象资源的拷贝
a = [1, 2, 3]
b = a # id(a) == id (b), 同一个标签，相当于引用
a.append(4) # a = [1, 2, 3, 4], and b also change to = [1, 2, 3, 4]
import copy
a = [1, 2, ['a', 'b']] # 二元列表
c = copy.copy(a)  # id(c) != id(a)
a.append('d') # a = [1, 2, ['a', 'b'], 'd'] but c keeps not changed
# 但只属于浅拷贝，只拷贝父对象
# 所以 id(a[0]) == id(c[0])，也就是说对a追加的元素不影响c，
# 但修改a被拷贝的数据后，c的对应数据也会改变，因为拷贝不会改变元素的地址
a[2].append('d') # will change c, too
a[1] = 3 # will change c, too
# 深拷贝
d = copy.deepcopy(a) # 全部拷贝，至此恩断义绝，两者各走
# 各的阳关道和独木桥，以后毫无瓜葛
```
**九、文件与目录**
**1、文件读写**
Python的文件操作和其他的语言没有太大的差别。通过open或者file类来访问。但python支持了很多的方法，以支持文件内容和list等类型的交互。具体如下：

```python
########################
## file and directory
# file_handler = open(filename, mode)
# mode is the same as other program langurage
## read
# method 1
fin = open('./test.txt')
fin.read()
fin.close()
# method 2, class file
fin = file('./test.txt')
fin.read()
fin.close()
## write
fin = open('./test.txt', 'r+') # r, r+, w, w+, a, a+, b, U
fin.write('hello')
fin.close()
### 文件对象的方法
## help(file)
for i in open('test.txt'):
	print i
str = fin.readline() # 每次读取一行
list = fin.readlines() # 读取多行，返回一个列表，每行作为列表的一个元素
fin.next() # 读取改行，指向下一行
# 用列表来写入多行
fin.writelines(list)
# 移动指针
fin.seek(0, 0)
fin.seek(0, 1)
fin.seek(-1, 2)
# 提交更新
fin.flush() # 平时写数据需要close才真正写入文件，这个函数可以立刻写入文件
```

**2、OS模块**
os模块提供了很多对系统的操作。例如对目录的操作等。我们需要用import os来插入这个模块以便使用。

```python
#########################
## OS module
## directory operation should import this
import os
os.mkdir('xiaoyi') # mkdir
os.makedirs('a/b/c', mode = 666) # 创建分级的目录
os.listdir() # ls 返回当前层所有文件或者文件夹名到一个列表中（不包括子目录）
os.chdir() # cd
os.getcwd() # pwd
os.rmdir() # rm
```

**3、目录遍历**
目录遍历的实现可以做很多普遍的功能，例如杀毒软件，垃圾清除软件，文件搜索软件等等。因为他们都涉及到了扫描某目录下所有的包括子目录下的文件。所以需要对目录进行遍历。在这里我们可以使用两种方法对目录进行遍历：
1）递归

```python
#!/usr/bin/python
#coding:utf8
import os
def dirList(path):
	fileList = os.listdir(path)
	allFile = []
	for fileName in fileList:
		# allFile.append(dirPath + '/' + fileName) # the same as below
		filePath = os.path.join(path, fileName)
		if os.path.isdir(filePath):
			dirList(filePath)
		allFile.append(filePath)
	return allFile
```
2）os.walk函数

```python
# os.walk 返回一个生成器，每次是一个三元组 [目录, 子目录, 文件]
gen = os.walk('/')
for path, dir, filelist in os.walk('/'):
	for filename in filelist:
		os.path.join(path, filename)
```
**十、异常处理**
异常意味着错误，未经处理的异常会中止程序运行。而异常抛出机制，为程序开发人员提供一种在运行时发现错误，并进行恢复处理，然后继续执行的能力。

```python
###################################
### 异常处理
# 异常抛出机制，为程序开发人员提供一种在运行时发现错误，
# 进行恢复处理，然后继续执行的能力
# 用try去尝试执行一些代码，如果错误，就抛出异常，
# 异常由except来捕获，并由我们写代码来处理这种异常
try:
	fin = open("abc.txt")
	print hello
	### your usually process code here
except IOError, msg:
	print "On such file!"
	### your code to handle this error
except NameError, msg:
	print msg
	### your code to handle this error
finally: # 不管上面有没有异常，这个代码块都会被执行
	print 'ok'
# 抛出异常，异常类型要满足python内定义的
if filename == "hello":
	raise TypeError("Nothing!!")
```



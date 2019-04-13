
# Python文件读取的3种方法及路径转义 - jiahaowanhao的博客 - CSDN博客


2018年02月14日 23:17:59[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：1686


[Python文件读取的3种方法及路径转义](http://cda.pinggu.org/view/24739.html)
这篇文章主要介绍了Python文件读取的3种方法及路径转义,本文分别给出读取文件的代码实例,最后讲解了路径转义的相关知识、小技巧,需要的朋友可以参考下

1.文件的读取和显示
方法1：
代码如下:
f=open(r'G:\2.txt')
print f.read()
f.close()
方法2：
代码如下:
try:
t=open(r'G:\2.txt')
print t.read()
finally:
if t:
t.close()
方法3：
代码如下:
with open(r'g:\2.txt') as g:
for line in g:
print line
python虽然每次打开文件都要关闭，但是可能会由于异常导致未关闭，因此我们最好是手动关闭，方法二通过异常处理来进行，方法三通过with来自动调用close方法，最简便。
这里open的地址需要注意，如果我们写成open('g:\2.txt','r')运行时会报错：IOError: [Errno 22] invalid mode ('r') or filename: 'g:\x02.txt'。这里是由于路径被转义了，因此可以用'/'代替'\':f=open('g:/2.txt','r')或者加上r'path':f=open(r'g:\2.txt','r')就可以了。
这里通过python自带的ide-GUI测试一下是怎样转义的：
代码如下:
Python 2.7.6 (default, Nov 10 2013, 19:24:18) [MSC v.1500 32 bit (Intel)] on win32
Type "copyright", "credits" or "license()" for more information.
>>> f='g:\a.txt'
>>> print f
g:.txt  \#这里被转义成一个特殊符号了。
>>> f1='g:\\a.txt'
>>> print f1
g:\a.txt  \#没被转义
>>> r'g:\a.txt'
'g:\\a.txt'  \#没被转义
>>> 'g:\a.txt'
'g:\x07.txt'  \#这里将a转义
>>> 'g:\\a.txt'
'g:\\a.txt'
>>>


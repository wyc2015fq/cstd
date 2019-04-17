# 【简易Python爬虫】 初试爬虫_简易Python图片爬虫实现 - CD's Coding - CSDN博客





2015年07月22日 16:48:05[糖果天王](https://me.csdn.net/okcd00)阅读数：1628








## 前言

 爬虫第一步，Python纸尿裤


之前的电话面试中被问到了是否会爬虫，简单的就行

啊，爬虫爬虫……我还是自学个最简单的吧，至少得理解下这是啥意思啥原理吧

于是，照葫芦画瓢……试试看咯，顺带记录在这里备忘，毕竟是第一次用，请允许我逐行百度逐词谷歌

所以就变成了这个完全注释版——

## 二话不说上代码



```python
#coding=utf-8
 
import urllib
import re
 
def downloadPage(url):
	h = urllib.urlopen(url)
	return h.read()
	
def downloadImg(content):
	pattern = r'src="(.+?\.jpg)" pic_ext'
	m = re.compile(pattern)
	urls = re.findall(m, content)
	
	for i, url in enumerate(urls):
		print("Saving %s.jpg ...... from %s" % (i,url) + '\n')
		urllib.urlretrieve(url, "%s.jpg" % (i, ))
		
content = downloadPage("http://tieba.baidu.com/p/2460150866")
downloadImg(content)
```

修改倒数第二行的url就可以自定义需要下载的页面。



## 然后慢悠悠解释（给自己听）



### #coding=utf-8

什么你问我这句？啊这是注释，嗯注释。提醒你请用UTF-8打开哦


### import urllib

在这里用到了Urllib库里的两个函数——

**urllib.urlopen(url[,data[,proxies]])**


```python
>>> import urllib
>>> f = urllib.urlopen('http://www.google.com.hk/')
>>> firstLine = f.readline()   #读取html页面的第一行
>>> firstLine
'<!doctype html><html itemscope="" itemtype="http://schema.org/WebPage"><head><meta content="/images/google_favicon_128.png" itemprop="image"><title>Google</title><script>(function(){\n'
```



urlopen返回对象提供方法：

-         read() , readline() ,readlines() , fileno() , close() ：这些方法的使用方式与文件对象完全一样

-         info()：返回一个httplib.HTTPMessage对象，表示远程服务器返回的头信息

-         getcode()：返回Http状态码。如果是http请求，200请求成功完成;404网址未找到

-         geturl()：返回请求的url


**urllib.urlretrieve(url[,filename[,reporthook[,data]]])**

urlretrieve方法将url定位到的html文件下载到你本地的硬盘中。如果不指定filename，则会存为临时文件。

urlretrieve()返回一个二元组(filename,mine_hdrs)


以下为示例代码 [引用]

**临时存放**


```python
>>> filename = urllib.urlretrieve('http://www.google.com.hk/')
>>> type(filename)
<type 'tuple'>
>>> filename[0]
'/tmp/tmp8eVLjq'
>>> filename[1]
<httplib.HTTPMessage instance at 0xb6a363ec>
```



**存放于本地的用法**


```python
>>> filename = urllib.urlretrieve('http://www.google.com.hk/',filename='/home/dzhwen/python文件/Homework/urllib/google.html')
>>> type(filename)
<type 'tuple'>
>>> filename[0]
'/home/dzhwen/python\xe6\x96\x87\xe4\xbb\xb6/Homework/urllib/google.html'
>>> filename[1]
<httplib.HTTPMessage instance at 0xb6e2c38c>
```





### import re

正则表达式包的库文件，此处使用到了——




re.findall()  # 根据正则表达式搜索字符串，将所有符合的子字符串放在一给表(list)中返回


re.findall可以获取字符串中所有匹配的字符串。如：re.findall(r'\w*oo\w*', text)；获取字符串中，包含'oo'的所有单词。





re.compile #可以把正则表达式编译成一个正则表达式对象。可以把那些经常使用的正则表达式编译成正则表达式对象，这样可以提高一定的效率。

下面是一个正则表达式对象的一个例子：[[引用]](http://www.cnblogs.com/sevenyuan/archive/2010/12/06/1898075.html)



```python
代码

import re
text = "JGood is a handsome boy, he is cool, clever, and so on..."
regex = re.compile(r'\w*oo\w*')
print regex.findall(text)   #查找所有包含'oo'的单词
print regex.sub(lambda m: '[' + m.group(0) + ']', text) #将字符串中含有'oo'的单词用[]括起来。
```




### def downloadPage(url):

在Python中，函数声明和函数定义是视为一体的。在Python中，函数定义的基本形式如下：



```python
def function(params):
    block
    return expression/value
```
（1）在Python中采用def关键字进行函数的定义，不用指定返回值的类型。

（2）函数参数params可以是零个、一个或者多个，同样的，函数参数也不用指定参数类型，因为在Python中变量都是弱类型的，Python会自动根据值来维护其类型。

（3）return语句是可选的，它可以在函数体内任何地方出现，表示函数调用执行到此结束；如果没有return语句，会自动返回NONE，如果有return语句，但是return后面没有接表达式或者值的话也是返回NONE。


### h = urllib.urlopen(url)

urllib.urlopen(url[, data[, proxies]]) :创建一个表示远程url的类文件对象，然后像本地文件一样操作这个类文件对象来获取远程数据。

参数url表示远程数据的路径，一般是网址；

参数data表示以post方式提交到url的数据(玩过web的人应该知道提交数据的两种方式：post与get。如果你不清楚，也不必太在意，一般情况下很少用到这个参数)；

参数proxies用于设置代理。

urlopen返回 一个类文件对象，它提供了如下方法：

read() , readline() , readlines() , fileno() , close() ：这些方法的使用方式与文件对象完全一样;

info()：返回一个httplib.HTTPMessage 对象，表示远程服务器返回的头信息

getcode()：返回Http状态码。如果是http请求，200表示请求成功完成;404表示网址未找到；

geturl()：返回请求的url；

代码示例：[[引用]](http://www.jb51.net/article/42630.htm)



```python
import urllib
url = "http://www.baidu.com/"
#urlopen()
sock = urllib.urlopen(url)
htmlCode = sock.read()
sock.close
fp = open("e:/1.html","wb")
fp.write(htmlCode)
fp.close
#urlretrieve()
urllib.urlretrieve(url, 'e:/2.html')
```





### return h.read()

返回值，返回的为h所读取的内容。


### def downloadImg(content):

定义函数Downloadimg，作用为下载传参的content中包含的jpg文件。

### pattern = r'src="(.+?\.jpg)" pic_ext'

正则表达式的常用语法：[[引用]](http://www.cnblogs.com/vamei/archive/2012/08/31/2661870.html)


1）单个字符:


.          任意的一个字符

a|b        字符a或字符b

[afg]      a或者f或者g的一个字符        

[0-4]      0-4范围内的一个字符

[a-f]      a-f范围内的一个字符

[^m]       不是m的一个字符

\s         一个空格

\S         一个非空格

\d         [0-9]

\D         [^0-9]

\w         [0-9a-zA-Z]

\W         [^0-9a-zA-Z]


2）重复


紧跟在单个字符之后，表示多个这样类似的字符

*         重复 >=0 次

+         重复 >=1 次

?         重复 0或者1 次

{m}       重复m次。比如说 a{4}相当于aaaa，再比如说[1-3]{2}相当于[1-3][1-3]

{m, n}    重复m到n次。比如说a{2, 5}表示a重复2到5次。小于m次的重复，或者大于n次的重复都不符合条件。


正则表达          相符的字符串举例

[0-9]{3,5}       9678

a?b              b

a+b              aaaaab


3) 位置

^         字符串的起始位置

$         字符串的结尾位置


正则表达          相符的字符串举例        不相符字符串

^ab.*c$          abeec               cabeec (如果用re.search(), 将无法找到。)


4）返回控制


我们有可能对搜索的结果进行进一步精简信息。比如下面一个正则表达式：

```python
output_(\d{4})
```


该正则表达式用括号()包围了一个小的正则表达式，\d{4}。 这个小的正则表达式被用于从结果中筛选想要的信息（在这里是四位数字）。这样被括号圈起来的正则表达式的一部分，称为群(group)。

我们可以m.group(number)的方法来查询群。group(0)是整个正则表达的搜索结果，group(1)是第一个群……


```python
import re
m = re.search("output_(\d{4})", "output_1986.txt")
print(m.group(1))
```


我们还可以将群命名，以便更好地使用m.group查询:

```python
import re
m = re.search("output_(?P<year>\d{4})", "output_1986.txt")   #(?P<name>...) 为group命名
print(m.group("year"))
```



### m = re.compile(pattern)

re.compile(pattern, flags=0)

编译正则表达式，返回RegexObject对象，然后可以通过RegexObject对象调用match()和search()方法。

prog = re.compile(pattern)

result = prog.match(string)

跟

result = re.match(pattern, string)

是等价的。

第一种方式能实现正则表达式的重用。


### urls = re.findall(m, content)

re.findall(pattern, string, flags=0)

找到 RE 匹配的所有子串，并把它们作为一个列表返回。这个匹配是从左到右有序地返回。如果无匹配，返回空列表。

>>> re.findall("a","bcdef") 

[]

>>> re.findall(r"\d+","12a32bc43jf3") 

['12', '32', '43', '3']


### for i, url in enumerate(urls):

遍历， for var in array 是py中常用的便捷句式

for语句是python中的循环控制语句。可用来遍历某一对象，还具有一个附带的可选的else块，主要用于处理for语句中包含的break语句。

如果for循环未被break终止，则执行else块中的语句。

break 在需要时终止for循环

continue 跳过位于其后的语句，开始下一轮循环。

for语句的格式如下：[[引用]](http://www.cnblogs.com/way_testlife/archive/2010/06/14/1758276.html)

```python
>>>for <> in <对象集合>：
...    if <条件>:
...        break
...    if <条件>:
...        continue
...    <其他语句>
...else:
...    <>
...
```



### print("Saving %s.jpg ...... from %s" % (i,url) + '\n')

显示一下保存到第几张图了，顺带告诉一下这张图的url地址，打印在控制台上。

### urllib.urlretrieve(url, "%s.jpg" % (i, ))

直接将远程数据下载到本地。


urllib.urlretrieve(url[, filename[, reporthook[, data]]])

参数说明：

url：外部或者本地url

filename：指定了保存到本地的路径（如果未指定该参数，urllib会生成一个临时文件来保存数据）；

reporthook：是一个回调函数，当连接上服务器、以及相应的数据块传输完毕的时候会触发该回调。我们可以利用这个回调函数来显示当前的下载进度。

data：指post到服务器的数据。该方法返回一个包含两个元素的元组(filename, headers)，filename表示保存到本地的路径，header表示服务器的响应头。


下面通过例子来演示一下这个方法的使用，这个例子将新浪首页的html抓取到本地，保存在D:/sina.html文件中，同时显示下载的进度。[[引用]](http://www.jb51.net/article/42630.htm)



```python
import urllib
def callbackfunc(blocknum, blocksize, totalsize):
    '''回调函数
    @blocknum: 已经下载的数据块
    @blocksize: 数据块的大小
    @totalsize: 远程文件的大小
    '''
    percent = 100.0 * blocknum * blocksize / totalsize
    if percent > 100:
        percent = 100
    print "%.2f%%"% percent
url = 'http://www.sina.com.cn'
local = 'd:\\sina.html'
urllib.urlretrieve(url, local, callbackfunc)
```



### content = downloadPage("http://tieba.baidu.com/p/2460150866")

调用函数传参Url，这里是一个有一些不错图片的百度贴吧文章地址

### downloadImg(content)


调用downloadimg函数，下载地址中的jpg








# Python 学习入门（6）—— 网页爬虫 - 阳光岛主 - CSDN博客

2013年11月02日 19:25:09[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：19031所属专栏：[Python 学习入门](https://blog.csdn.net/column/details/python-learning.html)



Python抓取网页方法，任务是批量下载网站上的文件。对于一个刚刚入门python的人来说，在很多细节上都有需要注意的地方，以下就分享一下在初学python过程中遇到的问题及解决方法。

**1、Python抓取网页**

```python
import urllib,urllib2
url = "http://blog.ithomer.net"
req = urllib2.Request(url)
content = urllib2.urlopen(req).read()
print content
```
python3.3没有urllib2，改写如下：

```python
import urllib.request
def getdata():
  url='http://www.baidu.com'
  data = urllib.request.urlopen(url).read()
  print(data)
getdata()
```
1)、url为网址，需要加'http://'
2)、content为网页的html源码
问题：
1.1、网站禁止爬虫，不能抓取或者抓取一定数量后封ip
解决：伪装成浏览器进行抓取，加入headers：

```python
import urllib,urllib2
headers = { 'Use-Agent':'Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.6) Gecko/20091201 Firefox/3.5.6' }
url = "http://blog.ithomer.net"
req = urllib2.Request(url, headers=headers)
content = urllib2.urlopen(req).read()
print content
```
更复杂的情况（需要登录，多线程抓取）可参考：[python爬虫抓站的一些技巧总结](http://www.pythonclub.org/python-network-application/observer-spider)

1.2、抓取网页中的中文乱码
解决：用BeautifulSoup解析网页，[BeautifulSoup](http://www.crummy.com/software/BeautifulSoup/bs4/doc/)是Python的一个用于解析网页的插件，其安装及使用方法下文会单独讨论。
首先需要介绍一下网页中的中文编码方式，一般网页的编码会在<meta>标签中标出，目前有三种，分别是GB2312，GBK，GB18030，三种编码是兼容的。从包含的中文字符个数比较：GB2312 < GBK < GB18030，因此如果网页标称的编码为GB2312，但内容里实际上用到了属于GBK或者GB18030的中文字符，那么编码工具就会解析错误，导致编码退回到最基本的windows-2152。所以解决此类问题分两种情况：
1)、若网页实际的中文编码和其标出的相符的话，即没有字符超出所标称的编码，下面即可解决

```python
import urllib,urllib2
import bs4
headers = { 'Use-Agent':'Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.6) Gecko/20091201 Firefox/3.5.6' }
url = "http://blog.ithomer.net"
req = urllib2.Request(url, headers=headers)
content = urllib2.urlopen(req).read()
content = bs4.BeautifulSoup(content)   # BeautifulSoup
print content
```

2)、若网页中的中文字符超出所标称的编码时，需要在BeautifulSoup中传递参数from_encoding，设置为最大的编码字符集GB18030即可

```python
import urllib,urllib2
import bs4
headers = { 'Use-Agent':'Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.6) Gecko/20091201 Firefox/3.5.6' }
url = "http://blog.ithomer.net"
req = urllib2.Request(url, headers=headers)
content = urllib2.urlopen(req).read()
content = bs4.BeautifulSoup(content, from_encoding='GB18030')   # BeautifulSoup
print content
```
详细的中文乱码问题分析参见：[python中文字符乱码](http://againinput4.blog.163.com/blog/static/1727994912011111011432810/)

**2、Python下载文件**
使用Python下载文件的方法有很多，在此只介绍最简单的三种：

```python
#!/url/bin/python
# -*- coding:utf-8 -*-
# ithomer.net 
import os, shutil
import urllib, urllib2
import requests
url = 'http://blog.ithomer.net/wp-content/themes/officefolders/images/logo.png'     # 下载原网址
savepath = './downloads/logo.png'       # 本地保存地址
if os.path.isdir("downloads"):
    shutil.rmtree("downloads")      # 如果存在文件夹, 则删除
if not os.path.isdir("downloads"):
    os.makedirs("downloads")        # 创建文件夹
# 方法1: urllib
urllib.urlretrieve(url, savepath)
# 方法2: urllib2
f = urllib2.urlopen(url)
data = f.read()
with open(savepath, 'wb') as code:
    code.write(data)
# 方法3: reqeusts
req = requests.get(url)
with open(savepath, 'wb') as code:
    code.write(req.content)
```
上面方法3： 需要安装 requests模块，方法如下：
1） 先安装 pip：sudo apt-get install python-pip python-dev build-essential
2） 安装 requests：sudopip install requests

**3、使用正则表达式分析网页**
将网页源码抓取下来后，就需要分析网页，过滤出要用到的字段信息，通常的方法是用正则表达式分析网页，一个例子如下：
```python
import re
content = '<a target="blank" href="http://blog.ithomer.net">ithomer</a>'
match = re.compile(r'(?<=href=["]).*?(?=["])')
print re.findall(match, content)        # ['http://blog.ithomer.net']
```
用re.compile()编写匹配模板，用findall查找，查找content中所有与模式match相匹配的结果，返回一个列表，上式的正则表达式意思为匹配以‘href="'起始，以'"'结束的字段，使用非贪婪的规则，只取中间的部分
关于正则表达式，系统的学习请参见：[正则表达式](http://blog.csdn.net/ithomer/article/details/16963857)或[正则表达式操作指南](http://wiki.ubuntu.org.cn/Python%E6%AD%A3%E5%88%99%E8%A1%A8%E8%BE%BE%E5%BC%8F%E6%93%8D%E4%BD%9C%E6%8C%87%E5%8D%97)，个人推荐第一篇，条理清晰，不重不漏。
在此就不赘述正则表达式的学习，只总结一下我在实际写正则时的认为需要注意的几个问题：
1)、一定要使用非贪婪模式进行匹配，即*?，+?（后加?），因为Python默认使用贪婪模式进行匹配，例如'a.*b'，它会匹配文档中从第一个a和最后一个b之间的文本，也就是说如果遇到一个b，它不会停止，会一直搜索至文档末尾，直到它确认找到的b是最后一个。而一般我们只想取某个字段的值，贪婪模式既不能返回正确的结果，还大大浪费了时间，所以非贪婪是必不可少的。
2)、raw字符串的使用：如果要匹配一个.,*这种元字符，就需要加'\'进行转义，即要表示一个'\'，正则表达式需要多加一个转义，写成'\\'，但是Python字符串又需要对其转义，最终变成re.compile('\\\\')，这样就不易理解且很乱，使用raw字符串让正则表达式变得易读，即写成re.compile(r'\\')，另一个方法就是将字符放到字符集中，即[\]，效果相同。
3)、()特殊构造的使用：一般来说，()中的匹配模式作为分组并可以通过标号访问，但是有一些特殊构造为例外，它们适用的情况是：想要匹配href="xxxx"这个模式，但是我只需要xxxx的内容，而不需要前后匹配的模式，这时就可以用特殊构造(?<=)，和(?=)来匹配前后文，匹配后不返回()中的内容，刚才的例子便用到了这两个构造。
4)、逻辑符的使用：如果想匹配多个模式，使用'|'来实现，比如
re.compile(r'.htm|.mid$')匹配的就是以.htm或.mid结尾的模式，注意没有'&'逻辑运算符

**4、使用BeautifulSoup分析网页**
[BeautifulSoup](http://www.crummy.com/software/BeautifulSoup/bs4/doc/)是Python的一个插件，用于解析HTML和XML，是替代正则表达式的利器，下文讲解BS4的安装过程和使用方法
1、安装bs4
下载地址：[Download Beautiful Soup](http://www.crummy.com/software/BeautifulSoup/#Download)
下载：[beautifulsoup4-4.3.2.tar.gz](http://www.crummy.com/software/BeautifulSoup/bs4/download/4.3/beautifulsoup4-4.3.2.tar.gz)，
解压： linux下 tar xvf beautifulsoup4-4.3.2.tar.gz，win7下直接解压即可
linux，进入目录执行：
1, python setup.py build
2, python setup.py install
或
easy_install BeautifulSoup
win7，cmd到控制台 -> 到安装目录 -> 执行上面两个语句即可

2、使用BeautifulSoup解析网页
本文只介绍一些常用功能，详细教程参见：[Beautiful Soup 中文文档](http://www.crummy.com/software/BeautifulSoup/bs3/documentation.zh.html)，[英文官方文档](http://www.crummy.com/software/BeautifulSoup/bs4/doc/)
1)、包含包：import bs4
2)、读入：

```python
import urllib,urllib2
import bs4
url = "http://www.dugukeji.com/"
req = urllib2.Request(url)
content = urllib2.urlopen(req).read()
content = bs4.BeautifulSoup(content, from_encoding='GB18030')   # BeautifulSoup
print content.prettify()        # BeautifulSoup 格式化代码
```
抓取打印结果：

```python
<html>
 <head>
  <meta content="text/html; charset=utf-8" http-equiv="Content-Type"/>
  <title>
   MIDI音乐欣赏MID下载
  </title>
  <meta content="Microsoft FrontPage 6.0" name="GENERATOR"/>
  <meta content="MID音乐，MID在线，MID下载，MID试听，MID欣赏，MID播放，背景音乐，MID，MID" name="DESCRIPTION"/>
  <meta content="MID音乐，MID在线，MID下载，MID试听，MID欣赏，MID播放，背景音乐，MID，MID" name="keywords"/>
  <meta content="连通在线:MID.lt263.COM" name="Author"/>
 </head>
 <frameset border="false" cols="160,*" frameborder="0" framespacing="0">
  <frame marginheight="0" marginwidth="0" name="left" noresize="" scrolling="no" src="lm1.htm" target="rtop"/>
  <frameset rows="27%,*">
   <frame name="m_rtop" src="tops.htm" target="m_rbottom"/>
   <frame name="m_rbottom" src="main.htm" target="m_rbottom"/>
  </frameset>
  <noframes>
   <body>
    <p>
     15000首MID
    </p>
   </body>
  </noframes>
 </frameset>
</html>
```

3)、查找内容a、按html标签名查找：
frameurl = content.findAll('frame', target='rtop')\#存储所有frame标签，且target='rtop'内容的列表
输出结果：
[<frame marginheight="0" marginwidth="0" name="left" noresize="" scrolling="no" src="lm1.htm" target="rtop"/>]

b、按标签属性查找
frameurl = content.findAll('frame', target=True)\#查找所有含target属性的标签运行结果：
[<frame marginheight="0" marginwidth="0" name="left" noresize="" scrolling="no" src="lm1.htm" target="rtop"/>, <frame name="m_rtop" src="tops.htm" target="m_rbottom"/>, <frame name="m_rbottom" src="main.htm" target="m_rbottom"/>]
查找所有含target属性且值为'm_rbottom'的标签

c、带有正则表达式的查找
rawlv2 = content.findAll(href=re.compile(r'.htm$'))\#查找所有含href属性且值为以'.htm'结尾的标签示例：

```python
#!/url/bin/python
# -*- coding:utf-8 -*-
# ithomer.net 
import urllib,urllib2
import bs4, re
import sys
reload(sys)
sys.setdefaultencoding('utf-8')
url = "http://www.dugukeji.com/lm1.htm"
req = urllib2.Request(url)
content = urllib2.urlopen(req).read()
content = bs4.BeautifulSoup(content, from_encoding='GB18030')   # BeautifulSoup
print content.prettify()
rawlv2 = content.findAll(href=re.compile(r'.htm$'))
print rawlv2
```
运行结果：

```python
[<a href="ftzw/index.htm" target="_blank"><font color="#FF6633">繁体中文版</font></a>, <a href="addopen.htm" target="_top">最新加入</a>, <a href="searchopen.htm" target="_top">歌曲搜寻</a>, <a href="chopen.htm" target="_top">国语歌曲</a>, <a href="twopen.htm" target="_top">台语歌曲</a>, <a href="wwopen.htm" target="_top">西洋歌曲</a>, <a href="hkopen.htm" target="_top">粤语歌曲</a>, <a href="jpopen.htm" target="_top">日本歌曲</a>, <a href="ctopen.htm" target="_top">卡通歌曲</a>, <a href="tvopen.htm" target="_top">影视歌曲</a>, <a href="pianopen.htm" target="_top">钢琴<font face="Arial Unicode MS">POP</font></a>, <a href="otopen.htm" target="_top">其它音乐</a>, <a href="newlyopen.htm" target="_top">网友创作</a>, <a href="eachopen.htm" target="_top">各国音乐</a>, <a href="clopen.htm" target="_top">古典音乐</a>, <a href="autochopen.htm" target="_top">原住民篇</a>, <a href="marchopen.htm" target="_top">进行曲篇</a>, <a href="schoolopen.htm" target="_top">校际校歌</a>, <a href="crystalopen.htm" target="_top">水晶音乐</a>, <a href="KLOK/index.htm" target="_blank">卡 拉 OK</a>, <a href="gbookopen.htm" target="_top">留  言  板</a>, <a href="index.htm" target="_top">返回首页</a>]
```

4)、访问标签属性值和内容
a、访问标签属性值
```python
rawlv2 = content.findAll(href=re.compile(r'.htm$'))  
href = rawlv2[i]['href']
```
通过[属性名]即可访问属性值，如上式返回的便是href属性的值b)、访问标签内容
```python
rawlv3 = content.findAll(href=re.compile(r'.mid$'))  
songname = str(rawlv3[i].text)
```
上式访问了<a href=...>(内容)</a>标签的实际内容，由于text为unicode类型，所以需要用str()做转换
附上最终的成果，程序功能是抓取www.dugukeji.com上的所有midi文件并下载，需要先建立./midi/dugukeji/文件夹和./midi/linklist文件

```python
#!/usr/bin/python
# -*- coding:utf-8 -*- 
#
# ithomer.net
import urllib2,urllib,cookielib,threading
import os
import re
import bs4
import sys
reload(sys)
sys.setdefaultencoding('utf-8')	#允许打印unicode字符

indexurl = 'http://www.dugukeji.com/'
databasepath = './midi/linklist'
path = './midi/dugukeji/'
totalresult = {}
oriresult = {}
def crawl(url):
    # 伪装为浏览器抓取
	headers = {	
    	'User-Agent':'Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.6) Gecko/20091201 Firefox/3.5.6'
	}
	req = urllib2.Request(url,headers=headers)
	content = urllib2.urlopen(req).read()
	content = bs4.BeautifulSoup(content, from_encoding='GB18030')
	return content

def crawlframe(sourceurl,target):
	global indexurl
	content = crawl(sourceurl)
	#match = re.compile(r'(?<=target=["]'+target+'["] src=["]).*?(?=["])')	#正则表达式方法
	#frameurl = re.findall(match,content)
	frameurl = content.findAll('frame',target=target)	#beautifulsoup方法
	result = indexurl+frameurl[0]['src']	### http://www.dugukeji.com/lm1.htm
	return result
def crawllv1(frameurl,st=-1,en=-1):
	global indexurl
	content = crawl(frameurl)
	#match = re.compile(r'(?<=href=["]).*?(?=["])')
	#rawlv2 = re.findall(match,content)
	rawlv2 = content.findAll(href=re.compile(r'.htm$'))
	result = []
	if st==-1 and en==-1:
		for i in range(len(rawlv2)):
			result.append(indexurl+rawlv2[i]['href'])	### http://www.dugukeji.com/ftzw/index.htm
	else:
		for i in range(st,en):
			result.append(indexurl+rawlv2[i]['href'])
	#result.sort()
	return result
def crawllv2(lv2url):
	global indexurl
	content = crawl(lv2url)
	#match = re.compile(r'(?<=href=["]\.\.\/).*?[">].*?(?=[<])')
	#rawlv3 = re.findall(match,content)
	rawlv3 = content.findAll(href=re.compile(r'[..].*?[0-9].htm|.mid$'))
	#print rawlv3
	result = {}	#结果字典，key：链接，value：歌曲名
	for i in range(len(rawlv3)):
		tmp = str(rawlv3[i]['href'])
		#print tmp
		link = indexurl + tmp[tmp.rfind('..')+3:]	#有多个'..'，找到最后一个
		songname = ''
		if tmp[-4:]=='.htm':	#需要访问3级页
			try:
				conlv3 = crawl(link)
			except:
				print 'WARNING: visit lv3 url failed!\n'
			else:
				rawlv4 = conlv3.findAll(href=re.compile(r'.mid$'))
				if not rawlv4:	#4级页没有.mid下载链接，略过
					continue
				else:
					tmp = str(rawlv4[0]['href'])
					link = indexurl + tmp[tmp.rfind('..')+3:]
		songname = str(rawlv3[i].text)	#将unicode类型的text转化为string
		#songname.decode('GBK')
		#songname.encode('utf-8')
		songname = songname.replace(' ','_')	#将songname中空格和换行转化为下划线
		songname = songname.replace('\n','_')	#原来存在的链接，直接略过
		if oriresult.has_key(link):
			continue
		if totalresult.has_key(link) and len(songname)<len(totalresult[link]):	#如果链接已保存且歌曲名长度比当前的长，略过
			continue
		else:
			totalresult[link] = songname
			result[link] = songname		#加入字典
	#result.sort()
	return result
def download(totalresult):
	for link in totalresult.keys():
		filepath = path + totalresult[link] + '.mid'
		print 'download: ',link,' -> ',filepath,'\n'
		urllib.urlretrieve(link, filepath)

def readdata(databasepath):
	datafile = open(databasepath,'r')	#读数据文件
	link = datafile.readline()
	while link:
		oriresult[link]=''
		link = datafile.readline()
	datafile.close()
def writedata(databasepath):
	datafile = open(databasepath,'a')	#追加打开数据文件，将新链接写入文件尾
	for link in totalresult.keys():
		datafile.write(link,'\n')
	datafile.close()
# main
if __name__ == '__main__':
    # 访问文件，记录已下载的链接
	try:
		readdata(databasepath)	
	except:
		print 'WARNING:read database file failed!\n'
	else:
		print 'There is ',len(oriresult),' links in database.\n'
    # 抓取主页中一级页url所在frame的url
	try:
		frameurl1 = crawlframe(indexurl,'rtop')	
	except:
		print 'WARNING: crawl lv1 frameurl failed!\n'

	try:
		urllv1 = crawllv1(frameurl1,4,20)		#抓取一级页url
	except:
		print 'WARNING: crawl lv1 url failed!\n'
	for i in urllv1:
		print 'lv1 url:',i
		try:
			frameurl2 = crawlframe(i,'rbottom')	#抓取一级页中二级页url所在frame的url
		except:
			print 'WARNING: crawl lv2 frameurl failed!\n'
		else:
			print '\tlv2 frameurl:',frameurl2
			try:
				urllv2 = crawllv1(frameurl2)	#抓取二级页url
			except:
				print 'WARNING: crawl lv2 url failed!\n'
			else:
				for j in urllv2:
					print '\t\tlv2 url:',j
					try:
						urllv3 = crawllv2(j)
					except:
						print 'WARNING: crawl lv3 url failed!\n'
					else:
						for k in urllv3.keys():
							print '\t\t\tlv3 url:',k,'\tname:',urllv3[k]
						#download(urllv3)
							
	print 'new added midi num:',len(totalresult)
	print '\nbegin to download...\n'
	download(totalresult)
	print '\nWrite database...\n'
	writedata(databasepath)
	print '\n\nDone!\n'
```
运行结果：
![](https://img-blog.csdn.net/20131207142841734)



**参考推荐：**
[Python抓取网页&批量下载文件方法](http://blog.csdn.net/maverick1990/article/details/8903729)
[[Python]网络爬虫（一）](http://blog.csdn.net/pleasecallmewhy/article/details/8922826)（系列教程）
[开源python网络爬虫框架Scrapy](http://blog.csdn.net/zbyufei/article/details/7554322)
[Python之HTML的解析（网页抓取一）](http://my.oschina.net/zhdkn/blog/111696)

[Python写爬虫——抓取网页并解析HTML](http://www.lovelucy.info/python-crawl-pages.html)
[详解抓取网站，模拟登陆，抓取动态网页的原理和实现（Python，C\#等）](http://www.crifan.com/files/doc/docbook/web_scrape_emulate_login/release/html/web_scrape_emulate_login.html)



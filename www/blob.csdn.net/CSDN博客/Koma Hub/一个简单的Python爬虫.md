# 一个简单的Python爬虫 - Koma Hub - CSDN博客
2018年04月24日 16:25:49[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：76
***一个简单测试爬虫的网页***：[http://www.pythonscraping.com/pages/page3.html](http://www.pythonscraping.com/pages/page3.html)
***1.获取网址的html代码***
```python
import urllib.request
request=urllib.request.urlopen("http://www.baidu.com")
str=request.read()
print(str)
```
***2.返回网页标题***
输出目标网页内title标签内的文本：<h1>An Interesting Title</h1>，getTitle()函数返回网页的标题，如果获取网页遇到问题就返回一个 None 对象。 如果服务器不存在， html 就是一个 None 对象，html.read() 就会抛出 AttributeError 。[https://www.cnblogs.com/Issac-Gan/p/6505808.html](https://www.cnblogs.com/Issac-Gan/p/6505808.html)
```python
from urllib.request import urlopen
from urllib.error import HTTPError
from bs4 import BeautifulSoup
def getTitle(url):
    try:
        html = urlopen(url)
    except HTTPError as e:
        return None
    try:
        bsObj = BeautifulSoup(html.read(),'lxml')
        title = bsObj.body.h1
    except AttributeError as e:
        return None
    return title
title = getTitle("http://www.pythonscraping.com/pages/page3.html")
if title == None:
    print("Title could not be found")
else:
    print(title)
```
网站中的信息是：
```
</style>
</head>
<body>
<div id="wrapper">
<img src="../img/gifts/logo.jpg" style="float:left;">
<h1>Totally Normal Gifts</h1>
```
则运行输出结果为：
```
D:\test\python>python reptile.py
<h1>Totally Normal Gifts</h1>
```
***3.查找HTML***
```python
#!/usr/bin/env python #代码来自:https://www.cnblogs.com/zw2002/p/8476389.html
#coding=utf-8
import requests
from bs4 import BeautifulSoup
url = 'http://www.pythonscraping.com/pages/page3.html'
r = requests.get(url)
soup = BeautifulSoup(r.content, 'lxml')
bbs_nes = soup.find_all(name='span',attrs={'class':'excitingNote'})
for news in bbs_nes:
    print (news.string)
```
规律是查找：
`<span class="excitingNote">`
在这个HTML源代码中一共有：
```
<span class="excitingNote">Now with super-colorful bell peppers!</span>
<span class="excitingNote">8 entire dolls per set! Octuple the presents!</span>
<span class="excitingNote">Also hand-painted by trained monkeys!</span>
<span class="excitingNote">Or maybe he's only resting?</span>
<span class="excitingNote">Keep your friends guessing!</span>
```
所以最后的输出结果为：
```python
D:\test\python>python reptile.py
Now with super-colorful bell peppers!
8 entire dolls per set! Octuple the presents!
Also hand-painted by trained monkeys!
Or maybe he's only resting?
Keep your friends guessing!
```

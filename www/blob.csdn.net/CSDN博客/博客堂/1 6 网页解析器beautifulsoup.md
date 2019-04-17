# 1.6 网页解析器beautifulsoup - 博客堂 - CSDN博客





2017年05月02日 16:10:56[最小森林](https://me.csdn.net/u012052268)阅读数：484








## 解析器的简介


- - [解析器的简介](#解析器的简介)- [解析器选择](#解析器选择)
- [结构化解析DOM](#结构化解析dom)

- [BeautifulSoup介绍](#beautifulsoup介绍)- [安装BeautifulSoup4](#安装beautifulsoup4)
- [beautifulsoup4语法](#beautifulsoup4语法)

- [BeautifulSoup实战](#beautifulsoup实战)




### 解析器选择

为了实现解析器，可以选择使用 

1. 正则表达式、 

2. html.parser、 

3. BeautifulSoup、 

4. lxml等，
这里我们选择BeautifulSoup。

其中，正则表达式基于模糊匹配，而另外三种则是基于DOM结构化解析。而且BeautifulSoup可以引用另外两个，所以更强大。

![image](http://img.mukewang.com/58c21c180001414312800720.jpg)

### 结构化解析DOM

结构化解析是www官方的解析方法，是以树的形式对文档进行逐层解析。 
![image](http://img.mukewang.com/58b84b0d0001183c12800720.jpg)
## BeautifulSoup介绍

### 安装BeautifulSoup4

![image](http://img.mukewang.com/58a0a2430001c18612800720.jpg)
`pip install beautifulsoup4`
### beautifulsoup4语法

使用示意图：

![image](http://img.mukewang.com/58bcba7d0001d84d12800720.jpg)
- 创建beautifulsoup对象

```xml
from bs4 import BeautifulSoup

html_doc = """  #   定义一个长字符串，存储html代码
<html><head><title>The Dormouse's story</title></head>
<body>
<p class="title"><b>The Dormouse's story</b></p>

<p class="story">Once upon a time there were three little sisters; and their names were
<a href="http://example.com/elsie" class="sister" id="link1">Elsie</a>,
<a href="http://example.com/lacie" class="sister" id="link2">Lacie</a> and
<a href="http://example.com/tillie" class="sister" id="link3">Tillie</a>;
and they lived at the bottom of a well.</p>

<p class="story">...</p>
"""
soup = BeautifulSoup(html_doc,'html.parser',from_encoding='UTF-8')
# 三个参数分为为：传入的html字符串；使用的解析器；编码方式 
print(soup.prettify())
```
- 搜索节点（从dom树中） 
![image](http://img.mukewang.com/58c3daa30001afad12800720.jpg)
find与find_all的参数都是这三个： 

1.html标签名 2.标签属性 3.标签里面的内容
- 匹配到了，之后如何输出？ 
![image](http://img.mukewang.com/58c3dc2d00016f2d12800720.jpg)
## BeautifulSoup实战

仔细研读！

```python
# coding:utf-8
from bs4 import BeautifulSoup
import re

html_doc = """  #   定义一个长字符串，存储html代码
<html><head><title>The Dormouse's story</title></head>
<body>
<p class="title"><b>The Dormouse's story</b></p>

<p class="story">Once upon a time there were three little sisters; and their names were
<a href="http://example.com/elsie" class="sister" id="link1">Elsie</a>,
<a href="http://example.com/lacie" class="sister" id="link2">Lacie</a> and
<a href="http://example.com/tillie" class="sister" id="link3">Tillie</a>;
and they lived at the bottom of a well.</p>

<p class="story">...</p>
"""
soup = BeautifulSoup(html_doc, 'html.parser')
# 创建bs对象，其三个参数分为为：传入的html字符串；使用的解析器；编码方式

print('提取所有的连接出来')
links = soup.find_all('a')
for link in links:
    print(link.name, link['href'], link['class'], link.get_text())

print('获取lacie连接')
link_node = soup.find('a', href='http://example.com/lacie')
print(link_node.name, link_node['href'], link_node.get_text())

print('用强大的 正则语法 匹配 ！！！！！！！！')
link_node = soup.find('a', href=re.compile(r"ill"))
print(link_node.name, link_node['href'], link_node.get_text())

print('获取P标签')
p_node = soup.find('p', class_=re.compile(r"ti"))
print(p_node.name, p_node['class'], p_node.get_text())

print('抓取暗链接')
link_node = soup.find_all(style='display:none;')
print(link_node)
# print(link_node.name, link_node.get_text())
# 这里发现  如果只有一个暗链接，则不能用findall，之恩那个用find，那么不知道有几个的情况怎么办呢？
```








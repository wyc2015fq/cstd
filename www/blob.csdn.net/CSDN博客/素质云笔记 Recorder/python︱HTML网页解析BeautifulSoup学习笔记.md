# python︱HTML网页解析BeautifulSoup学习笔记 - 素质云笔记/Recorder... - CSDN博客





2017年10月11日 15:54:40[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：2211








# 一、载入html页面信息

一种是网站在线的网页、一种是下载下来的静态网页。

## 1、在线网页

参考[《python用BeautifulSoup库简单爬虫入门+案例（爬取妹子图）》](https://segmentfault.com/a/1190000011192866)中的载入内容：

```
import requests
from bs4 import BeautifulSoup

headers={'User-Agent': 'Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/57.0.2987.110 Safari/537.36','referer':"www.mmjpg.com" }
all_url = 'http://www.mmjpg.com/' 
	#'User-Agent':请求方式  
	#'referer':从哪个链接跳转进来的

start_html = requests.get(all_url,  headers=headers)
	#all_url：起始的地址，也就是访问的第一个页面
	#headers：请求头，告诉服务器是谁来了。
	#requests.get：一个方法能获取all_url的页面内容并且返回内容。

Soup = BeautifulSoup(start_html.text, 'lxml')
	#BeautifulSoup：解析页面
	#lxml：解析器
	#start_html.text：页面的内容
```

## 2、本地的静态页面

```
url = ...\...\...
soup = BeautifulSoup(open(url,'r',encoding = 'utf-8'))
```

encoding 编码这边需要提前确认，直接open本地的html静态html文件

```
start_html.encoding='gb2312'
```

这个为Html的编码

.

# 二、界面结构简述

主要参考：[Python爬虫利器二之Beautiful Soup的用法](http://cuiqingcai.com/1319.html)

Beautiful Soup将复杂HTML文档转换成一个复杂的树形结构,每个节点都是Python对象,所有对象可以归纳为4种:
- Tag
- NavigableString
- BeautifulSoup
- Comment

以样本为例：

```
html = """
<html><head><title>The Dormouse's story</title></head>
<body>
<p class="title" name="dromouse"><b>The Dormouse's story</b></p>
<p class="story">Once upon a time there were three little sisters; and their names were
<a href="http://example.com/elsie" class="sister" id="link1"><!-- Elsie --></a>,
<a href="http://example.com/lacie" class="sister" id="link2">Lacie</a> and
<a href="http://example.com/tillie" class="sister" id="link3">Tillie</a>;
and they lived at the bottom of a well.</p>
<p class="story">...</p>
"""
```

.

## 1、基本构成——Tag

就是 HTML 中的一个个标签

```
<title>The Dormouse's story</title>
```

以上整个叫做tag。通过标签名获得里面的内容的方式：

```
print soup.title
#<title>The Dormouse's story</title>

print soup.head
#<head><title>The Dormouse's story</title></head>

print soup.a
#<a class="sister" href="http://example.com/elsie" id="link1"><!-- Elsie --></a>

print soup.p
#<p class="title" name="dromouse"><b>The Dormouse's story</b></p>
```

其中title，head,p,a都是tag里面的标签名

两个重要的属性，是 name 和 attrs：

```
print soup.name
print soup.head.name
#[document]
#head
```

整个tag是一个document类型，

其中标签名可以通过这样的方式获得（再如：[soup.a.name](http://soup.a.name)）

```
print soup.p.attrs
#{'class': ['title'], 'name': 'dromouse'}
```

p 标签的所有属性打印输出了出来，得到的类型是一个字典。

.

## 2、基本构成——NavigableString

标签内的文字，就是`<title>The Dormouse's story</title>`中的The Dormouse’s story

```
print soup.p.string
#The Dormouse's story
```

其中的文字相对来说就是标签的注释，利用 .string 来输出它的内容。其格式为：Comment 类型

```
if type(soup.a.string)==bs4.element.Comment:
    print soup.a.string
```

#### 延伸：strings 实践

一个tag仅有一个子节点,那么这个tag也可以使用 .string 方法

如果tag中包含多个字符串  ,可以使用 .strings 来循环获取

```
combine_html = """
 <p class="identical">
  Example of p tag with class identical
 </p>
 <div class="identical">
  Example of div tag with class identical
 <div>
 """
```

输出结果

```
list(combine_soup.strings)

['\n  Example of p tag with class identical\n ',
 '\n',
 '\n  Example of div tag with class identical\n ',
 '\n']
```

.

## 3、子节点——.contents .children

.contents，将tag的子节点以list列表的方式输出。

.children，返回的不是一个 list，是一个list生成器，

```
print soup.head.contents 
#[<title>The Dormouse's story</title>]
```

```
for child in  soup.body.children:
    print child
#<p class="title" name="dromouse"><b>The Dormouse's story</b></p>
```

## 4、子孙节点——.descendants

跟.children一样输出的是一个list生成器

```
for child in soup.descendants:
    print child
#<p class="title" name="dromouse">The Dormouse's story<b>The Dormouse's story</b></p>
```

可以看到与.children的区别，.descendants输出的内容比较多，不仅把.children的内容输出了 且加上了标签内的文字：The Dormouse’s story**The Dormouse’s story.**

## 5、父节点—— .parent

通过元素的 .parents 属性可以递归得到元素的所有父辈节点，例如

```
content = soup.head.title.string
for parent in  content.parents:
    print parent.name
#title
#head
#html
#[document]
```

## .6、兄弟节点—— .next_sibling .previous_sibling

```
print soup.p.next_sibling
#       实际该处为空白
print soup.p.prev_sibling
#None   没有前一个兄弟节点，返回 None
print soup.p.next_sibling.next_sibling
#<p class="story">Once upon a time there were three little sisters; and their names were
#<a class="sister" href="http://example.com/elsie" id="link1"><!-- Elsie --></a>,
#<a class="sister" href="http://example.com/lacie" id="link2">Lacie</a> and
#<a class="sister" href="http://example.com/tillie" id="link3">Tillie</a>;
#and they lived at the bottom of a well.</p>
#下一个节点的下一个兄弟节点是我们可以看到的节点
```

.

## 7、前后节点——.next_element .previous_element

与 .next_sibling  .previous_sibling 不同，它并不是针对于兄弟节点，而是在所有节点，不分层次

比如 head 节点为

```
<head><title>The Dormouse's story</title></head>
```

那么它的下一个节点便是 title，它是不分层次关系的

```
print soup.head.next_element
#<title>The Dormouse's story</title>
```

.

# 三、搜索文档树

主要参考：[Python爬虫利器二之Beautiful Soup的用法](http://cuiqingcai.com/1319.html)

## 1、find_all( name , attrs , recursive , text , **kwargs )

搜索当前tag的所有tag子节点,并判断是否符合过滤器的条件

### （1）搜索节点

```
soup.find_all('b')
# [<b>The Dormouse's story</b>]
```

### （2）正则表达——针对节点

```
import re
for tag in soup.find_all(re.compile("^b")):
    print(tag.name)
# body
# b
```

### （3）传列表

```
soup.find_all(["a", "b"])
# [<b>The Dormouse's story</b>,
#  <a class="sister" href="http://example.com/elsie" id="link1">Elsie</a>,
#  <a class="sister" href="http://example.com/lacie" id="link2">Lacie</a>,
#  <a class="sister" href="http://example.com/tillie" id="link3">Tillie</a>]
```

### （4）传 True

相当于遍历节点有啥。

```
for tag in soup.find_all(True):
    print(tag.name)
# html
# head
# title
# body
# p
# b
# p
# a
# a
```

### （5）搜索文档中的字符串内容

```
soup.find_all(text="Elsie")
# [u'Elsie']

soup.find_all(text=["Tillie", "Elsie", "Lacie"])
# [u'Elsie', u'Lacie', u'Tillie']

soup.find_all(text=re.compile("Dormouse"))
[u"The Dormouse's story", u"The Dormouse's story"]
```

### （6）href 参数——针对标签注释

传入 href 参数,Beautiful Soup会搜索每个tag的”href”属性:

```
soup.find_all(href=re.compile("elsie"))
# [<a class="sister" href="http://example.com/elsie" id="link1">Elsie</a>]
```

### （7）recursive 参数

检索当前tag的所有子孙节点,如果只想搜索tag的直接子节点

```
soup.html.find_all("title")
# [<title>The Dormouse's story</title>]

soup.html.find_all("title", recursive=False)
# []
```

### （8）加快效率——limit 参数

```
soup.find_all("a", limit=2)
# [<a class="sister" href="http://example.com/elsie" id="link1">Elsie</a>,
#  <a class="sister" href="http://example.com/lacie" id="link2">Lacie</a>]
```

### 延伸一：find实践

有这么一段html源码

```
combine_html = """
 <p class="identical">
  Example of p tag with class identical
 </p>
 <div class="identical">
  Example of div tag with class identical
 <div>
 """
combine_soup = BeautifulSoup(combine_html,'lxml')

# 搜索
combine_soup .find("div",class_="identical")
combine_soup .select("div.identical")
```

结果输出：

```
# 第一种输出
<div class="identical">
  Example of div tag with class identical
 <div>
</div></div>
# 第二种输出
[<div class="identical">
   Example of div tag with class identical
  <div>
 </div></div>]
```

.

## 2、其他 find

```
find_parents()  find_parent()
```

find_all() 和 find() 只搜索当前节点的所有子节点,孙子节点等. find_parents() 和 find_parent() 用来搜索当前节点的父辈节点,搜索方法与普通tag的搜索方法相同,搜索文档搜索文档包含的内容

```
find_next_siblings()  find_next_sibling()
```

这2个方法通过 .next_siblings 属性对当 tag 的所有后面解析的兄弟 tag 节点进行迭代, find_next_siblings() 方法返回所有符合条件的后面的兄弟节点,find_next_sibling() 只返回符合条件的后面的第一个tag节点

```
find_previous_siblings()  find_previous_sibling()
```

这2个方法通过 .previous_siblings 属性对当前 tag 的前面解析的兄弟 tag 节点进行迭代, find_previous_siblings() 方法返回所有符合条件的前面的兄弟节点, find_previous_sibling() 方法返回第一个符合条件的前面的兄弟节点

.

## 3、select——CSS选择器

### （1）通过标签名查找

```
案例一
print soup.select('title') 
#[<title>The Dormouse's story</title>]
```

```
# 案例二
print soup.select('a')
#[<a class="sister" href="http://example.com/elsie" id="link1"><!-- Elsie --></a>, <a class="sister" href="http://example.com/lacie" id="link2">Lacie</a>, <a class="sister" href="http://example.com/tillie" id="link3">Tillie</a>]
```

```
# 案例三
print soup.select('b')
#[<b>The Dormouse's story</b>]
```

### （2）通过类名查找

```
print soup.select('.sister')
#[<a class="sister" href="http://example.com/elsie" id="link1"><!-- Elsie --></a>, <a class="sister" href="http://example.com/lacie" id="link2">Lacie</a>, <a class="sister" href="http://example.com/tillie" id="link3">Tillie</a>]
```

### （3）id查找

```
print soup.select('#link1')
#[<a class="sister" href="http://example.com/elsie" id="link1"><!-- Elsie --></a>]
```

### （4）属性查找

查找时还可以加入属性元素，属性需要用中括号括起来，注意属性和标签属于同一节点，所以中间不能加空格，否则会无法匹配到。

```
print soup.select('a[class="sister"]')
#[<a class="sister" href="http://example.com/elsie" id="link1"><!-- Elsie --></a>, <a class="sister" href="http://example.com/lacie" id="link2">Lacie</a>, <a class="sister" href="http://example.com/tillie" id="link3">Tillie</a>]
```

```
print soup.select('p a[href="http://example.com/elsie"]')
#[<a class="sister" href="http://example.com/elsie" id="link1"><!-- Elsie --></a>]
```

.

### 主要参考：

[Beautiful Soup 4.4.0 文档](http://beautifulsoup.readthedocs.io/zh_CN/latest/#id18)
[Python爬虫利器二之Beautiful Soup的用法](http://cuiqingcai.com/1319.html)

## 延伸一：实践

```
#  读入内容
contents = BeautifulSoup(open(url,'r',encoding = 'utf-8')).find_all("div",class_="caption col-md-12") 
 
#1.re库用正则，提取标签中的html
内容:<a target="001" class="002" href="../..//003.html">
	re.findall(r'\"(.*html)\"',str(  content  )) 

#2.re库正则，在.find_all中使用
内容:<a target="001" class="002" href="../..//003.html">
re.findall(r'\"(.*html)\"',str(content.find_all("a",class_="002")[0])) 

#3.提取标签下的文本内容
内容:content = <h4><a href="../../../img/56b311675fe3329a703cf9de.html">独钓图
可以看到该内容前面有两个<>标签，可以直接：
content.find_all('a').strings[0]
#另一种：
'[<a href="#1">简介</a>]'.get_text()
>>> 简介
# 第三种：
re.findall(r"<a.*?href=.*?<\/a>",ss,re.I)


#4.相同标签,有类别属性
内容：	<span class="a">text1
		<span class="b">text2
以上有两个span相同的标签，可以通过class来辅助定位、查找：
content.find_all('span',class_='pull-right').strings  # 即为文本内容

#5.相同标签,无类别属性
内容：	<span >text1
		<span >text2

content.find_all('span').contents[0].strings
先生成一个列表，然后选中，再得到下面的文本材料

# 6.奇怪的副标题
内容： <td width="285" valign="top">1764－1815</td>
content.find_all('td',width="285", valign="top")
```




# BeautifulSoup 库的基本使用 - Alex_McAvoy的博客 - CSDN博客





2018年08月18日 08:07:26[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：97








# 【概述】

BeautifulSoup 库，也叫 bs4库，其将复杂的 HTML/XML 文档转化成一个树形结构，从而解析、遍历、维护标签树 。

# 【引用方式】

使用 bs4 库需要进行引用，目前最常用的引用方式如下：

```python
from bs4 import BeautifulSoup
```

这个引用方式从 bs4 库中引入了一个 BeautifulSoup 类，通过 BeautifulSoup 类使得标签树形成了一个变量，对这个变量进行处理，就是对标签树的处理，即：BeautifulSoup 类对应一个 HTML/XML 文档的全部内容。

如果需要对 bs4 库中点基本变量进行判断时，可以直接引用  bs4 库，其引用方式如下：

```python
import bs4
```

# 【解析器】

bs4 库支持如下 4 种解析器：
|解析器|使用方法|条件|优势|劣势|
|----|----|----|----|----|
|HTML解析器|BeautifulSoup(mk,'html.parser')|安装 bs4 库|执行速度中，内置标准库|容错能力差|
|HTML解析器|BeautifulSoup(mk,'lxml')|安装 lxml 库|执行速度快，文档容错强|安装C语言库|
|XML解析器|BeautifulSoup(mk,'xml')|安装 lxml 库|执行速度快，唯一支持XML的解析器|安装C语言库|
|html5lib解析器|BeautifulSoup(mk,'html5lib')|安装 html5lib 库|以浏览器的形式解析，生成HTML5格式文档|速度慢|

# 【基本元素】
|Tag|标签，最基本的信息组织单元，分别用<>和</>标明开头和结尾|
|----|----|
|Name|标签的名字，<p>…</p>的名字是'p'，格式：<tag>.name|
|Attributes|标签的属性，字典形式组织，格式：<tag>.attrs|
|NavigableString|标签内非属性字符串，<>…</>中字符串，格式：<tag>.string|
|Comment|标签内字符串的注释部分，一种特殊的Comment类型|






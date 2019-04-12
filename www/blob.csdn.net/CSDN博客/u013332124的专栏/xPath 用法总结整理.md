
# xPath 用法总结整理 - u013332124的专栏 - CSDN博客


2018年06月08日 12:41:14[疯狂哈丘](https://me.csdn.net/u013332124)阅读数：27579


最近在研究kafka，看了一堆理论的东西，想动手实践一些东西，奈何手上的数据比较少，突发奇想就打算写个爬虫去抓一些数据来玩，顺便把深入一下爬虫技术。
之前写过一些小爬虫，一般就是用python的requests+lxml来爬取数据。这次打算学一下python的scrapy框架来爬取数据。解析网页内容还是打算用lxml，lxml使用了xpath语法，由于太久没用都忘光了。所以打算重新学习一下xpath语法，并做个总结，方便以后忘了可以马上回顾。
## 一、xpath介绍
XPath 是一门在 XML 文档中查找信息的语言。XPath 用于在 XML 文档中通过元素和属性进行导航。
- XPath 使用路径表达式在 XML 文档中进行导航- XPath 包含一个标准函数库- XPath 是 XSLT 中的主要元素- XPath 是一个 W3C 标准
#### 节点
在 XPath 中，有七种类型的节点：元素、属性、文本、命名空间、处理指令、注释以及文档（根）节点。XML 文档是被作为节点树来对待的。
## 二、xpath语法
表达式描述nodename选取此节点的所有子节点。/从根节点选取。//从匹配选择的当前节点选择文档中的节点，而不考虑它们的位置。.选取当前节点。..选取当前节点的父节点。@选取属性。
#### 例子
以下面这个xml为例子
<?xml version="1.0" encoding="ISO-8859-1"?><bookstore><book><titlelang="eng">`Harry Potter`</title><price>`29.99`</price></book><book><titlelang="eng">`Learning XML`</title><price>`39.95`</price></book></bookstore>**xml.xpath(“bookstore”)**- 表示选取 bookstore 元素的所有子节点**xml.xpath(“/bookstore”)**- 表示选取根元素 bookstore。**xml.xpath(“bookstore/book”)**- 选取属于 bookstore 的子元素的所有 book 元素。**xml.xpath(“//book”)**- 选取所有 book 子元素，而不管它们在文档中的位置。**xml.xpath(“bookstore//book”)**- 选择属于 bookstore 元素的后代的所有 book 元素，而不管它们位于 bookstore 之下的什么位置。**xml.xpath(“//@lang”)**- 选取名为 lang 的所有属性。
#### 谓语
路径表达式结果/bookstore/book[1]选取属于 bookstore 子元素的第一个 book 元素。/bookstore/book[last()]选取属于 bookstore 子元素的最后一个 book 元素。/bookstore/book[last()-1]选取属于 bookstore 子元素的倒数第二个 book 元素。/bookstore/book[position()<3]选取最前面的两个属于 bookstore 元素的子元素的 book 元素。//title[@lang]选取所有拥有名为 lang 的属性的 title 元素。//title[@lang=’eng’]选取所有 title 元素，且这些元素拥有值为 eng 的 lang 属性。/bookstore/book[price>35.00]选取 bookstore 元素的所有 book 元素，且其中的 price 元素的值须大于 35.00。/bookstore/book[price>35.00]/title选取 bookstore 元素中的 book 元素的所有 title 元素，且其中的 price 元素的值须大于 35.00。
#### 选取未知节点
通配符描述*匹配任何元素节点。@*匹配任何属性节点。node()匹配任何类型的节点。例子：
路径表达式结果/bookstore/*选取 bookstore 元素的所有子元素。//*选取文档中的所有元素。//title[@*]选取所有带有属性的 title 元素。
#### 选取若干路径
通过在路径表达式中使用“|”运算符，您可以选取若干个路径。
`//book/title | //book/price`- 选取 book 元素的所有 title 和 price 元素。`//title | //price`- 选取文档中的所有 title 和 price 元素。`/bookstore/book/title | //price`- 选取属于 bookstore 元素的 book 元素的所有 title 元素，以及文档中所有的 price 元素。
## 三、轴
轴可定义相对于当前节点的节点集。
轴名称结果ancestor选取当前节点的所有先辈（父、祖父等）。ancestor-or-self选取当前节点的所有先辈（父、祖父等）以及当前节点本身。attribute选取当前节点的所有属性。child选取当前节点的所有子元素。descendant选取当前节点的所有后代元素（子、孙等）。descendant-or-self选取当前节点的所有后代元素（子、孙等）以及当前节点本身。following选取文档中当前节点的结束标签之后的所有节点。namespace选取当前节点的所有命名空间节点。parent选取当前节点的父节点。preceding选取文档中当前节点的开始标签之前的所有节点。preceding-sibling选取当前节点之前的所有同级节点。self选取当前节点。**步的语法：**
`轴名称::节点测试[谓语]`
例子：
例子结果child::book选取所有属于当前节点的子元素的 book 节点。attribute::lang选取当前节点的 lang 属性。child::*选取当前节点的所有子元素。attribute::*选取当前节点的所有属性。child::text()选取当前节点的所有文本子节点。child::node()选取当前节点的所有子节点。descendant::book选取当前节点的所有 book 后代。ancestor::book选择当前节点的所有 book 先辈。ancestor-or-self::book选取当前节点的所有 book 先辈以及当前节点（如果此节点是 book 节点）child::*/child::price选取当前节点的所有 price 孙节点。
## 四、一些函数
#### 1. starts-with函数
获取以xxx开头的元素
例子：xpath(‘//div[stars-with(@class,”test”)]’)
**2 contains函数**
获取包含xxx的元素
例子：xpath(‘//div[contains(@id,”test”)]’)
**3 and**
与的关系
例子：xpath(‘//div[contains(@id,”test”) and contains(@id,”title”)]’)
**4 text()函数**
例子1：xpath(‘//div[contains(text(),”test”)]’)
例子2：xpath(‘//div[@id=”“test]/text()’)
## 五、一个lxml的xpath示例
下面这个代码来自[http://www.cnblogs.com/descusr/archive/2012/06/20/2557075.html](http://www.cnblogs.com/descusr/archive/2012/06/20/2557075.html)
\#coding=utf-8from`lxml`import`etree
html =`'''
<html>
　　<head>
　　　　<meta name="content-type" content="text/html; charset=utf-8" />
　　　　<title>友情链接查询 - 站长工具</title>
　　　　<!-- uRj0Ak8VLEPhjWhg3m9z4EjXJwc -->
　　　　<meta name="Keywords" content="友情链接查询" />
　　　　<meta name="Description" content="友情链接查询" />
　　</head>
　　<body>
　　　　<h1 class="heading">Top News</h1>
　　　　<p style="font-size: 200%">World News only on this page</p>
　　　　Ah, and here's some more text, by the way.
　　　　<p>... and this is a parsed fragment ...</p>
　　　　<a href="http://www.cydf.org.cn/" rel="nofollow" target="_blank">青少年发展基金会</a> 
　　　　<a href="http://www.4399.com/flash/32979.htm" target="_blank">洛克王国</a> 
　　　　<a href="http://www.4399.com/flash/35538.htm" target="_blank">奥拉星</a> 
　　　　<a href="http://game.3533.com/game/" target="_blank">手机游戏</a>
　　　　<a href="http://game.3533.com/tupian/" target="_blank">手机壁纸</a>
　　　　<a href="http://www.4399.com/" target="_blank">4399小游戏</a> 
　　　　<a href="http://www.91wan.com/" target="_blank">91wan游戏</a>
　　</body>
</html>
'''`page = etree.HTML(html.lower().decode(`'utf-8'`))
hrefs = page.xpath(`u"//a"`)`for`href`in`hrefs:`print`href.attrib`打印出的结果为：
{‘href’: ‘[http://www.cydf.org.cn/](http://www.cydf.org.cn/)‘, ‘target’: ‘_blank’, ‘rel’: ‘nofollow’}
{‘href’: ‘[http://www.4399.com/flash/32979.htm](http://www.4399.com/flash/32979.htm)‘, ‘target’: ‘_blank’}
{‘href’: ‘[http://www.4399.com/flash/35538.htm](http://www.4399.com/flash/35538.htm)‘, ‘target’: ‘_blank’}
{‘href’: ‘[http://game.3533.com/game/](http://game.3533.com/game/)‘, ‘target’: ‘_blank’}
{‘href’: ‘[http://game.3533.com/tupian/](http://game.3533.com/tupian/)‘, ‘target’: ‘_blank’}
{‘href’: ‘[http://www.4399.com/](http://www.4399.com/)‘, ‘target’: ‘_blank’}
{‘href’: ‘[http://www.91wan.com/](http://www.91wan.com/)‘, ‘target’: ‘_blank’}
如果要获取标签a之间的内容，就可以用`print href.text`输出
## 六、总结
上面的内容大多都是抄自网上的一些资料。这里只是做了一个大概的总结，后面如果有漏的还会补充。


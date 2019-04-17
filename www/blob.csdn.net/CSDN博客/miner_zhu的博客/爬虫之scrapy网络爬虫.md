# 爬虫之scrapy网络爬虫 - miner_zhu的博客 - CSDN博客





2018年07月17日 20:08:58[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：115标签：[python																[爬虫](https://so.csdn.net/so/search/s.do?q=爬虫&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[爬虫](https://blog.csdn.net/miner_zhu/article/category/7809267)








# 第1章 初识scrapy

## 1.1 创建第一个项目
- 项目描述： 

	抓取电商网站上的图书名称和价格 

	地址：`http://books.toscrape.com/catalogue/page-1.html`
- 流程： 

	新建项目 —— 新建爬虫 —— 修改爬虫 ——运行爬虫并保存

```
# 1.新建项目
    scrapy startproject example 
# 2.新建爬虫
    scrapy genspider book_spider
# 3.修改爬虫
# 解析网页
for book in response.css('article.product_pod'):
    name = book.xpath('./h3/a/@title').extract_first()
    price = book.css('p.price_color::text').extract_first()
    yield {'name':name, 'pirce':price}

# 实现翻页
next_url = response.css('ul.pager li.next a::attr(href)').extract_first()
if next_url:
    next_url = response.urljoin(next_url)
    yield scrapy.Request(next_url, callback=self.parse)

# 4.运行并保存
scrapy crawl book_spider -o books.csv
```

# 第2章 编写spider

## 2.1 request 对象
- Request对象用来描述一个HTTP请求，下面是其构造器方法的参数列表：
- `scrapy.request( )`的参数 

	详情参见：[https://blog.csdn.net/weixin_37947156/article/details/74974208](https://blog.csdn.net/weixin_37947156/article/details/74974208)

	官方文档：[https://doc.scrapy.org/en/latest/topics/request-response.html](https://doc.scrapy.org/en/latest/topics/request-response.html)
- 虽然参数很多，但除了url外都带有默认值。通常我们只需给一个url参数或再加一个callback参数（默认调用parse）即可。

```
Request(url,
    callback, 
    headers, 
    body, meta, priority=0, errback)
```

## 2.2 response 对象
- 当一个页面下载完成时，下载器依据HTTP响应头部中的Content-Type信息创建某个Response的子类对象。
- 虽然HtmlResponse对象有很多属性，但常用的是以下三个方法。前两个方法用于提取数据，后一个方法用于构造绝对url。
- 
1.xpath(query) ：使用XPath选择器在Response中提取数据，实际上它是response.selector.xpath方法的快捷方式

2.css(query)：使用CSS选择器在Response中提取数据，实际上它是response.selector.css方法的快捷方式

3.urljoin(url)：用于构造绝对url。当传入的url参数是一个相对地址时，根据response.url计算出相应的绝对url。 

- 例如，response.url为[http://www.example.com/a](http://www.example.com/a)，url为b/index.html，调用response.urljoin(url)的结果为 [http://www.example.com/a/b/index.html](http://www.example.com/a/b/index.html)。
- 其他参数详情参见：书《精通Scrapy网络爬虫框架》P16

## 2.3 spider开发流程
- 
Scrapy框架提出以下问题让用户在Spider子类中作答
- 爬虫从哪个或哪些页面开始爬取？
- 对于一个已下载的页面，提取其中的哪些数据？
- 爬取完当前页面后，接下来爬取哪个或哪些页面？

- 实现一个Spider只需4步骤：
	- 继承scrapy.Spider
- 为Spider取名
- 设定起始爬取点
- 实现页面解析函数

- 
为爬虫设置起始爬取地址的方式
- 定义start_urls属性
- 实现start_requests方法

- 实现页面解析函数
- 页面解析函数也就是构造 Request 对象时通过 callback 参数指定的回调函数（或默认的 prase 方法）。他是实现Spider最核心的部分。
	- 用选择器提取页面中的数据，封装后（Item或字典）提交给Scrapy引擎
- 用选择器或LinkExtrator提取页面中的链接，构造新的 Request 对象并提交给Scrapy引擎


# 第三章 使用selector提取数据

## 3.1 selector对象
- 
Srapy综合BeautifulSoup和lxml实现了Selector 类，他是基于lxml 库构建的，并简化了API接口。

- 
在Scarpy中使用Selector 对象提取页面中的数据，使用时先通过Xpath 或CSS选择器选中页面中要提取的数据，然后提取

- 
提取数据的方法： 

	调用Selector或SelectorLis对象的以下方法可将选中的内容提取：
- extract()
- re()
- extract_first() (SelectorList专有)
- re_first() (SelectorList专有)

## 3.2 Response内置selector

- 
页面解析测试


```
from scrapy.http import HtmlResponse
response = HtmlResponse(url='http://www.example.com', body=body, encoding='utf8')
response.selector
```

## 3.3 XPath
- 
XPath即XML路径语言（XML Path Language），它是一种用来确定xml文档中某部分位置的语言。xml文档（html属于xml）是由一系列节点构成的树。

- 
xml文档的节点有多种类型，其中最常用的有以下几种：
- 根节点　整个文档树的根。
- 元素节点　html、body、div、p、a。
- 属性节点　href。
- 文本节点　Hello world、Click here。

- 
节点间的关系有以下几种：
- 父子　body是html的子节点，p和a是div的子节点。反过来，div是p和a的父节点。
- 兄弟　p和a为兄弟节点。
- 祖先／后裔　body、div、p、a都是html的后裔节点；反过来html是body、div、p、a的祖先节点。

- 
基础语法 

	详情参见：[http://www.w3school.com.cn/xpath/xpath_syntax.asp](http://www.w3school.com.cn/xpath/xpath_syntax.asp)






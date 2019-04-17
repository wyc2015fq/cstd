# 2.3 scrapy的重要组件 - 博客堂 - CSDN博客





2017年05月13日 16:14:25[最小森林](https://me.csdn.net/u012052268)阅读数：533










- [scrapy的重要组件](#scrapy的重要组件)- [spiser](#1spiser)- [1概念](#11概念)
- [2执行流程](#12执行流程)
- [基类scrapySpider类的介绍](#基类scrapyspider类的介绍)- [属性](#属性)
- [方法](#方法)

- [spider的子类的介绍](#spider的子类的介绍)- [CrawlSpider类](#1crawlspider类)
- [XMLFeedSpider类](#2xmlfeedspider类)
- [CSVFeedSpider类](#3csvfeedspider类)
- [SitemapSpider类](#4sitemapspider类)


- [selector](#2selector)- [1简介](#21简介)
- [2实例化](#22实例化)
- [3常用方法](#23常用方法)

- [items](#3items)- [1声明Item](#31声明item)

- [item pipelines](#4item-pipelines)- [1作用](#41作用)
- [2编写方法](#42编写方法)- [字段验证例子](#字段验证例子)
- [将item写入JSON文件](#将item写入json文件)
- [将item写入Mongo数据库](#将item写入mongo数据库)
- [去重](#去重)

- [3启用一个Item Pipeline组件](#43启用一个item-pipeline组件)

- [feedexports](#5feedexports)- [1存储爬取的数据格式有](#51存储爬取的数据格式有)
- [2存储端Storage backends](#52存储端storage-backends)






# scrapy的重要组件

## 1.spiser

### 1.1概念

spider 是一个类，定义了怎么爬取一个网站，包括： 

1. 怎样跟踪连接 

2. 怎样提取数据
### 1.2执行流程

```
1.generating the initial Requests ：
```

根据初始url去Requests然后产生response

```
2.parse the response
```

解析response

```
3.using selector
```

使用selector抽取器，去抽取想要的数据。

```
4.store item
```

存储所抽取的内容

### 基类：scrapy.Spider类的介绍

#### 属性:

```
name : spider的名字，要唯一

allowed_domains : 允许的域名

start_urls : 初始的urls 可以多个，用逗号分割。

custom_settings : 个性化设置，覆盖全局的设置。

crawler : 抓取器，将spider绑定在上面

settings ： 配置的实例，包含工程中所有配置变量。

logger : 日志实例
```

#### 方法：

```
from_crawler : 类方法，用于创建spiders

start_requests() ： 生成初始requests

make_requests_from_url(url) ： 根据url生成一个request

parse(response) ： 解析网页内容

log(message [, level, component] ： 记录日志

closed(reason) ： 当spider关闭的时候调用的方法
```

### spider的子类的介绍

#### 1.CrawlSpider类

爬取一般网站常用的spider。其提供了2个新的属性: 

1. rules ：抓取规则 

2. parse_start_url(response) ：解析初始的url
以下三个子类，都是用于抓取专门的页面的。

#### 2.XMLFeedSpider类

#### 3.CSVFeedSpider类

#### 4.SitemapSpider类

## 2.selector

### 2.1简介

网页抽取器。当抓取网页时，你做的最常见的任务是从HTML源码中提取数据。现有的一些库可以达到这个目的：
- BeautifulSoup 是在程序员间非常流行的网页分析库，它基于HTML代码的结构来构造一个Python对象， 对不良标记的处理也非常合理，但它有一个缺点：慢。
- lxml 是一个基于 ElementTree (不是Python标准库的一部分)的python化的XML解析库(也可以解析HTML)。

而Scrapy提取数据有自己的一套机制。它们被称作选择器(seletors)，因为他们通过特定的 XPath 或者 CSS 表达式来“选择” HTML文件中的某个部分。

### 2.2实例化

Scrapy selector是以 text 或 TextResponse 构实例。例子如下：

以text构造:

```python
>>> body = '<html><body><span>good</span></body></html>'
>>> Selector(text=body).xpath('//span/text()').extract()
[u'good']
```

以Response构造:

```python
>>> response = HtmlResponse(url='http://example.com', body=body)
>>> Selector(response=response).xpath('//span/text()').extract()
[u'good']
```

### 2.3常用方法
- xpath   ： 按顺序
- css  ：    按css标签
- re     ：  按正则匹配
- extract()

这几个方法是可以混用的，所以强大！对这个例子来说：

```xml
<html>
 <head>
  <base href='http://example.com/' />
  <title>Example website</title>
 </head>
 <body>
  <div id='images'>
   <a href='image1.html'>Name: My image 1 <br /><img src='image1_thumb.jpg' /></a>
   <a href='image2.html'>Name: My image 2 <br /><img src='image2_thumb.jpg' /></a>
   <a href='image3.html'>Name: My image 3 <br /><img src='image3_thumb.jpg' /></a>
   <a href='image4.html'>Name: My image 4 <br /><img src='image4_thumb.jpg' /></a>
   <a href='image5.html'>Name: My image 5 <br /><img src='image5_thumb.jpg' /></a>
  </div>
 </body>
</html>
```

分开使用的：

```python
# 普通
>>> response.xpath('//title/text()').extract()

>>> response.css('title::text').extract()


# 根据a标签所包含的字符抽取
>>> response.xpath('//a[contains(@href, "image")]/@href').extract()

>>> response.css('a[href*=image]::attr(href)').extract()
```

混合的：

```python
>>> response.xpath('//a[contains(@href, "image")]/text()').re(r'Name:\s*(.*)')
```

## 3.items

爬取的主要目标就是从非结构性的数据源提取结构性数据，例如网页。

为了定义常用的输出数据，Scrapy提供了 Item 类。 Item 对象是种简单的容器，保存了爬取到得数据。 其提供了 类似于词典(dictionary-like) 的API以及用于声明可用字段的简单语法。

### 3.1声明Item

```
import scrapy

class Product(scrapy.Item):
    name = scrapy.Field()
    price = scrapy.Field()
    stock = scrapy.Field()
    last_updated = scrapy.Field(serializer=str)
```

然后在spider中实例化Item，并为他的属性赋值。

## 4.item pipelines

当Item在Spider中被收集之后，它将会被传递到Item Pipeline，一些组件会按照一定的顺序执行对Item的处理。

他们接收到Item并通过它执行一些行为，同时也决定此Item是否继续通过pipeline，或是被丢弃而不再进行处理。

### 4.1作用

以下是item pipeline的一些典型应用：
- 清理HTML数据
- 验证爬取的数据(检查item包含某些字段)
- 查重(并丢弃)
- 将爬取结果保存到数据库中

### 4.2编写方法

#### 字段验证例子：

验证价格，同时丢弃没有价格的item

以下这个假设的pipeline，它为那些不含税(price_excludes_vat 属性)的item调整了 price 属性，同时丢弃了那些没有价格的item:

```python
from scrapy.exceptions import DropItem

class PricePipeline(object):

    vat_factor = 1.15

    def process_item(self, item, spider): # 这个方法必须被实现
        if item['price']:   # 判断price是否存在
            if item['price_excludes_vat']:
                item['price'] = item['price'] * self.vat_factor
            return item
        else:
            raise DropItem("Missing price in %s" % item)
```

#### 将item写入JSON文件

以下pipeline将所有(从所有spider中)爬取到的item，存储到一个独立地 items.jl 文件，每行包含一个序列化为JSON格式的item:

```python
import json

class JsonWriterPipeline(object):

    def __init__(self):
        self.file = open('items.jl', 'wb')

    def process_item(self, item, spider):
        line = json.dumps(dict(item)) + "\n"
        self.file.write(line)
        return item
```

#### 将item写入Mongo数据库

```python
import pymongo

class MongoPipeline(object):

    collection_name = 'scrapy_items'

    def __init__(self, mongo_uri, mongo_db):
        self.mongo_uri = mongo_uri
        self.mongo_db = mongo_db

    @classmethod
    def from_crawler(cls, crawler):
        return cls(
            mongo_uri=crawler.settings.get('MONGO_URI'),
            mongo_db=crawler.settings.get('MONGO_DATABASE', 'items')
        )

    def open_spider(self, spider):
        self.client = pymongo.MongoClient(self.mongo_uri)
        self.db = self.client[self.mongo_db]

    def close_spider(self, spider):
        self.client.close()

    def process_item(self, item, spider):
        self.db[self.collection_name].insert(dict(item))
        return item
```

#### 去重

用于去重的过滤器，丢弃那些已经被处理过的item。让我们假设我们的item有一个唯一的id，但是我们spider返回的多个item中包含有相同的id:

```python
from scrapy.exceptions import DropItem

class DuplicatesPipeline(object):

    def __init__(self):
        self.ids_seen = set()

    def process_item(self, item, spider):
        if item['id'] in self.ids_seen:
            raise DropItem("Duplicate item found: %s" % item)
        else:
            self.ids_seen.add(item['id'])
            return item
```

### 4.3启用一个Item Pipeline组件

要想Item Pipeline组件，必须去settings.py启用。启用方法如下： 

在settings.py中增加 ITEM_PIPELINES 

```bash
ITEM_PIPELINES = {
    'myproject.pipelines.PricePipeline': 300,
    'myproject.pipelines.JsonWriterPipeline': 800,
}
```

分配给每个类的整型值，确定了他们运行的顺序，item按数字从低到高的顺序，通过pipeline，通常将这些数字定义在0-1000范围内。

## 5.feedexports

### 5.1存储爬取的数据。格式有：
- JSON
- JSON lines
- CSV
- XML

### 5.2存储端(Storage backends)
- 本地文件系统
- FTP
- S3等等






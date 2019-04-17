# 【实战】scrapy 爬取果壳问答！ - zhusongziye的博客 - CSDN博客





2018年11月03日 20:15:04[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：47








**引言**

学爬虫的同学都知道，Scrapy是一个非常好用的框架，可以大大的简化我们编写代码的工作量。今天我们就从使用Scrapy爬取果壳问答。



**需求分析**

爬取果壳问答中精彩回答的标题和答案。



**知识点**

爬取数据：Scrapy

数据库：Mongo



**创建项目和爬虫**

创建项目：

scrapy的创建项目命令为startproject，这里我们使用这个命令创建一个果壳的项目。

![](https://img-blog.csdnimg.cn/2018110320132392.png)



创建完项目之后，进入项目文件夹创建爬虫。创建爬虫的命令为genspider。这里我们使用crawl模板来创建spider。

![](https://img-blog.csdnimg.cn/20181103201341772.png)



**部分代码**

创建完成项目和爬虫后，我们使用IDE打开项目。首先修改settings文件。将ROBOTSTXT_OBEY设置成False，将User-Agent的注释打开并根据自身使用的浏览器进行设置。将DOWNLOAD_DELAY设置为1.（这里是下载延时，为了防止被封IP，这里设置延时1秒，可以根据实际情况调整）

Items文件：在Items文件中，我们创建三个属性，分别是question、answer和_id。其中_id是用来存入mongoDB中使用。

```
class GuokrItem(scrapy.Item):
    # define the fields for your item here like:
    # name = scrapy.Field()
    question = scrapy.Field()
    answer = scrapy.Field()
    _id = scrapy.Field()
```



Spider文件：在Spider文件中，我们需要补齐两个rule，第一个规则是从列表中筛选出其他列表页面的url，第二个规则是从列表中筛选出问题的详细地址。

在parse_item中实现信息的提取，这里我们使用css选择器来提取想要的内容，提取完成之后，将item返回。

```
# -*- coding: utf-8 -*-
import scrapy
from scrapy.linkextractors import LinkExtractor
from scrapy.spiders import CrawlSpider, Rule
from Guokr.items import GuokrItem

class GuokrSpider(CrawlSpider):
    name = 'guokr'
    allowed_domains = ['guokr.com']
    start_urls = ['https://www.guokr.com/ask/highlight/?page=1']

    rules = (
        Rule(LinkExtractor(allow=r'page='), follow=True),
        Rule(LinkExtractor(allow=r'question'), callback='parse_item', follow=False),
    )

    def parse_item(self, response):
        item = GuokrItem()
        item['answer'] = response.css(".answer-txt p::text").extract()
        item['question'] = response.css("#articleTitle::text").extract_first()
        yield item
```



Pipelines文件：在pipelines文件中我们需要实现内容的保存，这里提供了两个方式，一种是写到本地txt文件，一种是写到mongoDB中。

```
# -*- coding: utf-8 -*-

# Define your item pipelines here
#
# Don't forget to add your pipeline to the ITEM_PIPELINES setting
# See: https://doc.scrapy.org/en/latest/topics/item-pipeline.html
from scrapy.exporters import JsonItemExporter
import pymongo

class GuokrPipeline(object):
    def open_spider(self,spider):
        self.file = open('guokr.txt','wb')
        self.exporter = JsonItemExporter(self.file)
        self.exporter.start_exporting()

        self.con = pymongo.MongoClient()
        self.database = self.con['guokr']
        self.conllection = self.database['guokr']

    def process_item(self, item, spider):
        self.exporter.export_item(item)

        self.conllection.insert_one(item)

    def close_spider(self,spider):
        self.exporter.finish_exporting()
        self.file.close()

        self.con.close()
```



最后我们需要在settings文件中打开pipelines。（此步略）

部分结果：

![](https://img-blog.csdnimg.cn/20181103201429394.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

至此，我们使用了很少的代码就完成了对果壳问答的爬取。



**源码**

链接：链接：https://pan.baidu.com/s/1MRnkfKIAvsxECMRqygdfCA 提取码 提取码：ls65




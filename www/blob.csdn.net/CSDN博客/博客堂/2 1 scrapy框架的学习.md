# 2.1 scrapy框架的学习 - 博客堂 - CSDN博客





2017年05月12日 16:28:00[最小森林](https://me.csdn.net/u012052268)阅读数：501










- [1scrapy框架的学习](#21scrapy框架的学习)- [为什么学习scrapy](#1为什么学习scrapy)- [2安装scrapy](#2安装scrapy)

- [scrapy简介](#2scrapy简介)- [优点](#优点)

- [scrapy的基本使用步骤](#3scrapy的基本使用步骤)- [1用控制台建立项目并运行](#31用控制台建立项目并运行)
- [2主要代码](#32主要代码)






# 2.1scrapy框架的学习

## 1.为什么学习scrapy

Scrapy是个框架，很多轮子不用自己这写了。 

不过Scrapy比较“重”，如果是小型、单一的爬虫任务，我觉得自己重头写反而比较轻便快速，够用就好。

刚开始学爬虫，建议先理解爬虫的几个模块，理解了原理再学Scrapy才会有效率。

我的这个博客就是按照这个逻辑，先自己写几个轮子，理解爬虫的框架（第一章），然后再学习scrapy框架（第二章）。

第二章是我看的系列视频小麦学院-Alan老师的《Python网络爬虫实战-Scrapy视频教程》的读书笔记，希望大家共同进步。

### 2安装scrapy

因为我安装了python集成框架Anaconda。它包含了几乎所有python数据分析相关的包，并且做了兼容化处理。所以安装scrapy非常容易： 

1.打开cmd输入：`conda list` 可以查看当前Anaconda所包含的所有包，发现没有scrapy。 

2.在cmd中输入`conda install scrapy` 坐等成功即可。
很多资料都说scrapy不支持python 3.x，这是因为资料太老。实践证明是支持。 现在几乎所有的包都支持python 3.x 特别是这种初学者常用的，所以建议大家全面拥抱python 3.x 特别是初学者。

## 2.scrapy简介

是开源的爬虫框架。

### 优点
- 速度快。只需要写规则。
- 易扩展。
- 便捷的，用python编写的。

## 3.scrapy的基本使用步骤❤

### 3.1用控制台建立项目并运行

1.在目标文件夹内按住shift加右键，进入cmd 

2.输入代码建立项目tutorial，并进入该项目

```bash
scrapy startproject tutorial

cd tutorial
```

3.创建默认格式的爬虫（可以不止一个）

```
scrapy genspider dmoz_spider domz.org
# 爬虫名字 和 作用域
```

4.更改dmoz_spider.py与item

5.运行

```
scrapy crawl dmoz_spider -o douban.csv

# （最后 -o 的意思是 把爬取的数据存储在douban.csv
中，同理 换成douban.json也就存在了json中）
```

### 3.2主要代码

代码主要更改两个部分：items.py与dmoz_spider.py 代码如下：

items.py

```
# 该类用于定义需要的数据
# 在这个例子中，需要获取book的名字，描述，连接。
import scrapy


class TutorialItem(scrapy.Item):
    # define the fields for your item here like:
    # name = scrapy.Field()

    title = scrapy.Field()
    desc = scrapy.Field()
    link = scrapy.Field()
```

dmoz_spider.py

```python
# 这个类用于构建爬虫，主要是解析器，
# 可以用bs和xpath和正则，这里用xpath

import scrapy
from tutorial.items import TutorialItem

class DmozSpiderSpider(scrapy.Spider):
    name = "dmoz_spider" # 爬虫的名字（唯一）
    allowed_domains = ["dmoztools.net"] # 爬虫作用域
    start_urls = ['http://dmoztools.net/Computers/Programming/Languages/Python/Books/'] # 初始url

    def parse(self, response): # 解析函数，注意这里的xpath地址是用谷歌浏览器的F12 直接获取。谷歌浏览器就是牛！
        divs = response.xpath('//*[@id="site-list-content"]/div')
        for div in divs:
            item = TutorialItem()
            item['title'] = div.xpath('div[3]/a/div/text()').extract()
            item['link'] = div.xpath('div[3]/a/@href').extract()
            item['desc'] = div.xpath('div[3]/div/text()').extract()
            yield item
```





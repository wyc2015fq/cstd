
# [Python爬虫] scrapy爬虫系列 .安装及入门介绍 - 杨秀璋的专栏 - CSDN博客

2015年11月08日 04:22:10[Eastmount](https://me.csdn.net/Eastmount)阅读数：9037所属专栏：[Python爬虫之Selenium+Phantomjs+CasperJS](https://blog.csdn.net/column/details/eastmount-spider.html)



前面介绍了很多Selenium基于自动测试的Python爬虫程序，主要利用它的xpath语句，通过分析网页DOM树结构进行爬取内容，同时可以结合Phantomjs模拟浏览器进行鼠标或键盘操作。但是，更为广泛使用的Python爬虫框架是——Scrapy爬虫。这是一篇在Windows系统下介绍 Scrapy爬虫安装及入门介绍的相关文章。
官方 Scrapy  ：[http://scrapy.org/](http://scrapy.org/)
官方英文文档：[http://doc.scrapy.org/en/latest/index.html](http://doc.scrapy.org/en/latest/index.html)
官方中文文档：[https://scrapy-chs.readthedocs.org/zh_CN/0.24/index.html](https://scrapy-chs.readthedocs.org/zh_CN/0.24/index.html)

## 一. 安装过程
本文主要讲述Windows下的安装过程，首先我的Python是2.7.8版本。
主要通过Python的PIP语句进行安装：

```python
pip install scrapy
```
安装PIP参考：[http://blog.csdn.net/eastmount/article/details/47785123](http://blog.csdn.net/eastmount/article/details/47785123)
通过**pip list outdated**命令查看软件最新版本，表示PIP安装成功。
![](https://img-blog.csdn.net/20151023221457744)
然后，输入**pip install scrapy**命令进行安装。
![](https://img-blog.csdn.net/20151023221734497)
安装成功后，通过cmd调用**scrapy**指令查看，表示安装成功。
![](https://img-blog.csdn.net/20151024010703682)
如果过程中存在如下图所示错误"**n****o module named win32api**"，则需要下载安装win32，选择2.7.8版本。地址为：[http://sourceforge.net/projects/pywin32/files/](http://sourceforge.net/projects/pywin32/files/)
![](https://img-blog.csdn.net/20151024011422321)
**此时，scrapy安装成功，可以进行第二步"第一个scrapy爬虫实现"了~**

正如[xifeijian大神](http://blog.csdn.net/xifeijian/article/details/12576455)所说：“作为Python爱好者，如果不知道easy_install或者pip中的任何一个的话，那么......”。easy_insall的作用和perl中的cpan，ruby中的gem类似，都提供了在线一键安装模块的傻瓜方便方式，而pip是easy_install的改进版，提供更好的提示信息，删除package等功能。老版本的python中只有easy_install，没有pip。常见的具体用法如下：
```python
easy_install的用法：    
1） 安装一个包    
 $ easy_install <package_name>    
 $ easy_install "<package_name>==<version>"    
2) 升级一个包    
 $ easy_install -U "<package_name>>=<version>"    
    
pip的用法    
1) 安装一个包    
 $ pip install <package_name>    
 $ pip install <package_name>==<version>    
2) 升级一个包 (如果不提供version号，升级到最新版本）    
 $ pip install --upgrade <package_name>>=<version>    
3）删除一个包    
 $ pip uninstall <package_name>
```

## 二. 第一个scrapy爬虫程序实现
官网介绍：
Scrapy是一个为了爬取网站数据，提取结构性数据而编写的应用框架。可以应用在包括数据挖掘，信息处理或存储历史数据等一系列的程序中。其最初是为了 页面抓取 (更确切来说, 网络抓取 )所设计的， 也可以应用在获取API所返回的数据(例如 Amazon Associates Web Services ) 或者通用的网络爬虫。
Anopen sourceand collaborative framework forextracting the datayou need from websites. In a fast, simple, yet extensible way.
下面是参考[官网](http://scrapy.org/)在windows下实现的第一个scrapy爬虫程序：

![](https://img-blog.csdn.net/20151024014553110)
打开**Python IDLE**，创建**myspider.py**文件，代码如下：
```python
import scrapy
class BlogSpider(scrapy.Spider):
    name = 'blogspider'
    start_urls = ['http://blog.scrapinghub.com']
    def parse(self, response):
        for url in response.css('ul li a::attr("href")').re(r'.*/\d\d\d\d/\d\d/$'):
            yield scrapy.Request(response.urljoin(url), self.parse_titles)
    def parse_titles(self, response):
        for post_title in response.css('div.entries > ul > li a::text').extract():
            yield {'title': post_title}
```
如果此时你试图点击Run运行程序或在IDLE中输入**scrapy runspider myspider.py**，似乎返回如下错误：
![](https://img-blog.csdn.net/20151024015316252)
此时我怀疑Windows下需要调用cmd运行程序，还是强烈推荐在Linux下学习使用Python相关编程知识。调用cd去到文件所在目录：
cd G:\software\Program software\Python\python insert\scrapy project
然后在运行程序，结果输出如下所示：
![](https://img-blog.csdn.net/20151024021905984)
**此时，程序能够运行成功了，不论结果如何、代码如何，总算在Windows下跑起来了。下面第三部分，我再简单介绍如何调用Scrapy爬虫进行一个入门相关的爬取~**

## 三. Scrapy入门介绍
入门介绍参考：[初窥Scrapy](https://scrapy-chs.readthedocs.org/zh_CN/0.24/intro/overview.html)和[Scrapy入门教程](https://scrapy-chs.readthedocs.org/zh_CN/0.24/intro/tutorial.html)
给大家简单举个例子，使用maxliaops的Scrapy爬虫爬取腾讯的职位招聘信息。
代码下载：[https://github.com/maxliaops/scrapy-itzhaopin](https://github.com/maxliaops/scrapy-itzhaopin)
源文链接：[http://blog.csdn.net/HanTangSongMing/article/details/24454453](http://blog.csdn.net/HanTangSongMing/article/details/24454453)
目标网址为：[http://hr.tencent.com/position.php](http://hr.tencent.com/position.php)
![](https://img-blog.csdn.net/20151108033016137)
Windows下Ctrl+R调用CMD命令行。输入命令如下：
```python
1.chcp 936
unknown encoding: cp65001异常时，需要将编码(UTF-8)修改为 简体中文(GBK)
2.cd G:\software\Program software\Python\python insert\scrapy project
去到安装Scrapy目录下
3.cd scrapy-itzhaopin-master\itzhaopin
再次去到下载的文件itzhaopin目录下
4.scrapy crawl tencent
运行代码启动这个Spider，进行下载
```
最后运行会在scrapy-itzhaopin-master\itzhaopin文件夹下生产一个tencent.json的结果。数据量很大，下图只展示部分日期是2015-11-07的数据，如下所示：
![](https://img-blog.csdn.net/20151108034930708)
![](https://img-blog.csdn.net/20151108035001763)

其中代码itzhaopin项目的结果图如下所示：参考原文作者[博客](http://blog.csdn.net/HanTangSongMing/article/details/24454453)

```python
├── itzhaopin
│   ├── itzhaopin
│   │   ├── __init__.py
│   │   ├── items.py
│   │   ├── pipelines.py
│   │   ├── settings.py
│   │   └── spiders
│   │      └── __init__.py
│   └── scrapy.cfg
scrapy.cfg: 项目配置文件
items.py: 需要提取的数据结构定义文件
pipelines.py:管道定义，用来对items里面提取的数据做进一步处理，如保存等
settings.py: 爬虫配置文件
spiders: 放置spider的目录
```
核心的几个py文件内容如下，详见github：
**1.items.py：****定义我们要抓取的数据**
```python
# Define here the models for your scraped items
#
# See documentation in:
# http://doc.scrapy.org/en/latest/topics/items.html
from scrapy.item import Item, Field  
class TencentItem(Item):  
    name = Field()                # 职位名称  
    catalog = Field()             # 职位类别  
    workLocation = Field()        # 工作地点  
    recruitNumber = Field()       # 招聘人数  
    detailLink = Field()          # 职位详情页链接  
    publishTime = Field()         # 发布时间
```
**2.spiders文件夹中tencent_spider.py文件：****实现Spider**
Spider是一个继承自scrapy.contrib.spiders.CrawlSpider的Python类，有三个必需的定义的成员
name: 名字，这个spider的标识
start_urls:一个url列表，spider从这些网页开始抓取
parse():一个方法，当start_urls里面的网页抓取下来之后需要调用这个方法解析网页内容，同时需要返回下一个需要抓取的网页，或者返回items列表
```python
import re
import json
from scrapy.selector import Selector
try:
    from scrapy.spider import Spider
except:
    from scrapy.spider import BaseSpider as Spider
from scrapy.utils.response import get_base_url
from scrapy.utils.url import urljoin_rfc
from scrapy.contrib.spiders import CrawlSpider, Rule
from scrapy.contrib.linkextractors.sgml import SgmlLinkExtractor as sle

from itzhaopin.items import *
from itzhaopin.misc.log import *

class TencentSpider(CrawlSpider):
    name = "tencent"
    allowed_domains = ["tencent.com"]
    start_urls = [
        "http://hr.tencent.com/position.php"
    ]
    rules = [
        Rule(sle(allow=("/position.php\?&start=\d{,4}#a")), follow=True, callback='parse_item')
    ]
    def parse_item(self, response):
        items = []
        sel = Selector(response)
        base_url = get_base_url(response)
        sites_even = sel.css('table.tablelist tr.even')
        for site in sites_even:
            item = TencentItem()
            item['name'] = site.css('.l.square a').xpath('text()').extract()[0]
            relative_url = site.css('.l.square a').xpath('@href').extract()[0]
            item['detailLink'] = urljoin_rfc(base_url, relative_url)
            item['catalog'] = site.css('tr > td:nth-child(2)::text').extract()[0]
            item['workLocation'] = site.css('tr > td:nth-child(4)::text').extract()[0]
            item['recruitNumber'] = site.css('tr > td:nth-child(3)::text').extract()[0]
            item['publishTime'] = site.css('tr > td:nth-child(5)::text').extract()[0]
            items.append(item)
            #print repr(item).decode("unicode-escape") + '\n'
        sites_odd = sel.css('table.tablelist tr.odd')
        for site in sites_odd:
            item = TencentItem()
            item['name'] = site.css('.l.square a').xpath('text()').extract()[0]
            relative_url = site.css('.l.square a').xpath('@href').extract()[0]
            item['detailLink'] = urljoin_rfc(base_url, relative_url)
            item['catalog'] = site.css('tr > td:nth-child(2)::text').extract()[0]
            item['workLocation'] = site.css('tr > td:nth-child(4)::text').extract()[0]
            item['recruitNumber'] = site.css('tr > td:nth-child(3)::text').extract()[0]
            item['publishTime'] = site.css('tr > td:nth-child(5)::text').extract()[0]
            items.append(item)
            #print repr(item).decode("unicode-escape") + '\n'
        info('parsed ' + str(response))
        return items

    def _process_request(self, request):
        info('process ' + str(request))
        return request
```
**3.pipelines.py：****实现PipeLine**
PipeLine用来对Spider返回的Item列表进行保存操作，可以写入到文件、或者数据库等。PipeLine只有一个需要实现的方法：process_item，例如我们将Item保存到JSON格式文件中：
```python
# Define your item pipelines here
#
# Don't forget to add your pipeline to the ITEM_PIPELINES setting
# See: http://doc.scrapy.org/en/latest/topics/item-pipeline.html
from scrapy import signals
import json
import codecs
class JsonWithEncodingTencentPipeline(object):
    def __init__(self):
        self.file = codecs.open('tencent.json', 'w', encoding='utf-8')
    def process_item(self, item, spider):
        line = json.dumps(dict(item), ensure_ascii=False) + "\n"
        self.file.write(line)
        return item
    def spider_closed(self, spider):
        self.file.close(
)
```
**4.settings.py：设置文件**
```python
# Scrapy settings for itzhaopin project
#
# For simplicity, this file contains only the most important settings by
# default. All the other settings are documented here:
#
#     http://doc.scrapy.org/en/latest/topics/settings.html
#
BOT_NAME = 'itzhaopin'
SPIDER_MODULES = ['itzhaopin.spiders']
NEWSPIDER_MODULE = 'itzhaopin.spiders'
# Crawl responsibly by identifying yourself (and your website) on the user-agent
#USER_AGENT = 'itzhaopin (+http://www.yourdomain.com)'
ITEM_PIPELINES = {
    'itzhaopin.pipelines.JsonWithEncodingTencentPipeline': 300,
}
LOG_LEVEL = 'INFO'
```
看了这个简单的例子后，后面的文章就会根据原文进行一些自定义的爬虫实验了，希望对你有所帮助吧~同时还是感觉Linux下学习这些更适合些。最后推荐两篇文章：
[Python爬虫框架Scrapy实战之定向批量获取职位招聘信息](http://blog.csdn.net/HanTangSongMing/article/details/24454453)
[
Scrapy研究探索专栏 作者：young-hz](http://blog.csdn.net/column/details/younghz-scrapy.html)
[[Python]网络爬虫（12）：爬虫框架Scrapy的第一个爬虫示例入门教程](http://blog.csdn.net/pleasecallmewhy/article/details/19642329)
(By:Eastmount
 2015-11-08 深夜4点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))



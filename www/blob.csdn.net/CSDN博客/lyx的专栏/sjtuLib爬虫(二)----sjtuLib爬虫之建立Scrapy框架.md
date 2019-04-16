# sjtuLib爬虫(二)----sjtuLib爬虫之建立Scrapy框架 - lyx的专栏 - CSDN博客





2016年03月08日 15:18:01[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：843








还是爬交大图书馆的数据，不过这次我们用Scrapy的开源爬虫框架。上次我们在Anaconda平台下安装了Scrapy，这回就可以开始用啦。

scrapy.cfg: 整个项目的配置文件

goodBook/: 项目各个模块,代码就主要在这里了

goodBook/items.py: 话说爬虫爬下来的东西总得存起来吧，存的话得有一个标准格式吧，这个就是了。就是存储的容器标准，可以当做一个结构体理解。

goodBook/pipelines.py: 处理获取的items的，是存在电脑上，还是发给别人，是存成表格还是存到数据库，就看这个啦。
goodBook/settings.py: 配置文件
goodBook/spiders: 这就是放爬虫的文件夹。





这些文件呢，先给一个sample，这样以后不同的应用往里面放就可以了。先上一个最简单的吧，爬取一个页面的。

首先是items.py，也就是一种自己说明的结构体哦。



```python
from scrapy.item import Item,Field


class GoodbookItem(Item):
    # define the fields for your item here like:
    # name = scrapy.Field()
    #book name
    bookName = Field()
    #the request number
    bookRq = Field()
    #pass
```


这里就定义个一个容器标准，以后呢，爬下来的数据都按照这样的标准存储，也就是存成两个部分，bookName和bookRq。



然后是

pipelines.py



```python
pipelines.pyimport json  
import codecs  
 
 
class GoodbookPipeline(object):  
    def __init__(self):  
        self.file = codecs.open('goodBook.json', 'wb', encoding='utf-8')  
 
    def process_item(self, item, spider):  
        line = json.dumps(dict(item)) + '\n'  
        # print line  
        self.file.write(line.decode("unicode_escape"))  
        return item
```
这里你可以看到，
```python
self.file = codecs.open('goodBook.json', 'wb', encoding='utf-8')
```
建立了一个json文件，什么事json文件呢，把他当成字典的组合就可以了。在后面的处理中，没我们可以发现，这段代码是吧爬取到的items写到json文件里面了。



spiders

这是最核心的文件了。



```python
#!/usr/bin/python
# -*- coding:utf-8 -*-

from scrapy.spider import Spider
from scrapy.selector import Selector
#from scrapy import log

from goodBook.items import GoodbookItem


class goodBookSpider(Spider):
 
    name = "goodBook"
    allowed_domains = ["你要爬取的网页的域名"]
    start_urls = [
         "爬取开始的url"
    ]

    def parse(self, response):

        sel = Selector(response)
        sites = sel.xpath('xpath的处理')
        items = []

        for site in sites:
            item = GoodbookItem()

            title = site.xpath('xpath的获取代码').extract()
            link = site.xpath('xpath的获取代码').extract()
            
            item['bookName'] = [t.encode('utf-8') for t in title]
            item['bookRq'] = [l.encode('utf-8') for l in link]
            items.append(item)

        return items
```


xpath是什么东西呢，就是如何从那么多的html代码中检索出你想要的。例如下面这一段，如何提取第四行“钱塘小甲子_新浪博客”这几个字呢？就是xpath的作用了。





```
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>钱塘小甲子_新浪博客</title>
<meta name="keywords" content="" />
<meta name="description" content="" />
<meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7" />
<meta http-equiv="mobile-agent" content="format=html5; url=http://blog.sina.cn/dpool/blog/u/2335065822">
<meta http-equiv="mobile-agent" content="format=wml; url=http://blog.sina.cn/dpool/blog/ArtList.php?uid=2335065822&vt=1">
<!–[if lte IE 6]>
<script type="text/javascript">
try{
document.execCommand("BackgroundImageCache", false, true);
}catch(e){}
</script>
<![endif]–>
<script type="text/javascript">
window.staticTime=new Date().getTime();
</script>
<link rel="pingback" href="http://upload.move.blog.sina.com.cn/blog_rebuild/blog/xmlrpc.php" />
<link rel="EditURI" type="application/rsd+xml" title="RSD" href="http://upload.move.blog.sina.com.cn/blog_rebuild/blog/xmlrpc.php?rsd" />
<link href="http://blog.sina.com.cn/blog_rebuild/blog/wlwmanifest.xml" type="application/wlwmanifest+xml" rel="wlwmanifest" />
<link rel="alternate" type="application/rss+xml" href="http://blog.sina.com.cn/rss/2335065822.xml" title="RSS" />
<link href="http://simg.sinajs.cn/blog7style/css/conf/blog/indexM.css" type="text/css" rel="stylesheet" /><style id="tplstyle" type="text/css">@charset "utf-8";@import url("http://simg.sinajs.cn/blog7newtpl/css/4/4_17/t.css");
</style>
<style id="positionstyle"  type="text/css">
.sinabloghead .blogtoparea{ left:85px;top:77.1374969482422px;}
.sinabloghead .blognav{ left:10px;top:119.699996948242px;}
</style>
<style id="bgtyle"  type="text/css">
</style>
<style id="headtyle"  type="text/css">
</style>
<style id="navtyle"  type="text/css">
</style>
</head>
<body>
```








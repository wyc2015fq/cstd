# 三十、十分钟掌握最强大的python爬虫 - jiangjingxuan的博客 - CSDN博客





2017年01月25日 11:52:20[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：1557












![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_125_default_big.jpeg)



如今大数据是互联网技术的热门，应用也很广泛，所以无论是做互联网产品还是学术研究，抓取他人的资源是快速有效的方法，只要不盗取版权就不为过。开源的爬虫软件很多，本节来介绍最流行也是使用最多的python爬虫开源项目scrapy

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 安装方法



执行

```
yum install libffi-devel
yum install openssl-devel
pip install scrapy
```



scrapy的代码会安装在
`/usr/local/lib/python2.7/site-packages/scrapy`


中文文档在

[http://scrapy-chs.readthedocs.io/zh_CN/latest/](http://scrapy-chs.readthedocs.io/zh_CN/latest/)



## 使用样例



创建文件shareditor.py如下：



```python
# -*- coding: utf-8 -*-
import scrapy


class ShareditorSpider(scrapy.Spider):
    name = 'shareditor'
    start_urls = ['http://www.shareditor.com/']

    def parse(self, response):
        for href in response.css('a::attr(href)'):
            full_url = response.urljoin(href.extract())
            yield scrapy.Request(full_url, callback=self.parse_question)

    def parse_question(self, response):
        yield {
            'title': response.css('h1 a::text').extract()[0],
            'link': response.url,
        }
```

然后执行：
`scrapy runspider shareditor.py`
会看到抓取打印的debug信息，它爬取了www.shareditor.com网站的全部网页

是不是很容易掌握？



## 创建网络爬虫常规方法



上面是一个最简单的样例，真正网络爬虫需要有精细的配置和复杂的逻辑，所以介绍一下scrapy的常规创建网络爬虫的方法

执行
`[root@centos7vm tmp]# scrapy startproject myfirstpro`
自动创建了myfirstpro目录，进去看下内容：

```
[root@centos7vm tmp]# cd myfirstpro/myfirstpro/
[root@centos7vm myfirstpro]# ls
__init__.py  items.py  pipelines.py  settings.py  spiders
```



## 讲解一下几个文件



settings.py是爬虫的配置文件，讲解其中几个配置项：



USER_AGENT是ua，也就是发http请求时指明我是谁，因为我们的目的不纯，所以我们伪造成浏览器，改成

```bash
USER_AGENT = 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_4) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/50.0.2661.102 Safari/537.36'
```

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

ROBOTSTXT_OBEY表示是否遵守robots协议（被封禁的不抓取），因为我们的目的不纯，所以我们不遵守，改成
`ROBOTSTXT_OBEY = False`


DOWNLOAD_DELAY表示对同一个站点抓取延迟，也就是抓一个，歇一会，再抓一个，再歇一会，为了对对方站点冲(yi)击(bei)太(fa)大(xian)，我们调整为1，也就是一秒抓一个

CONCURRENT_REQUESTS_PER_DOMAIN表示对同一个站点并发有多少个线程抓取，同样道理，我们也调整为1

CONCURRENT_REQUESTS_PER_IP同理也调整为1



items.py是抓取结果解析出来的结构体，一会再说



下面我们进入spiders目录，创建我们的第一个spider程序，如下：

```python
import scrapy

class ShareditorSpider(scrapy.Spider):
    name = "shareditor"
    allowed_domains = ["shareditor.com"]
    start_urls = [
        "http://www.shareditor.com/"
    ]

    def parse(self, response):
        filename = response.url.split("/")[-2] + '.html'
        with open(filename, 'wb') as f:
            f.write(response.body)
```



这里面start_urls是初始抓取时的种子链接，parse方法在抓取完成后自动调用，会把抓取回来的body内容写到以.html为结尾的文件中



退到上一级目录执行：
`[root@centos7vm myfirstpro]# scrapy crawl shareditor`


执行完成后会多出来一个www.shareditor.com.html文件，内容就是http://www.shareditor.com/网页的内容



## 页面解析



下面说一下页面解析，这是所有的网络爬虫不能忽略的功能，而且是最核心的部分。python库用于页面解析的有BeautifulSoup（速度慢）和Ixml（非标准库），但两者都有一些缺点，所以scrapy基于Ixml实现了一套页面解析工具，叫做Selectors



Selector的使用方法非常简单，创建TestSelectors.py如下：

```python
from scrapy.selector import Selector
from scrapy.http import HtmlResponse
body = '<html><body><span>good</span></body></html>'
span_text=Selector(text=body).xpath('//span/text()').extract()
print span_text
```



执行python TestSelectors.py输出：

```python
[root@centos7vm tmp]# python TestSelectors.py
[u'good']
```



如果能直接拿到HtmlResponse对象，也可以直接调用：

```bash
response.selector.xpath('//span').extract()
```



有关xpath的内容，可以参考[https://www.w3.org/TR/xpath/#location-paths](https://www.w3.org/TR/xpath/#location-paths)



## 调试页面解析



scrapy提供了非常方便的页面解析方法，直接执行
`scrapy shell http://www.shareditor.com/`
进入调试终端，可以直接使用request、response等变量来做各种操作尝试



注意：

1. 如果想获取文本，调用selector的xpath方法后要调用extract()方法

2. 如果想找到第一个匹配xpath的内容，需要调用extract_first()方法





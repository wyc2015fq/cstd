
# Python scrapy 安装与开发 - 阳光岛主 - CSDN博客

2017年08月18日 23:12:04[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：1536个人分类：[NetWork																](https://blog.csdn.net/sunboy_2050/article/category/698436)



Scrapy是采用Python开发的一个快速、高层次的屏幕抓取和web抓取框架，用于抓取采集web站点信息并从页面中提取结构化的数据。
Scrapy用途广泛，可以用于数据挖掘、监测和自动化测试等。
Scrapy吸引人的地方在于它是一个框架，任何人都可以根据需求方便的修改。
它也提供了多种类型爬虫的基类，如BaseSpider、sitemap爬虫等，最新版本又提供了web2.0爬虫的支持。
Scrapy is a fast high-level web crawling and web scraping framework, used to crawl websites and extract structured data from their pages.
It can be used for a wide range of purposes, from data mining to monitoring and automated testing.
For more information including a list of features check the Scrapy homepage at:[http://scrapy.org](http://scrapy.org/)
**Scrapy 官网**：[https://scrapy.org](https://scrapy.org/)
**Scrapy 插件**：[https://pypi.python.org/pypi/Scrapy](https://pypi.python.org/pypi/Scrapy)
Scratch，是抓取的意思，这个Python的爬虫框架叫Scrapy，大概也是这个意思吧，就叫它：小刮刮吧。
Scrapy 使用了 Twisted异步网络库来处理网络通讯，其设计整体架构大致如下图：
![python-scrapy-an-zhuang-yu-kai-fa-01](http://blog.mimvp.com/wp-content/uploads/2017/08/python-scrapy-an-zhuang-yu-kai-fa-01.png)
Scrapy主要包括了以下组件：
引擎(Scrapy)
用来处理整个系统的数据流处理, 触发事务(框架核心)
调度器(Scheduler)
用来接受引擎发过来的请求, 压入队列中, 并在引擎再次请求的时候返回. 可以想像成一个URL（抓取网页的网址或者说是链接）的优先队列, 由它来决定下一个要抓取的网址是什么, 同时去除重复的网址
下载器(Downloader)
用于下载网页内容, 并将网页内容返回给蜘蛛(Scrapy下载器是建立在twisted这个高效的异步模型上的)
爬虫(Spiders)
爬虫是主要干活的, 用于从特定的网页中提取自己需要的信息, 即所谓的实体(Item)。用户也可以从中提取出链接,让Scrapy继续抓取下一个页面
项目管道(Pipeline)
负责处理爬虫从网页中抽取的实体，主要的功能是持久化实体、验证实体的有效性、清除不需要的信息。当页面被爬虫解析后，将被发送到项目管道，并经过几个特定的次序处理数据。
下载器中间件(Downloader Middlewares)
位于Scrapy引擎和下载器之间的框架，主要是处理Scrapy引擎与下载器之间的请求及响应。
爬虫中间件(Spider Middlewares)
介于Scrapy引擎和爬虫之间的框架，主要工作是处理蜘蛛的响应输入和请求输出。
调度中间件(Scheduler Middewares)
介于Scrapy引擎和调度之间的中间件，从Scrapy引擎发送到调度的请求和响应。
Scrapy运行流程大概如下：
引擎从调度器中取出一个链接(URL)用于接下来的抓取
引擎把URL封装成一个请求(Request)传给下载器
下载器把资源下载下来，并封装成应答包(Response)
爬虫解析Response
解析出实体（Item）,则交给实体管道进行进一步的处理
解析出的是链接（URL）,则把URL交给调度器等待抓取
**Scrapy 安装**
因为python3并不能完全支持Scrapy，因此为了完美运行Scrapy，我们使用python2.7来编写和运行Scrapy。
pip install Scrapy          \# python2.7
pip3 install Scrapy        \# python3.6
验证安装成功：
|1
|2
|3
|4
|5
|6
|7
|$ python
|Python 2.7.13 (v2.7.13:a06454b1afa1, Dec 17 2016, 12:39:47)
|[GCC 4.2.1 (Apple Inc. build 5666) (dot 3)] on darwin
|Type|"help"|,|"copyright"|,|"credits"|or|"license"|for|more|information.
|>>>|import|scrapy
|>>> scrapy
|<module|'scrapy'|from|'/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/site-packages/scrapy/__init__.pyc'|>
|

**Scrapy 使用**
**1、创建项目****mimvp_proxy_python_scrapy**
运行命令:
|1
|2
|3
|4
|5
|$|which|scrapy
|/Library/Frameworks/Python|.framework|/Versions/3|.6|/bin/scrapy
|$
|$|cd|MimvpProxyDemo|/PythonScrapy/
|$ scrapy startproject mimvp_proxy_python_scrapy
|

**2、查看目录结构**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|$ tree
|.
||____mimvp_proxy_python_scrapy
|| |____mimvp_proxy_python_scrapy
|| | |______init__.py
|| | |______pycache__
|| | |____items.py
|| | |____middlewares.py
|| | |____pipelines.py
|| | |____settings.py
|| | |____spiders
|| | | |______init__.py
|| | | |______pycache__
|| |____scrapy.cfg
|

导入Eclipse，查看自动创建的目录结构：
![python-scrapy-an-zhuang-yu-kai-fa-02](http://blog.mimvp.com/wp-content/uploads/2017/08/python-scrapy-an-zhuang-yu-kai-fa-02.png)
目录文件说明：
scrapy.cfg  项目的配置信息，主要为Scrapy命令行工具提供一个基础的配置信息
items.py    设置数据存储模板，用于结构化数据，如：Django的Model
pipelines    数据处理行为，如：一般结构化的数据持久化
settings.py 配置文件，如：递归的层数、并发数，延迟下载等爬虫相关的配置
spiders      爬虫目录，如：创建文件、编写爬虫规则
注意：一般创建爬虫文件时，以网站域名命名
**3、Scrapy 编写爬虫**
在spiders目录中新建mimvp_spider.py文件
示例代码：
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|\#!/usr/bin/env python
|\# -*- coding:utf-8 -*-
|\#
|\# mimvp.com
|\# 2009.10.1

|import|scrapy
|class|MimvpSpider(scrapy.spiders.Spider):
|name|=|"mimvp"
|allowed_domains|=|[|"mimvp.com"|]
|start_urls|=|[
|"http://mimvp.com"|,
|]
|def|parse(|self|, response):
|\# print(response, type(response))
|\# from scrapy.http.response.html import HtmlResponse
|\# print(response.body_as_unicode())
|mimvp_url|=|response.url|\# 爬取时请求的url
|body|=|response.body|\# 返回网页内容
|unicode_body|=|response.body_as_unicode()|\# 返回的html unicode编码
|print|(|"mimvp_url : "|+|str|(mimvp_url))
|print|(|"body : "|+|str|(body))
|print|(|"unicode_body : "|+|str|(unicode_body))
|

为了创建一个Spider，必须继承`scrapy.Spider`类， 且定义以下三个属性:
name: 用于区别Spider。 该名字必须是唯一的，您不可以为不同的Spider设定相同的名字。
start_urls: 包含了Spider在启动时进行爬取的url列表。 因此，第一个被获取到的页面将是其中之一。 后续的URL则从初始的URL获取到的数据中提取。
parse()是spider的一个方法。 被调用时，每个初始URL完成下载后生成的Response对象将会作为唯一的参数传递给该函数。 该方法负责解析返回的数据(response data)，提取数据(生成item)以及生成需要进一步处理的URL的Request对象。
**详细说明：**
1. 爬虫文件需要定义一个类，并继承scrapy.spiders.Spider，即类定义MimvpSpider(scrapy.spiders.Spider)
2. 必须定义name，即爬虫名，如果没有name，会报错，因为源码中是这样定义的：
|1
|2
|3
|4
|5
|6
|7
|8
|def|__init__(|self|, name|=|None|,|*|*|kwargs):
|if|name|is|not|None|:
|self|.name|=|name
|elif|not|getattr|(|self|,|'name'|,|None|):
|raise|ValueError(|"%s must have a name"|%|type|(|self|).__name__)
|self|.__dict__.update(kwargs)
|if|not|hasattr|(|self|,|'start_urls'|):
|self|.start_urls|=|[]
|

3. 编写函数parse，这里需要注意的是，该函数名不能改变，因为Scrapy源码中默认callback函数的函数名就是parse；
4. 定义需要爬取的url，放在列表中，因为可以爬取多个url，Scrapy源码是一个For循环，从上到下爬取这些url，使用生成器迭代将url发送给下载器下载url的html。源码如下：
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|def|start_requests(|self|):
|cls|=|self|.__class__
|if|method_is_overridden(|cls|, Spider,|'make_requests_from_url'|):
|for|url|in|self|.start_urls:
|yield|self|.make_requests_from_url(url)
|else|:
|for|url|in|self|.start_urls:
|yield|Request(url, dont_filter|=|True|)
|def|make_requests_from_url(|self|, url):
|""" This method is deprecated. """
|return|Request(url, dont_filter|=|True|)
|

**Request 类源码：**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|class|Request(object_ref):
|def|__init__(|self|, url, callback|=|None|, method|=|'GET'|, headers|=|None|, body|=|None|,
|cookies|=|None|, meta|=|None|, encoding|=|'utf-8'|, priority|=|0|,
|dont_filter|=|False|, errback|=|None|, flags|=|None|):
|self|._encoding|=|encoding|\# this one has to be set first
|self|.method|=|str|(method).upper()
|self|._set_url(url)
|self|._set_body(body)
|assert|isinstance|(priority,|int|),|"Request priority not an integer: %r"|%|priority
|self|.priority|=|priority
|assert|callback|or|not|errback,|"Cannot use errback without a callback"
|self|.callback|=|callback
|self|.errback|=|errback
|self|.cookies|=|cookies|or|{}
|self|.headers|=|Headers(headers|or|{}, encoding|=|encoding)
|self|.dont_filter|=|dont_filter
|self|._meta|=|dict|(meta)|if|meta|else|None
|self|.flags|=|[]|if|flags|is|None|else|list|(flags)
|

**4、Scrapy 运行**
格式：scrapy crawl + 爬虫名  –nolog    \# 不显示日志
进入项目 mimvp_proxy_python_scrapy 目录下，运行命令：
|1
|2
|$|cd|mimvp_proxy_python_scrapy/
|$ scrapy crawl mimvp --nolog
|

运行结果：
$scrapy crawl mimvp --nolog
mimvp_url : http://mimvp.com
body : b'<!DOCTYPE html>\n<html lang="zh-CN">\n<head>\n<meta charset="utf-8">\n<meta http-equiv="Content-Type" content="text/html; charset=utf-8">\n<meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">\n<meta http-equiv="Cache-Control" content="no-transform">
..........
**5、Scrapy 查询语法**
当我们爬取大量的网页，如果自己写正则匹配，会很麻烦，也很浪费时间，令人欣慰的是，scrapy内部支持更简单的查询语法，帮助我们去html中查询我们需要的标签和标签内容以及标签属性。
下面逐一进行介绍：
查询子子孙孙中的某个标签(以div标签为例)：//div
查询儿子中的某个标签(以div标签为例)：/div
查询标签中带有某个class属性的标签：//div[@class=’c1′]即子子孙孙中标签是div且class=‘c1’的标签
查询标签中带有某个class=‘c1’并且自定义属性name=‘alex’的标签：//div[@class=’c1′][@name=’alex’]
查询某个标签的文本内容：//div/span/text() 即查询子子孙孙中div下面的span标签中的文本内容
查询某个属性的值（例如查询a标签的href属性）：//a/@href
示例代码：
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|def|parse(|self|, response):
|\# 分析页面
|\# 找到页面中符合规则的内容（校花图片），保存
|\# 找到所有的a标签，再访问其他a标签，一层一层的搞下去
|hxs|=|HtmlXPathSelector(response)|\#创建查询对象
|\# 如果url是 http://www.xiaohuar.com/list-1-\d+.html
|if|re.match(|'http://www.xiaohuar.com/list-1-\d+.html'|, response.url):|\#如果url能够匹配到需要爬取的url，即本站url
|items|=|hxs.select(|'//div[@class="item_list infinite_scroll"]/div'|)|\#select中填写查询目标，按scrapy查询语法书写
|for|i|in|range|(|len|(items)):
|src|=|hxs.select(|'//div[@class="item_list infinite_scroll"]/div[%d]//div[@class="img"]/a/img/@src'|%|i).extract()|\#查询所有img标签的src属性，即获取校花图片地址
|name|=|hxs.select(|'//div[@class="item_list infinite_scroll"]/div[%d]//div[@class="img"]/span/text()'|%|i).extract()|\#获取span的文本内容，即校花姓名
|school|=|hxs.select(|'//div[@class="item_list infinite_scroll"]/div[%d]//div[@class="img"]/div[@class="btns"]/a/text()'|%|i).extract()|\#校花学校
|if|src:
|ab_src|=|"http://www.xiaohuar.com"|+|src[|0|]|\#相对路径拼接
|file_name|=|"%s_%s.jpg"|%|(school[|0|].encode(|'utf-8'|), name[|0|].encode(|'utf-8'|))|\#文件名，因为python27默认编码格式是unicode编码，因此我们需要编码成utf-8
|file_path|=|os.path.join(|"/Users/wupeiqi/PycharmProjects/beauty/pic"|, file_name)
|urllib.urlretrieve(ab_src, file_path)
|

注：urllib.urlretrieve(ab_src, file_path) ，接收文件路径和需要保存的路径，会自动去文件路径下载并保存到我们指定的本地路径。
**6、递归爬取网页**
上述代码仅仅实现了一个url的爬取，如果该url的爬取的内容中包含了其他url，而我们也想对其进行爬取，那么如何实现递归爬取网页呢？
示例代码：
|1
|2
|3
|4
|5
|\# 获取所有的url，继续访问，并在其中寻找相同的url
|all_urls|=|hxs.select(|'//a/@href'|).extract()
|for|url|in|all_urls:
|if|url.startswith(|'http://www.xiaohuar.com/list-1-'|):
|yield|Request(url, callback|=|self|.parse)
|

即通过yield生成器向每一个url发送request请求，并执行返回函数parse，从而递归获取校花图片和校花姓名学校等信息。
注：可以修改settings.py 中的配置文件，以此来指定“递归”的层数，如： DEPTH_LIMIT = 1
**7、Scrapy 设置代理爬取网页**
Python Scrapy 设置代理有两种方式，使用时两种方式选择一种即可
方式1： 直接在代码里设置，如 MimvpSpider ——> start_requests
方式2： 通过 middlewares + settings.py 配置文件设置，步骤：
2.1 middlewares.py 添加代理类 ProxyMiddleware，并添加代理
2.2 settings.py 开启 DOWNLOADER_MIDDLEWARES，并且添加 'mimvp_proxy_python_scrapy.middlewares.ProxyMiddleware': 100,
**方式1：直接在代码里设置**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|31
|32
|33
|34
|35
|36
|37
|38
|39
|40
|41
|42
|43
|44
|45
|46
|47
|48
|49
|50
|51
|52
|53
|\#!/usr/bin/env python
|\# -*- coding:utf-8 -*-
|\#
|\# Python scrapy 支持 http、https
|\#
|\# 米扑代理示例：
|\# http://proxy.mimvp.com/demo2.php
|\#
|\# 米扑代理购买：
|\# http://proxy.mimvp.com
|\#
|\# mimvp.com
|\# 2009.10.1
|\# Python Scrapy 设置代理有两种方式，使用时两种方式选择一种即可
|\# 方式1： 直接在代码里设置，如 MimvpSpider ——> start_requests
|\# 方式2： 通过 middlewares + settings.py 配置文件设置，步骤：
|\#        2.1 middlewares.py 添加代理类 ProxyMiddleware，并添加代理
|\#        2.2 settings.py 开启 DOWNLOADER_MIDDLEWARES，并且添加 'mimvp_proxy_python_scrapy.middlewares.ProxyMiddleware': 100,

|import|scrapy
|class|MimvpSpider(scrapy.spiders.Spider):
|name|=|"mimvp"
|allowed_domains|=|[|"mimvp.com"|]
|start_urls|=|[
|"http://proxy.mimvp.com/exist.php"|,
|"https://proxy.mimvp.com/exist.php"|,
|]
|\#\# 代理设置方式1：直接在代理里设置
|def|start_requests(|self|):
|urls|=|[
|"http://proxy.mimvp.com/exist.php"|,
|"https://proxy.mimvp.com/exist.php"|,
|]
|for|url|in|urls:
|meta_proxy|=|""
|if|url.startswith(|"http://"|):
|meta_proxy|=|"http://180.96.27.12:88"|\# http代理
|elif|url.startswith(|"https://"|):
|meta_proxy|=|"http://109.108.87.136:53281"|\# https代理
|yield|scrapy.Request(url|=|url, callback|=|self|.parse, meta|=|{|'proxy'|: meta_proxy})

|def|parse(|self|, response):
|mimvp_url|=|response.url|\# 爬取时请求的url
|body|=|response.body|\# 返回网页内容
|print|(|"mimvp_url : "|+|str|(mimvp_url))
|print|(|"body : "|+|str|(body))
|

**方式2：配置文件里设置**
a） middlewares.py 文件里，添加代码：
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|\#\# 代理设置方式2： 通过 middlewares + settings.py 配置文件设置
|\#\# mimvp custom by yourself
|class|ProxyMiddleware(|object|):
|def|process_request(|self|,request,spider):
|if|request.url.startswith(|"http://"|):
|request.meta[|'proxy'|]|=|"http://180.96.27.12:88"|\# http代理
|elif|request.url.startswith(|"https://"|):
|request.meta[|'proxy'|]|=|"http://109.108.87.136:53281"|\# https代理
|\#         \# proxy authentication
|\#         proxy_user_pass = "USERNAME:PASSWORD"
|\#         encoded_user_pass = base64.encodestring(proxy_user_pass)
|\#         request.headers['Proxy-Authorization'] = 'Basic ' + encoded_user_pass
|

b） settings.py 文件里，开启设置
|1
|2
|3
|4
|DOWNLOADER_MIDDLEWARES|=|{
|'scrapy.downloadermiddlewares.httpproxy.HttpProxyMiddleware'|:|110|,
|'mimvp_proxy_python_scrapy.middlewares.ProxyMiddleware'|:|100|,
|}
|

以上两种代理设置方式，都验证测试成功，测试代理由米扑代理免费提供，感谢！
本示例采用的米扑代理，支持 http、https、socks4、socks5等多种协议，覆盖全球120多个国家，中国34个省市
推荐米扑代理：[http://proxy.mimvp.com](http://proxy.mimvp.com/)



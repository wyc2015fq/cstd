
# Python pyspider 安装与开发 - 阳光岛主 - CSDN博客

2017年08月15日 23:31:57[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：3930


**PySpider 简介**
PySpider是一个国人编写的强大的网络爬虫系统并带有强大的WebUI。采用Python语言编写，分布式架构，支持多种数据库后端，强大的WebUI支持脚本编辑器、任务监视器，项目管理器以及结果查看器。
PySpider 来源于以前做的一个垂直搜索引擎使用的爬虫后端。我们需要从200个站点（由于站点失效，不是都同时啦，同时有100+在跑吧）采集数据，并要求在5分钟内将对方网站的更新更新到库中。所以，灵活的抓取控制是必须的。
同时，由于100个站点，每天都可能会有站点失效或者改版，所以需要能够监控模板失效，以及查看抓取状态。
为了达到5分钟更新，我们使用抓取最近更新页上面的最后更新时间，以此来判断页面是否需要再次抓取。
可见，这个项目对于爬虫的监控和调度要求是非常高的。
PySpider 中文网：[http://www.pyspider.cn](http://www.pyspider.cn/)
PySpider 官网：[http://docs.pyspider.org](http://docs.pyspider.org/)
PySpider 演示：[http://demo.pyspider.org](http://demo.pyspider.org/)
PySpider 源码：[https://github.com/binux/pyspider](https://github.com/binux/pyspider)
**PySpider 特性**
python 脚本控制，可以用任何你喜欢的html解析包（内置 pyquery）
WEB 界面编写调试脚本、起停脚本、监控执行状态，查看活动历史，获取结果产出
数据存储支持MySQL、MongoDB、Redis、SQLite、 ElasticSearch; PostgreSQL 及 SQLAlchemy
队列服务支持RabbitMQ、Beanstalk、Redis、Kombu
支持抓取 JavaScript 的页面
组件可替换，支持单机/分布式部署，支持 Docker 部署
强大的调度控制，支持超时重爬及优先级设置
支持Python 2.{6, 7}, 3.{3, 4, 5, 6}
**PySpider 安装**
**1） pip 安装**
pip可以在以下版本的CPython下运行：2.6, 2.7, 3.1, 3.2, 3.3, 3.4 和 pypy.
pip可以在Unix/Linux、Mac OS X、Windows系统中运行.
a）脚本安装
python get-pip.py
如果 setuptools (或 distribute) 未安装，`get-pip.py`会 自动为你安装 setuptools
如果需要升级 setuptools (或 distribute)，运行`pip install -U setuptools`
b）命令安装
sudo apt-get install python-pip// Debian、Ubuntu
sudo yum install python-pip// CentOS、Redhat、Fedora
**2）PhantomJS 安装**
PhantomJS 是一个基于 WebKit 的服务器端 JavaScript API。它全面支持web而不需浏览器支持，其快速、原生支持各种Web标准：DOM 处理、CSS 选择器、JSON、Canvas 和 SVG。 PhantomJS 可以用于页面自动化、网络监测、网页截屏以及无界面测试等。支持Windows、Linux、Mac OS X等多操作系统。
PhantomJS 下载：[http://phantomjs.org/download.html](http://phantomjs.org/download.html)
PhantomJS不需要安装，解压后，配置环境变量后，便可直接使用，详见[PhantomJS 安装与开发](http://blog.mimvp.com/2017/10/phantomjs-an-zhuang-yu-kai-fa/)
PhantomJS 安装命令：
sudo apt-get install phantomjs// Debian、Ubuntu
sudo pkg install phantomjs// FreeBSD
brew install phantomjs// Mac OS X
**3）PySpider 安装**
PySpider 安装的依赖包[requirements.txt](https://github.com/binux/pyspider/blob/master/requirements.txt)
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
|Flask>|=|0.10
|Jinja2>|=|2.7
|chardet>|=|2.2
|cssselect>|=|0.9
|lxml
|pycurl
|pyquery
|requests>|=|2.2
|tornado>|=|3.2
|mysql|-|connector|-|python>|=|1.2|.|2
|pika>|=|0.9|.|14
|pymongo>|=|2.7|.|2
|unittest2>|=|0.5|.|1
|Flask|-|Login>|=|0.2|.|11
|u|-|msgpack|-|python>|=|1.6
|click>|=|3.3
|SQLAlchemy>|=|0.9|.|7
|six>|=|1.5|.|0
|amqp>|=|1.3|.|0|,<|2.0
|redis
|redis|-|py|-|cluster
|kombu
|psycopg2
|elasticsearch
|tblib
|

PySpider 安装命令：
pip install pyspider
Ubuntu 用户，请提前安装好以下支持类库：
sudo apt-get install python python-dev python-distribute python-pip libcurl4-openssl-dev libxml2-dev libxslt1-dev python-lxml
**4）验证安装成功**
控制台输入命令：
pyspider all
使用浏览器访问[http://localhost:5000](http://localhost:5000/)
![python-pyspider-an-zhuang-yu-kai-fa-01](http://blog.mimvp.com/wp-content/uploads/2017/08/python-pyspider-an-zhuang-yu-kai-fa-01.png)
正常出现 PySpider 的页面，那证明一切 OK
**PySpider 示例**
**1）示例1：**爬取米扑科技首页（[mimvp.com](http://mimvp.com/)）
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
|\# -*- encoding: utf-8 -*-
|\# Created on 2017-07-28 13:44:53
|\# Project: pyspiderdemo
|\# mimvp.com
|from|pyspider.libs.base_handler|import|*

|class|Handler(BaseHandler):
|crawl_config|=|{
|}
|@every|(minutes|=|24|*|60|)
|def|on_start(|self|):
|self|.crawl(|'mimvp.com'|, callback|=|self|.index_page)
|@config|(age|=|10|*|24|*|60|*|60|)
|def|index_page(|self|, response):
|for|each|in|response.doc(|'a[href^="http"]'|).items():
|self|.crawl(each.attr.href, callback|=|self|.detail_page)
|@config|(priority|=|2|)
|def|detail_page(|self|, response):
|return|{
|"url"|: response.url,
|"title"|: response.doc(|'title'|).text(),
|}
|

运行结果：
![python-pyspider-an-zhuang-yu-kai-fa-02](http://blog.mimvp.com/wp-content/uploads/2017/08/python-pyspider-an-zhuang-yu-kai-fa-02.png)
**2）示例2：设置代理爬取网页**
PySpider 支持使用代理爬取网页，其使用代理有两种方式：
方式1：
--phantomjs-proxy TEXT          phantomjs proxy ip:port
启动命令例如：
pyspider --phantomjs-proxy "188.226.141.217:8080" all
方式2：
设置代理全局变量，如下图：
crawl_config = {
'proxy' : '188.226.141.217:8080'
}
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
|29
|30
|31
|\#!/usr/bin/env python
|\# -*- encoding: utf-8 -*-
|\# Created on 2017-07-28 14:13:14
|\# Project: mimvp_proxy_pyspider
|\#
|\# mimvp.com
|from|pyspider.libs.base_handler|import|*

|class|Handler(BaseHandler):
|crawl_config|=|{
|'proxy'|:|'http://188.226.141.217:8080'|,|\# http
|'proxy'|:|'https://182.253.32.65:3128'|\# https
|}
|@every|(minutes|=|24|*|60|)
|def|on_start(|self|):
|self|.crawl(|'http://proxy.mimvp.com/exist.php'|, callback|=|self|.index_page)
|@config|(age|=|10|*|24|*|60|*|60|)
|def|index_page(|self|, response):
|for|each|in|response.doc(|'a[href^="http"]'|).items():
|self|.crawl(each.attr.href, callback|=|self|.detail_page)
|@config|(priority|=|2|)
|def|detail_page(|self|, response):
|return|{
|"url"|: response.url,
|"title"|: response.doc(|'title'|).text(),
|}
|

运行结果：
![python-pyspider-an-zhuang-yu-kai-fa-03](http://blog.mimvp.com/wp-content/uploads/2017/08/python-pyspider-an-zhuang-yu-kai-fa-03.png)



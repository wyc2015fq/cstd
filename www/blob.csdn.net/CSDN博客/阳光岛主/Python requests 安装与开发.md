
# Python requests 安装与开发 - 阳光岛主 - CSDN博客

2017年08月16日 11:06:34[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：1041


Requests 是用Python语言编写HTTP客户端库，跟urllib、urllib2类似，基于 urllib，但比 urllib 更加方便，可以节约我们大量的工作，完全满足 HTTP 测试需求，编写爬虫和测试服务器响应数据时经常会用到。
Requests 的哲学是以 PEP 20 的习语为中心开发的，所以它比 urllib 更加 Pythoner，更重要的一点是它支持 Python3
Beautiful is better than ugly. (美丽优于丑陋)
Explicit is better than implicit. (清楚优于含糊)
Simple is better than complex. (简单优于复杂)
Complex is better than complicated. (复杂优于繁琐)
Readability counts. (重要的是可读性)
**Requests 官网**：
[https://pypi.python.org/pypi/requests](https://pypi.python.org/pypi/requests)
**安装 Requests**
方式1）pip 安装
pip install requests      \# python2.7
pip3 install requests    \# python3.6
方式2）源码安装
下载[requests-2.18.2.tar.gz](https://pypi.python.org/packages/07/2e/81fdfdfac91cf3cb2518fb149ac67caf0e081b485eab68e9aee63396f7e8/requests-2.18.2.tar.gz#md5=49bd9924d3be341871bc922cde6f372e)
解压安装：
tar zxvf requests-2.18.2.tar.gz
cd requests
python setup.py install
验证安装：
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
|>>>|import|requests
|>>> requests
|<module|'requests'|from|'/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/site-packages/requests/__init__.pyc'|>
|

**使用 Requests**
requests 封装了Python的urlib和urllib2，所以爬取网页非常简洁
**1. 爬取网页**
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
|import|requests
|\# 爬取米扑科技首页
|req|=|requests.get(url|=|'http://mimvp.com'|)
|print|(|"status_code : "|+|str|(req.status_code))
|print|(|"mimvp text : "|+|req.text)
|\# 爬取米扑代理（含请求参数）
|req|=|requests.get(url|=|'http://proxy.mimvp.com/free.php'|, params|=|{|'proxy'|:|'out_tp'|,|'sort'|:|'p_ping'|})
|print|(|"status_code : "|+|str|(req.status_code))
|print|(|"mimvp text : "|+|req.text)
|

爬取网页非常简洁吧，Python urllib 三行代码，requests只需要一行代码搞定
urllib ：urllib.urlopen('http://mimvp.com').read()
urllib2：urllib2.urlopen('http://mimvp.com').read()
或者
|1
|2
|3
|4
|import|urllib2
|req|=|urllib2.Request(|'http://mimvp.com'|)
|res|=|urllib2.urlopen(req)
|page|=|res.read()
|

**requests 接口格式：**
|1
|2
|3
|4
|5
|6
|requests.get(|'https://mimvp.com/timeline.json'|)|\# GET请求
|requests.post(|'http://mimvp.com/post'|)|\# POST请求
|requests.put(|'http://mimvp.com/put'|)|\# PUT请求
|requests.delete(|'http://mimvp.com/delete'|)|\# DELETE请求
|requests.head(|'http://mimvp.com/get'|)|\# HEAD请求
|requests.options(|'http://mimvp.com/get'|)|\# OPTIONS请求
|

**requests 接口示例：**
|1
|2
|3
|import|requests
|requests.get(|'http://mimvp.com'|, params|=|{|'love'|:|'mimvp'|})|\# GET参数实例
|requests.post(|'http://mimvp.com'|, data|=|{|'love'|:|'mimvp'|})|\# POST参数实例
|

**Requests 设置代理**
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
|54
|55
|56
|57
|58
|59
|60
|61
|62
|63
|\#!/usr/bin/env python
|\# -*- encoding: utf-8 -*-
|\#
|\# Python requests 支持 http、https、socks4、socks5
|\#
|\# 米扑代理示例：
|\# http://proxy.mimvp.com/demo2.php
|\#
|\# 米扑代理购买：
|\# http://proxy.mimvp.com
|\#
|\# mimvp.com
|\# 2016-09-16

|import|requests
|import|ssl
|import|socks, socket|\# 需要引入socks.py文件，请到米扑代理下载

|mimvp_url|=|"http://proxy.mimvp.com/exist.php"
|mimvp_url2|=|"https://proxy.mimvp.com/exist.php"
|mimvp_url3|=|"https://apps.bdimg.com/libs/jquery-i18n/1.1.1/jquery.i18n.min.js"

|\# 使用代理 http, https
|proxies|=|{
|"http"|:|"http://120.77.155.249:8888"|,
|"https"|:|"http://54.255.211.38:80"|,
|}
|req|=|requests.get(mimvp_url2, proxies|=|proxies, timeout|=|30|, verify|=|False|)
|print|(|"mimvp text : "|+|req.text)

|\# 使用代理 socks4
|proxies|=|{
|'socks4'|:|'163.121.188.2:4000'|,
|}
|socks4_ip|=|proxies[|'socks4'|].split(|":"|)[|0|]
|socks4_port|=|int|(proxies[|'socks4'|].split(|":"|)[|1|])
|socks.setdefaultproxy(socks.PROXY_TYPE_SOCKS4, socks4_ip, socks4_port)
|socket.socket|=|socks.socksocket
|req|=|requests.get(mimvp_url2, timeout|=|30|, verify|=|False|)
|print|(|"mimvp text : "|+|req.text)

|\# 使用代理 socks5
|proxies|=|{
|'socks5'|:|'190.9.58.211:45454'|,
|}
|socks5_ip|=|proxies[|'socks5'|].split(|":"|)[|0|]
|socks5_port|=|int|(proxies[|'socks5'|].split(|":"|)[|1|])
|socks.setdefaultproxy(socks.PROXY_TYPE_SOCKS5, socks5_ip, socks5_port)
|socket.socket|=|socks.socksocket
|req|=|requests.get(mimvp_url2, timeout|=|30|, verify|=|False|)
|print|(|"mimvp text : "|+|req.text)
|

本示例采用的米扑代理，支持 http、https、socks4、socks5等多种协议，覆盖全球120多个国家，中国34个省市
推荐米扑代理：[http://proxy.mimvp.com](http://proxy.mimvp.com/)

**参考推荐：**
[Python pyspider 安装与开发](http://blog.mimvp.com/2017/08/python-pyspider-an-zhuang-yu-kai-fa/)
[Python3 urllib 用法详解](http://blog.mimvp.com/2017/08/python3-urllib-yong-fa-xiang-jie/)
[PhantomJS 安装与开发](http://blog.mimvp.com/2017/10/phantomjs-an-zhuang-yu-kai-fa/)
[Node.js 安装与开发](http://blog.mimvp.com/2017/10/node-js-an-zhuang-yu-kai-fa/)
[Node.js SuperAgent 安装与开发](http://blog.mimvp.com/2017/10/nodejs-superagent-an-zhuang-yu-kai-fa/)



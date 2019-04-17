# Requests 库的基本使用 - Alex_McAvoy的博客 - CSDN博客





2018年08月16日 16:01:52[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：917








# 【概述】

Requests 库是用 Python 语言基于 urllib 编写的，采用的是 Apache2 Licensed 开源协议的 HTTP 库，可自动爬取 HTML 页面，自动网络请求提交，是网络数据爬取和网页解析的基本库，常用于网络爬虫与信息提取。

安装 Python 后，是没有安装 requests 模块的，需要单独通过 pip 进行安装。

HTTP 协议简介：[点击这里](https://blog.csdn.net/u011815404/article/details/81748842)

# 【Response 对象】

Response 对象包含服务器返回的所有信息，也包含请求的 Request 信息，其有5个属性：
|属性|说明|
|----|----|
|r.status_code|HTTP 请求的返回状态|
|r.text|HTTP 响应内容的字符串形式，即：URL 对应的页面内容|
|r.encoding|从 HTTP header 中猜测的响应内容编码方式|
|r.apparent_encoding|从内容中分析出的响应内容编码方式（备选编码方式）|
|r.content|HTTP 响应内容的二进制形式|

# 【连接异常】

requests 库支持六种常见的连接异常：
|异常|说明|
|----|----|
|requests.ConnectionError|网络连接错误异常，如 DNS 查询失败、拒绝连接等|
|requests.HTTPError|HTTP 错误异常|
|requests.URLRequired|URL 缺失异常|
|requests.TooManyRedirects|超过最大重定向次数，产生重定向异常|
|requests.ConnectTimeout|连接远程服务器超时异常|
|requests.Timeout|请求 URL 超时，产生超时异常|

# 【requests 库方法】

requests 库一共有7个主要方法，除最基础的 request() 方法外，其余6个与HTTP协议的方法的功能一一对应。
|方法|描述|与 HTTP 对应方法|
|----|----|----|
|requests.request()|构造一个请求，支撑以下各方法的基础方法|无|
|requests.get()|获取 HTML 网页的主要方法|GET|
|requests.head()|获取 HTML 网页头信息的方法|HEAD|
|requests.post()|向 HTML 网页提交 POST 请求的方法|POST|
|requests.put()|向 HTML 网页提交 PUT 请求的方法|PUT|
|requests.patch()|向 HTML 网页提交局部修改请求|PATCH|
|requests.delete()|向 HTML 网页提交删除请求|DELETE|

## 1.request() 方法

request() 方法是所有方法的基础方法，其余的所有方法都是基于 request() 方法来封装的

格式：**requests.request(method, url, **kwargs)**

method : 请求方式，不同的方式对应不同的 HTTP 请求功能

url : 拟获取页面的url链接

**kwargs: 控制访问的参数，共13个

### 1）请求方式

一共有 7 个请求方式，为：GET/HEAD/POST/PUT/PATCH/delete/OPTIONS，分别对应 HTTP 中的请求功能。

如果想发送一个请求，除了改变 request() 方法中的请求方式外，还可以直接使用 request 库中对应的方法。

例如：r=r.request(GET,"http://www.baidu.com") 与 r=r.get("http://www.baidu.com") 等价。

### 2）访问控制参数

访问控制参数一共有13个，其是可选的，使用时，需要用命名方法来调用，例如：params=kv（kv 为事先定义好的一个字典）
|params|字典或字节序列，作为参数增加到 url 中|
|----|----|
|data|字典、字节序列或文件对象，作为 Request 的内容|
|json|JSON 格式的数据，作为 Request 的内容|
|headers|字典，可以用其来控制访问链接的 HTTP 头|
|cookies|字典或 CookieJar，Request 中的 cookie|
|auth|元组，支持 HTTP 认证功能|
|files|字典类型，向服务器传输文件时使用的字段|
|timeout|设定超时时间，以秒为单位，如果设定时间内未返回，将产生一个 TimeoutError 异常|
|proxies|字典类型，设定访问代理服务器，还可增加登录认证，能有效的隐藏用户爬取网页的源 IP，从而防止对爬虫的逆追踪|
|allow_redirects|重定向开关，True/False，默认为True|
|stream|获取内容立即下载开关，默认为True|
|verify|认证 SSL 证书开关，默认为True|
|cert|保存本地 SSL 证书路径的字段|

## 2.get() 方法

1）作用：通过给定 url 来构造一个向 HTTP 请求资源的 Request 对象

2）返回值：返回一个包含 HTTP 资源的 Response 对象，其包含从 HTTP 返回的所有相关资源

3）格式：**r = requests.get(url, **kwargs)**

## 3.head() 方法

1）作用：通过给定 url 来构造一个向 HTTP 请求资源的 Request 对象

2）返回值：返回一个包含 HTTP 资源的 Response 对象，其包含从 HTTP 返回的所有头部资源

3）格式：**requests.head(url, **kwargs)**

## 4.post() 方法

1）作用：向 url 提交新增数据，格式为字典，自动编码为 form(表单)

2）格式：**requests.post(url, **kwargs)**

## 5.put() 方法

1）作用：向 url 提交新增数据，格式为字符串，自动编码为 data

2）格式：**requests.put(url,, **kwargs)**

## 6.patch() 方法

1）作用：向 url 提交局部修改请求

2）格式：**requests.patch(url,, **kwargs)**

## 7.delete() 方法

1）作用：向 url 提交删除请求

2）格式：**requests.delete(url, **kwargs)**

# 【编码】

网络上的资源均有其特定的编码，如果没有编码，将无法通过有效的解析方式使得人类可读，因此我们需要通过编码来进行解析

r.encoding 是通过对 header 进行分析，如果其中不存在 charset 字段，则认为编码为 ISO‐8859‐1

r.apparent_encoding 根据网页内容而分析出的编码方式，其解析方式要比 r.encoding  更为准确，可看作是 r.encoding 的备选

r.text 则是根据 r.encoding 来显示网页内容

# 【get() 方法获取网络资源基本流程】

![](https://img-blog.csdn.net/20180815204301311?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```python
>>> import requests
>>> r=requests.get("http://www.baidu.com")
>>> r.status_code
200
>>> r.text
'<!DOCTYPE html>\r\n<!--STATUS OK--><html> <head><meta http-equiv=content-type content=text/html;charset=utf-8><meta http-equiv=X-UA-Compatible content=IE=Edge><meta content=always name=referrer><link rel=stylesheet type=text/css href=http://s1.bdstatic.com/r/www/cache/bdorz/baidu.min.css><title>ç\x99¾åº¦ä¸\x80ä¸\x8bï¼\x8cä½\xa0å°±ç\x9f¥é\x81\x93</title></head> <body link=#0000cc> <div id=wrapper> <div id=head> <div class=head_wrapper> <div class=s_form> <div class=s_form_wrapper> <div id=lg> <img hidefocus=true src=//www.baidu.com/img/bd_logo1.png width=270 height=129> </div> <form id=form name=f action=//www.baidu.com/s class=fm> <input type=hidden name=bdorz_come value=1> <input type=hidden name=ie value=utf-8> <input type=hidden name=f value=8> <input type=hidden name=rsv_bp value=1> <input type=hidden name=rsv_idx value=1> <input type=hidden name=tn value=baidu><span class="bg s_ipt_wr"><input id=kw name=wd class=s_ipt value maxlength=255 autocomplete=off autofocus></span><span class="bg s_btn_wr"><input type=submit id=su value=ç\x99¾åº¦ä¸\x80ä¸\x8b class="bg s_btn"></span> </form> </div> </div> <div id=u1> <a href=http://news.baidu.com name=tj_trnews class=mnav>æ\x96°é\x97»</a> <a href=http://www.hao123.com name=tj_trhao123 class=mnav>hao123</a> <a href=http://map.baidu.com name=tj_trmap class=mnav>å\x9c°å\x9b¾</a> <a href=http://v.baidu.com name=tj_trvideo class=mnav>è§\x86é¢\x91</a> <a href=http://tieba.baidu.com name=tj_trtieba class=mnav>è´´å\x90§</a> <noscript> <a href=http://www.baidu.com/bdorz/login.gif?login&tpl=mn&u=http%3A%2F%2Fwww.baidu.com%2f%3fbdorz_come%3d1 name=tj_login class=lb>ç\x99»å½\x95</a> </noscript> <script>document.write(\'<a href="http://www.baidu.com/bdorz/login.gif?login&tpl=mn&u=\'+ encodeURIComponent(window.location.href+ (window.location.search === "" ? "?" : "&")+ "bdorz_come=1")+ \'" name="tj_login" class="lb">ç\x99»å½\x95</a>\');</script> <a href=//www.baidu.com/more/ name=tj_briicon class=bri style="display: block;">æ\x9b´å¤\x9aäº§å\x93\x81</a> </div> </div> </div> <div id=ftCon> <div id=ftConw> <p id=lh> <a href=http://home.baidu.com>å\x85³äº\x8eç\x99¾åº¦</a> <a href=http://ir.baidu.com>About Baidu</a> </p> <p id=cp>©2017 Baidu <a href=http://www.baidu.com/duty/>ä½¿ç\x94¨ç\x99¾åº¦å\x89\x8då¿\x85è¯»</a>  <a href=http://jianyi.baidu.com/ class=cp-feedback>æ\x84\x8fè§\x81å\x8f\x8dé¦\x88</a> äº¬ICPè¯\x81030173å\x8f·  <img src=//www.baidu.com/img/gs.gif> </p> </div> </div> </div> </body> </html>\r\n'
>>> r.encoding
'ISO-8859-1'
>>> r.apparent_encoding
'utf-8'
>>> r.encoding="utf-8"
>>> r.text
'<!DOCTYPE html>\r\n<!--STATUS OK--><html> <head><meta http-equiv=content-type content=text/html;charset=utf-8><meta http-equiv=X-UA-Compatible content=IE=Edge><meta content=always name=referrer><link rel=stylesheet type=text/css href=http://s1.bdstatic.com/r/www/cache/bdorz/baidu.min.css><title>百度一下，你就知道</title></head> <body link=#0000cc> <div id=wrapper> <div id=head> <div class=head_wrapper> <div class=s_form> <div class=s_form_wrapper> <div id=lg> <img hidefocus=true src=//www.baidu.com/img/bd_logo1.png width=270 height=129> </div> <form id=form name=f action=//www.baidu.com/s class=fm> <input type=hidden name=bdorz_come value=1> <input type=hidden name=ie value=utf-8> <input type=hidden name=f value=8> <input type=hidden name=rsv_bp value=1> <input type=hidden name=rsv_idx value=1> <input type=hidden name=tn value=baidu><span class="bg s_ipt_wr"><input id=kw name=wd class=s_ipt value maxlength=255 autocomplete=off autofocus></span><span class="bg s_btn_wr"><input type=submit id=su value=百度一下 class="bg s_btn"></span> </form> </div> </div> <div id=u1> <a href=http://news.baidu.com name=tj_trnews class=mnav>新闻</a> <a href=http://www.hao123.com name=tj_trhao123 class=mnav>hao123</a> <a href=http://map.baidu.com name=tj_trmap class=mnav>地图</a> <a href=http://v.baidu.com name=tj_trvideo class=mnav>视频</a> <a href=http://tieba.baidu.com name=tj_trtieba class=mnav>贴吧</a> <noscript> <a href=http://www.baidu.com/bdorz/login.gif?login&tpl=mn&u=http%3A%2F%2Fwww.baidu.com%2f%3fbdorz_come%3d1 name=tj_login class=lb>登录</a> </noscript> <script>document.write(\'<a href="http://www.baidu.com/bdorz/login.gif?login&tpl=mn&u=\'+ encodeURIComponent(window.location.href+ (window.location.search === "" ? "?" : "&")+ "bdorz_come=1")+ \'" name="tj_login" class="lb">登录</a>\');</script> <a href=//www.baidu.com/more/ name=tj_briicon class=bri style="display: block;">更多产品</a> </div> </div> </div> <div id=ftCon> <div id=ftConw> <p id=lh> <a href=http://home.baidu.com>关于百度</a> <a href=http://ir.baidu.com>About Baidu</a> </p> <p id=cp>©2017 Baidu <a href=http://www.baidu.com/duty/>使用百度前必读</a>  <a href=http://jianyi.baidu.com/ class=cp-feedback>意见反馈</a> 京ICP证030173号  <img src=//www.baidu.com/img/gs.gif> </p> </div> </div> </div> </body> </html>\r\n'
```

# 【应用】

## 1.通用爬取框架

通用爬取框架，实质上就是一组代码，其最大的作用是使用户更有效、稳定、可靠的爬取网页上的内容。

在使用 requests 库进行网页访问时，经常使用 get() 函数来获得 URL 的相关内容，但这样的内容不是一定成立的，因为网络连接有一定的异常，此时，异常处理就极为重要。

Response 对象提供了一个与处理异常的方法：**r.raise_for_status()**

其功能是判断返回的 Response 类型是不是 200，如果不是 200，将产生异常 requests.HTTPError

因此有了如下的通用代码框架

```python
import requests

def getHTMLText(url):
    try:
        r=requests.get(url,timeout=30)
        r.raise_for_status()#如果状态不是200，则会引发HTTPError异常
        r.encoding=r.apparent_encoding
        return r.text
    except:
        return "产生异常"

if __name__=="__main__":
    url="http://www.baidu.com"
    print(getHTMLText(url))
```

## 2.爬取图片

网络图片链接的格式均为：http://www.xxx.com/picture.jpg

我们可以通过读写文件的方式，通过图片的 url 来获取图片并保存在主机中

```python
import requests

def getHTMLPictures(url,path):
    try:
        r=requests.get(url)
        r.raise_for_status()
        with open(path,'wb') as f:
            f.write(r.content)
            f.close()
        return "保存成功"
    except:
        return "产生异常，爬取失败" 

if __name__=="__main__":
    path="C:/Users/93177/Desktop/test.jpg"
    url="https://timgsa.baidu.com/timg?image&quality=80&size=b9999_10000&sec=1534518539577&di=f2a7ae12fb26ffda29d9f9902473739c&imgtype=0&src=http%3A%2F%2Fi0.hdslb.com%2Fbfs%2Farchive%2F59945cbdf8b97168fec9f8c1380ca8b63ee35751.jpg"
    print(getHTMLPictures(url,path))
```






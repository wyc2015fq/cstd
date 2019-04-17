# 1.5 网页下载器与urllib2模块（核心） - 博客堂 - CSDN博客





2017年05月02日 15:53:01[最小森林](https://me.csdn.net/u012052268)阅读数：640








# 网页下载器

![image](http://img.mukewang.com/58b3ba210001a9b612800720.jpg)

网页下载器：将URL对应的网页以HTML下载到本地，用于后续分析

常见网页下载器： 

1. Python官方基础模块：urllib2 

2. 第三方功能包：requests
# urllib2模块

## urllib2下载网页的方法

3种方法，一种比一种强大，后一种是在前一种的扩展： 

1. 基本方法：直接URL 

2. 添加data、http header 

3. 添加特殊情景
3种方法的详细介绍

### 1. 基本方法：直接URL

示意图： 
![image](http://img.mukewang.com/58c3603000018f4612800720.jpg)
代码： 
![image](http://img.mukewang.com/58b3ba6d0001fd7712800720.jpg)
### 2. 添加data、http header

示意图： 
![image](http://img.mukewang.com/58aae10f00017c4a12800720.jpg)
代码： 
![image](http://img.mukewang.com/58b3bac50001d90612800720.jpg)

1. 数据项a的值为1 ：可以写用户名密码啥的。 

2. 伪装爬虫为Mozilla浏览器。
### 3. 添加特殊情景

示意图： 
![image](http://img.mukewang.com/58abe809000197f612800720.jpg)
代码： 
![image](http://img.mukewang.com/58b3bb650001965a12800720.jpg)- HTTPCookie用户登录情景
- Proxy代理信息
- HTTPS加密信息
- Readirect防止URL互相跳转

## urllib2下载的实例

```python
import urllib.request
import http.cookiejar

url = "http://www.baidu.com"
print('第一种方法：')
response1 = urllib.request.urlopen(url)
print(response1.getcode()) # 200
print(len(response1.read()))

print('第二种方法')
request = urllib.request.Request(url)
request.add_header("user-agent", "Mozilla/5.0")
response2 = urllib.request.urlopen(request)
print(response2.getcode()) # 200
print(len(response2.read()))


print('第三种方法')
cj = http.cookiejar.CookieJar()
opener = urllib.request.build_opener(urllib.request.HTTPCookieProcessor(cj))
urllib.request.install_opener(opener)
response3 =urllib.request.urlopen(url)
print(response3.getcode())
print(cj)
print(response3.read())
```

**上面的教程是PYthon2的有些方法已经不能使用。**

**下面是Python3的使用方法:**

## 1.基本方法
`urllib.request.urlopen(url, data=None, [timeout, ]*, cafile=None, capath=None, cadefault=False, context=None)`- url:  需要打开的网址
- data：Post提交的数据
- timeout：设置网站的访问超时时间

直接用urllib.request模块的urlopen（）获取页面，page的数据格式为bytes类型，需要decode（）解码，转换成str类型。

```python
from urllib import request

response = request.urlopen(r'http://python.org/') # <http.client.HTTPResponse object at 0x00000000048BC908> HTTPResponse类型
page = response.read()
page = page.decode('utf-8')
```

urlopen返回对象提供方法： 

- read() , readline() ,readlines() , fileno() , close() ：对HTTPResponse类型数据进行操作 

- info()：返回HTTPMessage对象，表示远程服务器返回的头信息 

- getcode()：返回Http状态码。如果是http请求，200请求成功完成;404网址未找到 

- geturl()：返回请求的url
## 2.使用Request
`urllib.request.Request(url, data=None, headers={}, method=None)`
使用request（）来包装请求，再通过urlopen（）获取页面。

```python
url = r'http://www.lagou.com/zhaopin/Python/?labelWords=label'
headers = {
    'User-Agent': r'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) '
                  r'Chrome/45.0.2454.85 Safari/537.36 115Browser/6.0.3',
    'Referer': r'http://www.lagou.com/zhaopin/Python/?labelWords=label',
    'Connection': 'keep-alive'
}
req = request.Request(url, headers=headers)
page = request.urlopen(req).read()
page = page.decode('utf-8')
```

用来包装头部的数据： 

- User-Agent ：这个头部可以携带如下几条信息：浏览器名和版本号、操作系统名和版本号、默认语言 

- Referer：可以用来防止盗链，有一些网站图片显示来源http://***.com，就是检查Referer来鉴定的 

- Connection：表示连接状态，记录Session的状态。
## 3.Post数据
`urllib.request.urlopen(url, data=None, [timeout, ]*, cafile=None, capath=None, cadefault=False, context=None)`
urlopen（）的data参数默认为None，当data参数不为空的时候，urlopen（）提交方式为Post。

```python
from urllib import request, parse
url = r'http://www.lagou.com/jobs/positionAjax.json?'
headers = {
    'User-Agent': r'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) '
                  r'Chrome/45.0.2454.85 Safari/537.36 115Browser/6.0.3',
    'Referer': r'http://www.lagou.com/zhaopin/Python/?labelWords=label',
    'Connection': 'keep-alive'
}
data = {
    'first': 'true',
    'pn': 1,
    'kd': 'Python'
}
data = parse.urlencode(data).encode('utf-8')
req = request.Request(url, headers=headers, data=data)
page = request.urlopen(req).read()
page = page.decode('utf-8')
```
`urllib.parse.urlencode(query, doseq=False, safe='', encoding=None, errors=None)`
urlencode（）主要作用就是将url附上要提交的数据。 

```
data = {
    'first': 'true',
    'pn': 1,
    'kd': 'Python'
}
data = parse.urlencode(data).encode('utf-8')
```

经过urlencode（）转换后的data数据为?first=true?pn=1?kd=Python，最后提交的url为
`http://www.lagou.com/jobs/positionAjax.json?first=true?pn=1?kd=Python`
Post的数据必须是bytes或者iterable of bytes，不能是str，因此需要进行encode（）编码
` page = request.urlopen(req, data=data).read()`
当然，也可以把data的数据封装在urlopen（）参数中

## 4.异常处理

```python
def get_page(url):
    headers = {
        'User-Agent': r'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) '
                    r'Chrome/45.0.2454.85 Safari/537.36 115Browser/6.0.3',
        'Referer': r'http://www.lagou.com/zhaopin/Python/?labelWords=label',
        'Connection': 'keep-alive'
    }
    data = {
        'first': 'true',
        'pn': 1,
        'kd': 'Python'
    }
    data = parse.urlencode(data).encode('utf-8')
    req = request.Request(url, headers=headers)
    try:
        page = request.urlopen(req, data=data).read()
        page = page.decode('utf-8')
    except error.HTTPError as e:
        print(e.code())
        print(e.read().decode('utf-8'))
    return page
```

## 5、使用代理
`urllib.request.ProxyHandler(proxies=None)`
当需要抓取的网站设置了访问限制，这时就需要用到代理来抓取数据。

```
data = {
        'first': 'true',
        'pn': 1,
        'kd': 'Python'
    }
proxy = request.ProxyHandler({'http': '5.22.195.215:80'})  # 设置proxy
opener = request.build_opener(proxy)  # 挂载opener
request.install_opener(opener)  # 安装opener
data = parse.urlencode(data).encode('utf-8')
page = opener.open(url, data).read()
page = page.decode('utf-8')
return page
```














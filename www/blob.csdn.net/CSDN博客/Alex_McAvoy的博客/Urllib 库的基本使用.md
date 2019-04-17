# Urllib 库的基本使用 - Alex_McAvoy的博客 - CSDN博客





2018年08月20日 22:00:49[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：64








# 【概述】

Urllib 库是 Python 内置的 HTTP 请求库，无需安装，可直接使用，其包含以下模块：
- 请求模块：urllib.request
- 异常处理模块：urllib.error
- URL 解析模块：urllib.parse
- robots.txt 解析模块：urllib.robotparser

爬虫所需要的功能，基本上在 urllib 库中都能找到

# 【请求模块】

## 1.urlopen 语法

urllib.request.urlopen() 函数原型：

```python
urllib.request.urlopen(url, data=None, [timeout, ]*, cafile=None, capath=None, cadefault=False, context=None)
```

urlopen 有三个常用参数，因此一般写为：

```python
urllib.requeset.urlopen(url,data,timeout)
```

### 1）url 参数的使用

当没有 data 参数时，相当于一次 GET 请求

实例：

```python
# request:GET
import urllib.request
response = urllib.request.urlopen('http://www.baidu.com')
print(response.read().decode('utf-8'))
```

### 2）data 参数的使用

当有 data 参数时，相当于一次 POST 请求

实例：

```python
# request: POST
import urllib.parse
import urllib.request

data = bytes(urllib.parse.urlencode({'word': 'hello'}), encoding='utf8')
print(data)

response = urllib.request.urlopen('http://httpbin.org/post', data=data)
print(response.read())
```

这里用到了 urllib.parse，通过 bytes(urllib.parse.urlencode()) 将 POST 数据进行转换放到 urllib.request.urlopen 的 data 参数中，这样就完成了一次 POST 请求。

### 3）timeout 参数的使用

在某些网络情况不好或者服务器端异常时，会出现请求慢或请求异常的情况，此时需要给请求设置一个超时时间，而不是让程序一直在等待结果。

实例：

```python
# 超时设置
import urllib.request
response = urllib.request.urlopen('http://httpbin.org/get', timeout=1)
print(response.read())
```

### 4）响应

当进行一次请求后，可以通过 response.status、response.getheaders().response.getheader("server") 来获取状态码以及头部信息。

实例：

```python
# 响应类型
>>> import urllib.request
>>> response = urllib.request.urlopen('https://www.python.org')
>>> print(type(response))
<class 'http.client.HTTPResponse'>

# 状态码
>>> import urllib.request
>>> response = urllib.request.urlopen('https://www.python.org')
>>> print(response.status)
200

# 响应头
>>> import urllib.request
>>> response = urllib.request.urlopen('https://www.python.org')
>>> print(response.getheaders())
[('Server', 'nginx'), ('Content-Type', 'text/html; charset=utf-8'), ('X-Frame-Options', 'SAMEORIGIN'), ('x-xss-protection', '1; mode=block'), ('X-Clacks-Overhead', 'GNU Terry Pratchett'), ('Via', '1.1 varnish'), ('Content-Length', '48809'), ('Accept-Ranges', 'bytes'), ('Date', 'Mon, 20 Aug 2018 13:13:00 GMT'), ('Via', '1.1 varnish'), ('Age', '708'), ('Connection', 'close'), ('X-Served-By', 'cache-iad2138-IAD, cache-bur17551-BUR'), ('X-Cache', 'HIT, HIT'), ('X-Cache-Hits', '1, 20'), ('X-Timer', 'S1534770781.676286,VS0,VE0'), ('Vary', 'Cookie'), ('Strict-Transport-Security', 'max-age=63072000; includeSubDomains')]
>>> print(response.getheader('Server'))
nginx
```

## 2.request

urllib.request.urlopen() 只能用于一些简单的请求，其无法添加一些 header 信息，很多情况下需要添加头部信息去访问目标站，这个时候就用到了 urllib.request.Request() 函数。

### 1）基本使用

首先声明一个 request 对象，该对象可以包括 header 等信息，然后用 urlopen 打开。

```python
import urllib.request
request = urllib.request.Requests('https://python.org')
response = urllib.request.urlopen(request)
print(response.read().decode('utf-8'))
```

### 2）添加 header 信息

如果想要添加 header 信息，可以在使用 Request() 函数发送请求时直接填入：

```python
from urllib import request
from urllib import parse

# 增加header
url = 'http://httpbin.org/post'
headers = {
    'User-Agent': 'Mozilla/4.0 (compatible; MSIE 5.5; Windows NT)',
    'Host': 'httpbin.org'
}

# 构造POST表单
dict = {'name': 'zhaofan'}
data = bytes(parse.urlencode(dict), encoding='utf8')

req = request.Request(url=url, data=data, headers=headers, method='POST')
response = request.urlopen(req)

print(response.read().decode('utf-8'))
```

除了在使用 Request() 函数发送请求时直接填入 header 信息，还可以使用 add_hader() 方法稍后添加 header 信息，这种添加的好处是，可以定义一个请求头字典，然后循环进行添加。

```python
from urllib import request 
from urllib import parse

url = 'http://httpbin.org/post'
dict = {'name':'Germey'}

req = request.Request(url=url,data=data,method='POST')

#添加 header 信息
req.add_hader('User-Agent':'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/57.0.2987.133 Safari/537.36')

response = request.urlopen(req)
print(response.read().decode('utf-8'))
```

### 3）代理设置

网站会检测某一段时间 IP 的访问次数，如果访问次数过多，它会禁止访问，因此这个时候需要通过设置代理来爬取数据，通过rulllib.request.ProxyHandler() 可以设置代理。

```python
import urllib.request

proxy_handler = urllib.request.ProxyHandler({
    'http': 'http://127.0.0.1:9743',
    'https': 'https://127.0.0.1:9743'
})

opener = urllib.request.build_opener(proxy_handler)
response = opener.open('http://httpbin.org/get')

print(response.read())
```

### 4）Cookie 信息

在 cookie 中，保存了常见的登录信息，有时候爬取网站需要携带 cookie 信息访问，此时可以使用 http.cookijar 来获取、存储 cookie

```python
import http.cookiejar
import urllib.request

cookie = http.cookiejar.CookieJar()
handler = urllib.request.HTTPCookieProcessor(cookie)
opener = urllib.request.build_opener(handler)
response = opener.open("http://www.baidu.com")
for item in cookie:
    print(item.name+"="+item.value)
```

同时，cookie 可以写入文件保存，有以下两种方式

http.cookiejar.MozillaCookieJar() 方式

```python
import http.cookiejar
import urllib.request

filename = "cookie.txt"

cookie = http.cookiejar.MozillaCookieJar(filename)
handler = urllib.request.HTTPCookieProcessor(cookie)
opener = urllib.request.build_opener(handler)

response = opener.open('http://www.baidu.com')

cookie.save(ignore_discard=True, ignore_expires=True)
```

http.cookiejar.LWPCookieJar() 方式

```python
import http.cookiejar
import urllib.request

filename = 'cookie.txt'
cookie = http.cookiejar.LWPCookieJar(filename)

handler = urllib.request.HTTPCookieProcessor(cookie)
opener = urllib.request.build_opener(handler)
response = opener.open('http://www.baidu.com')

cookie.save(ignore_discard=True, ignore_expires=True)
```

相应的，如果想获取文件中的 cookie 可以通过 load 方式，以何种方式写入，就以何种方式读取。

```python
import http.cookiejar
import urllib.request

cookie = http.cookiejar.LWPCookieJar()
cookie.load('cookie.txt', ignore_discard=True, ignore_expires=True)

handler = urllib.request.HTTPCookieProcessor(cookie)
opener = urllib.request.build_opener(handler)
response = opener.open('http://www.baidu.com')

print(response.read().decode('utf-8'))
```

# 【异常处理模块】

当 urllib.urlopen() 正常运行后看到可以正常的返回结果，但当其不正常运行时，会出现异常，运行程序就会提示出现错误，因此需要对异常进行抓取。

在 urllib 异常中有两个异常错误：URLError、HTTPError，其中 HTTPError 是 URLError 的子类

URLError 里只有一个属性：reason，即抓异常的时候只能打印错误信息

HTTPError 里有三个属性：code、reason、headers，即抓异常的时候可以获得 code、reason、headers 三个信息

```python
from urllib import request,error
try:
    response = request.urlopen("http://pythonsite.com/1111.html")
except error.HTTPError as e:
    print(e.reason)
    print(e.code)
    print(e.headers)
except error.URLError as e:
    print(e.reason)

else:
    print("reqeust successfully")
```

# 【解析模块】

## 1.urlparse

URL 解析功能侧重于将 URL 字符串拆分为其组件，或者将 URL 组件组合到 URL 字符串中。

函数原型：

```python
urllib.parse.urlparse(urlstring, scheme='', allow_fragments=True)
```

其可以对传入的 url 地址进行拆分，这样拆分的时候协议类型部分就会是你指定的部分，当然如果 url 里面已经带了协议，再通过 scheme 指定的协议就不会生效。

```python
>>> from urllib.parse import urlparse
>>> result = urlparse("http://www.baidu.com/index.html;user?id=5#comment",scheme="https")
>>> print(result)
ParseResult(scheme='http', netloc='www.baidu.com', path='/index.html', params='user', query='id=5', fragment='comment')
```

## 2.urlunpars

urlunpars 功能与 urlparse 功能相反，是 urlparse 的反向操作，其主要是用于拼接 url

```python
>>> from urllib.parse import urlunparse
>>> data = ['http','www.baidu.com','index.html','user','a=123','commit']
>>> print(urlunparse(data))
http://www.baidu.com/index.html;user?a=123#commit
```

## 3.urljoin

urljoin 功能是用于拼接两个 url，拼接的时候后面的 url 的优先级高于前面的 url 的优先级。

```python
>>> from urllib.parse import urljoin
>>> print(urljoin('http://www.baidu.com', 'FAQ.html'))
http://www.baidu.com/FAQ.html
>>> print(urljoin('www.baidu.com', '?category=2#comment'))
www.baidu.com?category=2#comment
```

## 4.urlencode

urlencode 功能是将字典转换为 url 参数，即将字典对象转换成 GET 请求对象。

```python
>>> from urllib.parse import urlencode
>>> params = {'name':'germey','age': 22}
>>> base_url = 'http://www.baidu.com?'
>>> url = base_url + urlencode(params)
>>> print(url)
http://www.baidu.com?name=germey&age=22
```




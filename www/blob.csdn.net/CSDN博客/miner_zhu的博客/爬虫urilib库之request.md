# 爬虫urilib库之request - miner_zhu的博客 - CSDN博客





2018年08月19日 22:30:03[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：85








官方文档链接：[https://docs.python.org/3/library/urllib.html](https://docs.python.org/3/library/urllib.html)

首先，了解一下urllib库，它是Python内置的HTTP请求库，也就是说不需要额外安装即可使用。它包含如下4个模块。
- **`request`**：它是最基本的HTTP请求模块，可以用来模拟发送请求。只需要给库方法传入URL以及额外的参数，就可以模拟实现请求过程了。
- **`error`**：异常处理模块，如果出现请求错误，我们可以捕获这些异常，然后进行修正。
- **`parse`**：一个工具模块，提供了许多URL处理方法，比如拆分、解析、合并等。
- **`robotparser`**：主要是用来识别网站的robots.txt文件，判断哪些网站可以爬哪些网站不可以爬，其实用得比较少。

这里重点讲解一下前3个模块。

# request

使用urllib的`request`模块，我们可以方便地实现请求的发送并得到响应

## `1.urlopen()`

`urllib.request`模块提供了最基本的构造HTTP请求的方法，利用它可以模拟浏览器的一个请求发起过程，同时它还带有处理授权验证（authenticaton）、重定向（redirection)、浏览器Cookies以及其他内容。

```python
import urllib.request

response = urllib.request.urlopen('https://www.python.org')
print(response.read().decode('utf-8'))
```

这里我们只用了两行代码，便完成了Python官网的抓取，输出了网页的源代码。

得到源代码之后呢？我们想要的链接、图片地址、文本信息不就都可以提取出来了吗？

response返回的是一个`HTTPResposne`类型的对象。它主要包含`read()`、`readinto()`、`getheader(name)`、`getheaders()`、`fileno()`等方法，以及`msg`、`version`、`status`、`reason`、`debuglevel`、`closed`等属性。得到这个对象之后，我们把它赋值为`response`变量，然后就可以调用这些方法和属性，得到返回结果的一系列信息了。例如，调用`read()`方法可以得到返回的网页内容，调用`status`属性可以得到返回结果的状态码。

利用最基本的`urlopen()`方法，可以完成最基本的简单网页的GET请求抓取。

如果想给链接传递一些参数，该怎么实现呢？首先看一下`urlopen()`函数的API：

```python
urllib.request.urlopen(url, data=None, [timeout,]*,cafile=None,capath=None,cadefault=False, context=None)
```

 下面我们详细说明下这几个参数的用法。
`data`参数是可选的。如果要添加该参数，并且如果它是字节流编码格式的内容，即`bytes`类型，则需要通过`bytes()`方法转化。另外，如果传递了这个参数，则它的请求方式就不再是GET方式，而是POST方式。
`timeout`参数用于设置超时时间，单位为秒，意思就是如果请求超出了设置的这个时间，还没有得到响应，就会抛出异常。如果不指定该参数，就会使用全局默认时间。它支持HTTP、HTTPS、FTP请求。
`context`参数，它必须是`ssl.SSLContext`类型，用来指定SSL设置。
`cafile`和`capath`这两个参数分别指定CA证书和它的路径，这个在请求HTTPS链接时会有用。
`cadefault`参数现在已经弃用了，其默认值为`False`。

## `2.Request`

我们知道利用`urlopen()`方法可以实现最基本请求的发起，但这几个简单的参数并不足以构建一个完整的请求。如果请求中需要加入Headers等信息，就可以利用更强大的`Request`类来构建。

我们依然用`urlopen()`方法来发送请求，只不过这次的参数不再是URL，而是一个`Request`类型的对象。通过构造这个数据结构，一方面我们可以将请求独立成一个对象，另一方面可更加丰富和灵活地配置参数。

`Request`的构造方法如下：

```python
class urllib.request.Request(url, data=None, headers={}, origin_req_host=None, unverifiable=False, method=None)
```
- `url`用于请求URL，这是必传参数，其他都是可选参数。
- `data`如果要传，必须传`bytes`（字节流）类型的。如果它是字典，可以先用`urllib.parse`模块里的`urlencode()`编码。
- `headers`是一个字典，它就是请求头，我们可以在构造请求时通过`headers`参数直接构造，也可以通过调用请求实例`add_header()`方法添加。
	
添加请求头最常用的用法就是通过修改`User-Agent`来伪装浏览器，默认的`User-Agent`是Python-urllib，我们可以通过修改它来伪装浏览器。

- `origin_req_host`指的是请求方的host名称或者IP地址。
- `unverifiable`表示这个请求是否是无法验证的，默认是`False`，意思就是说用户没有足够权限来选择接收这个请求的结果。例如，我们请求一个HTML文档中的图片，但是我们没有自动抓取图像的权限，这时unverifiable`的值就是`True`。
- `method`是一个字符串，用来指示请求使用的方法，比如GET、POST和PUT等。

下面我们传入多个参数构建请求来看一下：

```python
from urllib import request, parse
 
url = 'http://httpbin.org/post'
headers = {
    'User-Agent': 'Mozilla/4.0 (compatible; MSIE 5.5; Windows NT)',
    'Host': 'httpbin.org'
}
dict = {
    'name': 'Germey'
}
data = bytes(parse.urlencode(dict), encoding='utf8')
req = request.Request(url=url, data=data, headers=headers, method='POST')
response = request.urlopen(req)
print(response.read().decode('utf-8'))
```

这里我们通过4个参数构造了一个请求，其中`url`即请求URL，`headers`中指定了`User-Agent`和`Host`，参数`data`用`urlencode()`和`bytes()`方法转成字节流。另外，指定了请求方式为POST。

## 3. 高级用法

在上面的过程中，我们虽然可以构造请求，但是对于一些更高级的操作（比如Cookies处理、代理设置等），我们该怎么办呢？

接下来，就需要更强大的工具Handler登场了。简而言之，我们可以把它理解为各种处理器，有专门处理登录验证的，有处理Cookies的，有处理代理设置的。利用它们，我们几乎可以做到HTTP请求中所有的事情。

首先，介绍一下`urllib.request`模块里的`BaseHandler`类，它是所有其他`Handler`的父类，它提供了最基本的方法，例如`default_open()`、`protocol_request()`等。

接下来，就有各种`Handler`子类继承这个`BaseHandler`类，举例如下。
- **`HTTPDefaultErrorHandler`**：用于处理HTTP响应错误，错误都会抛出`HTTPError`类型的异常。
- **`HTTPRedirectHandler`**：用于处理重定向。
- **`HTTPCookieProcessor`**：用于处理Cookies。
- **`ProxyHandler`**：用于设置代理，默认代理为空。
- **`HTTPPasswordMgr`**：用于管理密码，它维护了用户名和密码的表。
- **`HTTPBasicAuthHandler`**：用于管理认证，如果一个链接打开时需要认证，那么可以用它来解决认证问题。

另外，还有其他的`Handler`类，这里就不一一列举了，详情可以参考官方文档：[https://docs.python.org/3/library/urllib.request.html#urllib.request.BaseHandler](https://docs.python.org/3/library/urllib.request.html#urllib.request.BaseHandler)。

另一个比较重要的类就是`OpenerDirector`，我们可以称为`Opener`。我们之前用过`urlopen()`这个方法，实际上它就是urllib为我们提供的一个`Opener`。为什么要引入`Opener`呢？因为需要实现更高级的功能。之前使用的`Request`和`urlopen()`相当于类库为你封装好了极其常用的请求方法，利用它们可以完成基本的请求，但是现在不一样了，我们需要实现更高级的功能，所以需要深入一层进行配置，使用更底层的实例来完成操作，所以这里就用到了`Opener`。

`Opener`可以使用`open()`方法，返回的类型和`urlopen()`如出一辙。那么，它和`Handler`有什么关系呢？简而言之，就是利用`Handler`来构建`Opener`。

下面用几个实例来看看它们的用法。

### 验证

有些网站在打开时就会弹出提示框，直接提示你输入用户名和密码，验证成功后才能查看页面。

如果要请求这样的页面，借助`HTTPBasicAuthHandler`就可以完成，相关代码如下：

```python
from urllib.request import HTTPPasswordMgrWithDefaultRealm, HTTPBasicAuthHandler, build_opener
from urllib.error import URLError
 
username = 'username'
password = 'password'
url = 'http://localhost:5000/'
 
p = HTTPPasswordMgrWithDefaultRealm()
p.add_password(None, url, username, password)
auth_handler = HTTPBasicAuthHandler(p)
opener = build_opener(auth_handler)
 
try:
    result = opener.open(url)
    html = result.read().decode('utf-8')
    print(html)
except URLError as e:
    print(e.reason)
```

这里首先实例化`HTTPBasicAuthHandler`对象，其参数是`HTTPPasswordMgrWithDefaultRealm`对象，它利用`add_password()`添加进去用户名和密码，这样就建立了一个处理验证的`Handler`。

接下来，利用这个`Handler`并使用`build_opener()`方法构建一个`Opener`，这个`Opener`在发送请求时就相当于已经验证成功了。

接下来，利用`Opener`的`open()`方法打开链接，就可以完成验证了。

### 代理

在做爬虫的时候，免不了要使用代理，如果要添加代理，可以这样做：

```python
from urllib.error import URLError
from urllib.request import ProxyHandler, build_opener
 
proxy_handler = ProxyHandler({
    'http': 'http://127.0.0.1:9743',
    'https': 'https://127.0.0.1:9743'
})
opener = build_opener(proxy_handler)
try:
    response = opener.open('https://www.baidu.com')
    print(response.read().decode('utf-8'))
except URLError as e:
    print(e.reason)
```

这里我们在本地搭建了一个代理，它运行在9743端口上。

这里使用了`ProxyHandler`，其参数是一个字典，键名是协议类型（比如HTTP或者HTTPS等），键值是代理链接，可以添加多个代理。

然后，利用这个Handler及`build_opener()`方法构造一个`Opener`，之后发送请求即可。

### Cookies

Cookies的处理就需要相关的`Handler`了。我们先用实例来看看怎样将网站的Cookies获取下来，相关代码如下：

```python
import http.cookiejar, urllib.request
 
cookie = http.cookiejar.CookieJar()
handler = urllib.request.HTTPCookieProcessor(cookie)
opener = urllib.request.build_opener(handler)
response = opener.open('http://www.baidu.com')
for item in cookie:
    print(item.name+"="+item.value)
```

首先，我们必须声明一个`CookieJar`对象。接下来，就需要利用`HTTPCookieProcessor`来构建一个`Handler`，最后利用`build_opener()`方法构建出`Opener`，执行`open()`函数即可。

如果我们想输出成文件格式呢？我们知道Cookies实际上也是以文本形式保存的。这时`CookieJar`就需要换成`MozillaCookieJar`，它在生成文件时会用到，是`CookieJar`的子类，可以用来处理Cookies和文件相关的事件，比如读取和保存Cookies，可以将Cookies保存成Mozilla型浏览器的Cookies格式。

```python
filename = 'cookies.txt'
cookie = http.cookiejar.MozillaCookieJar(filename)
handler = urllib.request.HTTPCookieProcessor(cookie)
opener = urllib.request.build_opener(handler)
response = opener.open('http://www.baidu.com')
cookie.save(ignore_discard=True, ignore_expires=True)
```

另外，`LWPCookieJar`同样可以读取和保存Cookies，但是保存的格式和`MozillaCookieJar`不一样，它会保存成libwww-perl(LWP)格式的Cookies文件。要保存成LWP格式的Cookies文件，可以在声明时就改为：

```python
cookie = http.cookiejar.LWPCookieJar(filename)
```

那么，生成了Cookies文件后，怎样从文件中读取并利用呢？

下面我们以`LWPCookieJar`格式为例来看一下：

```python
cookie = http.cookiejar.LWPCookieJar()
cookie.load('cookies.txt', ignore_discard=True, ignore_expires=True)
handler = urllib.request.HTTPCookieProcessor(cookie)
opener = urllib.request.build_opener(handler)
response = opener.open('http://www.baidu.com')
print(response.read().decode('utf-8'))
```

可以看到，这里调用`load()`方法来读取本地的Cookies文件，获取到了Cookies的内容。不过前提是我们首先生成了LWPCookieJar格式的Cookies，并保存成文件，然后读取Cookies之后使用同样的方法构建Handler和Opener即可完成操作。



原文网址：[https://cuiqingcai.com/5500.html](https://cuiqingcai.com/5500.html)




# 爬虫基础之Requests库入门 - miner_zhu的博客 - CSDN博客





2018年07月19日 18:35:23[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：46标签：[python																[爬虫](https://so.csdn.net/so/search/s.do?q=爬虫&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[爬虫](https://blog.csdn.net/miner_zhu/article/category/7809267)








## Requests库入门

```python
import requests
r = requests.get("http://www.baidu.com")
r.status_code
r.encoding = 'utf-8'
r.text
```

### Requests库的7个主要方法
|方法|说明|
|----|----|
|requests.request()|构造一个请求，支持以下各方法的基础方法|
|requests.get()|获取HTML网页的主要方法，对应HTTP的GET|
|requests.head()|获取HTML网页头信息的方法，对应HTTP的HEAD|
|requests.post()|向HTML网页提交POST请求，对应HTTP的POST|
|requests.put()|向HTML网页提交PUT请求，对应HTTP的PUT|
|requests.patch()|向HTML网页提交局部修改请求，对应HTTP的PATCH|
|requests.delete()|向HTML页面提交删除请求，对应HTTP的DELETE|

```python
r = requests.get(url, params=None, **kwargs)
```

构造一个向服务器请求资源的Request对象，返回一个包含服务器资源的Response对象。

url: 拟获取页面的url链接。

params（可选参数）：url中的额外参数，字典或字节流格式。

**kwargs（可选参数）：12个控制访问的参数。

Requests库的其他6个其实都是调用request方法实现的。例如get方法在源代码中是这样写的：

```python
def get(url, params=None, **kwargs):
    kwargs.setdefault('allow redirects', True)
    return request('get', url, params=params, **kwargs)
```

### Response对象的属性
|属性|说明|
|----|----|
|r.status_code|HTTP请求的返回状态，200表示连接成功，404表示连接失败|
|r.text|HTTP响应内容的字符串形式，即url对应的页面内容|
|r.encoding|从HTTP header中猜测的响应内容编码方式|
|r.apparent_encoding|从内容中分析出的响应内容编码方式（备选编码方式）|
|r.content|HTTP响应内容的二进制形式|

使用r.encoding获取编码时，如果header中不存在charset属性，则认为编码方式是ISO-8859-1。r.apparent_encoding是根据内容来分析出的编码方式，会比r.encoding更准确。

### Requests库的异常
|异常|说明|
|----|----|
|requests.ConnectionError|网络连接错误异常，如DNS查询失败、拒绝连接等|
|requests.HTTPError|HTTP错误异常|
|requests.URLRequired|URL缺失异常|
|requests.TooManyRedirects|超过最大重定向次数，产生重定向异常|
|requests.ConnectTimeout|连接远程服务器超时异常|
|requests.Timeout|请求URL超时，产生超时异常|
|r.raise_for_status()|如果不是200，产生异常requests.HTTPError|

```python
# 爬取网页的通用代码框架
import requests

def getHTMLText(url):
    try:
        r = requests.get(url, timeout=30)
        r.raise_for_status()
        r.encoding = r.apparent_encoding
        return r.text
    except:
        return "产生异常"

if __name__ == "__main__":
    url = "http://www.baidu.com"
    print(getHTMLText(url))
```







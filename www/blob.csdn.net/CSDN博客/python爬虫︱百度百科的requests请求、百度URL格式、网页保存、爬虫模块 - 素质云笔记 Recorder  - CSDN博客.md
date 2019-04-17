# python爬虫︱百度百科的requests请求、百度URL格式、网页保存、爬虫模块 - 素质云笔记/Recorder... - CSDN博客





2018年03月08日 16:48:01[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：767








## 1 、 百科网页请求

### 1.1 网站解析`requests`与`urllib.request`对比：

py3中主要用`requests`以及`urllib.request`两款库用来做网页内容的解析，两者的使用较多以`requests`为优，解析的内容会做较多的优化。 
**两款内容的对比：**

```
from bs4 import BeautifulSoup
url="http://finance.qq.com/gdyw.htm"

##使用urllib.request的代码：
import urllib.request
html=urllib.request.urlopen(url).read()
html=html.decode('gb2312',errors='ignore')
soup1=BeautifulSoup(html,'lxml')
lfls1 = str(soup1).split('<!-- 左侧列表 -->',2)

##使用requests的代码：
import requests
response = requests.get(url)
soup2 = BeautifulSoup(response.text,'lxml')
lfls2 = str(soup2).split('<!-- 左侧列表 -->',2)

print(lfls1[1][:500])
print('='*70)
print(lfls2[1][:500])
```

百度百科请求响应的过程中，两款请求的方式都不太相同。requests会报重定向的错误，重定向意味着两个词具有同样的意思，所以需要设置请求头。（参考来源：[requests关于Exceeded 30 redirects问题得出的结论](https://www.cnblogs.com/Jeffding/p/8391957.html)）

```
# requests的错误示范
all_url = 'http://baike.baidu.com/view/21087.htm'
html = requests.get(all_url)
>>> TooManyRedirects: Exceeded 30 redirects.

# requests的正确使用
headers = {"User-Agent" : "User-Agent:Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0;"}
html = requests.get(all_url, headers=headers).content.decode('utf8')
```

`urllib.request`的使用会好一些，是能够直接call通。

### 1.2 requests的细节

主要参考：[requests快速上手](http://docs.python-requests.org/zh_CN/latest/user/quickstart.html)

##### 1.2.1 传递URL的`参数params`：

```
>>> payload = {'key1': 'value1', 'key2': 'value2'}
>>> r = requests.get("http://httpbin.org/get", params=payload)
```

##### 1.2.2 传递URL的`参数stream`：

你可能想获取来自服务器的原始套接字响应，那么你可以访问 r.raw。 如果你确实想这么干，那请你确保在初始请求中设置了 stream=True。具体你可以这么做：

```
>>> r = requests.get('https://github.com/timeline.json', stream=True)
>>> r.raw
<requests.packages.urllib3.response.HTTPResponse object at 0x101194810>
>>> r.raw.read(10)
'\x1f\x8b\x08\x00\x00\x00\x00\x00\x00\x03'
```

但一般情况下，你应该以下面的模式将文本流保存到文件：

```
with open(filename, 'wb') as fd:
    for chunk in r.iter_content(chunk_size):
        fd.write(chunk)
```

使用 Response.iter_content 将会处理大量你直接使用 Response.raw 不得不处理的。 当流下载时，上面是优先推荐的获取内容方式。

##### 1.2.3 定制请求头

```python
>>> url = 'https://api.github.com/some/endpoint'
>>> headers = {'user-agent': 'my-app/0.0.1'}

>>> r = requests.get(url, headers=headers)
```

##### 1.2.4 data 参数

```
>>> payload = {'key1': 'value1', 'key2': 'value2'}

>>> r = requests.post("http://httpbin.org/post", data=payload)
>>> print(r.text)
{
  ...
  "form": {
    "key2": "value2",
    "key1": "value1"
  },
  ...
}
```

##### 1.2.5 重定向与请求历史

```python
>>> r = requests.get('http://github.com', allow_redirects=False)
>>> r.status_code
301
>>> r.history
[]
```

##### 1.2.6 超时

你可以告诉 requests 在经过以 timeout 参数设定的秒数时间之后停止等待响应。基本上所有的生产代码都应该使用这一参数。如果不使用，你的程序可能会永远失去响应：

```
>>> requests.get('http://github.com', timeout=0.001)
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
requests.exceptions.Timeout: HTTPConnectionPool(host='github.com', port=80): Request timed out. (timeout=0.001)
```

##### 1.2.7 一些基本参数

传递之后，内容所拥有的属性：

```
r.text
>>> u'[{"repository":{"open_issues":0,"url":"https://github.com/...
r.encoding  # 了解编码
>>>'utf-8'
r.encoding = 'ISO-8859-1' # 修改编码
r.json()  # 变为json格式
>>> [{u'repository': {u'open_issues': 0, u'url': 'https://github.com/...
```

## 2、百度搜索链接问题

百度在post链接的时候，有两种键入方式：`'http://baike.baidu.com/item/体质人类学'` 以及 `"http://baike.baidu.com/view/21087.htm"`

第二种比较好进行遍历。 

第一种会发现不能直接输入中文，需要进行编码。 
**举个例子：**
```
# 编码前
all_url = 'http://baike.baidu.com/item/体质人类学' 
# 编码后
'https://baike.baidu.com/item/%E4%BD%93%E8%B4%A8%E4%BA%BA%E7%B1%BB%E5%AD%A6'
```

如何进行编码呢？需要利用`urllib.request.urlopen(url, data=None, [timeout, ]*, cafile=None, capath=None, cadefault=False, context=None)`

主要参考：[Python3学习笔记（urllib模块的使用）](https://www.cnblogs.com/Lands-ljk/p/5447127.html)

```
search_word = '体质人体学'
values = {'wd': search_word}
word = urllib.parse.urlencode(values)
>>> 'wd=%E4%BD%93%E8%B4%A8%E4%BA%BA%E4%BD%93%E5%AD%A6'
```

## 3、网页保存函数

```python
def saveHtml(file_name, file_content):  
    #    注意windows文件命名的禁用符，比如 /  
    with open(file_name.replace('/', '_') + ".html", "wb") as f:  
        #   写文件用bytes而不是str，所以要转码  
        f.write(file_content)  

def html_downloader(all_url):
    headers = {"User-Agent" : "User-Agent:Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0;"}
    html = requests.get(all_url, headers=headers).content#.decode('utf8')
    #soup = BeautifulSoup(html, 'html.parser', from_encoding='utf-8')
    #if save_path :
    #    saveHtml(save_path, html)  
    return html

# 使用
url = 'https://baike.baidu.com/item/%E4%BD%93%E8%B4%A8%E4%BA%BA%E7%B1%BB%E5%AD%A6'
save_path = './'
html = html_downloader(url)
saveHtml(save_path, html) # 保存
```

`saveHtml`即为保存函数，即可以html的方式讲网页进行保存。

## 4、  fifths/python_baike_spider爬虫库解析

[fifths/python_baike_spider](https://github.com/fifths/python_baike_spider)，对于新手来说不太友好… 但是功能强大，目前 

该库可以实现：百度百科、百度词典的爬取。 

必要的库：**beautifulsoup4**
#### 1、baike_spider模块：百度百科
- 功能：输入URL
- 输出：HTML网址 

（详情见[output1.html](https://github.com/mattzheng/BaiduSpider/blob/master/baike_spider/output1.html)）
- 主文件：[spider_main.py](https://github.com/mattzheng/BaiduSpider/blob/master/baike_spider/spider_main.py)

我这里对原作者的内容进行简单修改，可以指定路径保存 + 保存名称。

```
root_url = "http://baike.baidu.com/view/21087.htm"
save_path = './baike_spider/'
filename = 'output2.html'
obj_spider = SpiderMain()
obj_spider.craw(root_url,filename,save_path)
```

#### 2、dict_spider模块：百度字典

主要用来解析百度词典： 
![这里写图片描述](https://github.com/mattzheng/BaiduSpider/blob/master/dict_spider/dict_baidu.png?raw=true)
```
word='人'
values = {
    'wd': word,
    'ptype': 'char'
}
data = urllib.parse.urlencode(values)
root_url = "http://dict.baidu.com/s?" + data + '#'
obj_spider = SpiderMain()
obj_spider.craw(root_url)
```

生成的链接为： `'http://dict.baidu.com/s?ptype=char&wd=%E4%BA%BA#'`


**公众号“素质云笔记”定期更新博客内容：**
![这里写图片描述](https://img-blog.csdn.net/20180226155348545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)








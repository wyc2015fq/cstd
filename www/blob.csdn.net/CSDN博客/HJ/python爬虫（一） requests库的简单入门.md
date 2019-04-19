# python爬虫（一） requests库的简单入门 - HJ - CSDN博客
2018年12月01日 17:33:02[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：84
### 爬虫是什么
网络爬虫：简单来说，就是写一段程序，通过电脑访问服务器的方式获取我们所需要的资源。从两个方面来考虑，学习一下爬虫不仅能提升我们平时的做事效率，也能改变我们的做事方式；
下面，我们就从一个简单的问题入门吧！
问题：小月是一名公司的UI设计，现在在一个产品页面的设计上缺乏优秀的图片展示，她需要从平时常常浏览的网站上获取一些好看的图片；
### 爬虫的特点
人工查找 VS  爬虫实现：
如果使用爬虫，完全可以先把所有分类的图片爬取到电脑分类存放，然后在电脑中查找获取；这样带来的好处是，可以专心一张一张选择，而不需要频繁无目的的点开各种网页，提高效率和减少工作时间；
### 爬虫入门：requests库
首先我要说的是，这个并不难实现，只需使用python中的requests库即可；requests库是⽤Python语⾔编写的 HTTP 库。
安装教程：在命令行中输入pip install requests即可（Windows下的cmd，Linux和Mac下的命令行界面）；
```
注意：如果系统中同时又python2和python3，则需要指定一下：
pip install requests   #默认是安装在python2的目录下
pip3 install requests  #如果需要安装在python3上则需要用pip3
另外，可能会提示你先更新pip命令，这个按照提示输入命令让其更新即可
```
最简单的爬虫演示：
```python
import requests
r = requests.get("http://www.baidu.com")  #抓取百度首页的页面
```
我们可以知道，r的参数类型是response，那么可以从r中获取的参数就有很多了，比如说状态码，响应头，响应体等等；
但是需要特别注意的是，爬虫初学者可能并不太了解编码和解码机制。这里我需要说一下，正常来说，浏览器显示的信息的字节码，如果在GET请求头中没有charset选项，即限制编码格式，那么浏览器默认的编码格式是ISO-8859-1，但是这个字节码是不支持中文的，输出显示会乱码！如果我们需要支持中文格式，就需要把转码为utf-8。那么现在，类型为response的r查看编码的方式有两种，一种是encoding，一种是apparent_encoding；前者是自己猜测响应体字节编码格式（也就前面所说的，如果没有charset选项，则默认为ISO-8859-1），后者则是根据根据网页内容分析出的编码方式；如果我们打印输出一下，会出现如下结果：（通常来说，apparent_encoding是响应体正文的正确格式）
```python
print(r.encoding)    
print(r.apparent_encoding)
#  返回结果
#  ISO-8859-1
#  utf-8
```
接下来我们了解一下r对应的Response对象的属性:
```
r.status_code：HTTP请求的返回状态，200表示连接成功，404表示失败；
r.text：HTTP响应内容的字符串形式，即URL对应的页面内容；
r.encoding：从HTTP header中猜测的响应内容编码方式；
r.apparent_encoding： 从内容中分析出的响应内容编码方式（备选编码方式）；
r.content：HTTP响应内容的二进制形式；
r.encoding：从header中的charset字段获取，如果header中不存在charset，则认为编码为ISO-8859-1；
r.apparent_encoding：根据网页内容分析出的编码方式；
```
那么接下来就会有一个问题了，平时在访问浏览器会出现很多种情况，比如请求超时、访问受限、请求失败等等信息；于是，为了完善我们的爬虫机制，需要了解一下requests库中的一些异常：
```
requests.ConnectionError：网络连接错误异常，如DNS查询失败、拒绝连接等；
requests.HTTPError：HTTP错误异常；
requests.URLRequired：URL缺失异常；
requests.TooManyRedirects：超过最大重定向数，产生重定向异常；
requests.ConnectTimeout：连接远程服务器超时异常；
requests.Timeout：请求URL超时，产生超时异常；
```
另外，我们除了向网站提交一个GET请求之外，还有类似的比较常见的POST请求，为了避免麻烦，我们可以学习一个统一的函数requests.request。其实大家去看看源码就可以知道，不管是requests.get还是requests.post，其底层都调用了requests.request；
` 格式： requests.request(method,url,**kwargs)`
其中method表示请求方法，url表示请求路径，**kwargs表示不定参数，作为可选项，通常是12个，下面是一些常见的不定参数：
①params：字典或字节序列，作为参数增加到url中；
```python
kv = {'key1':'value1','key2':'value2'}
r = requests.request('GET','http://www.baidu.com',params=kv)
print(r)
# http://www.baidu.com?key1=value1&key2=value2
```
②data：字典、字节序列或文件对象，作为Request的内容；
```python
body = '主题内容'
r = requests.request('POST','http://www.baidu.com',data=body)
```
③json：JSON格式的数据，作为Request的内容；
④headers：字典，HTTP定制头；
⑤cookies：字典或CookieJar，Request中的cookie；
⑥auth：元祖，支持HTTP认证功能；
⑦files：字典类型，传输文件；
⑧timeout：设置超时时间，秒为单位；
⑨proxies：字典类型，设定访问代理服务器，可以增加登录认证；
### 简单模板
那么学了这么多，为了方便每次调用，我们可以将每次调用的功能封装为一个函数：
```python
def getHTMLText(url):
    try:
        r = requests.get(url, timeout=30)
        r.raise_for_status()
        r.encoding = r.apparent_encoding
        return r.text
    except:
        return "产生异常"
```
其中r.raise_for_status()在判断状态码不是200的时候，能够触发异常；
另外，大家需要非常注意的一点就是，有一些网站会有一些反爬的操作。其最主要的判断依据就是你发出去的Request请求的请求头中的User-Agent的值，这个值表示你是用什么浏览器发送请求的；通常的有Chrome、Firfox、IE等等，但是假如你用爬虫的话，则会显示python相关值；因此需要修改一下该值：
```python
def getHTMLText(url):
    try:
        kv = {'user-agent':'Mozilla/5.0'} 
        r = requests.get(url,headers=kv,timeout=30)   #构造一个访问浏览器添加到header中
        r.raise_for_status()   #如果返回的状态码不是200 则会产生一个异常
        r.encoding = r.apparent_encoding  #转码
        return r.text
    except:
        return "产生异常"
```
### 爬虫知识兴趣了解
爬虫规模分类
1）爬取网页，玩转网页：小规模，数据量小，爬取速度不敏感，Request库；
2）爬取网站、系列网站：中规模，数据规模较大，爬取速度敏感，Scrapy库；
3）爬取全网：大规模啊，搜索引擎，爬取速度关键，定制开发；

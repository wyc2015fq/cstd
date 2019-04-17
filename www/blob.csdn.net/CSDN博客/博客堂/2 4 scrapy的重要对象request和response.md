# 2.4 scrapy的重要对象request和response - 博客堂 - CSDN博客





2017年05月13日 17:42:24[最小森林](https://me.csdn.net/u012052268)阅读数：1212










- [scrapy的重要对象](#06-scrapy的重要对象)- [requests](#1requests)- [1初始化的参数](#11初始化的参数)
- [2属性](#12属性)
- [4子类](#14子类)- [实例使用FormRequestfrom_response方法模拟用户登录](#实例使用formrequestfromresponse方法模拟用户登录)


- [responses](#2responses)- [1初始化的参数](#21初始化的参数)
- [2成员属性与方法](#22成员属性与方法)
- [3子类](#23子类)

- [要搞懂的问题](#要搞懂的问题)





# 06 scrapy的重要对象

web服务器收到客户端的HTTP请求，会针对每一次请求分别创建一个用于代表请求的request对象和代表响应的response对象。
- 
要得到客户机提交过来的数据，只需要找request对象就行了。

- 
要向客户机输出数据，只需要找response对象就行了。


## 1.requests

### 1.1初始化的参数

```
class scrapy.http.Request(url 
[ , callback, 
method='GET',
headers, 
body,
cookies, 
meta,encoding='utf-8',  
priority=0, 
don't_filter=False, 
errback ] )
```

### 1.2属性

```python
url

method

headers

body

meta

copy() 复制一个相同的request

replace()

### 1.3实例
1，生成Request的方法

 ```
def parse_page1(self, response):
    return scrapy.Request("http://www.example.com/some_page.html",
                          callback=self.parse_page2)

def parse_page2(self, response):
    # this would log http://www.example.com/some_page.html
    self.logger.info("Visited %s", response.url)
```

2，通过Request传递数据的方法

在两个不同的解析函数之间传递数据的方法。

```python
def parse_page1(self, response):
    item = MyItem()
    item['main_url'] = response.url
    request = scrapy.Request("http://www.example.com/some_page.html",
                             callback=self.parse_page2)
    request.meta['item'] = item
    return request

def parse_page2(self, response):
    item = response.meta['item']
    item['other_url'] = response.url
    return item
```

3 Request.meta中的特殊关键字

### 1.4子类

主要的子类是FormRequest。主要用来“登录”作用。所以非常重要。

```bash
1,新参数: formdata
    #用于存储用户名，密码等数据

2,新的类方法: from_reponse
    # 从一个reponse中生成一个request
```

#### 实例：使用FormRequest.from_response()方法模拟用户登录

通常网站通过  实现对某些表单字段（如数据或是登录界面中的认证令牌等）的预填充。 使用Scrapy抓取网页时，如果想要预填充或重写像用户名、用户密码这些表单字段， 可以使用 FormRequest.from_response() 方法实现。下面是使用这种方法的爬虫例子:

```python
import scrapy

class LoginSpider(scrapy.Spider):
    name = 'example.com'
    start_urls = ['http://www.example.com/users/login.php']

    def parse(self, response):
        return scrapy.FormRequest.from_response( # 从respongse返回一个request（FormRequest）
            response,
            formdata={'username': 'john', 'password': 'secret'},
            callback=self.after_login
        )

    def after_login(self, response):
        # check login succeed before going on
        if "authentication failed" in response.body:
            self.logger.error("Login failed")
            return

        # continue scraping with authenticated session...
```

## 2.responses

response是scrapy对request的相应，所以是自动产生。一般不用自己生成。

### 2.1初始化的参数

```
class scrapy.http.Response(

url [ , 

status=200,

 headers,

 body,

 flags 

] )
```

### 2.2成员属性与方法

```
url

status

headers

body

request # 是产生该response的request

meta # 是request.meta的简要形式

flags

copy()

replace()

urljoin()  # 经常用到！用相对连接生成绝对连接。
```

### 2.3子类

```
class scrapy.http.TextResponse(url [ , encoding [ , ... ]] )

class scrapy.http.HtmlResponse(url [ , ... ] )

class scrapy.http.XmlResponse(url [ , ... ] )
```

## 要搞懂的问题

1，掌握Request初始化参数的意义，并学会使用； 

了解Request中的剩余属性和方法

2，掌握通过Request传递数据的方法；

3，掌握FormRequest的用法；

4，掌握Response中属性方法的意义

5，了解Response的子类都有那些，是干什么用的




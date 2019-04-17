# Python网络爬虫精要 - zhusongziye的博客 - CSDN博客





2018年10月20日 16:54:19[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：109标签：[Python网络爬虫精要](https://so.csdn.net/so/search/s.do?q=Python网络爬虫精要&t=blog)
个人分类：[Python爬虫](https://blog.csdn.net/zhusongziye/article/category/7611456)









**目的**

学习如何从互联网上获取数据。数据科学必须掌握的技能之一。

本文所用到的第三方库如下: requests, parsel, selenium

requests负责向网页发送HTTP请求并得到响应，parsel负责解析响应字符串，selenium负责JavaScript的渲染。

**网络爬虫是什么**

网络爬虫是一种按照一定的规则，自动地抓取网站信息的程序或者脚本。

**如何爬取网站信息**

写爬虫之前，我们必须确保能够爬取目标网站的信息。

不过在此之前必须弄清以下三个问题:
- 
网站是否已经提供了api

- 
网站是静态的还是动态的

- 
网站是否有反爬的对策


**情形1：开放api的网站**

一个网站倘若开放了api，那你就可以直接GET到它的json数据。

比如xkcd的about页就提供了api供你下载

```
import requests
requests.get('https://xkcd.com/614/info.0.json').json()
```

那么如何判断一个网站是否开放api呢？有3种方法：
- 
在站内寻找api入口

- 
用搜索引擎搜索“某网站 api”

- 
抓包。有的网站虽然用到了ajax（比如果壳网的瀑布流文章），但是通过抓包还是能够获取XHR里的json数据的。


怎么抓包：F12 - Network - F5刷新即可 | 或者用fiddle等工具也可以

**情形2：不开放api的网站**

如果此网站是静态页面，那么你就可以用requests库发送请求，再用HTML解析库（lxml、parsel等）来解析响应的text

解析库强烈推荐parsel，不仅语法和css选择器类似，而且速度也挺快，Scrapy用的就是它。

你需要了解一下css选择器的语法（xpath也行），并且学会看网页的审查元素。

比如获取konachan的所有原图链接

```
from parsel import Selector
res = requests.get('https://konachan.com/post')
tree = Selector(text=res.text)
imgs = tree.css('a.directlink::attr(href)').extract()
```

如果此网站是动态页面，先用selenium来渲染JS，再用HTML解析库来解析driver的page_source。

比如获取hitomi.la的数据（这里把chrome设置成了无头模式）

```
from selenium import webdriver
options = webdriver.ChromeOptions()
options.add_argument('--headless')
driver = webdriver.Chrome(options=options)
driver.get('https://hitomi.la/type/gamecg-all-1.html')
tree = Selector(text=driver.page_source)
gallery_content = tree.css('.gallery-content > div')
```

**情形3：反爬的网站**

目前的反爬策略常见的有：验证码、登录、封ip等。

验证码：利用打码平台破解（如果硬上的话用opencv或keras训练图）

登录：利用requests的post或者selenium模拟用户进行模拟登陆

封ip：买些代理ip（免费ip一般都不管用），requests中传入proxies参数即可

其他防反爬方法：伪装User-Agent，禁用cookies等

推荐用fake-useragent来伪装User-Agent

```
from fake_useragent import UserAgent
headers = {'User-Agent': UserAgent().random}
res = requests.get(url, headers=headers)
```

**如何编写结构化的爬虫**

如果能成功地爬取网站信息，那么你已经成功了一大半。

其实爬虫的架构很简单，无非就是创造一个tasklist，对tasklist里的每一个task调用crawl函数。

大多数网页的url构造都是有规律的，你只需根据它用列表推倒式来构造出tasklist对于那些url不变的动态网页，先考虑抓包，不行再用selenium点击下一页

如果追求速度的话，可以考虑用concurrent.futures或者asyncio等库。

```
import requests
from parsel import Selector
from concurrent import futures

domain = 'https://www.doutula.com'

def crawl(url):
    res = requests.get(url)
    tree = Selector(text=res.text)
    imgs = tree.css('img.lazy::attr(data-original)').extract()
    # save the imgs ...

if __name__ == '__main__':
    tasklist = [f'{domain}/article/list/?page={i}' for i in range(1, 551)]
    with futures.ThreadPoolExecutor(50) as executor:
        executor.map(crawl, tasklist)
```

数据存储的话，看你的需求，一般都是存到数据库中，只要熟悉对应的驱动即可。

常用的数据库驱动有：pymysql(MySQL),pymongo(MongoDB)

**如果你需要框架的话**

文章读到这里，你应该对爬虫的基本结构有了一个清晰的认识，这时你可以去上手框架了。

轻量级框架（looter）：https://github.com/alphardex/looter

工业级框架（scrapy）：https://github.com/scrapy/scrapy




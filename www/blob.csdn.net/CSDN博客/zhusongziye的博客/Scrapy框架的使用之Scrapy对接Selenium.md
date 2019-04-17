# Scrapy框架的使用之Scrapy对接Selenium - zhusongziye的博客 - CSDN博客





2018年05月16日 20:48:23[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：6044








Scrapy抓取页面的方式和requests库类似，都是直接模拟HTTP请求，而Scrapy也不能抓取JavaScript动态渲染的页面。抓取JavaScript渲染的页面有两种方式。一种是分析Ajax请求，找到其对应的接口抓取，Scrapy同样可以用此种方式抓取。另一种是直接用Selenium或Splash模拟浏览器进行抓取，我们不需要关心页面后台发生的请求，也不需要分析渲染过程，只需要关心页面最终结果即可，可见即可爬。那么，如果Scrapy可以对接Selenium，那Scrapy就可以处理任何网站的抓取了。

## 一、本节目标

本节我们来看看Scrapy框架如何对接Selenium，以PhantomJS进行演示。我们依然抓取淘宝商品信息，抓取逻辑和前文中用Selenium抓取淘宝商品完全相同。

## 二、准备工作

请确保PhantomJS和MongoDB已经安装好并可以正常运行，安装好Scrapy、Selenium、PyMongo库。

## 三、新建项目

首先新建项目，名为scrapyseleniumtest，命令如下所示：

**scrapy startproject scrapyseleniumtest**

新建一个Spider，命令如下所示：

**scrapy genspider taobao www.taobao.com**

修改`ROBOTSTXT_OBEY`为`False`，如下所示：

**ROBOTSTXT_OBEY = False**

## 四、定义 Item

首先定义`Item`对象，名为`ProductItem`，代码如下所示：

```python
from scrapy import Item, Field

class ProductItem(Item):

   collection = 'products'
   image = Field()
   price = Field()
   deal = Field()
   title = Field()
   shop = Field()
   location = Field()
```

这里我们定义了6个Field，也就是6个字段，跟之前的案例完全相同。然后定义了一个`collection`属性，即此Item保存的MongoDB的Collection名称。

初步实现Spider的`start_requests()`方法，如下所示：

```python
from scrapy import Request, Spider
from urllib.parse import quote
from scrapyseleniumtest.items import ProductItem

class TaobaoSpider(Spider):
   name = 'taobao'
   allowed_domains = ['www.taobao.com']
   base_url = 'https://s.taobao.com/search?q='

   def start_requests(self):
       for keyword in self.settings.get('KEYWORDS'):
           for page in range(1, self.settings.get('MAX_PAGE') + 1):
               url = self.base_url + quote(keyword)
               yield Request(url=url, callback=self.parse, meta={'page': page}, dont_filter=True)
```

首先定义了一个`base_url`，即商品列表的URL，其后拼接一个搜索关键字就是该关键字在淘宝的搜索结果商品列表页面。

关键字用`KEYWORDS`标识，定义为一个列表。最大翻页页码用`MAX_PAGE`表示。它们统一定义在setttings.py里面，如下所示：

KEYWORDS = ['iPad']

MAX_PAGE = 100

在`start_requests()`方法里，我们首先遍历了关键字，遍历了分页页码，构造并生成Request。由于每次搜索的URL是相同的，所以分页页码用`meta`参数来传递，同时设置`dont_filter`不去重。这样爬虫启动的时候，就会生成每个关键字对应的商品列表的每一页的请求了。

## 五、对接 Selenium

接下来我们需要处理这些请求的抓取。这次我们对接Selenium进行抓取，采用Downloader Middleware来实现。在Middleware里面的`process_request()`方法里对每个抓取请求进行处理，启动浏览器并进行页面渲染，再将渲染后的结果构造一个`HtmlResponse`对象返回。代码实现如下所示：

```python
from selenium import webdriver
from selenium.common.exceptions import TimeoutException
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from scrapy.http import HtmlResponse
from logging import getLogger

class SeleniumMiddleware():
   def __init__(self, timeout=None, service_args=[]):
       self.logger = getLogger(__name__)
       self.timeout = timeout
       self.browser = webdriver.PhantomJS(service_args=service_args)
       self.browser.set_window_size(1400, 700)
       self.browser.set_page_load_timeout(self.timeout)
       self.wait = WebDriverWait(self.browser, self.timeout)

   def __del__(self):
       self.browser.close()

   def process_request(self, request, spider):
       """
       用PhantomJS抓取页面
       :param request: Request对象
       :param spider: Spider对象
       :return: HtmlResponse
       """
       self.logger.debug('PhantomJS is Starting')
       page = request.meta.get('page', 1)
       try:
           self.browser.get(request.url)
           if page > 1:
               input = self.wait.until(
                   EC.presence_of_element_located((By.CSS_SELECTOR, '#mainsrp-pager div.form > input')))
               submit = self.wait.until(
                   EC.element_to_be_clickable((By.CSS_SELECTOR, '#mainsrp-pager div.form > span.btn.J_Submit')))
               input.clear()
               input.send_keys(page)
               submit.click()
           self.wait.until(EC.text_to_be_present_in_element((By.CSS_SELECTOR, '#mainsrp-pager li.item.active > span'), str(page)))
           self.wait.until(EC.presence_of_element_located((By.CSS_SELECTOR, '.m-itemlist .items .item')))
           return HtmlResponse(url=request.url, body=self.browser.page_source, request=request, encoding='utf-8', status=200)
       except TimeoutException:
           return HtmlResponse(url=request.url, status=500, request=request)

   @classmethod
   def from_crawler(cls, crawler):
       return cls(timeout=crawler.settings.get('SELENIUM_TIMEOUT'),
                  service_args=crawler.settings.get('PHANTOMJS_SERVICE_ARGS'))
```

首先我们在`__init__()`里对一些对象进行初始化，包括`PhantomJS`、`WebDriverWait`等对象，同时设置页面大小和页面加载超时时间。在`process_request()`方法中，我们通过Request的`meta`属性获取当前需要爬取的页码，调用PhantomJS对象的`get()`方法访问Request的对应的URL。这就相当于从Request对象里获取请求链接，然后再用PhantomJS加载，而不再使用Scrapy里的Downloader。

随后的处理等待和翻页的方法在此不再赘述，和前文的原理完全相同。最后，页面加载完成之后，我们调用PhantomJS的`page_source`属性即可获取当前页面的源代码，然后用它来直接构造并返回一个`HtmlResponse`对象。构造这个对象的时候需要传入多个参数，如`url`、`body`等，这些参数实际上就是它的基础属性。可以在官方文档查看`HtmlResponse`对象的结构：https://doc.scrapy.org/en/latest/topics/request-response.html。这样我们就成功利用PhantomJS来代替Scrapy完成了页面的加载，最后将Response返回即可。



有人可能会纳闷：为什么实现这么一个Downloader Middleware就可以了？之前的Request对象怎么办？Scrapy不再处理了吗？Response返回后又传递给了谁？

是的，Request对象到这里就不会再处理了，也不会再像以前一样交给Downloader下载。Response会直接传给Spider进行解析。

我们需要回顾一下Downloader Middleware的`process_request()`方法的处理逻辑，内容如下所示：

当`process_request()`方法返回Response对象的时候，更低优先级的Downloader Middleware的`process_request()`和`process_exception()`方法就不会被继续调用了，转而开始执行每个Downloader Middleware的`process_response()`方法，调用完毕之后直接将Response对象发送给Spider来处理。

这里直接返回了一个`HtmlResponse`对象，它是Response的子类，返回之后便顺次调用每个Downloader Middleware的`process_response()`方法。而在`process_response()`中我们没有对其做特殊处理，它会被发送给Spider，传给Request的回调函数进行解析。

到现在，我们应该能了解Downloader Middleware实现Selenium对接的原理了。

在settings.py里，我们设置调用刚才定义的`SeleniumMiddleware`，如下所示：

```python
DOWNLOADER_MIDDLEWARES = {
   'scrapyseleniumtest.middlewares.SeleniumMiddleware': 543,
}
```

## 六、解析页面

Response对象就会回传给Spider内的回调函数进行解析。所以下一步我们就实现其回调函数，对网页来进行解析，代码如下所示：

```python
def parse(self, response):
   products = response.xpath(
       '//div[@id="mainsrp-itemlist"]//div[@class="items"][1]//div[contains(@class, "item")]')
   for product in products:
       item = ProductItem()
       item['price'] = ''.join(product.xpath('.//div[contains(@class, "price")]//text()').extract()).strip()
       item['title'] = ''.join(product.xpath('.//div[contains(@class, "title")]//text()').extract()).strip()
       item['shop'] = ''.join(product.xpath('.//div[contains(@class, "shop")]//text()').extract()).strip()
       item['image'] = ''.join(product.xpath('.//div[@class="pic"]//img[contains(@class, "img")]/@data-src').extract()).strip()
       item['deal'] = product.xpath('.//div[contains(@class, "deal-cnt")]//text()').extract_first()
       item['location'] = product.xpath('.//div[contains(@class, "location")]//text()').extract_first()
       yield item
```

在这里我们使用XPath进行解析，调用`response`变量的`xpath()`方法即可。首先我们传递选取所有商品对应的XPath，可以匹配所有商品，随后对结果进行遍历，依次选取每个商品的名称、价格、图片等内容，构造并返回一个`ProductItem`对象。

## 七、存储结果

最后我们实现一个Item Pipeline，将结果保存到MongoDB，如下所示：

```python
import pymongo

class MongoPipeline(object):
   def __init__(self, mongo_uri, mongo_db):
       self.mongo_uri = mongo_uri
       self.mongo_db = mongo_db

   @classmethod
   def from_crawler(cls, crawler):
       return cls(mongo_uri=crawler.settings.get('MONGO_URI'), mongo_db=crawler.settings.get('MONGO_DB'))

   def open_spider(self, spider):
       self.client = pymongo.MongoClient(self.mongo_uri)
       self.db = self.client[self.mongo_db]

   def process_item(self, item, spider):
       self.db[item.collection].insert(dict(item))
       return item

   def close_spider(self, spider):
       self.client.close()
```

此实现和前文中存储到MongoDB的方法完全一致，原理不再赘述。记得在settings.py中开启它的调用，如下所示：

ITEM_PIPELINES = {
'scrapyseleniumtest.pipelines.MongoPipeline': 300,

}

其中，`MONGO_URI`和`MONGO_DB`的定义如下所示：

MONGO_URI = 'localhost'

MONGO_DB = 'taobao'

## 八、运行

整个项目就完成了，执行如下命令启动抓取即可：

scrapy crawl taobao

运行结果如下图所示。

![](https://img-blog.csdn.net/20180516204749337?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

查看MongoDB，结果如下图所示。

![](https://img-blog.csdn.net/20180516204759542?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这样我们便成功在Scrapy中对接Selenium并实现了淘宝商品的抓取。

## 九、本节代码

本节代码地址为：https://github.com/Python3WebSpider/ScrapySeleniumTest。

## 十、结语

我们通过实现Downloader Middleware的方式实现了Selenium的对接。但这种方法其实是阻塞式的，也就是说这样就破坏了Scrapy异步处理的逻辑，速度会受到影响。为了不破坏其异步加载逻辑，我们可以使用Splash实现。




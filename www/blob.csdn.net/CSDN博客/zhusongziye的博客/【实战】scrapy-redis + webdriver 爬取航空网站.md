# 【实战】scrapy-redis + webdriver 爬取航空网站 - zhusongziye的博客 - CSDN博客





2018年12月26日 20:51:31[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：173








引言

今天给大家带来的是scrapy-redis + webdriver实战案例。在爬虫编写过程中，我们经常会遇到以下的情况，想要用scrapy框架，但是因为网站的原因，还想要用webdriver，那么要如何实现scrapy + webdriver呢？其实很简单，大家都知道，在scrapy中，我们可以在中间件中扩展自己想要的功能，所以webdriver也是可以添加在中间件中的。



需求分析

爬取东航网站中航班信息（http://www.ceair.com/）。分析网站后可以发现，在查询航班信息的时候，网站做了接口的调用，我们可以采用分析接口的方式，分析接口，然后伪造接口数据爬取结果，但这和我们这次的主题不一致，暂时先放弃。我们这次采用webdriver的方式爬取航班信息。



下载中间件的编写

其实整个项目主要是在编写下载中间件。下面是下载中间件的内容。

```
class SeleniumMiddleware():
    # Middleware中会传递进来一个spider，这就是我们的spider对象，从中可以获取__init__时的chrome相关元素
    def process_request(self, request, spider):
        '''
        用chrome抓取页面
        :param request: Request请求对象
        :param spider: Spider对象
        :return: HtmlResponse响应
        '''
        print(f"chrome is getting page")

        # 初始化ChromeOption
        option = webdriver.ChromeOptions()
        option.add_argument('headless') # 无头模式
        # 添加UA
        option.add_argument(
            '--user-agent="Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.110 Safari/537.36"')
        option.add_argument('--disable-infobars') # 禁用webdriver控制浏览器的提示

        # 创建driver实例
        driver = webdriver.Chrome(chrome_options=option)

        # 访问目标网址
        driver.get(request.url)

        # 等待加载完成，这里应该写成动态加载，由于是演示，就偷懒了
        time.sleep(5)

        body = driver.page_source

        # 关闭driver
        driver.quit()

        # 通过page_source构建新的response病返回
        return HtmlResponse(url=request.url,
                            body=body,
                            request=request,
                            # 最好根据网页的具体编码而定
                            encoding='utf-8',
                            status=200)
```



在下载中间件中，我们使用webdriver访问url地址，并将page_source构建成为新的response返回给spider。



其他部分



完成了中间件的编写之后，我们还需要完善一下其他部分。首先是要在settings文件中打开下载中间件，如果不打开中间件的话，虽然我们编写了下载中间件，但是却不会调用。

```
DOWNLOADER_MIDDLEWARES = {
    # 'example.middlewares.UserAgentMiddleware':543,
    'example.middlewares.SeleniumMiddleware':543,
}
```



在setting文件中设置好中间件后，我们需要完善一下spider文件。这里将分析省略，直接上代码。

```
from scrapy_redis.spiders import RedisSpider
from bs4 import BeautifulSoup
from example.items import ExampleItem
import time


class MySpider(RedisSpider):
    """Spider that reads urls from redis queue (myspider:start_urls)."""
    name = 'ceair'
    redis_key = 'ceair'

    def __init__(self, *args, **kwargs):
        # Dynamically define the allowed domains list.
        domain = kwargs.pop('domain', '')
        self.allowed_domains = filter(None, domain.split(','))
        super(MySpider, self).__init__(*args, **kwargs)

    def parse(self, response):

        html_body = BeautifulSoup(response.text,'lxml')
        flights = html_body.find_all(class_='flight')
        for flight_info in flights:
            item = ExampleItem()
            title = flight_info.find(class_='title')
            item['title'] = (title.get_text())

            start_info = flight_info.find(class_='airport r')
            item['start_info'] = (start_info.get_text())

            middle_info = flight_info.find(class_='mid')
            item['middle_info'] = (middle_info.get_text())

            end_info = flight_info.find(class_='airport')
            item['end_info'] = (end_info.get_text())

            cost_time = flight_info.find('dfn')
            item['cost_time'] = (cost_time.get_text())

            price_luxury = flight_info.find(class_='luxury')
            item['price_luxury'] = (price_luxury.get_text())

            price_economy = flight_info.find(class_='economy')
            item['price_economy'] = (price_economy.get_text())

            price_member = flight_info.find(class_='member')
            item['price_member'] = (price_member.get_text())

            item['update_time'] = time.strftime('%Y.%m.%d %H:%M:%S',time.localtime(time.time()))

            yield item
```



管道文件。同样省略讲解。

```
# Define your item pipelines here
#
# Don't forget to add your pipeline to the ITEM_PIPELINES setting
# See: http://doc.scrapy.org/topics/item-pipeline.html
from datetime import datetime
import pymongo

class ExamplePipeline(object):
    def process_item(self, item, spider):
        curr = pymongo.MongoClient()
        db = curr['ceair']
        collection = db['ceair']
        collection.insert_one(item)
        curr.close()
        print(item)
```



最后，为了方便运行爬虫，我们可以创建一个main文件，这样就不用每次都在命令行中敲命令了。下面是main文件内容。

```
from scrapy import cmdline
cmdline.execute("scrapy crawl ceair".split())
```

总结

这里我们只是写了分布式爬虫的从端文件，从端文件只负责爬取并存储内容，我们还需要编写主端文件，需要分析并构建url，并将url存储在redis数据库中。这里就不做详细介绍了。

源码：

链接:https://pan.baidu.com/s/1LAR3K0UhefPi-8s1TxUCZA  密码:jvx8



*Python软件开发测试交友群QQ:952490269（加群备注software），欢迎加入！*




# Python分布式爬虫详解（二） - zhusongziye的博客 - CSDN博客





2018年10月20日 16:07:59[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：106标签：[Python分布式爬虫详解（二）](https://so.csdn.net/so/search/s.do?q=Python分布式爬虫详解（二）&t=blog)
个人分类：[Python爬虫](https://blog.csdn.net/zhusongziye/article/category/7611456)









上一章[Python分布式爬虫详解（一）](http://mp.weixin.qq.com/s?__biz=MzAxMjUyNDQ5OA==&mid=2653557427&idx=1&sn=d414fea650540093795f568ec2c925bc&chksm=806e3c0eb719b5187764a1146d41087823c5e5ae04707981cd41025c3b724af4aa1691cb68fd&scene=21#wechat_redirect)简单的介绍了什么是分布式爬虫，废话不多说，本章开始从零搭建一个爬取电影天堂电影信息的分布式爬虫。

**本章知识点：**

a.CrawlSpider爬取电影天堂动作片第一页数据

b.将项目改为RedisCrawlSpider爬虫

**一、CrawlSpider爬虫**

要想搭建分布式爬虫，我们可以写一个普通的爬虫，然后对其修改，使之成为分布式爬虫。

首先，新建`CrawlSpider`项目：

```
scrapy genspider -t crawl dytt_slaver dy2018.com
```

1、定义爬取字段

```
import scrapy


class DyttRedisSlaverItem(scrapy.Item):
    # 译名
    name = scrapy.Field()
    # 年代
    year = scrapy.Field()
    # 语言
    language = scrapy.Field()
    # 上映日期
    release_date = scrapy.Field()
    # 评分
    score = scrapy.Field()
    # 文件大小
    file_size = scrapy.Field()
    # 片长
    film_time = scrapy.Field()
    # 简介
    introduction = scrapy.Field()
    # 海报
    posters = scrapy.Field()
    # 下载链接
    download_link = scrapy.Field()
```

2、定义Rule规则

查看网页源码发现，电影链接为`/i/[一串数字].html`的形式，但是我们只需要类目中的电影而不需要推荐的电影：

![](https://img-blog.csdn.net/20181020160544428?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

所以：

```
movie_links = LinkExtractor(allow=r'/i/\d*.html', restrict_xpaths=('//div[@class="co_content8"]'))

    rules = (
        Rule(movie_links, callback='parse_item'),
    )
```

3、定义提取影片信息规则

观察网页源码，发现页面结构并不是统一的：

![](https://img-blog.csdn.net/20181020160556710?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20181020160606343?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

有的信息在p标签中包裹，但是也有一些信息在div标签中。而且，由于主演人数不同，标签个数也不确定。所以，用xpath进行提取不是那么的方便了。这种情况就需要选用正则表达式来进行筛选。

观察网页编码，为`gb2312`

![](https://img-blog.csdn.net/20181020160618522?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

有些小众的电影并没有评分。

所以，筛选规则为：

```
str_resp = response.body.decode('gb2312', errors='ignore')
        rep_chars = [' ', '·', '“', '”', '…']
        for rep in rep_chars:
            str_resp = str_resp.replace(rep, '')

        title = re.search(r'◎片　　名(.*?)</.+>', str_resp).group(1).replace(u'\u3000', '')
        translation = re.search(r'◎译　　名(.*?)</.+>', str_resp).group(1).replace(u'\u3000', '')
        # 名字
        items['name'] = title + "|" + translation
        # 年代
        items['year'] = re.search(r'◎年　　代(.*?)</.+>', str_resp).group(1).replace(u'\u3000', '')
        # 评分
        try:
            items['score'] = response.xpath("//strong[@class='rank']/text()").extract()[0].replace(u'\u3000', '')
        except:
            items['score'] = '无评分'
            # 语言
        items['language'] = re.search(r'◎语　　言(.*?)</.+>', str_resp).group(1).replace(u'\u3000', '')
        # 上映日期
        items['release_date'] = re.search(r'◎上映日期(.*?)</.+>', str_resp).group(1).replace(u'\u3000', '')
        # 文件大小
        items['file_size'] = re.search(r'◎文件大小(.*?)</.+>', str_resp).group(1).replace(u'\u3000', '')
        # 片长
        items['film_time'] = re.search(r'◎片　　长(.*?)</.+>', str_resp).group(1).replace(u'\u3000', '')
        # 简介
        items['introduction'] = re.search(r'◎简　　介</.+>\r\n<.+>(.*?)</.+>', str_resp).group(1).replace(u'\u3000', '')
        # 海报
        items['posters'] = response.xpath("//div[@id='Zoom']/*[1]/img/@src").extract()[0]
```

经测试发现，网站的迅雷下载链接是用js动态生成的。这就要用到selenium了。

```
from selenium import webdriver

# 下载链接
items['download_link'] = self.get_download_link(response.url)

def get_download_link(self, url):
        chrome_options = webdriver.ChromeOptions()
        chrome_options.add_argument('--headless')
        chrome_options.add_argument('--disable-gpu')
        driver = webdriver.Chrome(chrome_options=chrome_options)
        driver.get(url)
        link = re.search(r'\"(thunder:.*?)\"',  driver.page_source).group(1)
        driver.close()
        return link
```

最后，pipelines中保存数据：

```
class DyttRedisSlaverPipeline(object):
    def __init__(self):
        self.file = open('movie.json', 'w')

    def process_item(self, item, spider):
        content = json.dumps(dict(item), ensure_ascii=False) + "\n"
        self.file.write(content)
        return item

    def close_spider(self, spider):
        self.file.close()
```

运行爬虫，得到第一页的30条数据：

![](https://img-blog.csdn.net/2018102016063434?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**二、修改项目为RedisCrawlSpider爬虫**

1、首先修改爬虫文件

① RedisCrawlSpider修改很简单，首先需要引入RedisCrawlSpider：

```
from scrapy_redis.spiders import RedisCrawlSpider
```

② 将父类中继承的`CrawlSpider`改为继承`RedisCrawlSpider`：

```
class DyttSlaverSpider(RedisCrawlSpider):
```

③ 因为slaver端要从redis数据库中获取爬取的链接信息，所以去掉`allowed_domains()` 和 `start_urls`，并添加`redis_key`

```
redis_key = 'dytt:start_urls'
```

④ 增加`__init__()`方法，动态获取`allowed_domains()`，[理论上要加这个，但是实测加了爬取的时候链接都被过滤了，所以我没加，暂时没发现有什么影响]

```
def __init__(self, *args, **kwargs):
         domain = kwargs.pop('domain', '')
         self.allowed_domains = filter(None, domain.split(','))
         super(DyttSlaverSpider, self).__init__(*args, **kwargs)
```

2、修改setting文件

① 首先要指定redis数据库的连接参数：

```
REDIS_HOST = '192.168.0.131'
REDIS_PORT = 6379
```

② 指定使用`scrapy-redis`的调度器

```
SCHEDULER = "scrapy_redis.scheduler.Scheduler"
```

③ 指定使用`scrapy-redis`的去重

```
DUPEFILTER_CLASS = 'scrapy_redis.dupefilter.RFPDupeFilter'
```

④ 指定排序爬取地址时使用的队列

```
# 默认的 按优先级排序(Scrapy默认)，由sorted set实现的一种非FIFO、LIFO方式。
SCHEDULER_QUEUE_CLASS = 'scrapy_redis.queue.SpiderPriorityQueue'
# 可选的 按先进先出排序（FIFO）
# SCHEDULER_QUEUE_CLASS = 'scrapy_redis.queue.SpiderQueue'
# 可选的 按后进先出排序（LIFO）
# SCHEDULER_QUEUE_CLASS = 'scrapy_redis.queue.SpiderStack'
```

⑤ 设置断点续传，也就是不清理redis queues

```
SCHEDULER_PERSIST = True
```

⑥ 默认情况下,`RFPDupeFilter`只记录第一个重复请求。将`DUPEFILTER_DEBUG`设置为`True`会记录所有重复的请求。

```
DUPEFILTER_DEBUG =True
```

⑦ 配置`RedisPipeline`将`item`写入`key`为 `spider.name : items` 的redis的list中，供后面的分布式处理item

```
ITEM_PIPELINES = {
   'dytt_redis_slaver.pipelines.DyttRedisSlaverPipeline': 300,
   'scrapy_redis.pipelines.RedisPipeline': 400
}
```

3、增加爬虫信息字段（可选）

由于会有多个slaver端，所以可加一个爬虫名字的字段和时间字段来区分是哪个爬虫在什么时间爬到的信息。

① item中增加字段

```
# utc时间
    crawled = scrapy.Field()
    # 爬虫名
    spider = scrapy.Field()
```

② pipelines中新增类：

```
class InfoPipeline(object):

    def process_item(self, item, spider):
        #utcnow() 是获取UTC时间
        item["crawled"] = datetime.utcnow()
        # 爬虫名
        item["spider"] = spider.name
        return item
```

③ setting中设置ITEM_PIPELINES

```
ITEM_PIPELINES = {
   'dytt_redis_slaver.pipelines.DyttRedisSlaverPipeline': 300,
   'dytt_redis_slaver.pipelines.InfoPipeline':350,
   'scrapy_redis.pipelines.RedisPipeline': 400
}
```

至此，项目修改完毕，现在可以爬取某一分类下的第一页的电影信息。

以Windows10为slaver端运行一下：

![](https://img-blog.csdn.net/20181020160653715?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

因为请求队列为空，所以爬虫会停下来进行监听，直到我们在Master端给它一个新的连接：

![](https://img-blog.csdn.net/20181020160703412?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

爬虫启动，开始爬取信息：

![](https://img-blog.csdn.net/2018102016071226?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

爬取完成后，项目不会结束，而是继续等待新的爬取请求的到来，爬取结果：

![](https://img-blog.csdn.net/20181020160725419?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**本章小结：**

本章将一个crawlspider爬虫改为了RedisCrawlSpider爬虫，可以实现分布式爬虫，但是由于数据量较小（只有30条）所以只用了一个slaver端。并且没有去设置代理ip和user-agent，下一章中，针对上述问题，将对项目进行更深一步的修改。

**项目源码：**
`https://github.com/ZhiqiKou/Scrapy_notes`



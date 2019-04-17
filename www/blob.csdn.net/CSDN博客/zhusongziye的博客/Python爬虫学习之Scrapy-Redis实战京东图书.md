# Python爬虫学习之Scrapy-Redis实战京东图书 - zhusongziye的博客 - CSDN博客





2018年09月17日 21:44:57[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：295










什么是scrapy-redis?

redis-based components for scrapy

scrapy-Redis就是结合了分布式数据库redis，重写了scrapy一些比较关键的代码，将scrapy变成一个可以在多个主机上同时运行的分布式爬虫。

scrapy-redis是github上的一个开源项目，可以直接下载到他的源代码：

> 
https://github.com/rolando/scrapy-redis


### scrapy-redis 详解

scrapy-redis流程图

![](https://img-blog.csdn.net/20180917214159921?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



redis的使用

参考前文写的redis交互使用：[Python | Python学习之Redis交互详解](http://mp.weixin.qq.com/s?__biz=MzIwNDI1NjUxMg==&mid=2651261225&idx=1&sn=d82fe2087574f8c397ea198572b2aee3&chksm=8d314d46ba46c450702ab9033b8cf3117f818400e6c9c9c1f81912379ed59d1b809c5ada6803&scene=21#wechat_redirect)

scrapy-redis example-project

scrapy-redis的源码中提供了scrapy-redis的示例项目,我们下载下来学习一下。

> 
https://github.com/rmax/scrapy-redis/tree/master/example-project


在`example-project`中有三个demo,分别是`dmoz`,`myspider_redis`,以及`mycrawler_redis`。

本次主要是对`dmoz`这个demo进行学习和实战练习。

dmoz spider文件解析

```
from scrapy.linkextractors import LinkExtractor
from scrapy.spiders import CrawlSpider, Rule


class DmozSpider(CrawlSpider):
    """Follow categories and extract links."""
    name = 'dmoz'
    allowed_domains = ['dmoztools.net']
    start_urls = ['http://dmoztools.net/']

    rules = [
        Rule(LinkExtractor(
            restrict_css=('.top-cat', '.sub-cat', '.cat-item')
        ), callback='parse_directory', follow=True),
    ]

    def parse_directory(self, response):
        for div in response.css('.title-and-desc'):
            yield {
                'name': div.css('.site-title::text').extract_first(),
                'description': div.css('.site-descr::text').extract_first().strip(),
                'link': div.css('a::attr(href)').extract_first(),
            }
```

可以看到,dmoz项目和我们平时创建的scrapy项目并没有太大的区别,之所以能够实现持久化爬虫主要的不同之处在setting中设置了去重类和scheduler队列。

dmoz setting文件解析

上面提到的setting中设置了去重类和scheduler队列的操作主要就是在setting文件中添加下面这些代码。

```
# 去重类--指定哪个去重方法给request对象去重
DUPEFILTER_CLASS = "scrapy_redis.dupefilter.RFPDupeFilter"
# 队列--指定scheduler队列，调度器内存的是待爬取链接和已爬取对象指纹。
SCHEDULER = "scrapy_redis.scheduler.Scheduler"
# 队列内容是否持久化保存--为False的时候，关闭redis的时候清空redis
SCHEDULER_PERSIST = True
REDIS_URL="redis://127.0.0.1:6379"
```

dmoz redis 数据库存取项

我们运行一下这个示例项目,并打开redis数据库,查看爬取到的结果。redis数据库中出现以下三个键，分别是：

dmoz request： 待爬取项

(先把爬取对象序列化存入数据库，再反序列化成爬取对，Scheduler队列，存放的待请求的request对象，获取的过程是pop操作，即获取一个会去除一个)

dmoz items：爬取的内容

(通过scrapy_redis.pipelines.RedisPipeline保存,屏蔽之后可以实现自定义对象存取位置,存放的获取到的item信息，在pipeline中开启RedisPipeline才会存入)

dmoz dumpfilter：抓到过的request对象指纹

(指纹集合，存放的是已经进入scheduler队列的request对象的指纹，指纹默认由请求方法，url和请求体组成)

dumpfilter的数量减去request的数量是已经抓爬取过的数量

关闭redispipeline之后,redis数据库中数据量变化：
- 
dmoz:requests 有变化(变多或者变少或者不变)

- 
dmoz:dupefilter 变多

- 
dmoz:items 不变


redispipeline中仅仅实现了item数据存储到redis的过程,我们可以新建一个pipeline（或者修改默认的ExamplePipeline）,让数据存储到任意地方,但是权重应该小于redis存储的pipline。

scrapy-redis 源码详解

scrapy redis 如何生成指纹的？

```
import hashlib
f = hashlib.hsa1()
f.update(url.encode())
f.hexdigest()
```

scrapy-redis 判断request对象是否入队

```
def enqueue_request(self, request):
    if not request.dont_filter and self.df.request_seen(request):
      # dont_filter=False Ture  True request指纹已经存在  #不会入队
      # dont_filter=False Ture  False  request指纹已经存在 全新的url  #会入队
      # dont_filter=Ture False  #会入队
        self.df.log(request, self.spider)
        return False
    if self.stats:
        self.stats.inc_value('scheduler/enqueued/redis', spider=self.spider)
    self.queue.push(request)
    return True
```
- 
dont_filter = True ,构造请求的时候，把dont_filter置为True，该url会被反复抓取（url地址对应的内容会更新的情况）

- 
一个全新的url地址被抓到的时候，构造request请求

- 
url地址在start_urls中的时候，会入队，不管之前是否请求过

- 
构造start_url地址的请求时候，dont_filter = True


scrapy-redis如何去重

```
fp = hashlib.sha1()
fp.update(to_bytes(request.method)) #请求方法
fp.update(to_bytes(canonicalize_url(request.url)))  #请求链接
fp.update(request.body or b'')  # 请求体
return fp.hexdigest()
```
- 
使用sha1加密request得到指纹

- 
把指纹存在redis的集合中

- 
下一次新来一个request，同样的方式生成指纹，判断指纹是否存在reids的集合中


判断数据是否存在redis的集合中，不存在插入

```
added = self.server.sadd(self.key, fp)
return added != 0
```

### scrapy-redis实战京东图书

爬取结果截图

![](https://img-blog.csdn.net/20180917214223635?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



页面分析

分析分类聚合页

打开待爬取页面：

`https://book.jd.com/booksort.html`

如下图：

![](https://img-blog.csdn.net/20180917214235519?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

分析分类聚合页

查看页面源代码,发现待爬取的内容存在其中,所以我们可以通过分析源码写出提取相应字段的xpath。

```
def parse(self, response):
        dl_list = response.xpath("//div[@class='mc']/dl/dt")
        for dl in dl_list:
            item = JdbookspiderItem()
            item['book_sort'] = dl.xpath("./a/text()").extract_first()
            em_list = dl.xpath("./following-sibling::dd/em")
            for em in em_list:
                item['book_cate'] = em.xpath("./a/text()").extract_first()
                item['book_cate_url'] = em.xpath("./a/@href").extract_first()
                if item['book_cate_url'] is not None:
                    item['book_cate_url'] = 'https:' + item['book_cate_url']
                yield scrapy.Request(
                    item['book_cate_url'],
                    callback=self.parse_cate_url,
                    meta={"item": deepcopy(item)}
                )
```

通过抓取分类页面分类链接,我们可以爬取到分类下的书籍列表页,这个页面包含书籍的全部信息,同样是可以使用xpath解析出来的。

分析书籍列表页

通过分析列表页的请求,可以发下列表页的请求重定向到了一个新的链接,所以我们只需要分析新请求链接的响应即可,scrapy可以自动帮我们执行重定向的操作。

![](https://img-blog.csdn.net/20180917214251619?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

页面重定向分析

分析书籍列表页,可以发现列表页除了价格字段外其余字段都可以在链接的响应中提取到。

![](https://img-blog.csdn.net/20180917214313152?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

书籍列表页分析

所以我们只需要找到价格字段的请求,就可以爬取到书籍的全部字段了。我们可以直接在相应中查找价格以查看是否有相关的响应。

![](https://img-blog.csdn.net/20180917214323805?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

书籍价格分析1

查找结果如上所示,根据我们查找到的结果,我试着请求一下对应的链接,很幸运这里返回的是json字符串。

![](https://img-blog.csdn.net/20180917214406121?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

书籍价格分析2

根据以上分析我们可以写出相应的分析代码：

```
def parse_cate_url(self, response):
        item = response.meta["item"]
        li_list = response.xpath("//div[@id='plist']/ul/li")
        for li in li_list:
            item['book_img'] = li.xpath(".//div[@class='p-img']//img/@src").extract_first()
            if item['book_img'] is None:
                item['book_img'] = li.xpath(".//div[@class='p-img']//img/@data-lazy-img").extract_first()
            item['book_img'] = "https:" + item['book_img'] if item['book_img'] is not None else None
            item['book_name'] = li.xpath(".//div[@class='p-name']/a/em/text()").extract_first().strip()
            item['book_author'] = li.xpath(".//span[@class='author_type_1']/a/text()").extract_first()
            item['publish_time'] = li.xpath(".//span[@class='p-bi-date']/text()").extract_first().strip()
            item['book_store'] = li.xpath(".//span[@class='p-bi-store']/a/@title").extract_first().strip()
            item['book_sku'] = li.xpath("./div/@data-sku").extract_first()
            yield scrapy.Request(
                'https://p.3.cn/prices/mgets?skuIds=J_{}'.format(item['book_sku']),
                callback=self.parse_book_price,
                meta={"item": deepcopy(item)}
            )
    def parse_book_price(self, response):
        item = response.meta["item"]
        item['book_price'] = json.loads(response.body.decode())[0]["op"]
        yield item
```

构建下一页请求

这里就比较简单,对比最后一页与其他页,发现最后一页是没有下一页链接的,根据以上分析可以成功构建相应代码：

```
# 下一页地址构建
        next_url = response.xpath("//a[@class='pn-next']/@href")
        if next_url:
            next_url = urllib.parse.join(response.url, next_url)
            yield scrapy.Request(
                next_url,
                callback=self.parse_cate_url,
                meta={"item": item}
            )
```

数据入库

```
class JdbookspiderPipeline(object):
    def process_item(self, item, spider):
        if isinstance(item, JdbookspiderItem):
            print(item)
            collection.insert(dict(item))
        return item
```

实现持久化爬虫

在setting文件中添加去重类和scheduler队列,同时修改redis数据库链接,并保证数据库是可用的。

```
DUPEFILTER_CLASS = "scrapy_redis.dupefilter.RFPDupeFilter"
SCHEDULER = "scrapy_redis.scheduler.Scheduler"
SCHEDULER_PERSIST = True
REDIS_URL = "redis://127.0.0.1:6379"

# 同时为保证能够把数据同时存储到mongodb中我们还要设置相关pipline
ITEM_PIPELINES = {
   'jdbookSpider.pipelines.JdbookspiderPipeline': 500,
   'scrapy_redis.pipelines.RedisPipeline': 400
}
```






# 从零开始的 Python 爬虫速成指南 - zhusongziye的博客 - CSDN博客





2018年06月10日 18:23:27[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：230标签：[从零开始的 Python 爬虫速成指南](https://so.csdn.net/so/search/s.do?q=从零开始的 Python 爬虫速成指南&t=blog)
个人分类：[Python爬虫](https://blog.csdn.net/zhusongziye/article/category/7611456)









**序**




本文主要内容：以最短的时间写一个最简单的爬虫，可以抓取论坛的帖子标题和帖子内容。




本文受众：没写过爬虫的萌新。




**入门**




0.准备工作




需要准备的东西： Python、scrapy、一个IDE或者随便什么文本编辑工具。




1.技术部已经研究决定了，你来写爬虫。




随便建一个工作目录，然后用命令行建立一个工程，工程名为miao，可以替换为你喜欢的名字。
`scrapy startproject miao`
随后你会得到如下的一个由scrapy创建的目录结构




![](https://img-blog.csdn.net/20180610182244602?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




在spiders文件夹中创建一个python文件，比如miao.py，来作为爬虫的脚本。

内容如下：

```
import scrapy


class NgaSpider(scrapy.Spider):
    name = "NgaSpider"
    host = "http://bbs.ngacn.cc/"
    # start_urls是我们准备爬的初始页
    start_urls = [
        "http://bbs.ngacn.cc/thread.php?fid=406",
    ]

    # 这个是解析函数，如果不特别指明的话，scrapy抓回来的页面会由这个函数进行解析。
    # 对页面的处理和分析工作都在此进行，这个示例里我们只是简单地把页面内容打印出来。
    def parse(self, response):
        print response.body
```

2.跑一个试试？




如果用命令行的话就这样：

```
cd miao
scrapy crawl NgaSpider
```

你可以看到爬虫君已经把你坛星际区第一页打印出来了，当然由于没有任何处理，所以混杂着html标签和js脚本都一并打印出来了。




解析




接下来我们要把刚刚抓下来的页面进行分析，从这坨html和js堆里把这一页的帖子标题提炼出来。

其实解析页面是个体力活，方法多的是，这里只介绍xpath。




0.为什么不试试神奇的xpath呢




看一下刚才抓下来的那坨东西，或者用chrome浏览器手动打开那个页面然后按F12可以看到页面结构。

每个标题其实都是由这么一个html标签包裹着的。举个例子：
`<a id="t_tt1_33" class="topic" href="/read.php?tid=10803874">[合作模式] 合作模式修改设想</a>`
可以看到href就是这个帖子的地址（当然前面要拼上论坛地址），而这个标签包裹的内容就是帖子的标题了。

于是我们用xpath的绝对定位方法，把class='topic'的部分摘出来。




1.看看xpath的效果




在最上面加上引用：
`from scrapy import Selector`
把parse函数改成：

```
def parse(self, response):
        selector = Selector(response)
        # 在此，xpath会将所有class=topic的标签提取出来，当然这是个list
        # 这个list里的每一个元素都是我们要找的html标签
        content_list = selector.xpath("//*[@class='topic']")
        # 遍历这个list，处理每一个标签
        for content in content_list:
            # 此处解析标签，提取出我们需要的帖子标题。
            topic = content.xpath('string(.)').extract_first()
            print topic
            # 此处提取出帖子的url地址。
            url = self.host + content.xpath('@href').extract_first()
            print url
```

再次运行就可以看到输出你坛星际区第一页所有帖子的标题和url了。




**递归**




接下来我们要抓取每一个帖子的内容。




这里需要用到python的yield。
`yield Request(url=url, callback=self.parse_topic)`
此处会告诉scrapy去抓取这个url，然后把抓回来的页面用指定的parse_topic函数进行解析。




至此我们需要定义一个新的函数来分析一个帖子里的内容。




完整的代码如下：

```
import scrapy
from scrapy import Selector
from scrapy import Request


class NgaSpider(scrapy.Spider):
    name = "NgaSpider"
    host = "http://bbs.ngacn.cc/"
    # 这个例子中只指定了一个页面作为爬取的起始url
    # 当然从数据库或者文件或者什么其他地方读取起始url也是可以的
    start_urls = [
        "http://bbs.ngacn.cc/thread.php?fid=406",
    ]

    # 爬虫的入口，可以在此进行一些初始化工作，比如从某个文件或者数据库读入起始url
    def start_requests(self):
        for url in self.start_urls:
            # 此处将起始url加入scrapy的待爬取队列，并指定解析函数
            # scrapy会自行调度，并访问该url然后把内容拿回来
            yield Request(url=url, callback=self.parse_page)

    # 版面解析函数，解析一个版面上的帖子的标题和地址
    def parse_page(self, response):
        selector = Selector(response)
        content_list = selector.xpath("//*[@class='topic']")
        for content in content_list:
            topic = content.xpath('string(.)').extract_first()
            print topic
            url = self.host + content.xpath('@href').extract_first()
            print url
            # 此处，将解析出的帖子地址加入待爬取队列，并指定解析函数
            yield Request(url=url, callback=self.parse_topic)
         # 可以在此处解析翻页信息，从而实现爬取版区的多个页面

    # 帖子的解析函数，解析一个帖子的每一楼的内容
    def parse_topic(self, response):
        selector = Selector(response)
        content_list = selector.xpath("//*[@class='postcontent ubbcode']")
        for content in content_list:
            content = content.xpath('string(.)').extract_first()
            print content
        # 可以在此处解析翻页信息，从而实现爬取帖子的多个页面
```

到此为止，这个爬虫可以爬取你坛第一页所有的帖子的标题，并爬取每个帖子里第一页的每一层楼的内容。

爬取多个页面的原理相同，注意解析翻页的url地址、设定终止条件、指定好对应的页面解析函数即可。




**Pipelines——管道**




此处是对已抓取、解析后的内容的处理，可以通过管道写入本地文件、数据库。




0.定义一个Item




在miao文件夹中创建一个items.py文件。

```
from scrapy import Item, Field


class TopicItem(Item):
    url = Field()
    title = Field()
    author = Field()  

class ContentItem(Item):
    url = Field()
    content = Field()
    author = Field()
```

此处我们定义了两个简单的class来描述我们爬取的结果。




1. 写一个处理方法




在miao文件夹下面找到那个pipelines.py文件，scrapy之前应该已经自动生成好了。




我们可以在此建一个处理方法。

```
from scrapy import Item, Field


class TopicItem(Item):
    url = Field()
    title = Field()
    author = Field()  

class ContentItem(Item):
    url = Field()
    content = Field()
    author = Field()
```

2.在爬虫中调用这个处理方法。




要调用这个方法我们只需在爬虫中调用即可，例如原先的内容处理函数可改为：

```
def parse_topic(self, response):
        selector = Selector(response)
        content_list = selector.xpath("//*[@class='postcontent ubbcode']")
        for content in content_list:
            content = content.xpath('string(.)').extract_first()
            ## 以上是原内容
            ## 创建个ContentItem对象把我们爬取的东西放进去
            item = ContentItem()
            item["url"] = response.url
            item["content"] = content
            item["author"] = "" ## 略
            ## 这样调用就可以了
            ## scrapy会把这个item交给我们刚刚写的FilePipeline来处理
            yield item
```

3.在配置文件里指定这个pipeline




找到settings.py文件，在里面加入

```
ITEM_PIPELINES = {
            'miao.pipelines.FilePipeline': 400,
        }
```

这样在爬虫里调用
`yield item`
的时候都会由经这个FilePipeline来处理。后面的数字400表示的是优先级。

可以在此配置多个Pipeline，scrapy会根据优先级，把item依次交给各个item来处理，每个处理完的结果会传递给下一个pipeline来处理。




可以这样配置多个pipeline:

```
ITEM_PIPELINES = {
            'miao.pipelines.Pipeline00': 400,
            'miao.pipelines.Pipeline01': 401,
            'miao.pipelines.Pipeline02': 402,
            'miao.pipelines.Pipeline03': 403,
            ## ...
        }
```

**Middleware——中间件**




通过Middleware我们可以对请求信息作出一些修改，比如常用的设置UA、代理、登录信息等等都可以通过Middleware来配置。




0.Middleware的配置




与pipeline的配置类似，在setting.py中加入Middleware的名字，例如

```
DOWNLOADER_MIDDLEWARES = {
            "miao.middleware.UserAgentMiddleware": 401,
            "miao.middleware.ProxyMiddleware": 402,
        }
```

1.破网站查UA, 我要换UA




某些网站不带UA是不让访问的。




在miao文件夹下面建立一个middleware.py

```
import random


agents = [
    "Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US) AppleWebKit/532.5 (KHTML, like Gecko) Chrome/4.0.249.0 Safari/532.5",
    "Mozilla/5.0 (Windows; U; Windows NT 5.2; en-US) AppleWebKit/532.9 (KHTML, like Gecko) Chrome/5.0.310.0 Safari/532.9",
    "Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US) AppleWebKit/534.7 (KHTML, like Gecko) Chrome/7.0.514.0 Safari/534.7",
    "Mozilla/5.0 (Windows; U; Windows NT 6.0; en-US) AppleWebKit/534.14 (KHTML, like Gecko) Chrome/9.0.601.0 Safari/534.14",
    "Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US) AppleWebKit/534.14 (KHTML, like Gecko) Chrome/10.0.601.0 Safari/534.14",
    "Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US) AppleWebKit/534.20 (KHTML, like Gecko) Chrome/11.0.672.2 Safari/534.20",
    "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/534.27 (KHTML, like Gecko) Chrome/12.0.712.0 Safari/534.27",
    "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.24 Safari/535.1",
]


class UserAgentMiddleware(object):

    def process_request(self, request, spider):
        agent = random.choice(agents)
        request.headers["User-Agent"] = agent
```

这里就是一个简单的随机更换UA的中间件，agents的内容可以自行扩充。




2.破网站封IP，我要用代理




比如本地127.0.0.1开启了一个8123端口的代理，同样可以通过中间件配置让爬虫通过这个代理来对目标网站进行爬取。

同样在middleware.py中加入：

```
class ProxyMiddleware(object):

    def process_request(self, request, spider):
        # 此处填写你自己的代理
        # 如果是买的代理的话可以去用API获取代理列表然后随机选择一个
        proxy = "http://127.0.0.1:8123"
        request.meta["proxy"] = proxy
```

很多网站会对访问次数进行限制，如果访问频率过高的话会临时禁封IP。

如果需要的话可以从网上购买IP，一般服务商会提供一个API来获取当前可用的IP池，选一个填到这里就好。




**一些常用配置**




在settings.py中的一些常用配置

```
# 间隔时间，单位秒。指明scrapy每两个请求之间的间隔。
DOWNLOAD_DELAY = 5

# 当访问异常时是否进行重试
RETRY_ENABLED = True
# 当遇到以下http状态码时进行重试
RETRY_HTTP_CODES = [500, 502, 503, 504, 400, 403, 404, 408]
# 重试次数
RETRY_TIMES = 5

# Pipeline的并发数。同时最多可以有多少个Pipeline来处理item
CONCURRENT_ITEMS = 200
# 并发请求的最大数
CONCURRENT_REQUESTS = 100
# 对一个网站的最大并发数
CONCURRENT_REQUESTS_PER_DOMAIN = 50
# 对一个IP的最大并发数
CONCURRENT_REQUESTS_PER_IP = 50
```

我就是要用Pycharm




如果非要用Pycharm作为开发调试工具的话可以在运行配置里进行如下配置：

Configuration页面：




Script填你的scrapy的cmdline.py路径，比如我的是
`/usr/local/lib/python2.7/dist-packages/scrapy/cmdline.py`
然后在Scrpit parameters中填爬虫的名字，本例中即为：
`crawl NgaSpider`
最后是Working diretory，找到你的settings.py文件，填这个文件所在的目录。




示例：




![](https://img-blog.csdn.net/20180610182316722?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




按小绿箭头就可以愉快地调试了。




**参考**




这里提供了对scrapy非常详细的介绍。




http://scrapy-chs.readthedocs.io/zh_CN/0.24/




以下是几个比较重要的地方：




scrapy的架构:

http://scrapy-chs.readthedocs.io/zh_CN/0.24/topics/architecture.html




xpath语法：

http://www.w3school.com.cn/xpath/xpath_syntax.asp




Pipeline管道配置：

http://scrapy-chs.readthedocs.io/zh_CN/0.24/topics/item-pipeline.html




Middleware中间件的配置：

http://scrapy-chs.readthedocs.io/zh_CN/0.24/topics/downloader-middleware.html




settings.py的配置：

http://scrapy-chs.readthedocs.io/zh_CN/0.24/topics/settings.html




> 
作者：舞鹤

来源：见文末https://segmentfault.com/a/1190000008135000

转载自：Python那些事





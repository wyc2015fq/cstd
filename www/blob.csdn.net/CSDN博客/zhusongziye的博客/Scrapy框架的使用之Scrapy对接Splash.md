# Scrapy框架的使用之Scrapy对接Splash - zhusongziye的博客 - CSDN博客





2018年05月19日 21:42:10[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：3081








之前我们实现了Scrapy对接Selenium抓取淘宝商品的过程，这是一种抓取JavaScript动态渲染页面的方式。除了Selenium，Splash也可以实现同样的功能。本节我们来了解Scrapy对接Splash来进行页面抓取的方式。


## 一、准备工作

请确保Splash已经正确安装并正常运行，同时安装好Scrapy-Splash库。


## 二、新建项目

首先新建一个项目，名为scrapysplashtest，命令如下所示：


scrapy startproject scrapysplashtest

新建一个 Spider，命令如下所示：

scrapy genspider taobao www.taobao.com

## 三、添加配置

可以参考Scrapy-Splash的配置说明进行一步步的配置，链接如下：https://github.com/scrapy-plugins/scrapy-splash#configuration。


修改settings.py，配置`SPLASH_URL`。在这里我们的Splash是在本地运行的，所以可以直接配置本地的地址：

SPLASH_URL = 'http://localhost:8050'

如果Splash是在远程服务器运行的，那此处就应该配置为远程的地址。例如运行在IP为120.27.34.25的服务器上，则此处应该配置为：

SPLASH_URL = 'http://120.27.34.25:8050'

还需要配置几个Middleware，代码如下所示：

DOWNLOADER_MIDDLEWARES = {
'scrapy_splash.SplashCookiesMiddleware': 723,
'scrapy_splash.SplashMiddleware': 725,
'scrapy.downloadermiddlewares.httpcompression.HttpCompressionMiddleware': 810,
}
SPIDER_MIDDLEWARES = {
'scrapy_splash.SplashDeduplicateArgsMiddleware': 100,
}

这里配置了三个Downloader Middleware和一个Spider Middleware，这是Scrapy-Splash的核心部分。我们不再需要像对接Selenium那样实现一个Downloader Middleware，Scrapy-Splash库都为我们准备好了，直接配置即可。

还需要配置一个去重的类`DUPEFILTER_CLASS`，代码如下所示：

DUPEFILTER_CLASS = 'scrapy_splash.SplashAwareDupeFilter'

最后配置一个Cache存储`HTTPCACHE_STORAGE`，代码如下所示：

HTTPCACHE_STORAGE = 'scrapy_splash.SplashAwareFSCacheStorage'

## 四、新建请求

配置完成之后，我们就可以利用Splash来抓取页面了。我们可以直接生成一个`SplashRequest`对象并传递相应的参数，Scrapy会将此请求转发给Splash，Splash对页面进行渲染加载，然后再将渲染结果传递回来。此时Response的内容就是渲染完成的页面结果了，最后交给Spider解析即可。


我们来看一个示例，如下所示：

yield SplashRequest(url, self.parse_result,
    args={
# optional; parameters passed to Splash HTTP API
'wait': 0.5,
# 'url' is prefilled from request url
# 'http_method' is set to 'POST' for POST requests
# 'body' is set to request body for POST requests
    },
    endpoint='render.json', # optional; default is render.html
    splash_url='<url>',     # optional; overrides SPLASH_URL
)

这里构造了一个`SplashRequest`对象，前两个参数依然是请求的URL和回调函数。另外我们还可以通过`args`传递一些渲染参数，例如等待时间`wait`等，还可以根据`endpoint`参数指定渲染接口。更多参数可以参考文档说明：https://github.com/scrapy-plugins/scrapy-splash#requests。

另外我们也可以生成Request对象，Splash的配置通过`meta`属性配置即可，代码如下：

yield scrapy.Request(url, self.parse_result, meta={
'splash': {
'args': {
# set rendering arguments here
'html': 1,
'png': 1,
# 'url' is prefilled from request url
# 'http_method' is set to 'POST' for POST requests
# 'body' is set to request body for POST requests
        },
# optional parameters
'endpoint': 'render.json',  # optional; default is render.json
'splash_url': '<url>',      # optional; overrides SPLASH_URL
'slot_policy': scrapy_splash.SlotPolicy.PER_DOMAIN,
'splash_headers': {},       # optional; a dict with headers sent to Splash
'dont_process_response': True, # optional, default is False
'dont_send_headers': True,  # optional, default is False
'magic_response': False,    # optional, default is True
    }
})

`SplashRequest`对象通过`args`来配置和Request对象通过`meta`来配置，两种方式达到的效果是相同的。



本节我们要做的抓取是淘宝商品信息，涉及页面加载等待、模拟点击翻页等操作。我们可以首先定义一个Lua脚本，来实现页面加载、模拟点击翻页的功能，代码如下所示：

functionmain(splash, args)
  args = {
    url="https://s.taobao.com/search?q=iPad",
    wait=5,
    page=5
  }
  splash.images_enabled = false
assert(splash:go(args.url))
assert(splash:wait(args.wait))
  js = string.format("document.querySelector('#mainsrp-pager div.form > input').value=%d;document.querySelector('#mainsrp-pager div.form > span.btn.J_Submit').click()", args.page)
  splash:evaljs(js)
assert(splash:wait(args.wait))
return splash:png()
end

我们定义了三个参数：请求的链接`url`、等待时间`wait`、分页页码`page`。然后禁用图片加载，请求淘宝的商品列表页面，通过`evaljs()`方法调用JavaScript代码，实现页码填充和翻页点击，最后返回页面截图。我们将脚本放到Splash中运行，正常获取到页面截图，如下图所示。

![](https://img-blog.csdn.net/20180519214042900?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

翻页操作也成功实现，如下图所示即为当前页码，和我们传入的页码`page`参数是相同的。

![](https://img-blog.csdn.net/20180519214106232?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

我们只需要在Spider里用`SplashRequest`对接Lua脚本就好了，如下所示：

from scrapy import Spider
from urllib.parse import quote
from scrapysplashtest.items import ProductItem
from scrapy_splash import SplashRequest

script = """
function main(splash, args)
  splash.images_enabled = false
  assert(splash:go(args.url))
  assert(splash:wait(args.wait))
  js = string.format("document.querySelector('#mainsrp-pager div.form > input').value=%d;document.querySelector('#mainsrp-pager div.form > span.btn.J_Submit').click()", args.page)
  splash:evaljs(js)
  assert(splash:wait(args.wait))
  return splash:html()
end
"""

classTaobaoSpider(Spider):
    name = 'taobao'
    allowed_domains = ['www.taobao.com']
    base_url = 'https://s.taobao.com/search?q='

defstart_requests(self):
for keyword in self.settings.get('KEYWORDS'):
for page in range(1, self.settings.get('MAX_PAGE') + 1):
                url = self.base_url + quote(keyword)
yield SplashRequest(url, callback=self.parse, endpoint='execute', args={'lua_source': script, 'page': page, 'wait': 7})

我们把Lua脚本定义成长字符串，通过`SplashRequest`的`args`来传递参数，接口修改为`execute`。另外，`args`参数里还有一个`lua_source`字段用于指定Lua脚本内容。这样我们就成功构造了一个`SplashRequest`，对接Splash的工作就完成了。

其他的配置不需要更改，Item、Item Pipeline等设置与上节对接Selenium的方式相同，`parse()`回调函数也是完全一致的。

## 五、运行

接下来，我们通过如下命令运行爬虫：


scrapy crawl taobao

运行结果如下图所示。

![](https://img-blog.csdn.net/20180519214129583?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

由于Splash和Scrapy都支持异步处理，我们可以看到同时会有多个抓取成功的结果。在Selenium的对接过程中，每个页面渲染下载是在Downloader Middleware里完成的，所以整个过程是阻塞式的。Scrapy会等待这个过程完成后再继续处理和调度其他请求，这影响了爬取效率。因此使用Splash的爬取效率比Selenium高很多。

最后我们再看看MongoDB的结果，如下图所示。

![](https://img-blog.csdn.net/20180519214145633?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

结果同样正常保存到MongoDB中。

## 六、本节代码

本节代码地址为：https://github.com/Python3WebSpider/ScrapySplashTest。


## 七、结语

因此，在Scrapy中，建议使用Splash处理JavaScript动态渲染的页面。这样不会破坏Scrapy中的异步处理过程，会大大提高爬取效率。而且Splash的安装和配置比较简单，通过API调用的方式实现了模块分离，大规模爬取的部署也更加方便。




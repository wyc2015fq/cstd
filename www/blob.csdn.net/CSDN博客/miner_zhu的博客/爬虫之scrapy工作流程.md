# 爬虫之scrapy工作流程 - miner_zhu的博客 - CSDN博客





2018年07月18日 10:43:21[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：1081








### **Scrapy****是什么？**

scrapy 是一个为了爬取网站数据，提取结构性数据而编写的应用框架，我们只需要实现少量代码，就能够快速的抓取到数据内容。Scrapy 使用了 Twisted`['tw``ɪ``st``ɪ``d]`(其主要对手是Tornado)异步网络框架来处理网络通讯，可以加快我们的下载速度，不用自己去实现异步框架，并且包含了各种中间件接口，可以灵活的完成各种需求。

异步与非阻塞的区别：异步：调用在发出之后，这个调用就直接返回，不管有无结果

                            非阻塞：关注的是程序在等待调用结果（消息，返回值）时的状态，指在不能立刻得到结果之前，该调用不会                               阻塞当前线程。

![](https://img-blog.csdn.net/20180718104139585?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21pbmVyX3podQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180718104208122?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21pbmVyX3podQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## **scrapy****详细工作流程：**

1.首先Spiders（爬虫）将需要发送请求的url(requests)经ScrapyEngine（引擎）交给Scheduler（调度器）。

2.Scheduler（排序，入队）处理后，经ScrapyEngine，DownloaderMiddlewares(可选，主要有User_Agent, Proxy代理)交给Downloader。

3.Downloader向互联网发送请求，并接收下载响应（response）。将响应（response）经ScrapyEngine，SpiderMiddlewares(可选)交给Spiders。　　　　　

4.Spiders处理response，提取数据并将数据经ScrapyEngine交给ItemPipeline保存（可以是本地，可以是数据库）。

5.提取url重新经ScrapyEngine交给Scheduler进行下一个循环。直到无Url请求程序停止结束。

## Scrapy主要包括了以下组件：
- 引擎(Scrapy): 用来处理整个系统的数据流处理, 触发事务(框架核心)
- 调度器(Scheduler): 用来接受引擎发过来的请求, 压入队列中, 并在引擎再次请求的时候返回. 可以想像成一个URL（抓取网页的网址或者说是链接）的优先队列, 由它来决定下一个要抓取的网址是什么, 同时去除重复的网址
- 下载器(Downloader): 用于下载网页内容, 并将网页内容返回给蜘蛛(Scrapy下载器是建立在twisted这个高效的异步模型上的)
- 爬虫(Spiders): 爬虫是主要干活的, 用于从特定的网页中提取自己需要的信息, 即所谓的实体(Item)。用户也可以从中提取出链接,让Scrapy继续抓取下一个页面
- 项目管道(Pipeline): 负责处理爬虫从网页中抽取的实体，主要的功能是持久化实体、验证实体的有效性、清除不需要的信息。当页面被爬虫解析后，将被发送到项目管道，并经过几个特定的次序处理数据。
- 下载器中间件(Downloader Middlewares): 位于Scrapy引擎和下载器之间的框架，主要是处理Scrapy引擎与下载器之间的请求及响应。
- 爬虫中间件(Spider Middlewares): 介于Scrapy引擎和爬虫之间的框架，主要工作是处理蜘蛛的响应输入和请求输出。
- 调度中间件(Scheduler Middewares): 介于Scrapy引擎和调度之间的中间件，从Scrapy引擎发送到调度的请求和响应。

## Scrapy运行流程大概如下：
- 首先，引擎从调度器中取出一个链接(URL)用于接下来的抓取
- 引擎把URL封装成一个请求(Request)传给下载器，下载器把资源下载下来，并封装成应答包(Response)
- 然后，爬虫解析Response
- 若是解析出实体（Item）,则交给实体管道进行进一步的处理。
- 若是解析出的是链接（URL）,则把URL交给Scheduler等待抓取





